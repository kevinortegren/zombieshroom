
#include <GL/glew.h>

#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>

#include <RootEngine/Render/Include/Renderer.h>
#include <RootEngine/Render/Include/RenderExtern.h>

#if defined(_DEBUG) && defined(WIN32)
#include <windows.h>
void APIENTRY PrintOpenGLError(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* param) 
{
	Render::g_context.m_logger->LogText(LogTag::RENDER, LogLevel::DEBUG_PRINT, "message: %s", message);
	Render::g_context.m_logger->LogText(LogTag::RENDER, LogLevel::DEBUG_PRINT,"type: ");
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		Render::g_context.m_logger->LogText(LogTag::RENDER, LogLevel::NON_FATAL_ERROR, "ERROR");
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		Render::g_context.m_logger->LogText(LogTag::RENDER, LogLevel::WARNING, "DEPRECATED_BEHAVIOR");
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		Render::g_context.m_logger->LogText(LogTag::RENDER, LogLevel::WARNING, "UNDEFINED_BEHAVIOR");
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		Render::g_context.m_logger->LogText(LogTag::RENDER, LogLevel::WARNING, "PORTABILITY");
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		Render::g_context.m_logger->LogText(LogTag::RENDER, LogLevel::WARNING, "PERFORMANCE");
		break;
	case GL_DEBUG_TYPE_OTHER:
		Render::g_context.m_logger->LogText(LogTag::RENDER, LogLevel::WARNING, "OTHER");
		break;
	}

	Render::g_context.m_logger->LogText(LogTag::RENDER, LogLevel::DEBUG_PRINT, "id: %i", id);
	Render::g_context.m_logger->LogText(LogTag::RENDER, LogLevel::DEBUG_PRINT, "severity: ");
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_LOW:
		Render::g_context.m_logger->LogText(LogTag::RENDER, LogLevel::DEBUG_PRINT, "LOW");
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		Render::g_context.m_logger->LogText(LogTag::RENDER, LogLevel::DEBUG_PRINT, "MEDIUM");
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		Render::g_context.m_logger->LogText(LogTag::RENDER, LogLevel::DEBUG_PRINT, "HIGH");
		break;
	}
}
#endif

namespace Render
{
	std::map<Semantic::Semantic, unsigned> GLRenderer::s_sizes;
	std::map<TextureSemantic::TextureSemantic, unsigned> GLRenderer::s_textureSlots;

	RootEngine::SubsystemSharedContext g_context;

	void GLRenderer::ParseCommands(std::stringstream* p_ss)
	{
		std::string module;
		std::string param;
		std::string value;

		std::getline(*p_ss, module, ' ');
		std::getline(*p_ss, module, ' ');

		if(module == "help")
		{
			g_context.m_logger->LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "[RENDER COMMANDS]");
			g_context.m_logger->LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "/r la [int] [1/0] -Activate or deactivate layer of choice");
			g_context.m_logger->LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "/r s [1/0] -Activate or deactivate shadows");
			g_context.m_logger->LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "/r l -Light settings");
			g_context.m_logger->LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "/r g -Glow settings ");
		}
		else if(module == "glow" || module == "g")
		{	
			std::getline(*p_ss, param, ' ');
			std::getline(*p_ss, value, ' ');

			if(param == "factor" || param == "f")
			{
				m_glowDevice.SetGlowFactor((float)atof(value.c_str()));
			}
			else if(param == "strength" || param == "s")
			{
				m_glowDevice.SetGlowStrength((float)atof(value.c_str()));
			}
			else if(param == "radius" || param == "r")
			{
				m_glowDevice.SetGlowRadius((float)atof(value.c_str()));
			}
			else if(param == "display" || param == "d")
			{
				m_glowDevice.Display((atoi(value.c_str()) == 1));
			}
			
		}
		else if(module == "shadow" || module == "s")
		{
			std::getline(*p_ss, value, ' ');
			m_shadowDevice.m_showShadows = (atoi(value.c_str()) == 1) ? true : false;
		}
		else if(module == "light" || module == "l")
		{
			std::getline(*p_ss, param, ' ');
			std::getline(*p_ss, value, ' ');

			if(param == "ambient" || param == "a")
			{
				m_lighting.ShowAmbient((atoi(value.c_str()) == 1));
			}
			else if(param == "directional" || param == "d")
			{
				m_lighting.ShowDirectional((atoi(value.c_str()) == 1));
			}
			else if(param == "point" || param == "p")
			{
				m_lighting.ShowPointLights((atoi(value.c_str()) == 1));
			}
			else if(param == "background" || param == "b")
			{
				m_lighting.ShowBackgroundBlend((atoi(value.c_str()) == 1));
			}
			else if(param == "setambient" || param == "sa")
			{
				float v = (float)atof(value.c_str());
				m_lighting.SetAmbientLight(glm::vec4(v, v, v, 1.0f));
			}
			else if(param == "ssao")
			{
				m_lighting.ShowSSAO((atoi(value.c_str()) == 1));
			}
		}
		else if(module == "layer" || module == "la")
		{
			std::getline(*p_ss, param, ' ');
			std::getline(*p_ss, value, ' ');

			m_layers[atoi(param.c_str())] = (atoi(value.c_str()) == 1);
		}
		else if(module == "forward" || module == "f")
		{
			std::getline(*p_ss, value, ' ');
			m_showForward = (atoi(value.c_str()) == 1);
		}
	}

	GLRenderer::GLRenderer()
		: m_allocator(10000 * sizeof(RenderJob))
	{
		
	}

	GLRenderer::~GLRenderer()
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

		int flags = 0;
