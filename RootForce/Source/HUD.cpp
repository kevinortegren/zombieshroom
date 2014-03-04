#include "HUD.h"
#include <Awesomium/STLHelpers.h>
#include <RootSystems/Include/PlayerSystem.h>
#include <RootSystems/Include/WorldSystem.h>

extern ECS::World* g_world;

namespace RootForce
{
	HUD::HUD()
	{
		m_chatSystem = std::shared_ptr<ChatSystem>(new ChatSystem());
	}

	HUD::~HUD()
	{
	}

	void HUD::Initialize( RootEngine::GUISystem::WebView* p_view, RootEngine::GameSharedContext* p_engineContext )
	{
		m_view = p_view;
		m_chatSystem->Initialize(p_view, p_engineContext);

		SetValue("PlayerScore", "0");
		SetValue("PlayerDeaths", "0");
		SetValue("TeamScore", "0");
		SetValue("EnemyScore", "0");
		SetValue("Health", "100");
		SetValue("TimeLeft", "00:00");
	}

	void HUD::SetValue( std::string p_key, std::string p_value )
	{
		m_commandBuffer = m_commandBuffer + "Set('" + p_key + "','" + p_value + "');";
	}

	void HUD::SetCooldown( int p_slot, float p_duration )
	{
		m_commandBuffer = m_commandBuffer + "SetCooldown('" + std::to_string(p_slot) + "'," + std::to_string(p_duration) + ");";
	}

	void HUD::SetSelectedAbility( int p_slot )
	{
		m_commandBuffer = m_commandBuffer + "SetAbilityFocus('" + std::to_string(p_slot) +  "');";
	}

	void HUD::SetAbility( int p_slot, std::string p_ability )
	{
		m_commandBuffer = m_commandBuffer + "SetAbility('" + std::to_string(p_slot) + "','" + p_ability + "');";
	}
	void HUD::SetScoreList(std::string p_score)
	{
		ECS::Entity* player = g_world->GetTagManager()->GetEntityByTag("Player");
		PlayerComponent* playerComponent = g_world->GetEntityManager()->GetComponent<PlayerComponent>(player);

		m_commandBuffer = m_commandBuffer + "UpdateScoreScreen(" + std::to_string(playerComponent->TeamID) + ",'" + playerComponent->Name + "'," + p_score + ");";
	}

	void HUD::Update()
	{
		m_view->BufferJavascript(m_commandBuffer);
		m_commandBuffer = "";
	}

	void HUD::SetCharges( int p_slot, int p_charges )
	{
		m_commandBuffer = m_commandBuffer + "SetCharges('" + std::to_string(p_slot) + "'," + std::to_string(p_charges) + ");";
	}

}