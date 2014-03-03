#include "Logging/Logging.h"
#include <ctime>
#include <RootEngine/Include/Logging/CMDColor.h>

Logging::Logging() : m_enableLogging(true)
{
	ColorCMD::ConsoleColorInit();
	SetConsoleTitle(L"RootLog");
	
	m_tagInfo[LogTag::RENDER]		= TagLevelInfo("RENDER  ", true);
	m_tagInfo[LogTag::NETWORK]		= TagLevelInfo("NETWORK ", true);
	m_tagInfo[LogTag::CLIENT]		= TagLevelInfo("CLIENT  ", true);
	m_tagInfo[LogTag::SERVER]		= TagLevelInfo("SERVER  ", true);
	m_tagInfo[LogTag::GENERAL]		= TagLevelInfo("GENERAL ", true);
	m_tagInfo[LogTag::NOTAG]		= TagLevelInfo("NOTAG   ", true);
	m_tagInfo[LogTag::PHYSICS]		= TagLevelInfo("PHYSICS ", true);
	m_tagInfo[LogTag::TOOLS]		= TagLevelInfo("TOOLS   ", true);
	m_tagInfo[LogTag::SOUND]		= TagLevelInfo("SOUND   ", true);
	m_tagInfo[LogTag::GAME]			= TagLevelInfo("GAME    ", true);
	m_tagInfo[LogTag::COMPONENT]	= TagLevelInfo("COMPSYS ", true);
	m_tagInfo[LogTag::GUI]			= TagLevelInfo("GUI     ", true);
	m_tagInfo[LogTag::INPUT]		= TagLevelInfo("INPUT   ", true);
	m_tagInfo[LogTag::RESOURCE]		= TagLevelInfo("RESSYS  ", true);
	m_tagInfo[LogTag::SCRIPT]		= TagLevelInfo("SCRIPT  ", true);
	m_tagInfo[LogTag::ANIMATION]	= TagLevelInfo("ANIM    ", true);
	m_tagInfo[LogTag::WATER]		= TagLevelInfo("WATER   ", true);
	m_tagInfo[LogTag::PARTICLE]		= TagLevelInfo("PARTICLE", false);

	m_levelInfo[LogLevel::FATAL_ERROR]		= TagLevelInfo("FATAL_ERR  ", true);
	m_levelInfo[LogLevel::NON_FATAL_ERROR]	= TagLevelInfo("NON_FAT_ERR", true);
	m_levelInfo[LogLevel::WARNING]			= TagLevelInfo("WARNING    ", true);
	m_levelInfo[LogLevel::SUCCESS]			= TagLevelInfo("SUCCESS    ", true);
	m_levelInfo[LogLevel::DEBUG_PRINT]		= TagLevelInfo("DEBUG_PRINT", true);
	m_levelInfo[LogLevel::INIT_PRINT]		= TagLevelInfo("INIT_PRINT ", true);
	m_levelInfo[LogLevel::START_PRINT]		= TagLevelInfo("START_PRINT", true);
	m_levelInfo[LogLevel::PINK_PRINT]		= TagLevelInfo("PINK_PRINT ", true);
	m_levelInfo[LogLevel::PACKET_PRINT]		= TagLevelInfo("PACKET     ", false);
	m_levelInfo[LogLevel::MASS_DATA_PRINT]	= TagLevelInfo("DATA_PRINT ", false);
	m_levelInfo[LogLevel::NOLEVEL]			= TagLevelInfo("NOLEVEL    ", true);
	m_levelInfo[LogLevel::HELP_PRINT]		= TagLevelInfo("HELP_PRINT ", true);

}

Logging::~Logging()
{
#ifdef _DEBUG
	CloseLogStream();
#endif // _DEBUG
}

bool Logging::OpenLogStream(std::string p_path)
{
#ifdef _DEBUG
	// current date/time based on current system
	time_t currentTime = time(0);

	tm gmtm;
	// convert now to tm struct for UTC
	gmtime_s(&gmtm, &currentTime);
	//Generate file name from date and time
	std::string fileName = std::to_string(gmtm.tm_year+1900) + std::to_string(gmtm.tm_mon+1) + std::to_string(gmtm.tm_mday) + "_" + GetTimeString(gmtm.tm_hour+1) + "-" + GetTimeString(gmtm.tm_min) + "-" + GetTimeString(gmtm.tm_sec);
	std::string logName = p_path + fileName + ".txt";
	//Open log file stream
	fopen_s(&m_logFile, logName.c_str(), "w");
#endif // _DEBUG
	return true;

}

