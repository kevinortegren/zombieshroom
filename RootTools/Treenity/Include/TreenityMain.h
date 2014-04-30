#pragma once

#include <Utility/ECS/Include/World.h>
#include <RootTools/Treenity/Include/EngineActions.h>

#include <RootEngine/Include/GameSharedContext.h>
RootEngine::GameSharedContext g_engineContext;

class TreenityMain
{
public:
	TreenityMain(const std::string& p_path);
	~TreenityMain();

	void HandleEvents();
	void Update(float dt);
	bool IsRunning();

private:
	void* m_engineModule;

	ECS::World m_world;
	Treenity m_treenityEditor;
	EngineActions m_engineActions;	
};