#include <ECS/ComponentSystem.h>
#include <ECS/World.h>

void ECS::ComponentSystem::Process()
{
	Begin();

	for(auto itr = m_activeEntities.begin(); itr != m_activeEntities.end(); ++itr)
	{
		ProcessEntity((*itr));
	}

	End();	
}

bool ECS::IntervalComponentSystem::CheckProcessing()
{
	float dt = m_world->GetDelta();
	m_time += dt;
	if(m_time >= m_interval)
	{
		m_time = 0.0f;
		return true;
	}
	return false;
}

void ECS::IntervalComponentSystem::Process()
{
	if(CheckProcessing())
	{
		Begin();

		for(auto itr = m_activeEntities.begin(); itr != m_activeEntities.end(); ++itr)
		{
			ProcessEntity((*itr));
		}

		End();
	}
}