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

	for(auto itr = m_activeEntities.begin(); itr != m_activeEntities.end(); ++itr)
	{
		assert((*itr)->GetId() != -1 && "-1 Entity in processing loop.");
		ProcessEntity((*itr));
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
		for(auto itr = m_entitiesToRemove.begin(); itr != m_entitiesToRemove.end(); ++itr)
		{
			m_activeEntities.erase((*itr));
		}
		m_entitiesToRemove.clear();

		Begin();

		for(auto itr = m_activeEntities.begin(); itr != m_activeEntities.end(); ++itr)
		{
			assert((*itr)->GetId() != -1 && "-1 Entity in processing loop.");
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
				assert((*itr)->GetId() != -1 && "-1 Entity in processing loop.");
				ProcessEntity((*itr));
			}

			End();

			m_run = false;
			std::cout << "Setting run = false (1)" << std::endl;
		}
		else
		{
			Sleep(10);
		}
	}

	m_run = false;
	std::cout << "Setting run = false (2)" << std::endl;
}

bool ECS::ConcurrentSystem::IsRunning()
{
	return m_run;
}

void ECS::ConcurrentSystem::Run()
{
	m_run = true;
	std::cout << "Setting run = true" << std::endl;
}

void ECS::ConcurrentSystem::Start()
{
	m_terminate = false;
	m_thread = std::thread(&ECS::ConcurrentSystem::Process, this);
}

void ECS::ConcurrentSystem::Terminate()
{
	std::cout << "Setting terminate = true" << std::endl;
	m_terminate = true;
	m_thread.join();
	std::cout << "Threads joined" << std::endl;
}

void ECS::ConcurrentSystem::Synch()
{
	while(m_run);
}


