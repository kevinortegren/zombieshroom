#pragma once
#include <RootForce/Include/Menu.h>
#include <RootForce/Include/Keymapper.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootSystems/Include/Network/Client.h>
#include <RootSystems/Include/Network/LanList.h>
#include <GameStates.h>

namespace RootForce
{
	/** Handles the game menu */
	class MenuState
	{
	public:
		MenuState(NetworkContext& p_networkContext);

		void Initialize(const std::string& p_workingDir);
		void Enter(Keymapper* p_keymapper);
		void Exit();
		GameStates::GameStates Update();

		const GameStates::PlayData& GetPlayData() const;
	private:
		NetworkContext& m_networkContext;
		std::shared_ptr<RootSystems::LanList> m_lanList;
		GameStates::PlayData m_playData;

		std::shared_ptr<Menu> m_menu;
		std::string m_workingDir;
	};
}