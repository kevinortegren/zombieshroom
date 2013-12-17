#pragma once

#include <RootSystems\Include\Transform.h>
#include <RootSystems\Include\RenderingSystem.h>
#include <RootSystems\Include\LightSystem.h>
#include <RootSystems\Include\CameraSystem.h>

#ifndef COMPILE_LEVEL_EDITOR
#include <RootSystems\Include\PhysicsSystem.h>
#include <RootSystems\Include\ScriptSystem.h>
#include <RootSystems\Include\PlayerControlSystem.h>
#include <RootSystems\Include\PlayerSystem.h>
#include <RootSystems\Include\CollisionSystem.h>
#include <RootSystems\Include\Network\ClientMessageSystem.h>
#include <RootSystems\Include\Network\ServerMessageSystem.h>
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
			PLAYER,
			PLAYERCONTROL,
			PHYSICS,
			NETWORK,
			NETWORKCLIENT,
			LOOKATBEHAVIOR,
			THIRDPERSONBEHAVIOR,
			SCRIPT,
			COLLISION,
			COLLISIONRESPONDER
		};
	}
}