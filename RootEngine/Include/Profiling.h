#pragma once
#ifndef COMPILE_LEVEL_EDITOR
#include <RootEngine/Include/DebugOverlay/DebugOverlay.h>
#endif
#include <RootEngine/Include/Memory/MemoryTracker.h>
#include <string>
#include <vector>
#include <map>
#include <windows.h>

namespace RootEngine
{
	class ProfilingInterface abstract
	{
	public:
		virtual void Update(float dt) = 0;
		virtual void StoreSample(std::string p_name, __int64 p_elapsedTime) = 0;
		virtual void SetMemoryTracker(MemoryTracker* p_memTracker) = 0;
#ifndef COMPILE_LEVEL_EDITOR
		virtual void SetDebugOverlay(DebugOverlayInterface* p_debugOverlay) = 0;
#endif
	private:
	};

	class Profiling : public ProfilingInterface
	{
	public:
		Profiling();
		virtual ~Profiling();
		
		void Update(float dt);
		void StoreSample(std::string p_name, __int64 p_elapsedTime);
		void SetMemoryTracker(MemoryTracker* p_memTracker);
#ifndef COMPILE_LEVEL_EDITOR
		void SetDebugOverlay(DebugOverlayInterface* p_debugOverlay);
#endif
	private:
		void Present();
		std::map<std::string, std::vector<__int64>> m_sampleMap;
		std::vector<std::string> m_ouputList;
#ifndef COMPILE_LEVEL_EDITOR
		DebugOverlayInterface* m_debugOverlay;
#endif
		MemoryTracker* m_memTracker;
		float m_time;
		int m_frames;
	};

	//////////////////////////////////////////////////////////////////////////
	//Profiling struct used as a timer object when calling functions
	//////////////////////////////////////////////////////////////////////////
	struct AutoProfile
	{
		AutoProfile(std::string p_name, ProfilingInterface* p_profiling)
		{
			m_profiling = p_profiling;
			m_name = p_name;
			QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);
		}
		~AutoProfile()
		{
			__int64 endTime; 
			QueryPerformanceCounter((LARGE_INTEGER*)&endTime);

			__int64 elapsedTime = endTime -	m_startTime;
		
			m_profiling->StoreSample(m_name, elapsedTime);
		}

		std::string m_name;
		__int64		m_startTime;
		ProfilingInterface* m_profiling;
	};

	#define PROFILE(name, profiler) RootEngine::AutoProfile profilingAutoProfile(name, profiler);
}
