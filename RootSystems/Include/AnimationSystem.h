#pragma once

#include <map>
#include <RootEngine/Include/ModelImporter.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/RenderingSystem.h>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>  
#include <glm/glm.hpp>
#include <RootEngine/Include/GameSharedContext.h>
namespace RootForce
{
	namespace AnimationClip
	{
		enum AnimationClip
		{
			WALKING,
			IDLE,
			STRAFE_LEFT,
			STRAFE_RIGHT,
			JUMP_START,
			ASCEND,
			DESCEND,
			LANDING,
			SHOOT,
			BACKWARDS,
			LEFTFORWARD,
			RIGHTFORWARD,
			LEFTBACK,
			RIGHTBACK,
			RAGDOLL
		};
	}

	struct Animation : public ECS::Component<Animation>
	{
		Animation():m_animClip(AnimationClip::WALKING), m_prevAnimClip(AnimationClip::WALKING), m_animTime(0.0f), m_blending(false), m_blendTime(0.0f), m_locked(0){}

		glm::mat4		m_bones[20];
		AnimationClip::AnimationClip	m_animClip;
		AnimationClip::AnimationClip	m_prevAnimClip;
		float			m_animTime;
		int				m_locked;
		bool			m_blending;
		float			m_blendTime;
		std::map<std::string, aiVector3D> m_blendPos;
		std::map<std::string, aiQuaternion> m_blendRot;
	};

	struct AnimationSystem : public ECS::ConcurrentSystem
	{
		AnimationSystem(ECS::World* p_world)
			: ECS::ConcurrentSystem(p_world)
		{
			SetUsage<Animation>();
			SetUsage<Renderable>();
			m_logger = nullptr;
			m_blendTime = 0.1f;
		}
		void Init();
		void Begin();
		void ProcessEntity(ECS::Entity* p_entity);
		void End();
		void SetLoggingInterface(Logging::LoggingInterface* p_logger);
		void SetGameSharedContext(RootEngine::GameSharedContext* p_context);

		ECS::ComponentMapper<Animation> m_animations;
		ECS::ComponentMapper<Renderable> m_renderables;

	private:
		void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform, Animation* p_anim, Renderable* p_render, const aiScene* p_aiScene);
		const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const std::string NodeName);
		unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
		unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
		unsigned int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
		void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim, Animation* p_anim);
		void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim, Animation* p_anim);
		void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);

		void CalcBlendedPosition(aiVector3D& Out, Animation* p_anim, const aiNodeAnim* pNodeAnim, unsigned int p_toKeyFrame);
		void CalcBlendedRotation(aiQuaternion& Out, Animation* p_anim, const aiNodeAnim* pNodeAnim, unsigned int p_toKeyFrame);

		Logging::LoggingInterface* m_logger;
		RootEngine::GameSharedContext* m_context;

		float m_blendTime;
	};
}