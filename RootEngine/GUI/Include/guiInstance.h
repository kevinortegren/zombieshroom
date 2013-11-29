#pragma once

#include "guiSysInterface.h"
#include "guiRenderInterface.h"
#include "guiFileInterface.h"
#include <Rocket/Core/Input.h>
#include <RootEngine/Include/SubsystemSharedContext.h>
#include <string>

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
			virtual void Initalize(float p_width, float p_height) = 0;

			//Attach a .rml document to the root context
			virtual std::shared_ptr<Rocket::Core::ElementDocument> AttachDocument(std::string p_path) = 0; 

			//Updates the elapsed time as well as updates and renders the gui context
			virtual void Update(float p_time) = 0;

			//Sets an already loaded rendering effect to the gui context
			virtual void SetEffect(Render::EffectInterface* p_effect) = 0;

			virtual void SetWorkingDir(std::string p_workingDir) = 0;

			virtual void LoadFont(std::string p_path) = 0;

			//Function for getting user input
			virtual void HandleEvent(SDL_Event p_event) = 0;

		};

		class guiInstance : public GUISystemInterface
		{
		public:
			void Startup();
			void Shutdown();
			void Initalize(float p_width, float p_height);

			std::shared_ptr<Rocket::Core::ElementDocument> AttachDocument(std::string p_path); 

			void Update(float p_time);

			void SetEffect(Render::EffectInterface* p_effect) { m_renderInterface->SetEffect(p_effect); }

			static guiInstance* GetInstance();

			void SetWorkingDir(std::string p_workingDir) { m_workingDir = p_workingDir; }

			void LoadFont(std::string p_path);

			void HandleEvent(SDL_Event p_event);

		private:
			void MouseMovement(glm::vec2 p_pos);
			void MouseButtonState(Rocket::Core::Input::KeyIdentifier p_buttonIndex, bool p_isDown);
			void KeyButtonState(Rocket::Core::Input::KeyIdentifier p_buttonIndex, bool p_isDown);

			Rocket::Core::Input::KeyIdentifier MapToRocket(SDL_Keycode p_key);

			guiSysInterface* m_sysInterface;
			guiRenderInterface* m_renderInterface;
			guiFileInterface* m_fileInterface;

			Rocket::Core::Context* m_rootContext;

			static guiInstance* s_gui;
			std::string m_workingDir;
		};
	}

}

extern "C"
{
	typedef RootEngine::GUISystem::GUISystemInterface* (*CREATEGUI)(RootEngine::SubsystemSharedContext);
	SUBSYSTEM_DLL_EXPORT RootEngine::GUISystem::GUISystemInterface* CreateGUI(RootEngine::SubsystemSharedContext p_context);
}