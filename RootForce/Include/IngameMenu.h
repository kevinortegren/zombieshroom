#pragma once
#include <Awesomium/WebCore.h>
#include <Awesomium/STLHelpers.h>
#include <RootEngine/GUI/Include/Dispatcher.h>
#include <RootEngine/GUI/Include/WebView.h>
#include <RootSystems/Include/Network/ServerInfo.h>
#include <RootEngine/Include/ConfigManager.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootForce/Include/SettingsMenu.h>

namespace RootForce
{
	class IngameMenu
	{
	public:
		IngameMenu(RootEngine::GUISystem::WebView* p_view, RootEngine::GameSharedContext p_context, Keymapper* p_keymapper, ChatSystem* p_chatSystem);
		~IngameMenu();
		RootEngine::GUISystem::WebView* GetView() { return m_view; }
		bool GetReturn() { return m_return; }
		bool GetExit() { return m_exit; }
		void Reset();
		void Update();
		void SetScoreList(std::string p_score);
		void SetClientPeerInterface(RakNet::RakPeerInterface* p_clientPeer) { m_clientPeer = p_clientPeer; }
		SettingsMenu* GetSettingsMenu() { return m_settingsMenu; }

	private:
		void Return(const Awesomium::JSArray& p_array);
		void Exit(const Awesomium::JSArray& p_array);
		void ChangeTeam(const Awesomium::JSArray& p_array);
		void ChangeName(const Awesomium::JSArray& p_array);

		SettingsMenu* m_settingsMenu;
		RootEngine::GUISystem::WebView* m_view;

		bool m_return;
		bool m_exit;
		int m_changeTeam;
		bool m_changeName;

		RakNet::RakPeerInterface* m_clientPeer;
	};
}