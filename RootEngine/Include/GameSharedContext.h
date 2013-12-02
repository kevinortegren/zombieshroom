#pragma once

#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>
#include <RootEngine/Render/Include/Renderer.h>

#ifndef COMPILE_LEVEL_EDITOR
#include <RootEngine/Network/Include/NetworkManager.h>
#include <RootEngine/GUI/Include/guiInstance.h>
#include <RootEngine/Physics/Include/RootPhysics.h>
#include <RootEngine/InputManager/Include/InputManager.h>
#endif


namespace RootEngine
{
	struct GameSharedContext
	{
		Logging* m_logger;
		MemoryTracker* m_memTracker;
		ResourceManager* m_resourceManager;
		Render::RendererInterface* m_renderer;

#ifndef COMPILE_LEVEL_EDITOR
		Network::NetworkInterface* m_network;
		GUISystem::GUISystemInterface* m_gui;
		InputManager::InputInterface* m_inputSys;
		Physics::PhysicsInterface* m_physics;
#endif

	};
}