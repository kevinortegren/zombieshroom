#include <RootEngine/Include/Profiling.h>
#include <RootEngine/Render/Include/Renderer.h>

namespace RootEngine
{
	Profiling::Profiling()
		: m_time(0.0f), m_frames(0), m_sort(true)
	{
	}

	Profiling::~Profiling()
	{
		
	}

	void Profiling::Present()
	{
		std::vector<double> avgTimes;
		std::vector<double> maxTimes;
		std::vector<unsigned int> samples;
		std::vector<std::string> names;

		MemInfo* memInfo = m_memTracker->GetProcessMemInfo();

		//Clear output list
		m_ouputList.clear();
		m_ouputList.shrink_to_fit();

		m_ouputList.push_back("<div style='text-align: left; display: inline-block;'>");

		std::string output	= "FPS: " + std::to_string(m_frames);
		m_ouputList.push_back(output);

		int bufferUsage, textureUsage, numBuffers, numTextures;
		m_renderer->GetResourceUsage(bufferUsage, textureUsage, numBuffers, numTextures);

		output = "Buffers (" + std::to_string(numBuffers) + ") : " + std::to_string(bufferUsage) + "Kb ";
		m_ouputList.push_back(output);
		output = "Textures (" + std::to_string(numTextures) + ") : " + std::to_string(textureUsage) + "Kb ";
		m_ouputList.push_back(output);
		output = "Working set memory: " + std::to_string(memInfo->m_workingSetMiB) + "MB  " + std::to_string(memInfo->m_workingSetKiB) + "KB  " + std::to_string(memInfo->m_workingSetB) + "B";
		m_ouputList.push_back(output);
		output = "Peak set memory: " + std::to_string(memInfo->m_peakSetMiB) + "MB  " + std::to_string(memInfo->m_peakSetKiB) + "KB  " + std::to_string(memInfo->m_peakSetB) + "B";
		m_ouputList.push_back(output);
		//Calculate average time
		__int64 qfreq;
		QueryPerformanceFrequency((LARGE_INTEGER*)&qfreq);
		double dFreq = (double)qfreq/1000.0f;

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
			
			avgTimes.push_back((collectedTime/itr->second.size()) / dFreq);
			maxTimes.push_back(maxTime / dFreq);
			names.push_back(itr->first);
			samples.push_back(itr->second.size());

			//Clear collected time vector
			itr->second.clear();
			itr->second.shrink_to_fit();
		}

		if(m_sort)
		{
			//Find the largest avgTime index and push it for output and set it to 0, then re-iterate. Sorted when loop is done.
			for(unsigned int i = 0; i < m_sampleMap.size(); i++)
			{
				unsigned int index = 0;
				double currAvg = 0.0;

				for(unsigned int j = 0; j < avgTimes.size(); j++)
				{
					if(avgTimes[j] > currAvg)
					{
						currAvg = avgTimes[j];
						index = j;
					}
				}

				//Store output in vector for presenting 
				m_ouputList.push_back("<div style='min-width: 180px; display: inline-block;'>" + names[index] + " :</div>" + "T: " + std::to_string(avgTimes[index]) + " <div style='color: #FFFFFF; display: inline-block;'> MaxT: "+ std::to_string(maxTimes[index]) + "</div> S: " + std::to_string(samples[index]));
				avgTimes[index] = 0.0;
			}
		}
		else
		{
			for(unsigned int i = 0; i < m_sampleMap.size(); i++)
			{
				m_ouputList.push_back("<div style='min-width: 180px; display: inline-block;'>" + names[i] + " :</div>" + "T: " + std::to_string(avgTimes[i]) + " <div style='color: #FFFFFF; display: inline-block;'> MaxT: "+ std::to_string(maxTimes[i]) + "</div> S: " + std::to_string(samples[i]));
			}
		}
		m_ouputList.push_back("</div>");
	}

	void Profiling::Update( float p_dt )
	{
#ifdef _DEBUG
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
			m_debugOverlay->AddHTMLToBuffer(s.c_str(), TextColor::GREEN, false);
		}

#endif
#endif
	}

	void Profiling::ToggleSorted()
	{
		m_sort = m_sort ? false : true;
	}

	void Profiling::StoreSample( std::string p_name, __int64 p_elapsedTime )
	{
		m_sampleMap[p_name].push_back(p_elapsedTime);
	}
	void Profiling::SetMemoryTracker( MemoryTracker* p_memTracker )
	{
		m_memTracker = p_memTracker;
	}
	void Profiling::SetRenderInterface(Render::RendererInterface* p_renderer)
	{
		m_renderer = p_renderer;
	}

	void Profiling::BeginGPUTimer()
	{
		glBeginQuery(GL_TIME_ELAPSED, m_queryID);
	}

	double Profiling::EndGPUTimer()
	{
		glEndQuery(GL_TIME_ELAPSED);
		GLint done = 0;
		while(!done)
		{
			glGetQueryObjectiv(m_queryID, GL_QUERY_RESULT_AVAILABLE, &done);
		}
		GLuint64 elapsedTime;
		glGetQueryObjectui64v(m_queryID, GL_QUERY_RESULT, &elapsedTime);
		return elapsedTime/1000000.0;
	}

	void Profiling::InitQuery()
	{
		glGenQueries(1, &m_queryID);
	}

#ifndef COMPILE_LEVEL_EDITOR
#ifdef _DEBUG
	void Profiling::SetDebugOverlay( DebugOverlayInterface* p_debugOverlay )
	{
		m_debugOverlay = p_debugOverlay;
	}
#endif
#endif
}
