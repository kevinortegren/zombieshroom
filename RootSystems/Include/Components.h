#pragma once

#include <RootSystems\Include\Transform.h>
#include <RootSystems\Include\RenderingSystem.h>
#include <RootSystems\Include\LightSystem.h>
#include <RootSystems\Include\CameraSystem.h>
#include <RootSystems\Include\PhysicsSystem.h>
#include <RootSystems\Include\ParticleSystem.h>

#ifndef COMPILE_LEVEL_EDITOR
#include <RootSystems\Include\ScriptSystem.h>
#include <RootSystems\Include\PlayerControlSystem.h>
#include <RootSystems\Include\PlayerSystem.h>
#include <RootSystems\Include\Network\NetworkComponents.h>
#include <RootSystems\Include\CollisionSystem.h>
#include <RootSystems\Include\AnimationSystem.h>
#endif

namespace RootForce
{
	namespace ComponentType
	{
		enum ComponentType
		{
			RENDERABLE,
			TRANSFORM,
			POINTLIGHT,
			CAMERA,
			HEALTH,
			PLAYERCONTROL,
			PHYSICS,
			NETWORK,
			NETWORKCLIENT,
			LOOKATBEHAVIOR,
			THIRDPERSONBEHAVIOR,
			SCRIPT,
			COLLISION,
			COLLISIONRESPONDER,
			ANIMATION,
			PARTICLE,
			SCORE,
			ABILITY,
			IDENTITY,
			TDMRULES, //TODO: name might be inappropriate?
			PLAYERACTION,

		};
	}
}