#pragma once

#include <Utility/ECS/Include/EntitySystem.h>

namespace RootForce
{
	class ServerSystem : public ECS::EntitySystem
	{
	public:
		void Init();
		void Begin();
		void ProcessEntity(ECS::Entity* p_entity);
		void End();
	};
}