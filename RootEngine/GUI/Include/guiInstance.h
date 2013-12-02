#pragma once

#include <RootEngine/Include/SubsystemSharedContext.h>
#include <Awesomium/WebCore.h>
#include <Awesomium/BitmapSurface.h>
#include <string>
#include <RootEngine/Render/Include/Effect.h>
#include <SDL2/SDL.h>
#include "gl_texture_surface.h"

#if defined(_WINDLL)
    #define SUBSYSTEM_DLL_EXPORT __declspec(dllexport)
#else
    #define SUBSYSTEM_DLL_EXPORT __declspec(dllimport)
#endif

namespace RootEngine
{
	namespace GUISystem
	{
		class GUISystemInterface abstract : public RootEngine::SubsystemInterface
		{
		public:
			//Sets the window properties to the gui core
			virtual void Initialize(int p_width, int p_height) = 0;

			virtual void Update() = 0;
			virtual void Render() = 0;

			//loads a single .html file into the view
			virtual void LoadURL(std::string p_path) = 0;

			virtual void SetWorkingDir(std::string p_path) = 0;

			virtual void HandleEvents(SDL_Event p_event) = 0;

			//returns the webView that displayes the .html file
			virtual Awesomium::WebView* GetView() = 0;
		};

		class guiInstance : public GUISystemInterface
		{
		public:
			void Startup();
			void Shutdown();

			void Initialize(int p_width, int p_height);
			void Update();
			void Render();
			void LoadURL(std::string p_path);
			void SetWorkingDir(std::string p_path) { m_workingDir = p_path; }
			void HandleEvents(SDL_Event p_event);
			Awesomium::WebView* GetView() { return m_view; }

			static guiInstance* GetInstance();

		private:
			static guiInstance* s_gui;
			Awesomium::WebCore* m_core;
			Awesomium::WebView* m_view;

			std::string m_workingDir;
			int m_width, m_height;

			GLuint m_texture;
			std::shared_ptr<Render::ProgramInterface> m_program;
			GLuint m_vertexArrayBuffer;
			GLTextureSurfaceFactory* m_glTexSurfaceFactory;
			Awesomium::Surface* m_surface;

			void SurfaceToTexture(GLTextureSurface* p_surface);
			int MapToAwesomium(SDL_Keycode p_key);
			int MapEventToAwesomium(SDL_Event p_event);
		};
	}

}

extern "C"
{
	typedef RootEngine::GUISystem::GUISystemInterface* (*CREATEGUI)(RootEngine::SubsystemSharedContext);
	SUBSYSTEM_DLL_EXPORT RootEngine::GUISystem::GUISystemInterface* CreateGUI(RootEngine::SubsystemSharedContext p_context);
}