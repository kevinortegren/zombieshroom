
#include <GL/glew.h>

#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>

#include <RootEngine/Render/Include/Renderer.h>
#include <RootEngine/Render/Include/RenderExtern.h>

#include <RootEngine/Render/Include/Math/Math.h>

#if defined(_DEBUG) && defined(WIN32)
#include <windows.h>
void APIENTRY PrintOpenGLError(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* param) 
{
	Render::g_context.m_logger->LogText("message: %s", message);
	Render::g_context.m_logger->LogText("type: ");
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		Render::g_context.m_logger->LogText("ERROR");
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		Render::g_context.m_logger->LogText("DEPRECATED_BEHAVIOR");
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		Render::g_context.m_logger->LogText("UNDEFINED_BEHAVIOR");
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		Render::g_context.m_logger->LogText("PORTABILITY");
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		Render::g_context.m_logger->LogText("PERFORMANCE");
		break;
	case GL_DEBUG_TYPE_OTHER:
		Render::g_context.m_logger->LogText("OTHER");
		break;
	}

	Render::g_context.m_logger->LogText("id: %i", id);
	Render::g_context.m_logger->LogText("severity: ");
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_LOW:
		Render::g_context.m_logger->LogText("LOW");
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		Render::g_context.m_logger->LogText("MEDIUM");
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		Render::g_context.m_logger->LogText("HIGH");
		break;
	}
}
#endif

namespace Render
{
	std::map<Semantic::Semantic, unsigned> GLRenderer::s_sizes;

	RootEngine::SubsystemSharedContext g_context;

	GLRenderer::GLRenderer()
	{
		
	}

	void GLRenderer::Startup()
	{
		g_context.m_logger->LogText(LogTag::RENDER, LogLevel::INIT_PRINT, "Renderer subsystem initialized!");
	}

	void GLRenderer::Shutdown()
	{
		SDL_GL_DeleteContext(m_glContext);
	}

	void GLRenderer::SetupSDLContext(SDL_Window* p_window)
	{
		m_window = p_window;

		int flags = SDL_GL_CONTEXT_PROFILE_CORE;
#if defined (_DEBUG)
		flags |= SDL_GL_CONTEXT_DEBUG_FLAG;
#endif

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, flags);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);

		m_glContext = SDL_GL_CreateContext(p_window);
		if(!m_glContext) {
			g_context.m_logger->LogText("%s", SDL_GetError());
		}

		SDL_GL_SetSwapInterval(0);

		int width, height;
		SDL_GetWindowSize(p_window, &width, &height);
		glViewport(0, 0, width, height);
		m_width = width;
		m_height = height;

		// Init GLEW.
		glewExperimental = GL_TRUE; 
		GLenum err = glewInit();
		if (err != GLEW_OK) {
			Render::g_context.m_logger->LogText(LogTag::RENDER,  LogLevel::FATAL_ERROR, "Failed to initialize glew!");
			return;
		}

		GLint major, minor;
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);
		Render::g_context.m_logger->LogText(LogTag::RENDER,  LogLevel::DEBUG_PRINT, "OpenGL context version: %d.%d", major, minor);

		glClearColor(0,0,0,1);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glEnable(GL_DEPTH_TEST);
		glFrontFace(GL_CCW);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

#if defined(_DEBUG) && defined(WIN32)
		if(glDebugMessageCallbackARB)
		{
			g_context.m_logger->LogText(LogTag::RENDER,  LogLevel::DEBUG_PRINT, "Register OpenGL debug callback ");
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallbackARB(PrintOpenGLError, nullptr);
			GLuint unusedIds = 0;
			glDebugMessageControlARB(GL_DONT_CARE,
				GL_DONT_CARE,
				GL_DONT_CARE,
				0,
				&unusedIds,
				true);
		}
		else
			g_context.m_logger->LogText(LogTag::RENDER,  LogLevel::NON_FATAL_ERROR, "glDebugMessageCallback not available");
