#include "ActionSystem.h"
#include <RootSystems\Include\ScriptSystem.h>
#include <RootSystems/Include/Network/NetworkEntityMap.h>
namespace RootSystems
{


	/*void ActionSystem::Process()
	{
		ActionEvent event = m_frameActionQueue.back();
		m_frameActionQueue.pop();

		ECS::Entity entity =  m_networkContext->m_networkEntityMap->GetPlayerEntityFromUserID(event.UserID);
		RootForce::PlayerActionComponent* action =  m_world->GetEntityManager()->GetComponent<RootForce::PlayerActionComponent>(&entity);

		bool jump = false;
		switch(event.ActionType)
		{
		case RootForce::Network::MessageType::MessageType::UserCommandMoveForward:
			action->MovePower = 1;
			break;
		case RootForce::Network::MessageType::MessageType::UserCommandMoveBackward:
			action->MovePower = -1;
			break;
		case RootForce::Network::MessageType::MessageType::UserCommandMoveStop:
			action->MovePower = 0;
			break;
		case RootForce::Network::MessageType::MessageType::UserCommandStrafeLeft:
			action->StrafePower = -1;
			break;
		case RootForce::Network::MessageType::MessageType::UserCommandStrafeRight:
			action->StrafePower = 1;
			break;
		case RootForce::Network::MessageType::MessageType::UserCommandStrafeStop:
			action->StrafePower = 0;
			break;
		case RootForce::Network::MessageType::MessageType::UserCommandJump:
			action->Jump = true;
			break;
		};

		UpdatePhysics();
	}*/

	void ActionSystem::ProcessEntity( ECS::Entity* p_entity )
	{
		// Get the properties we need.
		float dt = m_world->GetDelta();

		RootForce::Transform* transform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(p_entity);
		RootForce::PlayerPhysics* playphys = m_world->GetEntityManager()->GetComponent<RootForce::PlayerPhysics>(p_entity);
		RootForce::Collision* collision = m_world->GetEntityManager()->GetComponent<RootForce::Collision>(p_entity);
		RootForce::UserAbility* ability = m_world->GetEntityManager()->GetComponent<RootForce::UserAbility>(p_entity);
		RootForce::PlayerActionComponent* action = m_world->GetEntityManager()->GetComponent<RootForce::PlayerActionComponent>(p_entity);

		// Get the facing and calculate the right direction. Facing is assumed to be normalized, and up is assumed to be (0, 1, 0).
		glm::vec3 facing = transform->m_orientation.GetFront();
		glm::vec3 right = transform->m_orientation.GetRight();

		action->Angle.x = 0;

		// Calculate movement vector based on input values, the player's speed and delta time
		glm::vec3 movement = glm::normalize(facing * action->MovePower + right * action->StrafePower) * playphys->MovementSpeed * dt;
		m_engineContext->m_physics->SetPosition(*(collision->m_handle), movement + transform->m_position);


		if(action->Jump)
			m_engineContext->m_physics->PlayerJump(*(collision->m_handle), playphys->JumpForce);

		transform->m_orientation.YawGlobal(action->Angle.x);

		m_engineContext->m_physics->SetOrientation(*(collision->m_handle), transform->m_orientation.GetQuaternion());
		
		if(action->ActivateAbility)
			switch(ability->SelectedAbility)
			{
			case RootForce::Ability::ABILITY_TEST:
				{
					ECS::Entity* entity = m_world->GetEntityManager()->CreateEntity();
					RootForce::Script* script = m_world->GetEntityManager()->CreateComponent<RootForce::Script>(entity);
					script->m_name = m_engineContext->m_resourceManager->GetScript("AbilityTest");
					script->m_actions.push_back(RootForce::Action(RootForce::ActionType::ACTION_CREATE));
				}
				break;
			default:
				break;
			}
	}

}