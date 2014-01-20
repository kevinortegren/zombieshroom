#include "ActionSystem.h"
#include <RootSystems/Include/Script.h>

extern RootEngine::GameSharedContext g_engineContext;

namespace RootSystems
{

	void ActionSystem::ProcessEntity( ECS::Entity* p_entity )
	{
		// Get the properties we need.
		float dt = m_world->GetDelta();

		RootForce::Transform* transform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(p_entity);
		RootForce::PlayerPhysics* playphys = m_world->GetEntityManager()->GetComponent<RootForce::PlayerPhysics>(p_entity);
		RootForce::Collision* collision = m_world->GetEntityManager()->GetComponent<RootForce::Collision>(p_entity);
		RootForce::Player* player = m_world->GetEntityManager()->GetComponent<RootForce::Player>(p_entity);
		RootForce::PlayerActionComponent* action = m_world->GetEntityManager()->GetComponent<RootForce::PlayerActionComponent>(p_entity);
		RootForce::HealthComponent* health = m_world->GetEntityManager()->GetComponent<RootForce::HealthComponent>(p_entity);

		if( !transform || !playphys || !collision || !player || !action || !health )
			return;

		if( health->IsDead )
		{
			if(action->ActivateAbility)
			{
				health->WantsRespawn = true;
				action->ActivateAbility = false;
				action->Jump = false;
			}

			return;
		}

		// Get the facing and calculate the right direction. Facing is assumed to be normalized, and up is assumed to be (0, 1, 0).
		glm::vec3 facing = transform->m_orientation.GetFront();
		glm::vec3 right = transform->m_orientation.GetRight();

		// Calculate movement vector based on input values, the player's speed
		glm::vec3 movement = facing * action->MovePower + right * action->StrafePower;
		if(movement != glm::vec3(0))
		{
			movement = glm::normalize(movement) * playphys->MovementSpeed;
			m_engineContext->m_physics->Move(*(collision->m_handle), movement + transform->m_position);
		}

		// Issue a jump if applicable
		if(action->Jump)
		{
			m_engineContext->m_physics->PlayerJump(*(collision->m_handle), playphys->JumpForce);
			action->Jump = false;
		}

		// Rotate the model and reset the angle
		transform->m_orientation.YawGlobal(action->Angle.x);
		action->Angle.x = 0;

		m_engineContext->m_physics->SetOrientation(*(collision->m_handle), transform->m_orientation.GetQuaternion());
		
		// Activate ability! Pew pew!
		player->SelectedAbility = action->SelectedAbility - 1;
		if(action->ActivateAbility)
		{
			action->ActivateAbility = false;

			// TODO: Generate action ID here
			m_engineContext->m_script->SetFunction(m_engineContext->m_resourceManager->GetScript(player->AbilityScripts[player->SelectedAbility]), "OnCreate");
			m_engineContext->m_script->ExecuteScript();
		}
	}

}