#endif

		// Check for extensions.
		if(CheckExtension("GL_NVX_gpu_memory_info"))
		{
			g_context.m_logger->LogText(LogTag::RENDER,  LogLevel::DEBUG_PRINT, "Available video memory: %i KB", GetAvailableVideoMemory());
		}

		CheckExtension("NV_texture_multisample");

		// Setup GBuffer.
		m_gbuffer.Init(this, width, height);

		// Setup shadow device.
		m_shadowDevice.Init(this, 4092, 4092);

		// Setup lighting device.
		m_lighting.Init(this, width, height);
	
		// Setup render target for forward renderer and post processes to use.
		glGenFramebuffers(1, &m_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

		m_color0 = CreateTexture();
		m_color0->CreateEmptyTexture(width, height, TextureFormat::TEXTURE_RGBA);

		m_color1 = CreateTexture();
		m_color1->CreateEmptyTexture(width, height, TextureFormat::TEXTURE_RGBA);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_color0->GetHandle(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_color1->GetHandle(), 0);

		// Share depth attachment between gbuffer and forward.
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_gbuffer.m_depthTexture->GetHandle(), 0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Setup fullscreen quad.
		Render::Vertex1P1UV verts[4];
		verts[0].m_pos = glm::vec3(-1.0f, -1.0f, 0.0f);
		verts[1].m_pos = glm::vec3(+1.0f, -1.0f, 0.0f);
		verts[2].m_pos = glm::vec3(-1.0f, +1.0f, 0.0f);
		verts[3].m_pos = glm::vec3(+1.0f, +1.0f, 0.0f);

		verts[0].m_UV = glm::vec2(0.0f, 0.0f);
		verts[1].m_UV = glm::vec2(1.0f, 0.0f);
		verts[2].m_UV = glm::vec2(0.0f, 1.0f);
		verts[3].m_UV = glm::vec2(1.0f, 1.0f);
		
		unsigned int indices[6];
		indices[0] = 0; 
		indices[1] = 1; 
		indices[2] = 2;
		indices[3] = 2;
		indices[4] = 1; 
		indices[5] = 3;

		m_fullscreenQuad.SetVertexBuffer(CreateBuffer(GL_ARRAY_BUFFER));
		m_fullscreenQuad.SetElementBuffer(CreateBuffer(GL_ELEMENT_ARRAY_BUFFER));
		m_fullscreenQuad.SetVertexAttribute(CreateVertexAttributes());

		m_fullscreenQuad.CreateIndexBuffer(indices, 6);
		m_fullscreenQuad.CreateVertexBuffer1P1UV(verts, 4);

		// Load default forward rendering effect.
		auto renderEffect = g_context.m_resourceManager->LoadEffect("Renderer/Forward");
		m_renderTech = renderEffect->GetTechniques()[0];

		m_cameraVars.m_view = glm::mat4(1.0f);
		m_cameraVars.m_projection = glm::perspectiveFov<float>(45.0f, (float)width, (float)height, 0.1f, 100.0f);

		m_cameraVars.m_invView = glm::inverse(m_cameraVars.m_view);
		m_cameraVars.m_invProj = glm::inverse(m_cameraVars.m_projection);
		m_cameraVars.m_invViewProj = glm::inverse(m_cameraVars.m_projection * m_cameraVars.m_view);

		// PerFrame uniforms.
		m_cameraBuffer = CreateBuffer(GL_UNIFORM_BUFFER);
		m_cameraBuffer->BufferData(1, sizeof(m_cameraVars), &m_cameraVars);
		glBindBufferBase(GL_UNIFORM_BUFFER, RENDER_SLOT_PERFRAME, m_cameraBuffer->GetBufferId());

		// PerObject uniforms.
		m_uniforms = CreateBuffer(GL_UNIFORM_BUFFER);
		char data[RENDER_UNIFORMS_SIZE];
		memset(&data, 0, RENDER_UNIFORMS_SIZE);

		m_uniforms->BufferData(1, RENDER_UNIFORMS_SIZE, data);
		glBindBufferBase(GL_UNIFORM_BUFFER, RENDER_SLOT_PEROBJECT, m_uniforms->GetBufferId());

		InitializeSemanticSizes();

		// Initialize subsystems.
		m_particles.Init(this);
		m_lineRenderer.Init(this);

		PrintResourceUsage();

		// Test PPS.

		/*
		auto a = g_context.m_resourceManager->LoadEffect("PostProcess/Glow");

		float weights[10];

		float sum = 0;
		for( int i = 0;i < 10; i++ )
		{
			weights[i] = RootEngine::gauss(i, 9);
			sum += 2 * weights[i];
		}

		for( int i = 0; i < 10; i++ )
		{
			weights[i] = weights[i] / sum;
		}

		m_postProcessEffects.push_back(a);
		*/
	}

	void GLRenderer::InitializeSemanticSizes()
	{
		s_sizes[Semantic::MODEL]		= sizeof(glm::mat4);
		s_sizes[Semantic::NORMAL]		= sizeof(glm::mat4);
		s_sizes[Semantic::BONES]		= 20 * sizeof(glm::mat4);
		s_sizes[Semantic::SHADOW]		= sizeof(glm::mat4);
		s_sizes[Semantic::POSITION]		= sizeof(glm::vec3);
		s_sizes[Semantic::LIFETIMEMIN]	= sizeof(float);
		s_sizes[Semantic::LIFETIMEMAX]	= sizeof(float);
		s_sizes[Semantic::SPEEDMIN]		= sizeof(float);
		s_sizes[Semantic::SPEEDMAX]		= sizeof(float);
		s_sizes[Semantic::SIZEMIN]		= sizeof(glm::vec2);
		s_sizes[Semantic::SIZEMAX]		= sizeof(glm::vec2);
		s_sizes[Semantic::SIZEEND]		= sizeof(glm::vec2);
		s_sizes[Semantic::COLOR]		= sizeof(glm::vec4);
		s_sizes[Semantic::COLOREND]		= sizeof(glm::vec4);
		s_sizes[Semantic::GRAVITY]		= sizeof(glm::vec3);
		s_sizes[Semantic::DIRECTION]	= sizeof(glm::vec3);
		s_sizes[Semantic::SPREAD]		= sizeof(float);
		s_sizes[Semantic::SPAWNTIME]	= sizeof(float);
		s_sizes[Semantic::TRANSPOSITION]= sizeof(glm::vec3);
		s_sizes[Semantic::ORBITSPEED]	= sizeof(float);
		s_sizes[Semantic::ORBITRADIUS]	= sizeof(float);
		s_sizes[Semantic::MK1]			= sizeof(float);
		s_sizes[Semantic::MK2]			= sizeof(float);
		s_sizes[Semantic::MK3]			= sizeof(float);
	}

	void GLRenderer::SetResolution(int p_width, int p_height)
	{
		SDL_SetWindowSize(m_window, p_width, p_height);	

		m_gbuffer.Resize(p_width, p_height);
		m_lighting.Resize(p_width, p_height);

		glViewport(0, 0, p_width, p_height);

		m_width = p_width;
		m_height = p_height;
	}

	void GLRenderer::AddRenderJob(const RenderJob& p_job)
	{
		m_jobs.push_back(p_job);
	}

	void GLRenderer::SetAmbientLight(const glm::vec4& p_color)
	{
		m_lighting.SetAmbientLight(p_color);
	}

	void GLRenderer::AddDirectionalLight(const DirectionalLight& p_light, int index)
	{
		m_lighting.AddDirectionalLight(p_light, index);
	}

	void GLRenderer::AddPointLight(const PointLight& p_light, int index)
	{
		m_lighting.AddPointLight(p_light, index);
	}

	void GLRenderer::AddLine( glm::vec3 p_fromPoint, glm::vec3 p_toPoint, glm::vec4 p_color )
	{
		m_lineRenderer.AddLine(Line(p_fromPoint, p_toPoint, p_color));
	}

	void GLRenderer::Render()
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, RENDER_SLOT_PEROBJECT, m_uniforms->GetBufferId());
		glBindBufferBase(GL_UNIFORM_BUFFER, RENDER_SLOT_PERFRAME, m_cameraBuffer->GetBufferId());

		{
			PROFILE("Shadow pass", g_context.m_profiler);
			ShadowPass();
		}

		// Buffer Per Frame data.
		m_cameraVars.m_invViewProj = glm::inverse(m_cameraVars.m_projection * m_cameraVars.m_view);
		m_cameraBuffer->BufferSubData(0, sizeof(m_cameraVars), &m_cameraVars);

		{
			PROFILE("Geometry Pass", g_context.m_profiler);
			GeometryPass();
		}

		{
			PROFILE("Lighting Pass", g_context.m_profiler);
			LightingPass();	
		}

		{
			PROFILE("Forward Pass", g_context.m_profiler);
			ForwardPass();
		}

		{
			PROFILE("PostProcess Pass", g_context.m_profiler);
			PostProcessPass();
		}

		{
			PROFILE("Output", g_context.m_profiler);
			Output();
		}
	}

	void GLRenderer::Clear()
	{
		glDepthMask(GL_TRUE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_jobs.clear();
	}

	void GLRenderer::Swap()
	{
		SDL_GL_SwapWindow(m_window);
	}

	void GLRenderer::GeometryPass()
	{
		m_gbuffer.UnbindTextures();
		m_gbuffer.Enable();
		m_renderFlags = Render::TechniqueFlags::RENDER_DEFERRED;

		std::sort(m_jobs.begin(), m_jobs.end(), [](RenderJob& a, RenderJob& b)->bool{ return a.m_renderPass < b.m_renderPass; });

		ProcessRenderJobs();
		
		// Bind textures for read.
		m_gbuffer.BindTextures();
	}

	void GLRenderer::ProcessRenderJobs()
	{
		// Itterate jobs.
		for(auto job = m_jobs.begin(); job != m_jobs.end(); ++job)
		{
			// Bind VAO.
			(*job).m_mesh->Bind();

			// Bind textures.
			for(auto texture = (*job).m_material->m_textures.begin(); texture != (*job).m_material->m_textures.end(); ++texture)
			{
				if((*texture).second != nullptr)
				(*texture).second->Bind((*texture).first);
			}

			// Itterate techniques.
			for(auto tech = (*job).m_material->m_effect->GetTechniques().begin(); tech != (*job).m_material->m_effect->GetTechniques().end(); ++tech)
			{
				if((m_renderFlags & (*tech)->m_flags) == m_renderFlags)
				{
					// Buffer uniforms.
					for(auto param = (*job).m_params.begin(); param != (*job).m_params.end(); ++param)
					{	
						m_uniforms->BufferSubData((*tech)->m_uniformsParams[param->first], s_sizes[param->first], param->second);
					}

					// Itterate programs.
					for(auto program = (*tech)->GetPrograms().begin(); program != (*tech)->GetPrograms().end(); ++program)
					{
						// Apply program.
						(*program)->Apply();

						if(((*job).m_flags & RenderFlags::RENDER_TRANSFORMFEEDBACK) == RenderFlags::RENDER_TRANSFORMFEEDBACK)
						{
							(*job).m_mesh->DrawTransformFeedback();
						}
						else
						{
							(*job).m_mesh->Draw();		
						}
					}
				}
			}
			
			// Unbind textures.
			for(auto texture = (*job).m_material->m_textures.begin(); texture != (*job).m_material->m_textures.end(); ++texture)
			{
				if((*texture).second != nullptr)
				(*texture).second->Unbind((*texture).first);
			}

			// Unbind VAO.
			(*job).m_mesh->Unbind();
		}
	}

	void GLRenderer::ShadowPass()
	{
		m_shadowDevice.Process();

		glBindFramebuffer(GL_FRAMEBUFFER, m_shadowDevice.GetFramebuffer());
		glDrawBuffers(0, NULL);

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		// Buffer Per Frame data.
		struct
		{
			glm::mat4 m_projection;
			glm::mat4 m_view;
			glm::mat4 m_invView;
			glm::mat4 m_invProj;
			glm::mat4 m_invViewProj;

		} matrices;

		matrices.m_view = m_shadowDevice.m_shadowcasters[0].m_viewMatrix;
		matrices.m_projection = m_shadowDevice.m_shadowcasters[0].m_projectionMatrix;
		matrices.m_invView = glm::inverse(m_shadowDevice.m_shadowcasters[0].m_viewMatrix);
		matrices.m_invProj = glm::inverse(m_shadowDevice.m_shadowcasters[0].m_projectionMatrix);
		matrices.m_invViewProj = glm::inverse(matrices.m_projection * matrices.m_view);

		glm::mat4 viewProjection = m_shadowDevice.m_shadowcasters[0].m_projectionMatrix * m_shadowDevice.m_shadowcasters[0].m_viewMatrix;

		matrices.m_invViewProj = glm::inverse(viewProjection);
		m_cameraBuffer->BufferSubData(0, sizeof(matrices), &matrices);

		glCullFace(GL_FRONT);
		glViewport(0, 0, m_shadowDevice.GetWidth(), m_shadowDevice.GetHeight());

		for(auto job = m_jobs.begin(); job != m_jobs.end(); ++job)
		{
			if(((*job).m_flags & Render::RenderFlags::RENDER_IGNORE_CASTSHADOW) == Render::RenderFlags::RENDER_IGNORE_CASTSHADOW)
				continue;

			for(auto tech = (*job).m_material->m_effect->GetTechniques().begin(); tech != (*job).m_material->m_effect->GetTechniques().end(); ++tech)
			{
				if(((*tech)->m_flags &  Render::TechniqueFlags::RENDER_SHADOW) ==  Render::TechniqueFlags::RENDER_SHADOW)
				{
					if((*job).m_shadowMesh != nullptr)
					{
						(*job).m_shadowMesh->Bind();

						// Buffer uniforms.
						for(auto param = (*job).m_params.begin(); param != (*job).m_params.end(); ++param)
						{	
							m_uniforms->BufferSubData((*tech)->m_uniformsParams[param->first], s_sizes[param->first], param->second);
						}

						// Apply shadowing shader program.
						(*tech)->GetPrograms()[0]->Apply();

						if(((*job).m_flags & RenderFlags::RENDER_TRANSFORMFEEDBACK) == RenderFlags::RENDER_TRANSFORMFEEDBACK)
						{
							(*job).m_shadowMesh->DrawTransformFeedback();
						}
						else
						{
							(*job).m_shadowMesh->Draw();		
						}

						(*job).m_shadowMesh->Unbind();
					}
				}
			}
		}

		glCullFace(GL_BACK);
		glViewport(0, 0, m_width, m_height);
	}

	void GLRenderer::LightingPass()
	{
		m_shadowDevice.m_depthTexture->Bind(Render::TextureSemantic::DEPTH);

		glm::mat4 biasMatrix(
			0.5, 0.0, 0.0, 0.0, 
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.5, 0.5, 0.5, 1.0
			);

		// Buffer LightVP.
		glm::mat4 lvp = biasMatrix * m_shadowDevice.m_shadowcasters[0].m_projectionMatrix * m_shadowDevice.m_shadowcasters[0].m_viewMatrix;
		m_uniforms->BufferSubData(0, sizeof(glm::mat4), &lvp);

		// Apply lighting.
		m_lighting.Process(m_fullscreenQuad);

		// Bind forward target.
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

		GLenum buffers[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, buffers);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Bind la-texture.
		glActiveTexture(GL_TEXTURE0 + 5);
		glBindTexture(GL_TEXTURE_2D, m_lighting.m_laHandle);

		// Output lighting to forward buffer.
		m_renderTech->GetPrograms()[0]->Apply();

		m_fullscreenQuad.Bind();
		m_fullscreenQuad.Draw();
		m_fullscreenQuad.Unbind();
	}

	void GLRenderer::ForwardPass()
	{
		m_renderFlags = Render::TechniqueFlags::RENDER_FORWARD;

		ProcessRenderJobs();

		{
			PROFILE("Render Lines", g_context.m_profiler);
			m_lineRenderer.RenderLines();
		}
	}

	void GLRenderer::PostProcessPass()
	{
		// Bind gbuffer.
		m_gbuffer.BindTextures();

		// Bind la-texture.
		glActiveTexture(GL_TEXTURE0 + 3);
		glBindTexture(GL_TEXTURE_2D, m_lighting.m_laHandle);

		m_fullscreenQuad.Bind();

		m_activeTarget = GL_COLOR_ATTACHMENT1;
		m_activeTexture = m_color0->GetHandle();

		for(auto effect = m_postProcessEffects.begin(); effect != m_postProcessEffects.end(); ++effect)
		{
			for(auto tech = (*effect)->GetTechniques().begin(); tech != (*effect)->GetTechniques().end(); ++tech)
			{
				for(auto program = (*tech)->GetPrograms().begin(); program != (*tech)->GetPrograms().end(); ++program)
				{
					// Set input.
					glActiveTexture(GL_TEXTURE0 + 5);
					glBindTexture(GL_TEXTURE_2D, m_activeTexture);

					// Set Output.
					GLenum buffers[] = { m_activeTarget };
					glDrawBuffers(1, buffers);

					(*program)->Apply();

					m_fullscreenQuad.Draw();

					// Swap input/output.
					m_activeTarget = ((m_activeTarget == GL_COLOR_ATTACHMENT0) ? GL_COLOR_ATTACHMENT1 : GL_COLOR_ATTACHMENT0);
					m_activeTexture = ((m_activeTexture == m_color1->GetHandle()) ? m_color0->GetHandle() : m_color1->GetHandle());
				}
			}
		}

		m_fullscreenQuad.Unbind();
	}

	void GLRenderer::Output()
	{
		// Bind backbuffer.
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Bind active target.
		glActiveTexture(GL_TEXTURE0 + 5);
		glBindTexture(GL_TEXTURE_2D, m_activeTexture);

		m_renderTech->GetPrograms()[0]->Apply();

		m_fullscreenQuad.Bind();
		m_fullscreenQuad.Draw();
		m_fullscreenQuad.Unbind();

		// Unbind active target.
		glActiveTexture(GL_TEXTURE0 + 5);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	bool GLRenderer::CheckExtension(const char* p_extension)
	{
		if(glewIsExtensionSupported(p_extension) == GL_FALSE) {
			g_context.m_logger->LogText(LogTag::RENDER,  LogLevel::WARNING,  "Missing OpenGL extension: %s", p_extension);
			return false;
		}
		return true;
	}

	int GLRenderer::GetAvailableVideoMemory()
	{
		GLint cur_avail_mem_kb = 0;
		glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &cur_avail_mem_kb);

		return cur_avail_mem_kb;
	}

	void GLRenderer::PrintResourceUsage()
	{
		int bufferSize = 0;
		for(auto itr = m_buffers.begin(); itr != m_buffers.end(); ++itr)
		{
			bufferSize += (*itr)->GetBufferSize();
		}

		bufferSize /= 1000;

		g_context.m_logger->LogText(LogTag::RENDER, LogLevel::DEBUG_PRINT, "Buffer memory usage: %d kb", bufferSize);

		int textureSize = 0;
		for(auto itr = m_textures.begin(); itr != m_textures.end(); ++itr)
		{
			textureSize += ((*itr)->GetWidth() * (*itr)->GetHeight() * (*itr)->GetBytesPerPixel()) / (*itr)->GetCompressRatio();
			
		}

		textureSize /= 1000;

		g_context.m_logger->LogText(LogTag::RENDER, LogLevel::DEBUG_PRINT, "Texture memory usage: %d kb", textureSize);

	}

	int GLRenderer::GetWidth() const
	{
		return m_width;
	}

	int GLRenderer::GetHeight() const
	{
		return m_height;
	}

	void GLRenderer::SetViewMatrix( glm::mat4 p_viewMatrix )
	{
		m_cameraVars.m_view = p_viewMatrix;
		m_cameraVars.m_invView = glm::inverse(p_viewMatrix);
	}

	void GLRenderer::SetProjectionMatrix( glm::mat4 p_projectionMatrix )
	{
		m_cameraVars.m_projection = p_projectionMatrix;
		m_cameraVars.m_invProj = glm::inverse(p_projectionMatrix);
	}

	BufferInterface* GLRenderer::CreateBuffer(GLenum p_type)
	{ 
		Buffer* buffer = new Buffer(p_type);

		m_buffers.push_back(buffer);

		return buffer;
	}

	void GLRenderer::ReleaseBuffer(BufferInterface* p_buffer)
	{
		auto itr = std::find(m_buffers.begin(), m_buffers.end(), p_buffer);

		m_buffers.erase(itr);
	}

	TextureInterface* GLRenderer::CreateTexture()
	{
		Texture* texture = new Texture();

		m_textures.push_back(texture);

		return texture;
	}

	void GLRenderer::ReleaseTexture(TextureInterface* p_texture)
	{
		auto itr = std::find(m_textures.begin(), m_textures.end(), p_texture);

		m_textures.erase(itr);
	}

	std::shared_ptr<VertexAttributesInterface> GLRenderer::CreateVertexAttributes()
	{ 
		return std::shared_ptr<VertexAttributesInterface>(new VertexAttributes); 
	}

	std::shared_ptr<MeshInterface> GLRenderer::CreateMesh() 
	{ 
		return std::shared_ptr<MeshInterface>(new Mesh);
	}
		std::shared_ptr<EffectInterface> GLRenderer::CreateEffect() 
	{ 
		return std::shared_ptr<EffectInterface>(new Effect);
	}

	std::shared_ptr<Material> GLRenderer::CreateMaterial()
	{
		Material* mat = new Material;
		m_materialMeshMap[mat] = std::vector<MeshInterface*>(); //unknown if needed
		return std::shared_ptr<Material>(mat); 
	}

	ParticleSystem* GLRenderer::CreateParticleSystem()
	{
		return m_particles.Create(this);
	}

	void GLRenderer::SetParticleUniforms(Technique* p_technique, std::map<Render::Semantic::Semantic, void*> p_params)
	{
		m_particles.SetParticleUniforms(p_technique, p_params);
	}

	void GLRenderer::BeginTransform(float dt)
	{
		m_particles.BeginTransform(dt);
	}

	void GLRenderer::EndTransform()
	{
		m_particles.EndTransform();
	}

	void GLRenderer::AddShadowcaster( const Render::Shadowcaster& p_shadowcaster, int p_index)
	{
		m_shadowDevice.AddShadowcaster(p_shadowcaster, p_index);
	}

	void GLRenderer::Compute( ComputeJob* p_job )
	{
		for(auto texture = p_job->m_textures.begin(); texture != p_job->m_textures.end(); ++texture)
		{
				if((*texture).second != nullptr)
				{
					(*texture).second->Bind((*texture).first);
					(*texture).second->BindImage((*texture).first);
				}
		}

		for(auto tech = p_job->m_effect->GetTechniques().begin(); tech != p_job->m_effect->GetTechniques().end(); ++tech)
		{
			for(auto param = p_job->m_params.begin(); param != p_job->m_params.end(); ++param)
			{	
				m_uniforms->BufferSubData((*tech)->m_uniformsParams[param->first], s_sizes[param->first], param->second);
			}

			// Apply program.
			(*tech)->GetPrograms()[0]->Apply();
		
			glDispatchCompute(p_job->m_groupDim.x, p_job->m_groupDim.y, p_job->m_groupDim.z);
			glMemoryBarrier(GL_ALL_BARRIER_BITS);
		}

		for(auto texture = p_job->m_textures.begin(); texture != p_job->m_textures.end(); ++texture)
		{
			if((*texture).second != nullptr)
			{
				(*texture).second->Unbind((*texture).first);
				(*texture).second->UnBindImage((*texture).first);
			}
		}
	}

	void GLRenderer::FreeParticleSystem( ParticleSystemInterface* p_particleSys )
	{
		m_particles.Free((ParticleSystem*)p_particleSys);
	}

}

Render::RendererInterface* CreateRenderer(RootEngine::SubsystemSharedContext p_context)
{
	Render::g_context = p_context;

	return new Render::GLRenderer;
}
