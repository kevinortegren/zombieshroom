#include "Logging/Logging.h"
#include <ctime>
#include <RootEngine/Include/Logging/CMDColor.h>
#include <iostream>

Logging::Logging()
	:m_verboseLevel(LogLevel::PINK_PRINT), m_defaultVerbose(LogLevel::DEBUG_PRINT)
{
	ColorCMD::ConsoleColorInit();
	SetConsoleTitle(L"RootLog");

	m_stringTagList.push_back("RENDER ");
	m_stringTagList.push_back("NETWORK");
	m_stringTagList.push_back("CLIENT ");
	m_stringTagList.push_back("SERVER ");
	m_stringTagList.push_back("GENERAL");
	m_stringTagList.push_back("NOTAG  ");
	m_stringTagList.push_back("PHYSICS");
	m_stringTagList.push_back("TOOLS  ");
	m_stringTagList.push_back("SOUND  ");
	m_stringTagList.push_back("GAME   ");
	m_stringTagList.push_back("COMPSYS");
	m_stringTagList.push_back("GUI    ");
	m_stringTagList.push_back("INPUT  ");
	m_stringTagList.push_back("RESSYS ");
	m_stringTagList.push_back("SCRIPT ");
	m_stringTagList.push_back("ANIM   ");
	m_stringTagList.push_back("WATER  ");

	m_stringLevelList.push_back("FATAL_ERR  ");
	m_stringLevelList.push_back("NON_FAT_ERR");
	m_stringLevelList.push_back("WARNING    ");
	m_stringLevelList.push_back("SUCCESS    ");
	m_stringLevelList.push_back("DEBUG_PRINT");
	m_stringLevelList.push_back("INIT_PRINT ");
	m_stringLevelList.push_back("START_PRINT");
	m_stringLevelList.push_back("PINK_PRINT ");
	m_stringLevelList.push_back("PACKET     ");
	m_stringLevelList.push_back("DATA_PRINT ");

	OpenLogStream();
}

Logging::~Logging()
{
	CloseLogStream();
}

bool Logging::OpenLogStream()
{
	// current date/time based on current system
	time_t currentTime = time(0);

	tm gmtm;
	// convert now to tm struct for UTC
	gmtime_s(&gmtm, &currentTime);
	//Generate file name from date and time
	std::string fileName = std::to_string(gmtm.tm_year+1900) + std::to_string(gmtm.tm_mon+1) + std::to_string(gmtm.tm_mday) + "_" + GetTimeString(gmtm.tm_hour+1) + "-" + GetTimeString(gmtm.tm_min) + "-" + GetTimeString(gmtm.tm_sec);
	std::string logName = fileName + ".txt";
	std::string commaName = fileName + ".rlog";
	//Open log file stream
	fopen_s(&m_logFile, logName.c_str(), "w");
	//fopen_s(&m_commaFile, commaName.c_str(), "w");

	return true;
}

bool Logging::CloseLogStream()
{
	if(m_logFile)
	{   //Close stream
		fclose(m_logFile);
		return true;
	}
	//if(m_commaFile)
	//{   //Close stream
	//	fclose(m_commaFile);
	//	return true;
	//}

	return false;
}

std::string Logging::GetTimeString( int p_time )
{
	if(p_time < 10)
		return "0" + std::to_string(p_time);
	else
		return std::to_string(p_time);
}

//////////////////////////////////////////////////////////////////////////
//FILE AND CONSOLE LOGGING
//////////////////////////////////////////////////////////////////////////
void Logging::LT( std::string p_func, int p_line, LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel, const char* p_format, ... )
{
	va_list args;
	va_start (args, p_format);
	if(p_vLevel <= m_verboseLevel && CheckTag(p_tag))
	{
		WriteToConsole(p_func, p_line, p_tag, p_vLevel, p_format, args);
		WriteToFile(p_func, p_line, p_tag, p_vLevel, p_format, args);
	}
	va_end (args);
}

//////////////////////////////////////////////////////////////////////////
//SCRIPT LOGGING
//////////////////////////////////////////////////////////////////////////
void Logging::LogScript(std::string p_luaFunc, int p_luaLine, LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel, const char* p_format, ...)
{
	p_luaFunc = GetNameFromPath(p_luaFunc);

	va_list args;
	va_start (args, p_format);
	if(p_vLevel <= m_verboseLevel && CheckTag(p_tag))
	{
		WriteToConsole(p_luaFunc, p_luaLine, p_tag, p_vLevel, p_format, args, true);
		WriteToFile(p_luaFunc, p_luaLine, p_tag, p_vLevel, p_format, args);
	}
	va_end (args);
}

//////////////////////////////////////////////////////////////////////////
//Final write functions
//////////////////////////////////////////////////////////////////////////
void Logging::WriteToFile(std::string p_func, int p_line, LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel,std::string p_format, va_list p_args )
{
	
	std::string output = GetTimeFormatString() + "    " + GetStringFromTag(p_tag) + "    " + GetStringFromLevel(p_vLevel) +  "    " + p_format + "    [" + p_func + ", Line: " + std::to_string(p_line) + "]" + "\n";
	//std::string commaOutput = GetTimeFormatString() + ";;" + GetStringFromTag(p_tag) + ";;" + GetStringFromLevel(p_vLevel) +  ";;" + p_format + ";;" + p_func + ";;" + std::to_string(p_line) + "\n";

	vfprintf (m_logFile, output.c_str(), p_args);
	//vfprintf (m_commaFile, commaOutput.c_str(), p_args);
	fflush(m_logFile);
	//fflush(m_commaFile);
}

