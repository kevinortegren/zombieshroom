#include "IngameMenu.h"
#include <Utility/ECS/Include/World.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootEngine/Script/Include/RootScript.h>
#include <RakNet/GetTime.h>
#include <RootSystems/Include/Network/NetworkComponents.h>

extern RootEngine::GameSharedContext g_engineContext;
extern ECS::World* g_world;

namespace RootForce
{

	IngameMenu::IngameMenu( RootEngine::GUISystem::WebView* p_view, RootEngine::GameSharedContext p_context, Keymapper* p_keymapper )
	{
		m_return = false;
		m_exit = false;

		m_view = p_view;
		m_settingsMenu = new SettingsMenu(p_context, p_keymapper);
		p_keymapper->SetMenu(m_view);

		m_view->RegisterJSCallback("Exit", JSDelegate1(this, &IngameMenu::Exit));
		m_view->RegisterJSCallback("Return", JSDelegate1(this, &IngameMenu::Return));
		m_view->RegisterJSCallback("SelectTeam", JSDelegate1(this, &IngameMenu::ChangeTeam));

		m_settingsMenu->BindEvents(m_view);
		m_view->Focus();
	}

	IngameMenu::~IngameMenu()
	{
		delete m_settingsMenu;
	}

	void IngameMenu::Return( const Awesomium::JSArray& p_array )
	{
		m_return = true;
	}

	void IngameMenu::Exit( const Awesomium::JSArray& p_array )
	{
		m_exit = true;
	}
	void IngameMenu::ChangeTeam(const Awesomium::JSArray& p_array)
	{
		if(p_array[0].IsInteger())
		{
			m_changeTeam = p_array[0].ToInteger();
		}
	}

	void IngameMenu::Reset()
	{
		m_return = false;
		m_exit = false;
	}

	void IngameMenu::Update()
	{
		m_settingsMenu->Update();

		if(m_changeTeam >= 0)
		{
			if(m_clientPeer != nullptr)
			{
				Network::NetworkComponent* network = g_world->GetEntityManager()->GetComponent<Network::NetworkComponent>(g_world->GetTagManager()->GetEntityByTag("Player"));
				RootForce::NetworkMessage::PlayerTeamSelect m;
				m.TeamID = m_changeTeam;
				m.UserID = network->ID;

				RakNet::BitStream bs;
				bs.Write((RakNet::MessageID) ID_TIMESTAMP);
				bs.Write(RakNet::GetTime());
				bs.Write((RakNet::MessageID) RootForce::NetworkMessage::MessageType::AbilityClaimedBy);
				m.Serialize(true, &bs);

				m_clientPeer->Send(&bs, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
			}

			m_return = true;
			m_changeTeam = -1;
		}
	}

}

