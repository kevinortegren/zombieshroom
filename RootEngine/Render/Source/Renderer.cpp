
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

	void GLRenderer::SetAttributes()
	{
		int flags = SDL_GL_CONTEXT_PROFILE_CORE;
#if defined (_DEBUG)
		flags |= SDL_GL_CONTEXT_DEBUG_FLAG;
#endif
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, flags);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
	}

	void GLRenderer::SetupSDLContext(SDL_Window* p_window)
	{
		SetAttributes();

		m_glContext = SDL_GL_CreateContext(p_window);
		if(!m_glContext) {
			std::cout << SDL_GetError();
			//Logging::GetInstance()->LogTextToConsole("%s", SDL_GetError());
		}
		glewInit();
		SDL_GL_SetSwapInterval(0);

		int width, height;
		SDL_GetWindowSize(p_window, &width, &height);
		glViewport(0, 0, width, height);

		glClearColor(0,0,0,1);
		glFrontFace(GL_CW);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
#if defined(_DEBUG) && defined(WIN32)
		if(CheckExtension("KHR_debug"))
			glDebugMessageCallback(PrintOpenGLError, NULL);
		CheckExtension("GL_ARB_texture_compression");
#endif

		glClear(GL_COLOR_BUFFER_BIT);
		SDL_GL_SwapWindow(p_window);

		GLint major, minor;
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);
		printf("OpenGL context version: %d.%d\n", major, minor);
		//Logging::GetInstance()->LogTextToConsole("Successfully opened OpenGL 4.4 context");

		printf("Available video memory: %i KB\n", GetAvailableVideoMemory());

		Render::Shader myshader;
		myshader.LoadFromFile(GL_GEOMETRY_SHADER, "bajs");
	}

	void GLRenderer::Render()
	{
			
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
