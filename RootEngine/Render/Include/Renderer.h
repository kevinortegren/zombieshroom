#pragma once

#include <SDL2/SDL.h>

namespace Engine
{
	namespace Renderer
	{

		class GLRenderer 
		{
		public:
			static GLRenderer* GetInstance();

			void SetupSDLContext(SDL_Window* p_window);

			void Render();
			void Cleanup();


		private:
			GLRenderer();
			~GLRenderer();

			static GLRenderer* s_rendererInstance;
			SDL_GLContext m_glContext;
		};
	}
}