#if defined (_DEBUG)
		flags = SDL_GL_CONTEXT_DEBUG_FLAG;
#endif
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
		SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, flags);

		m_glContext = SDL_GL_CreateContext(p_window);
		if(!m_glContext) {
			g_context.m_logger->LogText(LogTag::RENDER, LogLevel::DEBUG_PRINT, "%s", SDL_GetError());
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

		glClearColor(1,0,0,0);
		
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glEnable(GL_DEPTH_TEST);
		glCullFace(GL_BACK);	
		glFrontFace(GL_CCW);

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

		m_jobs.reserve(10000);

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

		m_cameraVars.m_view = glm::mat4(1.0f);
		m_cameraVars.m_projection = glm::perspectiveFov<float>(45.0f, (float)width, (float)height, 0.1f, 100.0f);

		m_cameraVars.m_invView = glm::inverse(m_cameraVars.m_view);
		m_cameraVars.m_invProj = glm::inverse(m_cameraVars.m_projection);
		m_cameraVars.m_invViewProj = glm::inverse(m_cameraVars.m_projection * m_cameraVars.m_view);

		// PerFrame uniforms.
		m_cameraBuffer = CreateBuffer(GL_UNIFORM_BUFFER);
		m_cameraBuffer->BufferData(1, sizeof(m_cameraVars), &m_cameraVars, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, RENDER_SLOT_PERFRAME, m_cameraBuffer->GetBufferId());

		// PerObject uniforms.
		m_uniforms = CreateBuffer(GL_UNIFORM_BUFFER);
		char data[RENDER_UNIFORMS_SIZE];
		memset(&data, 0, RENDER_UNIFORMS_SIZE);

		m_uniforms->BufferData(1, RENDER_UNIFORMS_SIZE, data, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, RENDER_SLOT_PEROBJECT, m_uniforms->GetBufferId());

		// Setup geometry buffer.
		m_gbuffer.Init(this, width, height);

		// Setup shadow device.
		m_shadowDevice.Init(this, 2048, 2048, m_cameraBuffer, m_uniforms);

		// Setup lighting device.
		m_lighting.Init(this, width, height, &m_gbuffer, &m_fullscreenQuad);
	
		// Setup render target for forward renderer and post processes to use.
		glGenFramebuffers(2, m_forwardFramebuffers);
		glBindFramebuffer(GL_FRAMEBUFFER, m_forwardFramebuffers[0]);

		m_forwardColors[0] = CreateTexture();
		m_forwardColors[0]->CreateEmptyTexture(width, height, TextureFormat::TEXTURE_RGBA);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_forwardColors[0]->GetHandle(), 0);

		// Share depth attachment between gbuffer and forward.
		m_forwardDepth[0] = m_gbuffer.m_depthTexture;
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_forwardDepth[0]->GetHandle(), 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, m_forwardFramebuffers[1]);

		m_forwardColors[1] = CreateTexture();
		m_forwardColors[1]->CreateEmptyTexture(width, height, TextureFormat::TEXTURE_RGBA);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_forwardColors[1]->GetHandle(), 0);

		m_forwardDepth[1] = CreateTexture();
		m_forwardDepth[1]->CreateEmptyTexture(width, height, TextureFormat::TEXTURE_DEPTH_STENCIL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_forwardDepth[1]->GetHandle(), 0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		m_renderEffect = g_context.m_resourceManager->LoadEffect("Renderer/Render");

		m_fullscreenQuadTech = m_renderEffect->GetTechniques()[0];

		InitializeSemanticSizes();

		// Initialize subsystems.
		m_particles.Init(this);
		m_lineRenderer.Init(this);

		InitialziePostProcesses();

		m_layers[0] = true;
		m_layers[1] = true;

		//Generate query for gpu timer
		g_context.m_profiler->InitQuery();

		m_showForward = true;
	}

	void GLRenderer::InitializeSemanticSizes()
	{
		// Sizes for common semantics for use in render jobs uniform params.
		s_sizes[Semantic::MODEL]			= sizeof(glm::mat4);
		s_sizes[Semantic::NORMAL]			= sizeof(glm::mat4);
		s_sizes[Semantic::BONES]			= 20 * sizeof(glm::mat4);
		s_sizes[Semantic::SHADOW]			= sizeof(glm::mat4);
		s_sizes[Semantic::POSITION]			= sizeof(glm::vec3);
		s_sizes[Semantic::LIFETIMEMIN]		= sizeof(float);
		s_sizes[Semantic::LIFETIMEMAX]		= sizeof(float);
		s_sizes[Semantic::SPEEDMIN]			= sizeof(float);
		s_sizes[Semantic::SPEEDMAX]			= sizeof(float);
		s_sizes[Semantic::SIZEMIN]			= sizeof(glm::vec2);
		s_sizes[Semantic::SIZEMAX]			= sizeof(glm::vec2);
		s_sizes[Semantic::SIZEEND]			= sizeof(glm::vec2);
		s_sizes[Semantic::COLOR]			= sizeof(glm::vec4);
		s_sizes[Semantic::COLOREND]			= sizeof(glm::vec4);
		s_sizes[Semantic::GRAVITY]			= sizeof(glm::vec3);
		s_sizes[Semantic::DIRECTION]		= sizeof(glm::vec3);
		s_sizes[Semantic::SPREAD]			= sizeof(float);
		s_sizes[Semantic::SPAWNTIME]		= sizeof(float);
		s_sizes[Semantic::TRANSPOSITION]	= sizeof(glm::vec3);
		s_sizes[Semantic::ORBITSPEED]		= sizeof(float);
		s_sizes[Semantic::ORBITRADIUS]		= sizeof(float);
		s_sizes[Semantic::MK1]				= sizeof(float);
		s_sizes[Semantic::MK2]				= sizeof(float);
		s_sizes[Semantic::MK3]				= sizeof(float);
		s_sizes[Semantic::EYEWORLDPOS]		= sizeof(glm::vec3);
		s_sizes[Semantic::DX]				= sizeof(float);
		s_sizes[Semantic::ROTATIONSPEEDMIN] = sizeof(float);
		s_sizes[Semantic::ROTATIONSPEEDMAX] = sizeof(float);
		s_sizes[Semantic::MAXPERFRAME]		= sizeof(float);
		s_sizes[Semantic::ALIVE]			= sizeof(float);

		// Slots reserved for geometry buffer and lighting.
		s_textureSlots[TextureSemantic::GBUFFER_DIFFUSE_SPECULAR]	= 0;
		s_textureSlots[TextureSemantic::GBUFFER_NORMALS]			= 1;
		s_textureSlots[TextureSemantic::GBUFFER_DEPTH]				= 2;		
		s_textureSlots[TextureSemantic::GBUFFER_GLOW_TRANSLUCENCY]	= 3;
		s_textureSlots[TextureSemantic::GBUFFER_BACKGROUND_AMBIENT] = 4;
	
		// Slot reserved for lighting.
		s_textureSlots[TextureSemantic::LIGHTING_ACCUMULATION] = 5;
		s_textureSlots[TextureSemantic::FORWARD_COLOR] = 5;


		// Various texture semantics for render jobs.
		s_textureSlots[TextureSemantic::SHADER_INPUT]   = 6;
		s_textureSlots[TextureSemantic::DIFFUSE]		= 6;
		s_textureSlots[TextureSemantic::COMPUTEIN]		= 6;
		s_textureSlots[TextureSemantic::SPECULAR]		= 7;
		s_textureSlots[TextureSemantic::COMPUTEOUT]		= 7;
		s_textureSlots[TextureSemantic::SSAO]			= 7;
		s_textureSlots[TextureSemantic::NORMAL]			= 8;
		s_textureSlots[TextureSemantic::DIFFUSE2]		= 8;
		s_textureSlots[TextureSemantic::COMPUTENORMAL]	= 8;
		s_textureSlots[TextureSemantic::GLOW]			= 9;
		s_textureSlots[TextureSemantic::SHADOWDEPTHPCF] = 9;
		s_textureSlots[TextureSemantic::DEPTH]			= 10;
		s_textureSlots[TextureSemantic::RANDOM]			= 11;
		s_textureSlots[TextureSemantic::TEXTUREMAP]		= 12;
		s_textureSlots[TextureSemantic::SHADOWDEPTH]	= 13;
		s_textureSlots[TextureSemantic::TEXTURE_R]		= 14;
		s_textureSlots[TextureSemantic::TEXTURE_G]		= 15;
		s_textureSlots[TextureSemantic::TEXTURE_B]		= 16;
		s_textureSlots[TextureSemantic::TRANSLUCENCY]   = 17;
		s_textureSlots[TextureSemantic::DIFFUSE1]		= 18;
		s_textureSlots[TextureSemantic::DIFFUSE2]		= 19;
		s_textureSlots[TextureSemantic::DIFFUSE3]		= 20;
	}

	void GLRenderer::InitialziePostProcesses()
	{
		m_glowDevice.Init(this, m_width, m_height);		
	}

	void GLRenderer::SetResolution(bool p_fullscreen, int p_width, int p_height)
	{
		SDL_SetWindowFullscreen(m_window, false);

		m_width = p_width;
		m_height = p_height;

		if(p_fullscreen)
		{
			SDL_DisplayMode nativeMode;
			SDL_GetDesktopDisplayMode(0, &nativeMode);

			//m_width = nativeMode.w;
			//m_height = nativeMode.h;

			//SDL_SetWindowDisplayMode(m_window, &nativeMode);
		}

		SDL_SetWindowSize(m_window, m_width, m_height);
		SDL_SetWindowFullscreen(m_window, p_fullscreen);
		//SDL_SetWindowSize(m_window, m_width, m_height);

		glViewport(0, 0, m_width, m_height);

		m_gbuffer.Resize(m_width, m_height);
		m_lighting.Resize(m_width, m_height);
		m_glowDevice.Resize(m_width, m_height);

		// Resize forward framebuffers
		glBindFramebuffer(GL_FRAMEBUFFER, m_forwardFramebuffers[0]);
		m_forwardColors[0]->CreateEmptyTexture(m_width, m_height, TextureFormat::TEXTURE_RGBA);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_forwardColors[0]->GetHandle(), 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, m_forwardFramebuffers[1]);
		m_forwardColors[1]->CreateEmptyTexture(m_width, m_height, TextureFormat::TEXTURE_RGBA);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_forwardColors[1]->GetHandle(), 0);
		m_forwardDepth[1]->CreateEmptyTexture(m_width, m_height, TextureFormat::TEXTURE_DEPTH_STENCIL);
		m_forwardDepth[1]->SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		m_forwardDepth[1]->SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		m_forwardDepth[1]->SetParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		m_forwardDepth[1]->SetParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_forwardDepth[1]->GetHandle(), 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void GLRenderer::AddShadowJob(Render::ShadowJob& p_shadowJob)
	{
		m_shadowDevice.m_shadowJobs.push_back(new (m_shadowDevice.m_shadowJobAllocator.Alloc(sizeof(ShadowJob))) ShadowJob(p_shadowJob));
	}

	void GLRenderer::AddRenderJob(RenderJob& p_job)
	{
		RenderJob* job = new (m_allocator.Alloc(sizeof(RenderJob))) RenderJob(p_job);

		if((job->m_material->m_effect->GetTechniques().at(0)->m_flags & TechniqueFlags::RENDER_DEFERRED1) == TechniqueFlags::RENDER_DEFERRED1 || job->m_forward)
		{
			
			m_forwardJobs.push_back(job);
		}
		else
			m_jobs.push_back(job);
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
			PROFILE("Sorting", g_context.m_profiler);
			Sorting();
		}

		{
			PROFILE("Shadow pass", g_context.m_profiler);
			ShadowPass();
		}

		// Buffer Per Frame data.
		m_cameraVars.m_invViewProj = glm::inverse(m_cameraVars.m_projection * m_cameraVars.m_view);
		m_cameraBuffer->BufferSubData(0, sizeof(m_cameraVars), &m_cameraVars);

		// Buffer Lights data.
		m_lighting.BufferLights();

		glEnable(GL_STENCIL_TEST);
		m_gbuffer.UnbindTextures();	
		m_gbuffer.Enable();
		m_gbuffer.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		{
			PROFILE("Deferred Pass", g_context.m_profiler);
			GeometryPass(0);
		}
	
		{
			PROFILE("Lighting Pass", g_context.m_profiler);
			LightingPass(0);	
		}

		m_lighting.ResetLights();
		glDisable(GL_STENCIL_TEST);

		{
			PROFILE("PostProcess Pass", g_context.m_profiler);
			PostProcessPass();
		}

		{
			PROFILE("Forward Pass", g_context.m_profiler);
			ForwardPass();
		}

		{
			PROFILE("Render Lines", g_context.m_profiler);
			m_lineRenderer.RenderLines();
		}

		{
			PROFILE("Output", g_context.m_profiler);
			Output();
		}

		// Deconstruct the job batch.
		for(auto itr = m_jobs.begin(); itr != m_jobs.end(); ++itr)
		{
			(*itr)->~RenderJob();
		}

		for(auto itr = m_forwardJobs.begin(); itr != m_forwardJobs.end(); ++itr)
		{
			(*itr)->~RenderJob();
		}

		m_allocator.Clear();
		m_jobs.clear();
		m_forwardJobs.clear();
	}

	void GLRenderer::Clear()
	{
		glDepthMask(GL_TRUE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GLRenderer::Swap()
	{
		SDL_GL_SwapWindow(m_window);
	}

	static bool SortRenderJobs(RenderJob* a, RenderJob* b)
	{
		  if( a->m_renderPass < b->m_renderPass ) return true;
		  if( a->m_renderPass == b->m_renderPass ) return a->m_material->m_id < b->m_material->m_id;
		  return false;
	};

	static bool SortForwardRenderJobs(RenderJob* a, RenderJob* b)
	{
		if( a->m_renderPass < b->m_renderPass ) return true;
		return false;
	};

	static bool FindWaterJob(RenderJob* a)
	{
		if( a->m_renderPass == 3 ) return true;
		return false;
	};

	void GLRenderer::Sorting()
	{	
		//Sort deferred jobs based on material
		std::sort(m_jobs.begin(), m_jobs.end(), SortRenderJobs);

		//Sort forward jobs base on height compared to the water level
		std::sort(m_forwardJobs.begin(), m_forwardJobs.end(), SortForwardRenderJobs);

		if(m_forwardJobs.size() > 1)
		{
			//Look for water job
			auto waterItr = std::find_if(m_forwardJobs.begin(), m_forwardJobs.end(), FindWaterJob);
			
			//If no water, sort all jobs by distance to camera as normal
			if(waterItr == m_forwardJobs.end()) 
			{
				std::sort(m_forwardJobs.begin(), m_forwardJobs.end(), SortOnDistanceFunctor(*this));
				return;
			}
			//If there is a water job, check to see if it's the first or last job(Either everything is above or everything is beneath)
			if( waterItr == m_forwardJobs.begin() ) //First job(everything is over)
			{
				std::sort(m_forwardJobs.begin() + 1 , m_forwardJobs.end(), SortOnDistanceFunctor(*this));
			}
			else if( waterItr == m_forwardJobs.end() - 1 ) //Last job(everything is under)
			{
				std::sort(m_forwardJobs.begin(), m_forwardJobs.end() - 1, SortOnDistanceFunctor(*this));
			}
			else
			{
				//There are object both under and above the water, sort each side individually
				std::sort(m_forwardJobs.begin(), waterItr - 1, SortOnDistanceFunctor(*this));
				std::sort(waterItr + 1, m_forwardJobs.end(), SortOnDistanceFunctor(*this));
			}
		}
	}

	void GLRenderer::ShadowPass()
	{
		glCullFace(GL_FRONT);
		glViewport(0, 0, m_shadowDevice.GetWidth(), m_shadowDevice.GetHeight());

		m_shadowDevice.Process();
	
		glCullFace(GL_BACK);
		glViewport(0, 0, m_width, m_height);
	}

	void GLRenderer::GeometryPass(int p_layer)
	{
		m_gbuffer.UnbindTextures();	
		m_gbuffer.Enable();
		m_gbuffer.Clear(GL_STENCIL_BUFFER_BIT);

		glStencilFunc(GL_ALWAYS, 127, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilMask(0xFF);

		m_renderFlags = Render::TechniqueFlags::RENDER_DEFERRED0;

		ProcessRenderJobs(m_jobs);
	}

	void GLRenderer::ProcessRenderJobs(std::vector<RenderJob*>& p_jobs)
	{
		int currentMaterialID = -1;
		for(auto job = p_jobs.begin(); job != p_jobs.end(); ++job)
		{
			if((*job)->m_material->m_id != currentMaterialID)
			{
				UnbindTexture(TextureSemantic::DIFFUSE);
				UnbindTexture(TextureSemantic::SPECULAR);
				UnbindTexture(TextureSemantic::NORMAL);
				UnbindTexture(TextureSemantic::GLOW);

				for(auto texture = (*job)->m_material->m_textures.begin(); texture != (*job)->m_material->m_textures.end(); ++texture)
				{
					if((*texture).second != nullptr)
						(*texture).second->Bind(s_textureSlots[(*texture).first]);
				}

				currentMaterialID = (*job)->m_material->m_id;
			}

			(*job)->m_mesh->Bind();
			
			// Itterate techniques.
			for(auto tech = (*job)->m_material->m_effect->GetTechniques().begin(); tech != (*job)->m_material->m_effect->GetTechniques().end(); ++tech)
			{
				if((m_renderFlags & (*tech)->m_flags) == m_renderFlags)
				{
					(*tech)->Apply();

					// Buffer uniforms.
					for(auto param = (*job)->m_params.begin(); param != (*job)->m_params.end(); ++param)
					{	
						m_uniforms->BufferSubData((*tech)->m_uniformsParams[param->first], s_sizes[param->first], param->second);
					}

					// Itterate programs.
					for(auto program = (*tech)->GetPrograms().begin(); program != (*tech)->GetPrograms().end(); ++program)
					{
						(*program)->Apply();

						(*job)->m_mesh->Draw();						
					}
				}
			}
		
			(*job)->m_mesh->Unbind();
		}
	}

	void GLRenderer::LightingPass(int p_layer)
	{
		// Bind textures for read.
		m_gbuffer.BindTextures();

		// Bind cascade shadow map array.
		glActiveTexture(GL_TEXTURE0 + s_textureSlots[TextureSemantic::SHADOWDEPTHPCF]);
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_shadowDevice.m_depthTextureArray);
		glBindSampler(s_textureSlots[TextureSemantic::SHADOWDEPTHPCF], m_shadowDevice.m_samplerObjectPCF);
		glActiveTexture(GL_TEXTURE0 + s_textureSlots[TextureSemantic::SHADOWDEPTH]);
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_shadowDevice.m_depthTextureArray);
		glBindSampler(s_textureSlots[TextureSemantic::SHADOWDEPTH], m_shadowDevice.m_samplerObjectFloat);

		static glm::mat4 biasMatrix(
			0.5, 0.0, 0.0, 0.0, 
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.5, 0.5, 0.5, 1.0
			);

		for(int i = 0; i < RENDER_SHADOW_CASCADES; i++)
		{
			// Buffer LightVP.
			glm::mat4 lvp = biasMatrix * m_shadowDevice.m_shadowcasters[0].m_projectionMatrices[i] * m_shadowDevice.m_shadowcasters[0].m_viewMatrices[i];
			m_uniforms->BufferSubData(i * sizeof(glm::mat4), sizeof(glm::mat4), &lvp);
		}

		m_lighting.SSAO();
		m_lighting.Clear();
		m_lighting.Ambient();
		m_lighting.Directional();
		m_lighting.Point();
		m_lighting.BackgroundBlend(BackgroundBlend::ADDITIVE);

		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	}

	void GLRenderer::ForwardPass()
	{
		if(m_showForward)
		{
			m_renderFlags = Render::TechniqueFlags::RENDER_DEFERRED1;

			int currentMaterialID = -1;
			for(auto job = m_forwardJobs.begin(); job != m_forwardJobs.end(); ++job)
			{
				if((*job)->m_material->m_id != currentMaterialID)
				{
					UnbindTexture(TextureSemantic::DIFFUSE);
					UnbindTexture(TextureSemantic::SPECULAR);
					UnbindTexture(TextureSemantic::NORMAL);
					UnbindTexture(TextureSemantic::GLOW);

					for(auto texture = (*job)->m_material->m_textures.begin(); texture != (*job)->m_material->m_textures.end(); ++texture)
					{
						if((*texture).second != nullptr)
							(*texture).second->Bind(s_textureSlots[(*texture).first]);
					}

					currentMaterialID = (*job)->m_material->m_id;
				}

				
				// Itterate techniques.
				for(auto tech = (*job)->m_material->m_effect->GetTechniques().begin(); tech != (*job)->m_material->m_effect->GetTechniques().end(); ++tech)
				{
					if((m_renderFlags & (*tech)->m_flags) == m_renderFlags)
					{
						// Swap framebuffer if render job is refractive.
						if(((*tech)->m_flags & TechniqueFlags::REFRACTIVE) == TechniqueFlags::REFRACTIVE)
						{
							BindForwardDepthAndColor();	
							SwapForwardFramebuffer();
							CopyDepthAndColor();	
							BindForwardFramebuffer();

							m_fullscreenQuadTech->GetPrograms()[0]->Apply();
							m_fullscreenQuad.Bind();
							m_fullscreenQuad.Draw();
							m_fullscreenQuad.Unbind();
						}

						(*job)->m_mesh->Bind();

						(*tech)->Apply();

						// Buffer uniforms.
						for(auto param = (*job)->m_params.begin(); param != (*job)->m_params.end(); ++param)
						{	
							m_uniforms->BufferSubData((*tech)->m_uniformsParams[param->first], s_sizes[param->first], param->second);
						}

						// Itterate programs.
						for(auto program = (*tech)->GetPrograms().begin(); program != (*tech)->GetPrograms().end(); ++program)
						{
							(*program)->Apply();

							(*job)->m_mesh->Draw();						
						}
					}
				}

				(*job)->m_mesh->Unbind();
			}
		}
	}

	void GLRenderer::PostProcessPass()
	{
		// Clear framebuffer 1.
		m_activeForwardFramebuffer = 1;
		BindForwardFramebuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Bind framebuffer 0.
		m_activeForwardFramebuffer = 0;
		BindForwardFramebuffer();

		// Bind GBuffer for read.
		m_gbuffer.BindTextures();

		// Bind LA.
		m_lighting.m_la->Bind(s_textureSlots[TextureSemantic::LIGHTING_ACCUMULATION]);

		// Output LA to framebuffer 0.
		m_fullscreenQuadTech->GetPrograms()[0]->Apply();
		m_fullscreenQuad.Bind();
		m_fullscreenQuad.Draw();
		m_fullscreenQuad.Unbind();

		// Bind framebuffer 0 for read.
		BindForwardDepthAndColor();

		// Do Glow pass.
		m_fullscreenQuad.Bind();
		m_glowDevice.HorizontalPass(&m_fullscreenQuad);
		BindForwardFramebuffer();
		m_glowDevice.VerticalPass(&m_fullscreenQuad);
		m_fullscreenQuad.Unbind();
	}

	void GLRenderer::Output()
	{
		// Bind active framebuffer for read.
		BindForwardDepthAndColor();

		// Restore backbuffer.
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		m_fullscreenQuadTech->GetPrograms()[0]->Apply();

		m_fullscreenQuad.Bind();
		m_fullscreenQuad.Draw();
		m_fullscreenQuad.Unbind();
	}

	void GLRenderer::SwapForwardFramebuffer()
	{
		m_activeForwardFramebuffer = (m_activeForwardFramebuffer == 1) ? 0 : 1;
	}

	void GLRenderer::BindForwardFramebuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_forwardFramebuffers[m_activeForwardFramebuffer]);

		GLenum buffers[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, buffers);		
	}

	void GLRenderer::BindForwardDepthAndColor()
	{	
		m_forwardColors[m_activeForwardFramebuffer]->Bind(s_textureSlots[TextureSemantic::FORWARD_COLOR]);
		m_forwardDepth[m_activeForwardFramebuffer]->Bind(s_textureSlots[TextureSemantic::DEPTH]);
	}

	void GLRenderer::CopyDepthAndColor()
	{
		m_forwardDepth[m_activeForwardFramebuffer]->Bind(0);

		int halfWidth = m_width / 2;
		int halfHeight = m_height / 2;

		//glBindTexture(GL_TEXTURE_2D, m_forwardDepth[m_activeForwardFramebuffer]->GetHandle());
		glCopyTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, 0, 0, halfWidth, halfHeight );
		glCopyTexSubImage2D( GL_TEXTURE_2D, 0, 0, halfHeight, 0, halfHeight, halfWidth, halfHeight);
		glCopyTexSubImage2D( GL_TEXTURE_2D, 0, halfWidth, 0, halfWidth, 0, halfWidth, halfHeight );
		glCopyTexSubImage2D( GL_TEXTURE_2D, 0, halfWidth, halfHeight, halfWidth, halfHeight, halfWidth, halfHeight);

		//glCopyTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, 0, 0, m_width, m_height, 0);
		//glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, 0, 0, m_width, m_height, 0);

		//glReadBuffer(GL_COLOR_ATTACHMENT0);
		//glBindTexture(GL_TEXTURE_2D, m_forwardColors[m_activeForwardFramebuffer]->GetHandle());
		//glCopyTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, 0, 0, m_width, m_height );
		//glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 0, 0, m_width, m_height, 0);
		
	}

	void GLRenderer::UnbindTexture(TextureSemantic::TextureSemantic p_semantic)
	{
		glActiveTexture(GL_TEXTURE0 + s_textureSlots[p_semantic]);
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

	void GLRenderer::SetCameraPosition( glm::vec3 p_camPos )
	{
		m_camPos = p_camPos;
	}

	const glm::mat4& GLRenderer::GetProjectionMatrix()
	{
		return m_cameraVars.m_projection;
	}

	const glm::mat4& GLRenderer::GetViewMatrix()
	{
		return m_cameraVars.m_view;
	}

	void GLRenderer::GetResourceUsage(int& p_bufferUsage, int& p_textureUsage, int& p_numBuffers, int& p_numTextures)
	{
		m_resources.PrintResourceUsage(p_bufferUsage, p_textureUsage, p_numBuffers, p_numTextures);
	}

	BufferInterface* GLRenderer::CreateBuffer(GLenum p_type)
	{ 
		return m_resources.CreateBuffer(p_type);
	}

	void GLRenderer::ReleaseBuffer(BufferInterface* p_buffer)
	{
		m_resources.ReleaseBuffer(p_buffer);
	}

	TextureInterface* GLRenderer::CreateTexture()
	{
		return m_resources.CreateTexture();
	}

	void GLRenderer::ReleaseTexture(TextureInterface* p_texture)
	{
		m_resources.ReleaseTexture(p_texture);
	}

	Material* GLRenderer::CreateMaterial(const std::string& p_name)
	{
		return m_resources.CreateMaterial(p_name);
	}

	VertexAttributesInterface* GLRenderer::CreateVertexAttributes()
	{ 
		return m_resources.CreateVertexAttributes();
	}

	MeshInterface* GLRenderer::CreateMesh() 
	{ 
		return m_resources.CreateMesh();
	}

	EffectInterface* GLRenderer::CreateEffect() 
	{ 
		return m_resources.CreateEffect();
	}

	std::string GLRenderer::GetStringFromMaterial(Material* p_material)
	{
		return m_resources.GetStringFromMaterial(p_material);
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
#ifdef RENDER_USE_COMPUTE
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
			//g_context.m_profiler->BeginGPUTimer();
			glDispatchCompute(p_job->m_groupDim.x, p_job->m_groupDim.y, p_job->m_groupDim.z);
			glMemoryBarrier(GL_ALL_BARRIER_BITS);
			//double time = g_context.m_profiler->EndGPUTimer();
			//g_context.m_logger->LogText(LogTag::RENDER, LogLevel::DEBUG_PRINT, "Compute time: %f ms", time);
		}

		for(auto texture = p_job->m_textures.begin(); texture != p_job->m_textures.end(); ++texture)
		{
			if((*texture).second != nullptr)
			{
				(*texture).second->Unbind((*texture).first);
				(*texture).second->UnBindImage((*texture).first);
			}
		}
#endif
	}

	void GLRenderer::ClearJobs()
	{
		// Deconstruct the job batch.
		for(auto itr = m_jobs.begin(); itr != m_jobs.end(); ++itr)
		{
			(*itr)->~RenderJob();
		}

		for(auto itr = m_forwardJobs.begin(); itr != m_forwardJobs.end(); ++itr)
		{
			(*itr)->~RenderJob();
		}

		m_allocator.Clear();
		m_jobs.clear();
		m_forwardJobs.clear();

		m_shadowDevice.Clear();
	}

	void GLRenderer::CleanResources(unsigned p_flag)
	{
		m_resources.Clean(p_flag);
	}

	void GLRenderer::RemoveMesh(MeshInterface* p_mesh)
	{
		m_resources.RemoveMesh(p_mesh);
	}

	void GLRenderer::RemoveVAO(VertexAttributesInterface* p_vao)
	{
		m_resources.RemoveVAO(p_vao);
	}

	void GLRenderer::FreeParticleSystem( ParticleSystemInterface* p_particleSys )
	{
		m_particles.Free((ParticleSystem*)p_particleSys);
	}

	DirectionalLight* GLRenderer::GetDirectionalLight()
	{
		return m_lighting.GetDirectionalLight();
	}
}

Render::RendererInterface* CreateRenderer(RootEngine::SubsystemSharedContext p_context)
{
	Render::g_context = p_context;

	return new Render::GLRenderer;
}
