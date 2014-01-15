#pragma once

#include <memory>
#include <string>
#include <SDL2/SDL.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <Utility/ECS/Include/World.h>

RootEngine::GameSharedContext g_engineContext;
ECS::World* g_world;

namespace RootParticle
{
	class MainParticle
	{
	public:
		MainParticle(std::string p_workingDirectory);
		~MainParticle();

		void Start();
	private:
		bool m_running;
		std::string m_workingDirectory;
		void* m_engineModule;
		std::shared_ptr<SDL_Window> m_window;

		ECS::World m_world;

		void HandleEvents();
	};
}

