#pragma once
#include <vector>
#include <thread>

namespace RootServer
{
	class ConsoleInput
	{
	public:
		void Startup();
		void Shutdown();

		std::vector<std::string> PollCommand();
	private:
		std::vector<std::vector<std::string>> m_commandBuffer;
		std::thread* m_thread;
		bool m_shouldExit;

		void Process();
	};
}