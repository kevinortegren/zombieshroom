#include <RootEngine/Include/GameSharedContext.h>
#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/AnimationSystem.h>
#include <RootSystems/Include/RenderingSystem.h>
#include <RootSystems/Include/CollisionSystem.h>
namespace RootForce
{
	struct Ragdoll : public ECS::Component<Ragdoll>
	{
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

	private:

		RootEngine::GameSharedContext* m_engineContext;
	};
}