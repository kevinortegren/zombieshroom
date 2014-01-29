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
		void Initialize(Awesomium::WebView* p_view, RootEngine::GUISystem::DispatcherInterface* p_dispatcher, RootEngine::GameSharedContext* p_engineContext);
		std::shared_ptr<ChatSystem> GetChatSystem() { return m_chatSystem; }
		Awesomium::WebView* GetView() { return m_view; }

		//updates a single status value internally
		void SetValue(std::string p_key, std::string p_value);

		void StartCooldown(int p_slot, float p_duration);
		void SetSelectedAbility(int p_slot);
		//Expects a slot and a name that corresponds with the filename of an ability icon(without .png)
		void SetAbility(int p_slot, std::string p_ability);

		//Does one javascript inject for all the values and updates the hud
		void Update();
	private:
		Awesomium::WebView* m_view;
		std::shared_ptr<ChatSystem> m_chatSystem;
		std::string m_commandBuffer;
	};
}