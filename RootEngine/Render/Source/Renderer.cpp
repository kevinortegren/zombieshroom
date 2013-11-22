
#include <GL/glew.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Render/Include/Renderer.h>
#include <RootEngine/Render/Include/Shader.h>
#include <GL/glew.h>
#include <iostream>

#if defined(_DEBUG) && defined(WIN32)
#include <windows.h>
void APIENTRY PrintOpenGLError(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* param) {
	printf("Message : %s\n", message);
}
#endif

namespace Render
{
	GLRenderer* GLRenderer::s_rendererInstance = nullptr;

	GLRenderer::GLRenderer()
	{
		
	}

	GLRenderer::~GLRenderer()
	{

	}

	GLRenderer* GLRenderer::GetInstance()
	{
		if(!s_rendererInstance)
		{
			s_rendererInstance = new GLRenderer();
		}

		return s_rendererInstance;
	}

	void GLRenderer::SetupSDLContext(SDL_Window* p_window)
	{
		int flags = SDL_GL_CONTEXT_PROFILE_CORE;
#if defined (_DEBUG)
		flags |= SDL_GL_CONTEXT_DEBUG_FLAG;
#endif

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, flags);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

		m_glContext = SDL_GL_CreateContext(p_window);
		if(!m_glContext) {
			std::cout << SDL_GetError();
			//Logging::GetInstance()->LogTextToConsole("%s", SDL_GetError());
		}

		SDL_GL_SetSwapInterval(0);

		int width, height;
		SDL_GetWindowSize(p_window, &width, &height);
		glViewport(0, 0, width, height);

		// Init GLEW.
		glewExperimental = GL_TRUE; 
		GLenum err = glewInit();
		if (err != GLEW_OK) {
			printf("Error init glew.\n");
			return;
		}

		GLint major, minor;
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);
		printf("OpenGL context version: %d.%d\n", major, minor);
		//Logging::GetInstance()->LogTextToConsole("Successfully opened OpenGL 4.4 context");

		glClearColor(0,0,0,1);
		glFrontFace(GL_CW);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);

#if defined(_DEBUG) && defined(WIN32)
		glDebugMessageCallback(PrintOpenGLError, NULL);
#endif

		printf("Available video memory: %i KB", GetAvailableVideoMemory());

		float data[] = {
			0.5f, 0.5f, 1.f, 0.f, 0.f, 1.f,
			-0.5f, 0.5f, 1.f, 0.f, 0.f, 1.f,
			-0.5f, -0.5f, 1.f, 0.f, 0.f, 1.f
		};

		int numVertices = 3;
		int size = 6 * sizeof(float);

		m_buffer.Init(2);
		m_buffer.SetVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)NULL);
		m_buffer.SetVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)NULL + (3 * sizeof(float)));
		m_buffer.BufferData(m_buffer.m_vbo, 3, size, data); 

		m_effect.CreateEffect();
		m_effect.AttachShader( GL_VERTEX_SHADER, "Assets/Shaders/genericVertex.glsl");
		m_effect.AttachShader( GL_VERTEX_SHADER, "Assets/Shaders/genericVertex.glsl");
		if(m_effect.Compile() != GL_TRUE)
			std::cout << "Couldnt compile shader." << std::endl;
		m_effect.Apply();
	
		m_effect.SetUniformMatrix( "normalMatrix", glm::mat3(1) );
		m_effect.SetUniformMatrix( "modelMatrix", glm::mat4(1) );
		m_effect.SetUniformMatrix( "viewMatrix", glm::mat4(1) );
		m_effect.SetUniformMatrix( "projectionMatrix", glm::mat4(1) );
	
		m_effect.SetUniformVector( "lightDirection", glm::vec3( 0.f, 0.f, -1.f ) );
		m_effect.SetUniformVector( "intensityDiffuse", glm::vec3( 1.f, 1.f, 1.f ) );
		m_effect.SetUniformVector( "coefficientDiffuse", glm::vec3( 1.f, 1.f, 1.f ) );

		m_window = p_window;
	}

	void GLRenderer::Render()
	{
		m_buffer.BindVertexArray();

		glDrawArrays( GL_TRIANGLES, 0, 3 * 6);

		m_buffer.UnbindVertexArray();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		SDL_GL_SwapWindow(m_window);
	}

	void GLRenderer::Cleanup()
	{
		SDL_GL_DeleteContext(m_glContext);
	}

	bool GLRenderer::CheckExtension(const char* p_extension)
	{
		if(glewIsExtensionSupported(p_extension) == GL_FALSE) {
			printf( "Missing OpenGL extension: %s\n", p_extension);
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

	RendererInterface* CreateRenderer()
	{
		return GLRenderer::GetInstance();
	}
}