void Logging::WriteToConsole(std::string p_func, int p_line, LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel, std::string p_format, va_list p_args, bool writeFileLine )
{
	std::string output;
	if (!writeFileLine)
		output = GetTimeFormatString() + "    " + GetStringFromTag(p_tag) + "    " + GetStringFromLevel(p_vLevel) +  "    " + p_format  +"\n";
	else
		output = GetTimeFormatString() + "    " + GetStringFromTag(p_tag) + "    " + GetStringFromLevel(p_vLevel) +  "    " + p_format + "    [" + p_func + ", Line: " + std::to_string(p_line) + "]" + "\n";

	switch (p_vLevel)
	{
	case LogLevel::FATAL_ERROR:
		{
			ColorCMD::SetColor(ColorCMD::ConsoleColor::RED, ColorCMD::defbackcol);
			std::cout<<"";
			break;
		}
	case LogLevel::NON_FATAL_ERROR:
		{
			ColorCMD::SetColor(ColorCMD::ConsoleColor::RED, ColorCMD::defbackcol);
			std::cout<<"";
			break;
		}
	case LogLevel::WARNING:
		{
			ColorCMD::SetColor(ColorCMD::ConsoleColor::YELLOW, ColorCMD::defbackcol);
			std::cout<<"";
			break;
		}
	case LogLevel::SUCCESS:
		{
			ColorCMD::SetColor(ColorCMD::ConsoleColor::GREEN, ColorCMD::defbackcol);
			std::cout<<"";
			break;
		}
	case LogLevel::DEBUG_PRINT:
		{
			ColorCMD::SetColor(ColorCMD::ConsoleColor::WHITE, ColorCMD::defbackcol);
			std::cout<<"";
			break;
		}
	case LogLevel::INIT_PRINT:
		{
			ColorCMD::SetColor(ColorCMD::ConsoleColor::BLUE, ColorCMD::defbackcol);
			std::cout<<"";
			break;
		}
	case LogLevel::START_PRINT:
		{
			ColorCMD::SetColor(ColorCMD::ConsoleColor::AQUA, ColorCMD::defbackcol);
			std::cout<<"";
			break;
		}
	case LogLevel::PINK_PRINT:
		{
			ColorCMD::SetColor(ColorCMD::ConsoleColor::PINK, ColorCMD::defbackcol);
			std::cout<<"";
			break;
		}
	case LogLevel::MASS_DATA_PRINT:
		{
			ColorCMD::SetColor(ColorCMD::ConsoleColor::WHITE, ColorCMD::defbackcol);
			std::cout<<"";
			break;
		}
	default:
		break;
	}
	
	
	vprintf(output.c_str(), p_args);
	//std::cout<<"     ";
	//ColorCMD::SetColor(ColorCMD::ConsoleColor::GRAY, ColorCMD::BLACK);
	//std::cout << "[" + p_func + ", Line: " + std::to_string(p_line) + "]" + "\n";

	ColorCMD::SetColor(ColorCMD::ConsoleColor::WHITE, ColorCMD::defbackcol);
	std::cout<<"";
}

//////////////////////////////////////////////////////////////////////////
//Returns a correctly formatted time string
//////////////////////////////////////////////////////////////////////////
std::string Logging::GetTimeFormatString()
{
	time_t currentTime = time(0);

	tm gmtm;

	gmtime_s(&gmtm, &currentTime);

	return GetTimeString(gmtm.tm_hour+1) + ":" + GetTimeString(gmtm.tm_min) + ":" + GetTimeString(gmtm.tm_sec);
}

//////////////////////////////////////////////////////////////////////////
//Checks for filters
//////////////////////////////////////////////////////////////////////////
bool Logging::CheckTag(LogTag::LogTag p_tag)
{
	if(m_exTagList.size() == 0)
		return true;

	for (LogTag::LogTag s : m_exTagList ) 
	{
		if(p_tag == s)
			return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
//Enum-to-string converters CHANGE THIS TO MAP
//////////////////////////////////////////////////////////////////////////
std::string Logging::GetStringFromTag( LogTag::LogTag p_tag )
{
	return m_stringTagList.at(p_tag);
}

std::string Logging::GetStringFromLevel( LogLevel::LogLevel p_level )
{
	return m_stringLevelList.at(p_level);
}

std::string Logging::GetNameFromPath( std::string p_path )
{
	std::string cutPath;
	std::string::size_type slashIndex, dotIndex;

	// Extract the file name
	cutPath		= p_path;
	slashIndex	= cutPath.find_last_of("/")+1;
	if(slashIndex == 0)
		slashIndex	= cutPath.find_last_of("\\")+1;
	cutPath		= cutPath.substr(slashIndex, cutPath.size());
	dotIndex	= cutPath.find_last_of(".");
	cutPath		= cutPath.substr(0, dotIndex);
	return cutPath;
}

