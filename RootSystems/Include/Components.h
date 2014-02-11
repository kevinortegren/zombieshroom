#pragma once

#include <RootSystems/Include/ComponentTypes.h>
#include <RootSystems/Include/Transform.h>
#include <RootSystems/Include/RenderingSystem.h>
#include <RootSystems/Include/LightSystem.h>
#include <RootSystems/Include/CameraSystem.h>
#include <RootSystems/Include/PhysicsSystem.h>
#include <RootSystems/Include/ParticleSystem.h>
#include <RootSystems/Include/Script.h>
#include <RootSystems/Include/ShadowSystem.h>
#include <RootSystems/Include/PlayerControlSystem.h>
#include <RootSystems/Include/PlayerSystem.h>
#include <RootSystems/Include/Network/NetworkComponents.h>
#include <RootSystems/Include/CollisionSystem.h>
#include <RootSystems/Include/AnimationSystem.h>
#include <RootSystems/Include/MatchStateSystem.h>
#include <RootSystems/Include/RagdollSystem.h>
#include <RootSystems/Include/WaterSystem.h>
#include <RootSystems/Include/AbilityRespawnSystem.h>

namespace RootForce
{
	namespace ComponentType
	{
		void Initialize();
		void InitializeServerComponents();
	}
}