#pragma once

#include <RootEngine/Include/SubsystemSharedContext.h>
#include <Awesomium/WebCore.h>
#include <Awesomium/BitmapSurface.h>
#include <Awesomium/WebViewListener.h>
#include <string>
#include <RootEngine/Render/Include/Effect.h>
#include <WinSock2.h>
#include <SDL2/SDL.h>
#include "gl_texture_surface.h"
#include "Dispatcher.h"
#include "WebView.h"
#include <thread>
#include <atomic>
#include <mutex>


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
			virtual void Initialize(int p_width, int p_height, SDL_Window* p_window, SDL_GLContext p_glContext) = 0;

			virtual void Update() = 0;
			virtual void Render(WebView* p_view) = 0;

			//loads a single .html file into the view new view and returns the view
			virtual WebView* LoadURL(std::string p_callbackObjectName, std::string p_path) = 0;
			virtual void DestroyView(WebView* p_view) = 0;

			virtual void SetWorkingDir(std::string p_path) = 0;

			virtual void HandleEvents(SDL_Event p_event) = 0;
			virtual void ResizeAllViews(int p_width, int p_height) = 0;


			virtual Dispatcher* GetDispatcher() = 0; 
		};

		class guiInstance : public GUISystemInterface
		{
		public:
			~guiInstance()
			{
				Shutdown();
			}
			void Startup();
			void Shutdown();

			void Initialize(int p_width, int p_height, SDL_Window* p_window, SDL_GLContext p_glContext);
			void Update();
			void Render(WebView* p_view);
			WebView* LoadURL(std::string p_callbackObjectName, std::string p_path);
			void DestroyView(WebView* p_view);
			void SetWorkingDir(std::string p_path) { m_workingDir = p_path; }
			void HandleEvents(SDL_Event p_event);
			void ResizeAllViews(int p_width, int p_height);

			static guiInstance* GetInstance();
			Dispatcher* GetDispatcher() { return m_dispatcher; }

		private:
			void RunThread();

			static guiInstance* s_gui;
			Awesomium::WebCore* m_core;

			std::string m_workingDir;
			int m_width, m_height;

			GLuint m_texture;
			std::shared_ptr<Render::ProgramInterface> m_program;
			GLuint m_vertexArrayBuffer;
			GLuint m_vertexBufferObject;
			GLTextureSurfaceFactory* m_glTexSurfaceFactory;

			GLuint SurfaceToTexture(GLTextureSurface* p_surface);
			int MapToAwesomium(SDL_Keycode p_key);
			int MapEventToAwesomium(SDL_Event p_event);
			Dispatcher* m_dispatcher;
			
			std::mutex m_drawMutex;
			std::mutex m_viewBufferMutex;
			std::vector<WebViewImpl*> m_viewBuffer;
			std::mutex m_loadListMutex;
			std::vector<std::pair<WebViewImpl*,Awesomium::WebURL>> m_loadList;
			std::atomic_bool m_shouldTerminate;
			std::thread m_thread;
			SDL_GLContext m_glContext;
			SDL_Window* m_window;
		};

		//Classes used for testing the gui document load
		class guiTest : public Awesomium::WebViewListener::Load
		{
		public:
			void OnDocumentReady(Awesomium::WebView* called, const Awesomium::WebURL& url);

			void OnBeginLoadingFrame (Awesomium::WebView *caller, int64 frame_id, bool is_main_frame, const Awesomium::WebURL &url, bool is_error_page){}

			void Awesomium::WebViewListener::Load::OnFailLoadingFrame 	( 	Awesomium::WebView *  	caller,
				int64  	frame_id,
				bool  	is_main_frame,
				const Awesomium::WebURL &  	url,
				int  	error_code,
				const Awesomium::WebString &  	error_desc 
				){}

			void Awesomium::WebViewListener::Load::OnFinishLoadingFrame 	( 	Awesomium::WebView *  	caller,
				int64  	frame_id,
				bool  	is_main_frame,
				const Awesomium::WebURL &  	url 
				){}
			void InitTest() { m_testBool = false; }
			bool GetTestResult() { return m_testBool; }
		private:
			bool m_testBool;
		};

		class guiJSTest : public Awesomium::JSMethodHandler
		{
		public:
			void OnMethodCall (Awesomium::WebView *caller, unsigned int remote_object_id, const Awesomium::WebString &method_name, const Awesomium::JSArray &args);
			Awesomium::JSValue 	OnMethodCallWithReturnValue (Awesomium::WebView *caller, unsigned int remote_object_id, const Awesomium::WebString &method_name, const Awesomium::JSArray &args){ return Awesomium::JSValue();}
			void InitTest() { m_testBool = false; }
			bool GetTestResult() { return m_testBool; }
		private:
			bool m_testBool;
		};
	}

}

extern "C"
{
	typedef RootEngine::GUISystem::GUISystemInterface* (*CREATEGUI)(RootEngine::SubsystemSharedContext);
	SUBSYSTEM_DLL_EXPORT RootEngine::GUISystem::GUISystemInterface* CreateGUI(RootEngine::SubsystemSharedContext p_context);
}