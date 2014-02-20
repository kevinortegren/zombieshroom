#pragma once
#include <RootEngine/Include/GameSharedContext.h>
#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/AnimationSystem.h>
#include <RootSystems/Include/RenderingSystem.h>
#include <RootSystems/Include/CollisionSystem.h>
#include <RootSystems/Include/Transform.h>
namespace RootForce
{
	struct Ragdoll : public ECS::Component<Ragdoll>
	{
		bool m_firstTime;
		int padding;
		Ragdoll()
		{
			m_firstTime = true;
		}
		//Empty for now, might need stuff later
	};

	struct RagdollSystem : public ECS::EntitySystem
	{
	public:
		RagdollSystem(ECS::World* p_world, RootEngine::GameSharedContext* p_engineContext)
			: ECS::EntitySystem(p_world) , m_engineContext(p_engineContext)
		{
			SetUsage<Collision>();
			SetUsage<Ragdoll>();
			SetUsage<Animation>();
			SetUsage<Renderable>();
			SetUsage<Transform>();
		}
		~RagdollSystem();
		void Init();
		void Begin();
		void ProcessEntity(ECS::Entity* p_entity);
		void End();
		//void SetLoggingInterface(Logging::LoggingInterface* p_logger);

		ECS::ComponentMapper<Animation> m_animations;
		ECS::ComponentMapper<Ragdoll> m_ragdoll;
		ECS::ComponentMapper<Renderable> m_renderables;
		ECS::ComponentMapper<Collision> m_collisions;
		ECS::ComponentMapper<Transform> m_transforms;

	private:
		void NameMapper( std::map<std::string, int> * p_nameToIndex, aiNode* p_rootNode, RootEngine::RootAnimation::AnimationInterface* p_anim, const aiScene* p_aiScene);
		bool RagdollSystem::DoesAnimNodeExist(const aiAnimation* pAnimation, const std::string p_nodeName);
		RootEngine::GameSharedContext* m_engineContext;
	};
}