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
			SHOOTLEFT1,
			BACKWARDS,
			LEFTFORWARD,
			RIGHTFORWARD,
			LEFTBACK,
			RIGHTBACK,
			CHARGING1,
			SHOOTDOUBLE1,
			SHOOTRIGHT1,
			SHOOTDOUBLE2,
			CHANNELINGSPHERE,
			DOUBLEOVERTHROW1,
			DOUBLEUNDERTHROW1,
			RIGHTUNDERTHROW1,
			CHARGING2,
			DASH,
			GOTHIT1,
			GOTHIT2,
			GOTHIT3,
			RAGDOLL,
			NOCLIP,
		};
	}

	struct AnimBodyPart
	{
		AnimBodyPart() : m_animClip(AnimationClip::WALKING), m_prevAnimClip(AnimationClip::WALKING), m_animTime(0.0f), m_blending(false), m_blendTime(0.0f), m_locked(0), m_channelingClip(AnimationClip::AnimationClip::NOCLIP), m_chargingClip(AnimationClip::AnimationClip::NOCLIP){}
		AnimationClip::AnimationClip		m_animClip;
		AnimationClip::AnimationClip		m_prevAnimClip;
		AnimationClip::AnimationClip		m_chargingClip;
		AnimationClip::AnimationClip		m_channelingClip;
		std::map<std::string, aiVector3D>	m_blendPos;
		std::map<std::string, aiQuaternion> m_blendRot;
		float	m_animTime;
		int		m_locked;
		bool	m_blending;
		float	m_blendTime;
	};

	struct Animation : public ECS::Component<Animation>
	{
		Animation(){}
		glm::mat4 m_bones[20];
		AnimBodyPart LowerBodyAnim;
		AnimBodyPart UpperBodyAnim;
	};

	struct AnimationSystem : public ECS::ConcurrentSystem
	{
		AnimationSystem(ECS::World* p_world)
			: ECS::ConcurrentSystem(p_world)
		{
			SetUsage<Animation>();
			SetUsage<Renderable>();
			m_logger = nullptr;
			m_blendTime = 0.15f;
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
		void UpdateUpperBodyAnimation(Renderable* p_renderable, Animation* p_animation, float p_ticksPerSecond );
		void UpdateLowerBodyAnimation(Renderable* p_renderable, Animation* p_animation, float p_ticksPerSecond );
		void ReadNodeHeirarchyLower(const aiNode* pNode, const glm::mat4& ParentTransform, Animation* p_anim, Renderable* p_render, const aiScene* p_aiScene);
		void ReadNodeHeirarchyUpper(const aiNode* pNode, const glm::mat4& ParentTransform, Animation* p_anim, Renderable* p_render, const aiScene* p_aiScene);

		void CalcUpperRootNodeRotation(const aiNode* pNode, Animation* p_anim, Renderable* p_render, const aiScene* p_aiScene);

		const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const std::string NodeName);
		unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
		unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);

		void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim, Animation* p_anim);
		void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim, Animation* p_anim);

		void CalcBlendedPosition(aiVector3D& Out, Animation* p_anim, const aiNodeAnim* pNodeAnim, unsigned int p_toKeyFrame, const aiVector3D& p_startPosInNewPose, float p_animBlendTime);
		void CalcBlendedRotation(aiQuaternion& Out, Animation* p_anim, const aiNodeAnim* pNodeAnim, unsigned int p_toKeyFrame, const  aiQuaternion& p_startRotInNewPose, float p_animBlendTime);

		Logging::LoggingInterface* m_logger;
		RootEngine::GameSharedContext* m_context;

		float m_blendTime;

		float m_lowerAnimTime;
		float m_upperAnimTime;

		glm::mat4 m_upperRootRotation;
	};
}