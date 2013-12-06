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
			__int64 maxTime = 0;
			for (__int64 t : itr->second) 
			{
				collectedTime += t;
				if(t > maxTime)
					maxTime = t;
			}
			//Calculate average time
			__int64 qfreq;
			QueryPerformanceFrequency((LARGE_INTEGER*)&qfreq);
			double averageSample	= (collectedTime/itr->second.size()) / (double)qfreq;
			double maxSample		= maxTime / (double)qfreq;
			std::string output	= "<div style='min-width: 200px; display: inline-block;'>" + itr->first + " :</div>" + "Time: " + std::to_string(averageSample) + " <div style='color: #FFFFFF; display: inline-block;'> MaxTime: "+ std::to_string(maxSample) + "</div> Samples: " + std::to_string(itr->second.size());
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
