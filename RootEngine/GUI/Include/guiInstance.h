#pragma once

#include <RootEngine/Include/SubsystemSharedContext.h>
#include <Awesomium/WebCore.h>
#include <Awesomium/BitmapSurface.h>
#include <string>
#include <RootEngine/Render/Include/Effect.h>

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
			virtual void SetWindow(int p_width, int p_height) = 0;
			virtual void Update() = 0;
			virtual void Render() = 0;
			virtual void LoadURL(std::string p_path) = 0;
			virtual void SetWorkingDir(std::string p_path) = 0;
			virtual void SetRenderEffect(Render::EffectInterface* p_effect) = 0;
		};

		class guiInstance : public GUISystemInterface
		{
		public:
			void Startup();
			void Shutdown();

			void SetWindow(int p_width, int p_height);
			void Update();
			void Render();
			void LoadURL(std::string p_path);
			void SetWorkingDir(std::string p_path) { m_workingDir = p_path; }
			void SetRenderEffect(Render::EffectInterface* p_effect);
			

			static guiInstance* GetInstance();

		private:
			static guiInstance* s_gui;
			Awesomium::WebCore* m_core;
			Awesomium::WebView* m_view;

			std::string m_workingDir;
			int m_width, m_height;

			GLuint m_texture;
			Render::EffectInterface* m_effect;
			GLuint m_vertexArrayBuffer;

			void SurfaceToTexture(Awesomium::BitmapSurface* p_surface);
		};
	}

}

extern "C"
{
	typedef RootEngine::GUISystem::GUISystemInterface* (*CREATEGUI)(RootEngine::SubsystemSharedContext);
	SUBSYSTEM_DLL_EXPORT RootEngine::GUISystem::GUISystemInterface* CreateGUI(RootEngine::SubsystemSharedContext p_context);
}