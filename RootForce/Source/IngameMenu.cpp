#include "IngameMenu.h"
#include <Utility/ECS/Include/World.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootEngine/Script/Include/RootScript.h>

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
			// TODO: Instead, send a message to server, handle team change on response
			ECS::Entity* player = g_world->GetTagManager()->GetEntityByTag("Player");
			
			// Call the OnCreate script
			g_engineContext.m_script->SetFunction(g_engineContext.m_resourceManager->GetScript("Player"), "OnTeamSelect");
			g_engineContext.m_script->AddParameterUserData(player, sizeof(ECS::Entity*), "Entity");
			g_engineContext.m_script->AddParameterNumber(m_changeTeam);
			g_engineContext.m_script->ExecuteScript();

			m_return = true;
			m_changeTeam = -1;
		}
	}

	void IngameMenu::SetScoreList(std::string p_score)
	{
		ECS::Entity* player = g_world->GetTagManager()->GetEntityByTag("Player");
		PlayerComponent* playerComponent = g_world->GetEntityManager()->GetComponent<PlayerComponent>(player);

		m_view->BufferJavascript("UpdateScoreScreen(" + std::to_string(playerComponent->TeamID) + ",'" + playerComponent->Name + "'," + p_score + ");");
	}

}

