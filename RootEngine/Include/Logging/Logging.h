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
		GENERAL,
		NOTAG,
		PHYSICS,
		TOOLS,
		SOUND,
		GAME,
		COMPONENT,
		GUI,
		INPUT,
		RESOURCE,
		SCRIPT
	};
}

namespace LogLevel
{
	enum LogLevel
	{
		FATAL_ERROR,
		NON_FATAL_ERROR,
		WARNING,
		SUCCESS,
		DEBUG_PRINT,
		INIT_PRINT,
		MASS_DATA_PRINT
	};
}

class LoggingInterface
{
public:

		virtual void LTF(std::string p_func, int p_line, LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel, const char* p_format, ...) = 0;
		virtual void LTF(std::string p_func, int p_line,const char* p_format, ...) = 0;
		
		virtual void LTC(std::string p_func, int p_line, LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel, const char* p_format, ...) = 0;
		virtual void LTC(std::string p_func, int p_line,const char* p_format, ...) = 0;

		virtual void LT(std::string p_func, int p_line, LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel, const char* p_format, ...) = 0;
		virtual void LT(std::string p_func, int p_line,const char* p_format, ...) = 0;

		virtual bool OpenLogStream() = 0;
		virtual bool CloseLogStream() = 0;

		virtual void SetVerboseLevel(LogLevel::LogLevel p_vLevel) = 0;

		virtual void AddExclusiveTags(LogTag::LogTag p_tag) = 0;

		//Dummy methods to identify functions from other parts of the system
		void LogTextToFile(LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel, const char* p_format, ...){};
		void LogTextToConsole(LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel, const char* p_format, ...){};
		void LogText(LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel, const char* p_format, ...){};

};

class Logging : public LoggingInterface
{
	public:
		Logging();
		~Logging();

		
		void LTF(std::string p_func, int p_line, LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel, const char* p_format, ...);
		void LTF(std::string p_func, int p_line,const char* p_format, ...);
		
		void LTC(std::string p_func, int p_line, LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel, const char* p_format, ...);
		void LTC(std::string p_func, int p_line,const char* p_format, ...);

		void LT(std::string p_func, int p_line, LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel, const char* p_format, ...);
		void LT(std::string p_func, int p_line,const char* p_format, ...);

		bool OpenLogStream();
		bool CloseLogStream();

		void SetVerboseLevel(LogLevel::LogLevel p_vLevel);

		void AddExclusiveTags(LogTag::LogTag p_tag);
	private:

		FILE* m_logFile;
		FILE* m_commaFile;

		std::string GetTimeString(int p_time);
		std::string GetTimeFormatString();

		std::vector<LogTag::LogTag> m_exTagList;
		std::vector<std::string> m_stringTagList;

		std::vector<std::string> m_stringLevelList;

		void WriteToFile(std::string p_func, int p_line, LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel, std::string p_format, va_list p_args);
		void WriteToConsole(std::string p_func, int p_line, LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel, std::string p_format, va_list p_args);
		
		bool CheckTag(LogTag::LogTag p_tag);

		std::string GetStringFromTag(LogTag::LogTag p_tag);
		std::string GetStringFromLevel(LogLevel::LogLevel p_level);

		LogLevel::LogLevel m_verboseLevel, m_defaultVerbose;

		LogTag::LogTag m_defaultTag;

};
//(LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel, const char* p_format, ...)
#define LogText(...) LT(__FUNCTION__, __LINE__, ##__VA_ARGS__)
#define LogTextToFile(...) LTF(__FUNCTION__,  __LINE__, ##__VA_ARGS__)
#define LogTextToConsole(...) LTC(__FUNCTION__,  __LINE__, ##__VA_ARGS__)