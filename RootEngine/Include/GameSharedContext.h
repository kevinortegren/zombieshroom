#pragma once

#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>
#include <RootEngine/Render/Include/Renderer.h>
#include <RootEngine/Network/Include/NetworkManager.h>
#include <RootEngine/GUI/Include/guiInstance.h>
#include <RootEngine/Physics/Include/RootPhysics.h>
#include <RootEngine/InputManager/Include/InputManager.h>


namespace RootEngine
{
	struct GameSharedContext
	{
		Logging* m_logger;
		MemoryTracker* m_memTracker;
		ResourceManager* m_resourceManager;
		Render::RendererInterface* m_renderer;
		Network::NetworkInterface* m_network;
		GUISystem::GUISystemInterface* m_gui;
		InputManager::InputInterface* m_inputSys;
		Physics::PhysicsInterface* m_physics;
	};
}