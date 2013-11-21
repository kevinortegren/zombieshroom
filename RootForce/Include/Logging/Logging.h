#pragma once

#include <string>
#include <stdio.h>
#include <stdarg.h>

class Logging
{
	public:
		static Logging* GetInstance();
		void LogTextToFileTagVerbose(const char* p_tag, unsigned int p_vLevel, const char* p_format, ...);
		void LogTextToFileVerbose(unsigned int p_vLevel, const char* p_format, ...);
		void LogTextToFile(const char* p_format, ...);
		
		void LogTextToConsoleTagVerbose(const char* p_tag, unsigned int p_vLevel, const char* p_format, ...);
		void LogTextToConsoleVerbose(unsigned int p_vLevel, const char* p_format, ...);
		void LogTextToConsole(const char* p_format, ...);

		bool OpenLogStream();
		bool CloseLogStream();

		void SetVerboseLevel(unsigned int p_vLevel);

		void SetExclusiveTags(std::string p_tag, ...);

	protected:
	private:
		Logging();
		~Logging();

		static Logging* s_loggingInstance;

		FILE* m_logFile;

		std::string GetTimeString(int p_time);
		void WriteToFile(std::string p_format, va_list p_args);
		void WriteToConsole(std::string p_format, va_list p_args);

		unsigned int m_verboseLevel;


};
