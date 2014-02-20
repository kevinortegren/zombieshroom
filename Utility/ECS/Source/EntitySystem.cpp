#include <Utility\ECS\Include\EntitySystem.h>
#include <Utility\ECS\Include\World.h>
#include <vector>

void ECS::EntitySystem::Process()
{
	Begin();

	std::vector<ECS::Entity*> tempActiveEntities;
	tempActiveEntities.resize(m_activeEntities.size());
	tempActiveEntities.clear();
	for(auto itr = m_activeEntities.begin(); itr != m_activeEntities.end(); ++itr)
		tempActiveEntities.push_back(*itr);

	for(auto itr = tempActiveEntities.begin(); itr != tempActiveEntities.end(); ++itr)
		ProcessEntity((*itr));

	End();	
}

bool ECS::IntervalEntitySystem::CheckProcessing()
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

void ECS::IntervalEntitySystem::Process()
{
	if(CheckProcessing())
	{
		Begin();

		for(auto itr = m_activeEntities.begin(); itr != m_activeEntities.end(); ++itr)
		{
			if((*itr)->GetId() == -1)
				continue;
			ProcessEntity((*itr));
		}

		End();
	}
}

void ECS::ConcurrentSystem::Process()
{
	while(!m_terminate)
	{
		if(m_run)
		{
			Begin();

			for(auto itr = m_activeEntities.begin(); itr != m_activeEntities.end(); ++itr)
			{
				if((*itr)->GetId() == -1)
					continue;
				ProcessEntity((*itr));
			}

			End();

			m_run = false;
		}
	}
}

bool ECS::ConcurrentSystem::IsRunning()
{
	return m_run;
}

void ECS::ConcurrentSystem::Run()
{
	m_run = true;
}

void ECS::ConcurrentSystem::Start()
{
	m_terminate = false;
	m_thread = std::thread(&ECS::ConcurrentSystem::Process, this);
}

void ECS::ConcurrentSystem::Terminate()
{
	m_terminate = true;
	m_thread.join();
}

void ECS::ConcurrentSystem::Synch()
{
	while(m_run);
}


