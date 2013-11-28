#include <ECS/ComponentSystem.h>

void ECS::ComponentSystem::Process(float dt)
{
	Begin();

	for(auto itr = m_activeEntities.begin(); itr != m_activeEntities.end(); ++itr)
	{
		ProcessEntity((*itr), dt);
	}

	End();
}