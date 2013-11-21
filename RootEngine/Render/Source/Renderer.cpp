#include <RootEngine/Render/Include/Renderer.h>
#include <GL/glew.h>

namespace Engine
{
	namespace Renderer
	{
		GLRenderer* GLRenderer::s_rendererInstance = nullptr;

		GLRenderer::GLRenderer()
		{
			glewInit();
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
			SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);


			m_glContext = SDL_GL_CreateContext(p_window);
			if(!m_glContext) {
				//Logging::GetInstance()->LogTextToConsole("%s", SDL_GetError());
			}
			SDL_GL_SetSwapInterval(0);
			int width, height;
			SDL_GetWindowSize(p_window, &width, &height);

			glViewport(0, 0, width, height);
			glClearColor(0,0,0,1);
			glClear(GL_COLOR_BUFFER_BIT);
			SDL_GL_SwapWindow(p_window);
			//Logging::GetInstance()->LogTextToConsole("Successfully opened OpenGL 4.4 context");
		}

		void GLRenderer::Render()
		{

		}

		void GLRenderer::Cleanup()
		{
			SDL_GL_DeleteContext(m_glContext);
		}
	}
}