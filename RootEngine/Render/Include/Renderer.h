#pragma once

#include "RenderInterface.h"

namespace Render
{

	class GLRenderer : public RendererInterface
	{
	public:
		static GLRenderer* GetInstance();

		void SetupSDLContext(SDL_Window* p_window);
		void Render();
		void Cleanup();


	private:
		GLRenderer();
		~GLRenderer();

		void SetAttributes();
		bool CheckExtension(const char* p_extension);
		int GetAvailableVideoMemory(); //Returns VRAM in kilobytes

		static GLRenderer* s_rendererInstance;
		SDL_GLContext m_glContext;
	};

	extern "C"
	{
		RENDERSYS_DLL_EXPORT RendererInterface* CreateRenderer();
	}
}
