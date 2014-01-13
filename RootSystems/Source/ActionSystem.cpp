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

		RootForce::Transform* aimingDeviceTransform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_world->GetTagManager()->GetEntityByTag("AimingDevice"));

		RootForce::Transform* transform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(p_entity);
		RootForce::PlayerControl* controller = m_world->GetEntityManager()->GetComponent<RootForce::PlayerControl>(p_entity);
		RootForce::Collision* collision = m_world->GetEntityManager()->GetComponent<RootForce::Collision>(p_entity);
		RootForce::UserAbility* ability = m_world->GetEntityManager()->GetComponent<RootForce::UserAbility>(p_entity);
		RootForce::PlayerActionComponent* action = m_world->GetEntityManager()->GetComponent<RootForce::PlayerActionComponent>(p_entity);

		// Get the facing and calculate the right direction. Facing is assumed to be normalized, and up is assumed to be (0, 1, 0).
		glm::vec3 facing = transform->m_orientation.GetFront();
		glm::vec3 right = transform->m_orientation.GetRight();

		action->Angle.x = 0;

		// Get the speed of the player
		float speed = controller->m_speed;

		glm::vec3 movement = glm::normalize(facing * action->MovePower + right * action->StrafePower) * speed;
		m_engineContext->m_physics->SetPosition(*(collision->m_handle), movement  + transform->m_position);


		if(action->Jump)
			m_engineContext->m_physics->PlayerJump(*(collision->m_handle), controller->m_jumpForce);

		transform->m_orientation.YawGlobal(action->Angle.x);

		m_engineContext->m_physics->SetOrientation(*(collision->m_handle), transform->m_orientation.GetQuaternion());
	}

}