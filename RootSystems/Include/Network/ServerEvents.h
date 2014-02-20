#pragma once

#include <stdint.h>
#include <sstream>
#include <string>
#include <cstdarg>

namespace RootServer
{
	struct EventData
	{
		std::string EventType;
		std::stringstream Data;

		// Copy constructor and assignment operator needed for basic operations
		EventData() {}
		EventData( const EventData& other )
		{
			EventType = other.EventType;
			Data.clear();
			Data << other.Data.str();
		}
		void operator=( const EventData& other )
		{
			EventType = other.EventType;
			Data.clear();
			Data << other.Data.str();
		}
	};

	EventData EventFromString(std::string p_string);
	//Input is converted to CAPS
	bool MatchAny(std::string p_input, unsigned p_numValues, ...);
}