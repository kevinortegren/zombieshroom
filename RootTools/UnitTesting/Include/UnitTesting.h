#include <External/Include/gtest/gtest.h>
#define GTEST_BREAK_ON_FAILURE 1
#define GTEST_CATCH_EXCEPTIONS 0
#include <RootEngine/Include/GameSharedContext.h>
#include <Utility/ECS/Include/World.h>

#include <RootForce/Include/ComponentExporter.h>
#include <RootForce/Include/ComponentImporter.h>

extern RootEngine::GameSharedContext g_engineContext;
extern ECS::World* g_world;
extern RootForce::Network::NetworkEntityMap g_networkEntityMap;

inline ECS::World* CreateWorld()
{
	ECS::World* world = new ECS::World();

	// Setup the importer and exporter
	world->GetEntityImporter()->SetImporter(Importer);
	world->GetEntityExporter()->SetExporter(Exporter);

	world->GetEntityManager()->GetAllocator()->CreateList<RootForce::Renderable>(100000);
	world->GetEntityManager()->GetAllocator()->CreateList<RootForce::Transform>(100000);
	world->GetEntityManager()->GetAllocator()->CreateList<RootForce::PointLight>(100000);
	world->GetEntityManager()->GetAllocator()->CreateList<RootForce::Camera>(10);
	world->GetEntityManager()->GetAllocator()->CreateList<RootForce::HealthComponent>(20);
	world->GetEntityManager()->GetAllocator()->CreateList<RootForce::PlayerControl>(20);
	world->GetEntityManager()->GetAllocator()->CreateList<RootForce::Physics>(1000);
	world->GetEntityManager()->GetAllocator()->CreateList<RootForce::Network::NetworkComponent>(100000);
	world->GetEntityManager()->GetAllocator()->CreateList<RootForce::LookAtBehavior>(100);
	world->GetEntityManager()->GetAllocator()->CreateList<RootForce::ThirdPersonBehavior>(10);
	world->GetEntityManager()->GetAllocator()->CreateList<RootForce::Script>(1000);
	world->GetEntityManager()->GetAllocator()->CreateList<RootForce::Collision>(1000);
	world->GetEntityManager()->GetAllocator()->CreateList<RootForce::CollisionResponder>(100000);
	world->GetEntityManager()->GetAllocator()->CreateList<RootForce::PlayerComponent>(20);
	world->GetEntityManager()->GetAllocator()->CreateList<RootForce::Animation>(100);
	world->GetEntityManager()->GetAllocator()->CreateList<RootForce::ParticleEmitter>(100);
	world->GetEntityManager()->GetAllocator()->CreateList<RootForce::TDMRuleSet>(100);
	world->GetEntityManager()->GetAllocator()->CreateList<RootForce::PlayerActionComponent>(100);
	world->GetEntityManager()->GetAllocator()->CreateList<RootForce::PlayerPhysics>(100);
	world->GetEntityManager()->GetAllocator()->CreateList<RootForce::StateComponent>(100);
	world->GetEntityManager()->GetAllocator()->CreateList<RootForce::Shadowcaster>(100);
	world->GetEntityManager()->GetAllocator()->CreateList<RootForce::DirectionalLight>(100);
	world->GetEntityManager()->GetAllocator()->CreateList<RootForce::Network::ClientComponent>(100);
	world->GetEntityManager()->GetAllocator()->CreateList<RootForce::Network::ServerInformationComponent>(100);
	world->GetEntityManager()->GetAllocator()->CreateList<RootForce::AbilitySpawnComponent>(1);

	return world;
}