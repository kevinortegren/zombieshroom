#include <RootSystems/Include/Network/ServerEvents.h>

#include <algorithm>

namespace RootServer
{
	EventData EventFromString(std::string p_string)
	{
		std::stringstream ss(p_string);
		EventData ev;

		std::getline(ss, ev.EventType, ' ');
		std::transform(ev.EventType.begin(), ev.EventType.end(), ev.EventType.begin(), ::toupper);
		std::swap(ev.Data, ss);

		return ev;
	}

	bool MatchAny(std::string p_input, unsigned p_numValues, ...)
	{
		va_list p_values;

		va_start(p_values, p_numValues);
		for(unsigned i = 0; i < p_numValues; ++i)
			if(p_input.compare(va_arg(p_values, char*)) == 0)
				return true;
		return false;
		va_end(p_values);
	}
}