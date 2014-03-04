#pragma once
#include <RootSystems/Include/ChatSystem.h>
#include <memory>
#include <map>
namespace RootForce
{
	class HUD
	{
	public:
		HUD();
		~HUD();
		void Initialize(RootEngine::GUISystem::WebView* p_view, RootEngine::GameSharedContext* p_engineContext);
		std::shared_ptr<ChatSystem> GetChatSystem() { return m_chatSystem; }
		RootEngine::GUISystem::WebView* GetView() { return m_view; }

		//updates a single status value internally
		void SetValue(std::string p_key, std::string p_value);

		void SetCooldown(int p_slot, float p_duration);
		void SetCharges(int p_slot, int p_charges);
		void SetSelectedAbility(int p_slot);
		//Expects a slot and a name that corresponds with the filename of an ability icon(without .png)
		void SetAbility(int p_slot, std::string p_ability);
		
		//Updates the score list (on TAB or match end)
		void SetScoreList(std::string p_score);

		//Does one javascript inject for all the values and updates the hud
		void Update();
	private:
		RootEngine::GUISystem::WebView* m_view;
		std::shared_ptr<ChatSystem> m_chatSystem;
		std::string m_commandBuffer;
	};
}