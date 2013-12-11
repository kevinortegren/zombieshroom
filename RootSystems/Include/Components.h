#pragma once

#include <RootSystems\Include\Transform.h>
#include <RootSystems\Include\RenderingSystem.h>
#include <RootSystems\Include\LightSystem.h>
#include <RootSystems\Include\PlayerControlSystem.h>
#include <RootSystems\Include\PlayerSystem.h>
#include <RootSystems\Include\PhysicsSystem.h>
#include <RootSystems\Include\Network\ClientMessageSystem.h>
#include <RootSystems\Include\Network\ServerMessageSystem.h>>
#include <RootSystems\Include\CameraSystem.h>

namespace RootForce
{
	namespace ComponentType
	{
		enum ComponentType
		{
			RENDERABLE,
			TRANSFORM,
			POINTLIGHT,
			PLAYER,
			PLAYERCONTROL,
			PHYSICS,
			NETWORK,
			NETWORKCLIENT,
			CAMERA,
			LOOKATBEHAVIOR,
			THIRDPERSONBEHAVIOR
		};
	}
}