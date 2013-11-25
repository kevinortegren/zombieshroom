#pragma once

#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Render/Include/Renderer.h>
#include <RootEngine/Network/Include/NetworkManager.h>
#include <RootEngine/GUI/Include/guiInstance.h>
#include <RootEngine/Physics/Include/RootPhysics.h>

namespace RootEngine
{
	struct GameSharedContext
	{
		Logging* m_logger;
		MemoryTracker* m_memTracker;
		Render::RendererInterface* m_renderer;
		Network::NetworkInterface* m_network;
		GUISystem::GUISystemInterface* m_gui;
		Physics::PhysicsInterface* m_physics;
	};
}