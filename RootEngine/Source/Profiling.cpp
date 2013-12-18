#include <RootEngine/Include/Profiling.h>

namespace RootEngine
{
	Profiling::Profiling()
		: m_time(0.0f), m_frames(0)
	{

	}

	Profiling::~Profiling()
	{
		
	}

	void Profiling::Present()
	{
		//Clear output list
		m_ouputList.clear();
		m_ouputList.shrink_to_fit();

		float averageFrameTime = (m_time/(float)m_frames)*1000.0f;
		double collectedPercentages = 0;
		std::string output	= "FPS: " + std::to_string(m_frames);
		m_ouputList.push_back(output);

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
			if(itr->second.size() == 0)
				continue;
			//Calculate average time
			__int64 qfreq;
			QueryPerformanceFrequency((LARGE_INTEGER*)&qfreq);
			double averageSample	= (collectedTime/itr->second.size()) / ((double)qfreq/1000.0f);
			double maxSample		= maxTime / ((double)qfreq/1000.0f);
			double percentage		= (averageSample/(double)averageFrameTime)*100;
			collectedPercentages	+= percentage;
			std::string output		= "<div style='min-width: 180px; display: inline-block;'>" + itr->first + " :</div>" + "T: " + std::to_string(averageSample) + " <div style='color: #FFFFFF; display: inline-block;'> MaxT: "+ std::to_string(maxSample) + "</div> S: " + std::to_string(itr->second.size()) + " <div style='color: #FFFFFF; display: inline-block;'> P: "+ std::to_string(percentage) + "% </div>";

			//Store output in vector for presenting 
			m_ouputList.push_back(output);

			//Clear collected time vector
			itr->second.clear();
			itr->second.shrink_to_fit();
		}

		std::string otherOutput	= "Other: " + std::to_string(100 - collectedPercentages) + "%";
		m_ouputList.push_back(otherOutput);

	}

	void Profiling::Update( float p_dt )
	{
		m_time += p_dt;
		m_frames += 1;

		if(m_time >= 1.0f)
		{
			Present();
			m_frames = 0;
			m_time = 0;
		}
#ifndef COMPILE_LEVEL_EDITOR
		for(std::string s : m_ouputList)
		{
			m_debugOverlay->AddHTMLToBuffer(s.c_str(), TextColor::GREEN, true);
		}
#endif
	}

	void Profiling::StoreSample( std::string p_name, __int64 p_elapsedTime )
	{
		m_sampleMap[p_name].push_back(p_elapsedTime);
	}
#ifndef COMPILE_LEVEL_EDITOR
	void Profiling::SetDebugOverlay( DebugOverlayInterface* p_debugOverlay )
	{
		m_debugOverlay = p_debugOverlay;
	}
#endif
}
