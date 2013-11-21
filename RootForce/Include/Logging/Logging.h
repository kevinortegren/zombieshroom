#pragma once

#include <string>
#include <vector>
#include <stdio.h>
#include <stdarg.h>

enum LOG_TAG
{
	RENDER,
	NETWORK,
	DEBUG,
	GENERAL,
	NOTAG,
	PHYSICS,
	TOOLS,
	SOUND,
	GAME,
	COMPONENT
};

class Logging
{
	public:
		static Logging* GetInstance();
		void LogTextToFileTagVerbose(LOG_TAG p_tag, unsigned int p_vLevel, const char* p_format, ...);
		void LogTextToFileVerbose(unsigned int p_vLevel, const char* p_format, ...);
		void LogTextToFile(const char* p_format, ...);
		
		void LogTextToConsoleTagVerbose(LOG_TAG p_tag, unsigned int p_vLevel, const char* p_format, ...);
		void LogTextToConsoleVerbose(unsigned int p_vLevel, const char* p_format, ...);
		void LogTextToConsole(const char* p_format, ...);

		bool OpenLogStream();
		bool CloseLogStream();

		void SetVerboseLevel(unsigned int p_vLevel);

		void AddExclusiveTags(LOG_TAG p_tag);

	protected:
	private:
		Logging();
		~Logging();

		static Logging* s_loggingInstance;

		FILE* m_logFile;

		std::string GetTimeString(int p_time);
		std::string GetTimeFormatString();

		std::vector<LOG_TAG> m_exTagList;
		std::vector<std::string> m_stringTagList;

		void WriteToFile(LOG_TAG p_tag, unsigned int p_vLevel, std::string p_format, va_list p_args);
		void WriteToConsole(LOG_TAG p_tag, unsigned int p_vLevel, std::string p_format, va_list p_args);
		
		bool CheckTag(LOG_TAG p_tag);
		std::string GetStringFromTag(LOG_TAG p_tag);

		unsigned int m_verboseLevel, m_defaultVerbose;

		LOG_TAG m_defaultTag;
};
