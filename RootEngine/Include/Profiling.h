#pragma once
#include <RootEngine/Include/DebugOverlay/DebugOverlay.h>
#include <string>
#include <vector>
#include <map>

namespace RootEngine
{
	class ProfilingInterface abstract
	{
	public:
		virtual void Update(float dt) = 0;
		virtual void StoreSample(std::string p_name, __int64 p_elapsedTime) = 0;
		virtual void SetDebugOverlay(DebugOverlayInterface* p_debugOverlay) = 0;
	private:
	};

	class Profiling : public ProfilingInterface
	{
	public:
		Profiling();
		virtual ~Profiling();
		
		void Update(float dt);
		void StoreSample(std::string p_name, __int64 p_elapsedTime);
		void SetDebugOverlay(DebugOverlayInterface* p_debugOverlay);
	private:
		void Present();

		std::map<std::string, std::vector<__int64>> m_sampleMap;
		std::vector<std::string> m_ouputList;
		DebugOverlayInterface* m_debugOverlay;
		float m_time;
	};

	//////////////////////////////////////////////////////////////////////////
	//Profiling struct used as a timer object when calling functions
	//////////////////////////////////////////////////////////////////////////
	struct AutoProfile
	{
		AutoProfile(std::string p_name)
		{
			m_name = p_name;
			QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);
		}
		~AutoProfile()
		{
			__int64 endTime; 
			QueryPerformanceCounter((LARGE_INTEGER*)&endTime);

			__int64 elapsedTime = endTime -	m_startTime;

			//Profiling::StoreSample(m_name, elapsedTime);
		}

		std::string m_name;
		__int64		m_startTime;

	};

	#define PROFILE AutoProfile p(__FUNCTION__);
}
