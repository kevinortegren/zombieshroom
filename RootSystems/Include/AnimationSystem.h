#pragma once

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
	struct Animation : public ECS::Component<Animation>
	{
		Animation():m_animClip(0), m_animTime(0.0f){}
		glm::mat4 m_bones[20];
		unsigned int m_animClip;
		float m_animTime;
	};

	struct AnimationSystem : public ECS::EntitySystem
	{
		AnimationSystem(ECS::World* p_world)
			: ECS::EntitySystem(p_world), m_time(0)
		{
			SetUsage<Animation>();
			SetUsage<Renderable>();
			m_logger = nullptr;
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
		void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform, Animation* p_anim, Renderable* p_render, const aiScene* p_aiScene, unsigned int p_animClip );
		const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const std::string NodeName);
		unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
		unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
		unsigned int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
		void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
		void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
		void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);

		Logging::LoggingInterface* m_logger;
		RootEngine::GameSharedContext* m_context;

		float m_time;
	};
}