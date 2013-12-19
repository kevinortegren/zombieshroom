#include <RootForce/Include/ConnectingState.h>

namespace RootForce
{
	ConnectingState::ConnectingState()
	{

	}

	void ConnectingState::Initialize(const GameStates::PlayData& playData)
	{
		if (playData.Host)
		{

		}
		else
		{

		}
	}

	GameStates::GameStates ConnectingState::Update()
	{
		return GameStates::Ingame;
	}
}