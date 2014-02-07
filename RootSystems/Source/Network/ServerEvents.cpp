#include <RootSystems/Include/Network/ServerEvents.h>

#include <sstream>
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
}