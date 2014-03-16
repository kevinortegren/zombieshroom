#ifndef COMPILE_LEVEL_EDITOR
#include "RagdollSystem.h"
#include <RootEngine/Physics/Include/RootPhysics.h>
#include <RootSystems/Include/CameraSystem.h>
namespace RootForce
{
	void RagdollSystem::Init()
	{
		m_animations.Init(m_world->GetEntityManager());
		m_renderables.Init(m_world->GetEntityManager());
		m_collisions.Init(m_world->GetEntityManager());
		m_transforms.Init(m_world->GetEntityManager());
		
	}

	void RagdollSystem::Begin()
	{

	}

	void RagdollSystem::ProcessEntity( ECS::Entity* p_entity )
	{
		Animation* animation = m_animations.Get(p_entity);
		Renderable* renderable = m_renderables.Get(p_entity);
		Collision* collision = m_collisions.Get(p_entity);
		Transform* transform = m_transforms.Get(p_entity);
		Ragdoll* ragdoll = m_world->GetEntityManager()->GetComponent<Ragdoll>(p_entity);
		ECS::Entity* camera = m_world->GetTagManager()->GetEntityByTag("Camera");
		
		if(animation->UpperBodyAnim.m_animClip == AnimationClip::RAGDOLL && animation->LowerBodyAnim.m_animClip == AnimationClip::RAGDOLL)
		{
			
			if(ragdoll->m_firstTime)
			{	
				//Lock the camera if the local player are the entity entering ragdoll mode
				if(m_world->GetTagManager()->GetEntityByTag("Player") == p_entity)
				{
					ThirdPersonBehavior* cameraThird = m_world->GetEntityManager()->GetComponent<ThirdPersonBehavior>(camera);
					if(cameraThird)
					{
						cameraThird->m_rotateWithTarget = false;
					}
				}

				std::map<std::string, int>  nameToIndex ;
				NameMapper(&nameToIndex,renderable->m_model->m_animation->GetScene()->mRootNode, renderable->m_model->m_animation, renderable->m_model->m_animation->GetScene() );
				glm::mat4 bones[20], boneOffset[20];
				for(unsigned int i = 0; i < nameToIndex.size() ; i++ )
				{
					bones[i] = animation->m_bones[i];
					
					boneOffset[i] = renderable->m_model->m_animation[0].GetBoneOffset(i);
				}
				//Starts the ragdoll in physics
				m_engineContext->m_physics->BuildRagdoll(*(collision->m_handle), bones, renderable->m_model->m_animation->GetScene()->mRootNode, nameToIndex, boneOffset, transform->m_orientation.GetRight());
				
				ragdoll->m_firstTime = false;
				
			}
			else
			{
				
				glm::mat4* bones;
				//get the updated bonetransformations from the physics
				bones = m_engineContext->m_physics->GetBones(*(collision->m_handle));
				if(bones != nullptr)
				{
					for(int i = 0; i < 20; i++)
					{
						animation->m_bones[i] = bones[i];/** (renderable->m_model->m_animation[0].GetBoneOffset(i))*/;
					}
					delete[] bones;
				}
				
			}
		
		}
		else
		{
			//If the client gets "pushed" back to alive state from the server we need to deactivate the ragdoll, otherwise bad stuff might happen
			if(m_engineContext->m_physics->IsRagdoll(*(collision->m_handle)))
			{
				m_engineContext->m_physics->DeactivateRagdoll(*(collision->m_handle));
				transform->m_orientation.SetOrientation(m_engineContext->m_physics->GetOrientation(*(collision->m_handle)));		
			}
			//Restart rotations following if the local player leaving ragdoll mode
			if(m_world->GetTagManager()->GetEntityByTag("Player") == p_entity)
			{
				ThirdPersonBehavior* cameraThird = m_world->GetEntityManager()->GetComponent<ThirdPersonBehavior>(camera);
				if(cameraThird)
				{
					cameraThird->m_rotateWithTarget = true;
				}
			}
			ragdoll->m_firstTime = true;
			return;
		}
		
	}

	void RagdollSystem::End()
	{

	}

	void RagdollSystem::NameMapper( std::map<std::string, int>  *p_nameToIndex, aiNode* p_rootNode, RootEngine::RootAnimation::AnimationInterface* p_anim, const aiScene* p_aiScene)
	{
		
		if (p_anim->BoneExists(p_rootNode->mName.C_Str())) 
		{
			int index  = p_anim->GetIndexFromBoneName(p_rootNode->mName.C_Str());
			(*p_nameToIndex)[p_rootNode->mName.C_Str()] = index; //Here be magic
		}

		for(unsigned int i = 0; i < p_rootNode->mNumChildren; i++)
			NameMapper(p_nameToIndex, p_rootNode->mChildren[i], p_anim, p_aiScene);
	}

	bool RagdollSystem::DoesAnimNodeExist(const aiAnimation* pAnimation, const std::string p_nodeName)
	{
		if(p_nodeName.compare("Character1_Head") == 0 )
			return false;
		for (unsigned int i = 0 ; i < pAnimation->mNumChannels ; i++) 
		{
			const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

			if (std::string(pNodeAnim->mNodeName.data) ==  p_nodeName) 
			{
				return true;
			}
		}

		return false;
	}

}

#endif
