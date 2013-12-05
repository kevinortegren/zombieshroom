#include <RootEngine/Include/Profiling.h>

namespace RootEngine
{
	Profiling::Profiling()
		: m_time(0.0f)
	{

	}

	Profiling::~Profiling()
	{
		
	}

	void Profiling::Present()
	{
		m_time = 0.0f;
		//Clear output list
		m_ouputList.clear();
		m_ouputList.shrink_to_fit();

		for(auto itr = m_sampleMap.begin(); itr != m_sampleMap.end(); itr++)
		{
			__int64 collectedTime = 0;
			for (__int64 t : itr->second) 
			{
				collectedTime += t;
			}
			//Calculate average time
			__int64 averageTime = collectedTime/itr->second.size();
			//Create output string (Name + average time + number of samples)
			std::string output	= itr->first + ": " + std::to_string(averageTime) + "  Samples: " + std::to_string(itr->second.size());
			//Store output in vector for presenting 
			m_ouputList.push_back(output);

			//Clear collected time vector
			itr->second.clear();
			itr->second.shrink_to_fit();
		}
	}

	void Profiling::Update( float p_dt )
	{
		m_time += p_dt;
		if(m_time >= 1.0f)
			Present();

		for(std::string s : m_ouputList)
		{
			m_debugOverlay->AddHTML(s.c_str(), TextColor::GREEN, true);
		}
	}

	void Profiling::StoreSample( std::string p_name, __int64 p_elapsedTime )
	{
		m_sampleMap[p_name].push_back(p_elapsedTime);
	}

	void Profiling::SetDebugOverlay( DebugOverlayInterface* p_debugOverlay )
	{
		m_debugOverlay = p_debugOverlay;
	}

}
