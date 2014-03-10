#include <Windows.h>
#include <Utility\ECS\Include\EntitySystem.h>
#include <Utility\ECS\Include\World.h>
#include <vector>

void ECS::EntitySystem::Process()
{
	for(auto itr = m_entitiesToRemove.begin(); itr != m_entitiesToRemove.end(); ++itr)
	{
		m_activeEntities.erase((*itr));
	}
	m_entitiesToRemove.clear();

	Begin();

	for(auto itr = m_activeEntities.begin(); itr != m_activeEntities.end();)
	{
		if((*itr)->GetId() == -1)
		{
			itr = m_activeEntities.erase(itr);
		}
		else
		{
			ProcessEntity((*itr));
			itr++;
		}
	}

	End();	
}

bool ECS::IntervalEntitySystem::CheckProcessing()
{
	if(m_ticks >= m_ticksPerFrame)
		return false;

	float dt = m_world->GetDelta();
	m_time += dt;
	if(m_time >= m_interval)
	{
		m_time -= m_interval;
		m_ticks++;
		return true;
	}
	return false;
}

void ECS::IntervalEntitySystem::Process()
{
	while(CheckProcessing())
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

	m_ticks = 0;
}

float ECS::IntervalEntitySystem::GetSystemInterval()
{
	return m_interval;
}

void ECS::ConcurrentSystem::Process()
{
	while(!m_terminate)
	{
		if(m_run)
		{
			for(auto itr = m_entitiesToRemove.begin(); itr != m_entitiesToRemove.end(); ++itr)
			{
				m_activeEntities.erase((*itr));
			}
			m_entitiesToRemove.clear();

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
		else
		{
			Sleep(10);
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


