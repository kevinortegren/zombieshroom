#ifndef COMPILE_LEVEL_EDITOR
#include "TryPickupResetSystem.h"

namespace RootForce
{
	void TryPickupResetSystem::Init()
	{
		m_tryPickupComponents.Init(m_world->GetEntityManager());
	}

	void TryPickupResetSystem::ProcessEntity( ECS::Entity* p_entity )
	{
		TryPickupComponent* tryPickup = m_tryPickupComponents.Get(p_entity);
		tryPickup->TryPickup = false;
	}
}
#endif