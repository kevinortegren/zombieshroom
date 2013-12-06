#pragma once
#include <vector>
#include <thread>
#include "ServerEvents.h"

namespace RootServer
{
	class ConsoleInput
	{
	public:
		void Startup();
		void Shutdown();

		EventData PollEvent();
	private:
		std::vector<EventData> m_eventBuffer;
		std::thread* m_thread;
		bool m_shouldExit;

		void Process();
	};
}