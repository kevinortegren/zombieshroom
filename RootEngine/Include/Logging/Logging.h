#pragma once

#include <string>
#include <vector>
#include <stdio.h>
#include <stdarg.h>

namespace LogTag
{
	enum LogTag
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
		COMPONENT,
		ERR
	};
}

namespace LogLevel
{
	enum LogLevel
	{
		FATAL_ERROR,
		NON_FATAL_ERROR,
		WARNING,
		DEBUG_PRINT,
		MASS_DATA_PRINT
	};
}

class LoggingInterface
{
public:
		virtual void LogTextToFile(LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel, const char* p_format, ...) = 0;
		virtual void LogTextToFile( LogLevel::LogLevel p_vLevel, const char* p_format, ...) = 0;
		virtual void LogTextToFile(const char* p_format, ...) = 0;
		
		virtual void LogTextToConsole(LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel, const char* p_format, ...) = 0;
		virtual void LogTextToConsole(LogLevel::LogLevel p_vLevel, const char* p_format, ...) = 0;
		virtual void LogTextToConsole(const char* p_format, ...) = 0;

		virtual void LogText(LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel, const char* p_format, ...) = 0;
		virtual void LogText(LogLevel::LogLevel p_vLevel, const char* p_format, ...) = 0;
		virtual void LogText(const char* p_format, ...) = 0;

		virtual bool OpenLogStream() = 0;
		virtual bool CloseLogStream() = 0;

		virtual void SetVerboseLevel(LogLevel::LogLevel p_vLevel) = 0;

		virtual void AddExclusiveTags(LogTag::LogTag p_tag) = 0;
};

class Logging : public LoggingInterface
{
	public:
		Logging();
		~Logging();

		void LogTextToFile(LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel, const char* p_format, ...);
		void LogTextToFile(LogLevel::LogLevel p_vLevel, const char* p_format, ...);
		void LogTextToFile(const char* p_format, ...);
		
		void LogTextToConsole(LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel, const char* p_format, ...);
		void LogTextToConsole(LogLevel::LogLevel p_vLevel, const char* p_format, ...);
		void LogTextToConsole(const char* p_format, ...);

		void LogText(LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel, const char* p_format, ...);
		void LogText(LogLevel::LogLevel p_vLevel, const char* p_format, ...);
		void LogText(const char* p_format, ...);

		bool OpenLogStream();
		bool CloseLogStream();

		void SetVerboseLevel(LogLevel::LogLevel p_vLevel);

		void AddExclusiveTags(LogTag::LogTag p_tag);

	protected:
	private:

		FILE* m_logFile;

		std::string GetTimeString(int p_time);
		std::string GetTimeFormatString();

		std::vector<LogTag::LogTag> m_exTagList;
		std::vector<std::string> m_stringTagList;

		std::vector<std::string> m_stringLevelList;

		void WriteToFile(LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel, std::string p_format, va_list p_args);
		void WriteToConsole(LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel, std::string p_format, va_list p_args);
		
		bool CheckTag(LogTag::LogTag p_tag);
		std::string GetStringFromTag(LogTag::LogTag p_tag);
		std::string GetStringFromLevel(LogLevel::LogLevel p_level);

		LogLevel::LogLevel m_verboseLevel, m_defaultVerbose;

		LogTag::LogTag m_defaultTag;
};
