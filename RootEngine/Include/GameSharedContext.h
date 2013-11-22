#pragma once

#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Render/Include/Renderer.h>
#include <RootEngine/Network/Include/NetworkManager.h>

namespace RootEngine
{
	struct GameSharedContext
	{
		Logging* m_logger;
		Render::GLRenderer* m_renderer;
		Network::NetworkManager* m_network;
	};
}