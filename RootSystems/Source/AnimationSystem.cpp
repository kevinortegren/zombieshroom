#include <AnimationSystem.h>

namespace RootForce
{
	void AnimationSystem::Init()
	{
		m_animations.Init(m_world->GetEntityManager());
		m_renderables.Init(m_world->GetEntityManager());
	}

	void AnimationSystem::Begin()
	{
		int a = 0; //??
	}

	void AnimationSystem::ProcessEntity(ECS::Entity* p_entity)
	{
		m_time += m_world->GetDelta();

		if(m_logger)
		{
			Renderable* renderable = m_renderables.Get(p_entity);
			Animation* animation = m_animations.Get(p_entity);
			const aiScene* tempScene = renderable->m_model->m_animation->GetScene();
			glm::mat4 Identity = glm::mat4(1.0);
			
			float TicksPerSecond	= (float)(tempScene->mAnimations[animation->m_animClip]->mTicksPerSecond != 0 ? tempScene->mAnimations[animation->m_animClip]->mTicksPerSecond : 25.0f);
			float TimeInTicks		= m_time * TicksPerSecond;
			float AnimationTime		= fmod(TimeInTicks, (float)tempScene->mAnimations[animation->m_animClip]->mDuration);
		
			ReadNodeHeirarchy(AnimationTime, tempScene->mRootNode, Identity, animation, renderable, tempScene, animation->m_animClip);
		}
	}

	void AnimationSystem::ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform, Animation* p_anim, Renderable* p_render, const aiScene* p_aiScene, unsigned int p_animClip)
	{    
		std::string NodeName(pNode->mName.data);
		//std::cout << NodeName << "\n";
		if(NodeName == "Mesh2q" || NodeName == "Character1_Reference")
			return;

		const aiAnimation* pAnimation = p_aiScene->mAnimations[p_animClip];

		aiMatrix4x4 am = pNode->mTransformation;
		glm::mat4x4 gm = glm::mat4x4();
		memcpy(&gm[0][0], &am[0][0], sizeof(aiMatrix4x4));
		glm::mat4 NodeTransformation(glm::transpose(gm));

		const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

		if (pNodeAnim) 
		{
			/*
			// Interpolate scaling and generate scaling transformation matrix
			aiVector3D Scaling;
			CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
			glm::mat4 ScalingM = glm::scale(glm::mat4(1.0f), glm::vec3(Scaling.x, Scaling.y, Scaling.z));*/
			// Interpolate rotation and generate rotation transformation matrix
			aiQuaternion RotationQ;
			CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
			aiMatrix3x3 am3 = RotationQ.GetMatrix(); 
			glm::mat3 gm3 = glm::mat3();

			memcpy(&gm3[0][0], &am3[0][0], sizeof(aiMatrix3x3));

			glm::mat4 RotationM = glm::mat4(glm::transpose(gm3));

			// Interpolate translation and generate translation transformation matrix
			aiVector3D Translation;
			CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
			glm::mat4 TranslationM = glm::translate(glm::mat4(1.0f), glm::vec3(Translation.x, Translation.y, Translation.z));

			// Combine the above transformations
			NodeTransformation =  TranslationM * RotationM;
		}
		glm::mat4 GlobalTransformation = ParentTransform * NodeTransformation;

		if (p_render->m_model->m_animation->BoneExists(NodeName)) 
		{
			unsigned int BoneIndex = p_render->m_model->m_animation->GetIndexFromBoneName(NodeName);
			p_anim->m_bones[BoneIndex] = /*p_render->m_model->m_animation->GetGlobalInverseTransform() * */  GlobalTransformation * p_render->m_model->m_animation->GetBoneOffset(BoneIndex);
		}

		for (unsigned int i = 0 ; i < pNode->mNumChildren ; i++) 
		{
			ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation, p_anim, p_render, p_aiScene, p_animClip);
		}
	}

	unsigned int AnimationSystem::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{    
		for (unsigned int i = 0 ; i < pNodeAnim->mNumPositionKeys - 1 ; i++) 
		{
			if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) 
			{
				return i;
			}
		}

		assert(0);

		return 0;
	}


	unsigned int AnimationSystem::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		assert(pNodeAnim->mNumRotationKeys > 0);

		for (unsigned int i = 0 ; i < pNodeAnim->mNumRotationKeys - 1 ; i++) 
		{
			if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) 
			{
				return i;
			}
		}

		assert(0);

		return 0;
	}


	unsigned int AnimationSystem::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		assert(pNodeAnim->mNumScalingKeys > 0);

		for (unsigned int i = 0 ; i < pNodeAnim->mNumScalingKeys - 1 ; i++)
		{
			if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) 
			{
				return i;
			}
		}

		assert(0);

		return 0;
	}


	void AnimationSystem::CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		if (pNodeAnim->mNumPositionKeys == 1) 
		{
			Out = pNodeAnim->mPositionKeys[0].mValue;
			return;
		}

		unsigned int PositionIndex = FindPosition(AnimationTime, pNodeAnim);
		unsigned int NextPositionIndex = (PositionIndex + 1);
		assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
		float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
		float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
		assert(Factor >= 0.0f && Factor <= 1.0f);
		const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
		const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
		aiVector3D Delta = End - Start;
		Out = Start + Factor * Delta;
	}


	void AnimationSystem::CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		// we need at least two values to interpolate...
		if (pNodeAnim->mNumRotationKeys == 1) 
		{
			Out = pNodeAnim->mRotationKeys[0].mValue;
			return;
		}

		unsigned int RotationIndex = FindRotation(AnimationTime, pNodeAnim);
		unsigned int NextRotationIndex = (RotationIndex + 1);
		assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
		float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
		float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
		assert(Factor >= 0.0f && Factor <= 1.0f);
		const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
		const aiQuaternion& EndRotationQ   = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;    
		aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
		Out = Out.Normalize();
	}


	void AnimationSystem::CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		if (pNodeAnim->mNumScalingKeys == 1) 
		{
			Out = pNodeAnim->mScalingKeys[0].mValue;
			return;
		}

		unsigned int ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
		unsigned int NextScalingIndex = (ScalingIndex + 1);
		assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
		float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
		float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
		assert(Factor >= 0.0f && Factor <= 1.0f);
		const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
		const aiVector3D& End   = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
		aiVector3D Delta = End - Start;
		Out = Start + Factor * Delta;
	}
	const aiNodeAnim* AnimationSystem::FindNodeAnim(const aiAnimation* pAnimation, const std::string NodeName)
	{
		for (unsigned int i = 0 ; i < pAnimation->mNumChannels ; i++) 
		{
			const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

			if (std::string(pNodeAnim->mNodeName.data) == NodeName) 
			{
				return pNodeAnim;
			}
		}

		return NULL;
	}
	void AnimationSystem::End()
	{

	}

	void AnimationSystem::SetLoggingInterface(Logging::LoggingInterface* p_logger)
	{
		m_logger = p_logger;
	}

	void AnimationSystem::SetGameSharedContext( RootEngine::GameSharedContext* p_context )
	{
		m_context = p_context;
	}

}