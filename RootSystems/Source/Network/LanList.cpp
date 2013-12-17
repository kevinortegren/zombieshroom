#include "Network/LanList.h"

#include <External/Include/SDL2/SDL_timer.h>
#include <algorithm>

namespace RootSystems
{
	void LanList::Start()
	{
		for(int i = 0; i < m_list.size(); i++)
			delete m_list.at(i).second;
		m_list.clear();
		m_pingStart = SDL_GetPerformanceCounter();
	}
	void LanList::AddServer(RootSystems::ServerInfoInternal* p_server)
	{
		RootSystems::ServerInfoInternal* tmp = new RootSystems::ServerInfoInternal;
		memcpy(tmp, p_server, sizeof(RootSystems::ServerInfoInternal));
		m_list.push_back(std::pair<uint64_t,RootSystems::ServerInfoInternal*>(SDL_GetPerformanceCounter()-m_pingStart, tmp));
	}
	std::vector<std::pair<uint64_t,RootSystems::ServerInfoInternal*>> LanList::GetList()
	{
		std::sort(m_list.begin(), m_list.end(), &LanList::ServerCompare);
		std::vector<std::pair<uint64_t,RootSystems::ServerInfoInternal*>> tmp = m_list;
		m_list = std::vector<std::pair<uint64_t,RootSystems::ServerInfoInternal*>>();
		return tmp;
	}
	bool LanList::ServerCompare(std::pair<uint64_t,RootSystems::ServerInfoInternal*> &first, std::pair<uint64_t,RootSystems::ServerInfoInternal*> &second)
	{
		return first.first < second.first;
	}
}