bool Logging::CloseLogStream()
{
	if(m_logFile)
	{   //Close stream
		fclose(m_logFile);
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
//FILE AND CONSOLE LOGGING
//////////////////////////////////////////////////////////////////////////
void Logging::LT( std::string p_func, int p_line, LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel, const char* p_format, ... )
{
	va_list args;
	va_start (args, p_format);
	if(CheckLevel(p_vLevel) && CheckTag(p_tag))
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
	if(CheckLevel(p_vLevel) && CheckTag(p_tag))
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
#ifdef _DEBUG
	std::string output = GetTimeFormatString() + "    " + m_tagInfo[p_tag].Name + "    " + m_levelInfo[p_vLevel].Name +  "    " + p_format + "    [" + p_func + ", Line: " + std::to_string(p_line) + "]" + "\n";

	vfprintf (m_logFile, output.c_str(), p_args);
	fflush(m_logFile);
#endif // _DEBUG
}

void Logging::WriteToConsole(std::string p_func, int p_line, LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel, std::string p_format, va_list p_args, bool writeFileLine )
{
	if(!m_enableLogging)
		return;

	std::string output;
	if (!writeFileLine)
		output = GetTimeFormatString() + "    " + m_tagInfo[p_tag].Name + "    " + m_levelInfo[p_vLevel].Name +  "    " + p_format  +"\n";
	else
		output = GetTimeFormatString() + "    " + m_tagInfo[p_tag].Name + "    " + m_levelInfo[p_vLevel].Name +  "    " + p_format + "    [" + p_func + ", Line: " + std::to_string(p_line) + "]" + "\n";

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
	case LogLevel::PACKET_PRINT:
		{
			ColorCMD::SetColor(ColorCMD::ConsoleColor::DARK_PURPLE, ColorCMD::defbackcol);
			std::cout<<"";
			break;
		}
	case LogLevel::MASS_DATA_PRINT:
		{
			ColorCMD::SetColor(ColorCMD::ConsoleColor::WHITE, ColorCMD::defbackcol);
			std::cout<<"";
			break;
		}
	case LogLevel::NOLEVEL:
		{
			ColorCMD::SetColor(ColorCMD::ConsoleColor::GRAY, ColorCMD::defbackcol);
			std::cout<<"";
			break;
		}
	case LogLevel::HELP_PRINT:
		{
			ColorCMD::SetColor(ColorCMD::ConsoleColor::DARK_GREEN, ColorCMD::defbackcol);
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
	return m_tagInfo[p_tag].Enabled;
}

bool Logging::CheckLevel( LogLevel::LogLevel p_level )
{
	return m_levelInfo[p_level].Enabled;
}

//////////////////////////////////////////////////////////////////////////
//Help methods
//////////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////////
//Parse console commands
//////////////////////////////////////////////////////////////////////////
void Logging::ParseCommand( std::stringstream* p_data )
{
	std::string module;
	std::string value;

	std::getline(*p_data, module, ' ');
	std::getline(*p_data, module, ' ');

	if(module == "help")
	{
		LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "[LOGGING COMMANDS]");
		LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "/l clear - Clear log");
		LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "/l [on/off] - Turn logging on or off");
		LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "/l status - Print status of log prints and levels");
		LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "/l [LogLevel] [0/1] - Activate or deactivate a specific log level");
		LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "/l [LogTag] [0/1] - Activate or deactivate a specific log tag");
	}
	else if(module == "clear")
	{
		LogText(LogTag::NOTAG, LogLevel::NOLEVEL, "Cleared console log!");
		ClearLog();
	}
	else if(module == "on")
	{
		m_enableLogging = true;
		LogText(LogTag::NOTAG, LogLevel::NOLEVEL, "Enabled console logging");
	}
	else if(module == "off")
	{
		LogText(LogTag::NOTAG, LogLevel::NOLEVEL, "Disabled console logging");
		m_enableLogging = false;
	}
	else if(module == "status" || module == "s")
	{
		PrintStatus();
	}
	else if(module == "NOLEVEL" || module == "NOTAG" || module == "HELP_PRINT")
	{
	}
	else
	{
		for(auto itr = m_levelInfo.begin(); itr != m_levelInfo.end(); itr++)
		{
			std::string checkName = itr->second.Name;
			std::size_t found = checkName.find_last_not_of(" ");

			if (found != std::string::npos)
				checkName.erase(found+1);
			else
				checkName.clear(); // checkName is all whitespace

			if(module == checkName)
			{	
				std::getline(*p_data, value, ' ');
				itr->second.Enabled = (atoi(value.c_str()) == 1);
				LogText(LogTag::NOTAG, LogLevel::NOLEVEL, "Set %s to %s", checkName.c_str(), value.c_str());
				return;
			}
		}

		for(auto itr = m_tagInfo.begin(); itr != m_tagInfo.end(); itr++)
		{
			std::string checkName = itr->second.Name;
			std::size_t found = checkName.find_last_not_of(" ");

			if (found != std::string::npos)
				checkName.erase(found+1);
			else
				checkName.clear(); // checkName is all whitespace

			if(module == checkName)
			{	
				std::getline(*p_data, value, ' ');
				itr->second.Enabled = (atoi(value.c_str()) == 1);
				LogText(LogTag::NOTAG, LogLevel::NOLEVEL, "Set %s to %s", checkName.c_str(), value.c_str());
				return;
			}
		}
	}
}

void Logging::PrintStatus()
{
	LogText(LogTag::NOTAG, LogLevel::NOLEVEL, "LogLevel status");
	for(auto itr = m_levelInfo.begin(); itr != m_levelInfo.end(); itr++)
	{
		LogText(LogTag::NOTAG, LogLevel::NOLEVEL, "\t\t[%s] : %d", itr->second.Name.c_str(), (int)itr->second.Enabled);
	}
	LogText(LogTag::NOTAG, LogLevel::NOLEVEL, "LogTag status");
	for(auto itr = m_tagInfo.begin(); itr != m_tagInfo.end(); itr++)
	{
		LogText(LogTag::NOTAG, LogLevel::NOLEVEL, "\t\t[%s] : %d", itr->second.Name.c_str(), (int)itr->second.Enabled);
	}
}

void Logging::ClearLog()
{
	system("cls");
}
