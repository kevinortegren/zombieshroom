#pragma once

#include <Utility\ECS\Include\Component.h>
#include <vector>

namespace RootForce
{
	struct Collision : public ECS::Component<Collision>
	{
		int* m_handle;
	};

	struct CollisionRespond : public ECS::Component<CollisionRespond>
	{
		std::vector<unsigned int> m_collidedEntityId;
	};

	struct Physics : public ECS::Component<Physics>
	{
		// TODO: Add data.
		float m_mass;
	};

	

}