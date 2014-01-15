#include <External/Include/gtest/gtest.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <Utility/ECS/Include/World.h>

#include <RootForce/Include/ComponentExporter.h>
#include <RootForce/Include/ComponentImporter.h>

extern RootEngine::GameSharedContext g_engineContext;

inline ECS::World* CreateWorld()
{
	ECS::World* world = new ECS::World();

	// Setup the importer and exporter
	world->GetEntityImporter()->SetImporter(Importer);
	world->GetEntityExporter()->SetExporter(Exporter);

	return world;
}