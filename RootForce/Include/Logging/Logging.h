#pragma once

#include <string>
#include <stdio.h>
#include <stdarg.h>

class Logging
{
	public:
		static Logging* GetInstance();
		void LogTextToFile(const char * p_format, ...);
		void LogTextToConsole(const char * p_format, ...);
		bool OpenLogStream();
		bool CloseLogStream();
	protected:
	private:
		Logging();
		~Logging();

		static Logging* s_loggingInstance;

		FILE* m_logFile;

		std::string GetTimeString(int p_time);


};
