#pragma once

#include <RootEngine/Render/Include/RenderInterface.h>
#include <RootEngine/Render/Include/GraphicsBuffer.h>
#include <RootEngine/Render/Include/Effect.h>

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

		GraphicsBuffer m_buffer;
		Effect m_effect;
		SDL_Window* m_window;
	};

	extern "C"
	{
		RENDERSYS_DLL_EXPORT RendererInterface* CreateRenderer();
	}
}
