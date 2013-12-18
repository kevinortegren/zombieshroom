#include "Network/LanList.h"

#include <External/Include/SDL2/SDL_timer.h>
#include <algorithm>

namespace RootSystems
{
	void LanList::Start()
	{
		m_list.clear();
		m_pingStart = SDL_GetPerformanceCounter();
	}

	void LanList::AddServer(const RootSystems::ServerInfoInternal& p_server)
	{
		m_list.push_back(std::pair<uint64_t, RootSystems::ServerInfoInternal>(SDL_GetPerformanceCounter() - m_pingStart, p_server));
	}

	std::vector<std::pair<uint64_t,RootSystems::ServerInfoInternal>> LanList::GetList()
	{
		std::sort(m_list.begin(), m_list.end(), &LanList::ServerCompare);
		std::vector<std::pair<uint64_t,RootSystems::ServerInfoInternal>> tmp = m_list;
		m_list.clear();
		return tmp;
	}

	bool LanList::ServerCompare(std::pair<uint64_t,RootSystems::ServerInfoInternal>& first, std::pair<uint64_t,RootSystems::ServerInfoInternal>& second)
	{
		return first.first < second.first;
	}
}