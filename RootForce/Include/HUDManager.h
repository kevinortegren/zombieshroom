#pragma once

#include <RootForce/Include/HUD.h>
#include <RootForce/Include/IngameMenu.h>
#include <RootSystems/Include/MatchStateSystem.h>

namespace RootForce
{
	class HUDManager : public MatchStateListener
	{
	public:
		HUDManager(bool& p_displayIngameMenu);

		void SetHUD(HUD* p_hud);
		void SetMatchStateSystem(MatchStateSystem* p_matchStateSystem);
		void SetIngameMenu(IngameMenu* p_ingameMenu);

		void UpdateHUD();

		void MatchStateChanged(MatchState p_newState, MatchState p_previousState);
		void PlayerKilled(KillAnnouncement* p_killAnnouncement, ECS::Entity* p_murderer, ECS::Entity* p_victim);
	private:
		bool& m_displayIngameMenu;
		MatchStateSystem* m_matchStateSystem;
		HUD* m_hud;
		IngameMenu* m_ingameMenu;
	};
}