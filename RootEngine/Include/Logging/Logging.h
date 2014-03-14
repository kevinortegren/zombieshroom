#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdarg.h>
#include <map>

namespace LogTag
{
	enum LogTag
	{
		RENDER,
		NETWORK,
		CLIENT,
		SERVER,
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
		SCRIPT,
		ANIMATION,
		WATER,
		PARTICLE
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
		START_PRINT,
		PINK_PRINT,
		PACKET_PRINT,
		MASS_DATA_PRINT,
		NOLEVEL,
		HELP_PRINT,
		IDENTIFY_PRINT,
	};
}

class LoggingInterface
{
public:

		virtual void LT(std::string p_func, int p_line, LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel, const char* p_format, ...) = 0;

		virtual void LogScript(std::string p_luaFunc, int p_luaLine, LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel, const char* p_format, ...) = 0;

		virtual void ParseCommand( std::stringstream* p_data ) = 0;
		//Dummy methods to identify functions from other parts of the system
		void LogText(LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel, const char* p_format, ...){};

		virtual ~LoggingInterface(){};
};

class Logging : public LoggingInterface
{
	public:
		Logging();
		~Logging();

		void LT(std::string p_func, int p_line, LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel, const char* p_format, ...);

		void LogScript(std::string p_luaFunc, int p_luaLine, LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel, const char* p_format, ...);

		void ParseCommand( std::stringstream* p_data );

		bool OpenLogStream(std::string p_path);
	private:

		struct TagLevelInfo
		{
			TagLevelInfo(std::string p_name, bool p_enabled) : Name(p_name), Enabled(p_enabled){}
			TagLevelInfo(){}
			std::string Name;
			bool		Enabled;
		};

		bool m_enableLogging;

		bool CloseLogStream();

		FILE* m_logFile;

		std::string GetTimeString(int p_time);
		std::string GetTimeFormatString();

		void WriteToFile(std::string p_func, int p_line, LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel, std::string p_format, va_list p_args);
		void WriteToConsole(std::string p_func, int p_line, LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel, std::string p_format, va_list p_args, bool writeFileLine = false);
		
		bool CheckTag(LogTag::LogTag p_tag);
		bool CheckLevel(LogLevel::LogLevel p_level);

		std::string GetNameFromPath( std::string p_path );

		std::map<LogTag::LogTag, TagLevelInfo> m_tagInfo;
		std::map<LogLevel::LogLevel, TagLevelInfo> m_levelInfo;

		void PrintStatus();

		void ClearLog();



};
//(LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel, const char* p_format, ...)
#define LogText(...) LT(__FUNCTION__, __LINE__, ##__VA_ARGS__)

