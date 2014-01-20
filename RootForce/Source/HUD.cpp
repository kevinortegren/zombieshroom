#include "HUD.h"
#include <Awesomium/STLHelpers.h>
namespace RootForce
{
	HUD::HUD()
	{
		m_chatSystem = std::shared_ptr<ChatSystem>(new ChatSystem());
	}

	HUD::~HUD()
	{
	}

	void HUD::Initialize( Awesomium::WebView* p_view, RootEngine::GUISystem::DispatcherInterface* p_dispatcher, RootEngine::GameSharedContext* p_engineContext )
	{
		m_view = p_view;
		m_chatSystem->Initialize(p_view, p_dispatcher, p_engineContext);

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

	void HUD::StartCooldown( int p_slot, int p_duration )
	{
		m_commandBuffer = m_commandBuffer + "StartCooldown('" + std::to_string(p_slot) + "'," + std::to_string(p_duration) + ");";
	}

	void HUD::SetSelectedAbility( int p_slot )
	{
		m_commandBuffer = m_commandBuffer + "SetAbilityFocus('" + std::to_string(p_slot) +  "');";
	}

	void HUD::SetAbility( int p_slot, std::string p_ability )
	{
		m_commandBuffer = m_commandBuffer + "SetAbility('" + std::to_string(p_slot) + "','" + p_ability + "');";
	}

	void HUD::Update()
	{

		m_view->ExecuteJavascript(Awesomium::WSLit(m_commandBuffer.c_str()), Awesomium::WSLit(""));
		m_commandBuffer = "";
	}

}