#include <ECS/ComponentSystem.h>

void ECS::ComponentSystem::Process()
{
	Begin();

	for(auto itr = m_activeEntities.begin(); itr != m_activeEntities.end(); ++itr)
	{
		ProcessEntity((*itr));
	}

	End();
}