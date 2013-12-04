#include "Logging/Logging.h"
#include <ctime>
#include <RootForce/Include/CMDColor.h>
#include <iostream>
Logging::Logging()
	:m_verboseLevel(LogLevel::INIT_PRINT), m_defaultVerbose(LogLevel::DEBUG_PRINT)
{
	ColorCMD::ConsoleColorInit();
	SetConsoleTitle(L"RootLog");

	m_stringTagList.push_back("RENDER");
	m_stringTagList.push_back("NETWOR");
	m_stringTagList.push_back("GNERAL");
	m_stringTagList.push_back("NOTAG ");
	m_stringTagList.push_back("PHYSIC");
	m_stringTagList.push_back("TOOLS ");
	m_stringTagList.push_back("SOUND ");
	m_stringTagList.push_back("GAME  ");
	m_stringTagList.push_back("CMPSYS");
	m_stringTagList.push_back("GUI   ");
	m_stringTagList.push_back("INPUT ");
	m_stringTagList.push_back("RESSYS");
	m_stringTagList.push_back("SCRIPT");

	m_stringLevelList.push_back("FATAL_ERR  ");
	m_stringLevelList.push_back("NON_FAT_ERR");
	m_stringLevelList.push_back("WARNING    ");
	m_stringLevelList.push_back("SUCCESS    ");
	m_stringLevelList.push_back("DEBUG_PRINT");
	m_stringLevelList.push_back("INIT_PRINT ");
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
	fopen_s(&m_commaFile, commaName.c_str(), "w");

	return true;
}

bool Logging::CloseLogStream()
{
	if(m_logFile)
	{   //Close stream
		fclose(m_logFile);
		return true;
	}
	if(m_commaFile)
	{   //Close stream
		fclose(m_commaFile);
		return true;
	}

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
//FILE LOGGING
//////////////////////////////////////////////////////////////////////////
void Logging::LTF(std::string p_func, int p_line, LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel, const char* p_format, ... )
{
	va_list args;
	va_start (args, p_format);
	if(p_vLevel <= m_verboseLevel && CheckTag(p_tag))
		WriteToFile(p_func, p_line, p_tag, p_vLevel, p_format, args);
	va_end (args);
}

void Logging::LTF(std::string p_func, int p_line, const char * p_format, ... )
{
	va_list args;
	va_start (args, p_format);
	if(m_defaultVerbose <= m_verboseLevel && CheckTag(LogTag::NOTAG))
		WriteToFile(p_func, p_line, LogTag::NOTAG, m_defaultVerbose, p_format, args);
	va_end (args);
}

//////////////////////////////////////////////////////////////////////////
//CONSOLE LOGGING
//////////////////////////////////////////////////////////////////////////
void Logging::LTC(std::string p_func, int p_line, LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel, const char* p_format, ... )
{
	va_list args;
	va_start (args, p_format);
	if(p_vLevel <= m_verboseLevel && CheckTag(p_tag))
		WriteToConsole(p_func, p_line, p_tag, p_vLevel, p_format, args);
	va_end (args);
}

void Logging::LTC(std::string p_func, int p_line,const char * p_format, ...)
{
	va_list args;
	va_start (args, p_format);
	if(m_defaultVerbose <= m_verboseLevel && CheckTag(LogTag::NOTAG))
		WriteToConsole(p_func, p_line, LogTag::NOTAG, m_defaultVerbose, p_format, args);
	va_end (args);
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

void Logging::LT(std::string p_func, int p_line,const char* p_format, ... )
{
	va_list args;
	va_start (args, p_format);
	if(m_defaultVerbose <= m_verboseLevel && CheckTag(LogTag::NOTAG))
	{
		WriteToConsole(p_func, p_line, LogTag::NOTAG, m_defaultVerbose, p_format, args);
		WriteToFile(p_func, p_line, LogTag::NOTAG, m_defaultVerbose, p_format, args);
	}
	va_end (args);
}

//////////////////////////////////////////////////////////////////////////
//Set verbose level ( defualt is LogLevel::DEBUG_PRINT)
//////////////////////////////////////////////////////////////////////////
void Logging::SetVerboseLevel( LogLevel::LogLevel p_vLevel )
{
	m_verboseLevel = p_vLevel;
	LT(__FUNCTION__, __LINE__, LogTag::NOTAG, LogLevel::DEBUG_PRINT, "Verbose level set to %d", (int)p_vLevel);
}

//////////////////////////////////////////////////////////////////////////
//Add filters for tags. Only displays those added. No filters as default(show all filters)
//////////////////////////////////////////////////////////////////////////
void Logging::AddExclusiveTags( LogTag::LogTag p_tag)
{
	m_exTagList.push_back(p_tag);
}

//////////////////////////////////////////////////////////////////////////
//Final write functions
//////////////////////////////////////////////////////////////////////////
void Logging::WriteToFile(std::string p_func, int p_line, LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel,std::string p_format, va_list p_args )
{
	
	std::string output = GetTimeFormatString() + "    " + GetStringFromTag(p_tag) + "    " + GetStringFromLevel(p_vLevel) +  "    " + p_format + "    [" + p_func + ", Line: " + std::to_string(p_line) + "]" + "\n";
	std::string commaOutput = GetTimeFormatString() + ";;" + GetStringFromTag(p_tag) + ";;" + GetStringFromLevel(p_vLevel) +  ";;" + p_format + ";;" + p_func + ";;" + std::to_string(p_line) + "\n";

	vfprintf (m_logFile, output.c_str(), p_args);
	vfprintf (m_commaFile, commaOutput.c_str(), p_args);
	fflush(m_logFile);
	fflush(m_commaFile);
}

void Logging::WriteToConsole(std::string p_func, int p_line, LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel, std::string p_format, va_list p_args )
{
	std::string output = GetTimeFormatString() + "    " + GetStringFromTag(p_tag) + "    " + GetStringFromLevel(p_vLevel) +  "    " + p_format  + "\n";

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



