#include <ConsoleInput.h>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

namespace RootServer
{
	void ConsoleInput::Startup()
	{
		m_shouldExit = false;
		m_thread = new std::thread( [this](){ this->Process(); } );
	}

	void ConsoleInput::Shutdown()
	{
		m_shouldExit = true;
		m_thread->join();
	}

	EventData ConsoleInput::PollEvent()
	{
		if(m_eventBuffer.size() < 1)
			return EventData();
		EventData cmd = *(m_eventBuffer.end()-1);
		m_eventBuffer.pop_back();
		return cmd;
	}

	void ConsoleInput::Process()
	{
		while(!m_shouldExit)
		{
			std::string line;
			std::getline(std::cin, line);
			
			EventData ev = EventFromString(line);

			m_eventBuffer.push_back( ev );
			
			if(ev.EventType == UserCommands::SHUTDOWN)
			{
				m_shouldExit = true;
			}
		}
	}
}
