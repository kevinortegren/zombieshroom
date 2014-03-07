#pragma once

#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/Network/NetworkComponents.h>
#include <RakNet/RakPeerInterface.h>

namespace RootForce
{
    struct StatChange : public ECS::Component<StatChange>
	{
		float SpeedChange;
		float JumpHeightChange;
		float DamageResistance;
		float KnockbackResistance;
		StatChange()
			: SpeedChange(1.0f),
			JumpHeightChange(1.0f),
			DamageResistance(1.0f),
			KnockbackResistance(1.0f)
		{}
	};
}