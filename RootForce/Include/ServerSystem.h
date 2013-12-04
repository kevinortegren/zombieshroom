#pragma once

#include <Utility/ECS/Include/EntitySystem.h>

namespace RootForce
{
	class ServerSystem : public ECS::EntitySystem
	{
	public:
		ServerSystem(uint16_t p_serverPort);
		ServerSystem(const char* p_address, uint16_t p_port);

		void Init();
		void Begin();
		void ProcessEntity(ECS::Entity* p_entity);
		void End();
	};
}