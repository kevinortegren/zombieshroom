#include "Logging/Logging.h"
#include <ctime>

Logging::Logging()
	:m_verboseLevel(LogLevel::DEBUG_PRINT), m_defaultVerbose(LogLevel::DEBUG_PRINT)
{
	m_stringTagList.push_back("RENDER");
	m_stringTagList.push_back("NETWOR");
	m_stringTagList.push_back("DEBUG ");
	m_stringTagList.push_back("GNERAL");
	m_stringTagList.push_back("NOTAG ");
	m_stringTagList.push_back("PHYSIC");
	m_stringTagList.push_back("TOOLS ");
	m_stringTagList.push_back("SOUND ");
	m_stringTagList.push_back("GAME  ");
	m_stringTagList.push_back("CMPSYS");
	m_stringTagList.push_back("ERROR ");
	m_stringTagList.push_back("GUI   ");

	m_stringLevelList.push_back("FATAL_ERR  ");
	m_stringLevelList.push_back("NON_FAT_ERR");
	m_stringLevelList.push_back("WARNING    ");
	m_stringLevelList.push_back("DEBUG_PRINT");
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
	std::string fileName = std::to_string(gmtm.tm_year+1900) + std::to_string(gmtm.tm_mon+1) + std::to_string(gmtm.tm_mday) + "_" + GetTimeString(gmtm.tm_hour+1) + "-" + GetTimeString(gmtm.tm_min) + "-" + GetTimeString(gmtm.tm_sec) + ".rlog";
	//Open log file stream
	fopen_s(&m_logFile, fileName.c_str(), "w");

	if(m_logFile == NULL)
		return false;

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

void Logging::LogTextToFile(LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel, const char* p_format, ... )
{
	va_list args;
	va_start (args, p_format);
	if(p_vLevel <= m_verboseLevel && CheckTag(p_tag))
			WriteToFile(p_tag, p_vLevel, p_format, args);
	va_end (args);
}

void Logging::LogTextToFile( const char * p_format, ... )
{
	va_list args;
	va_start (args, p_format);
	if(m_defaultVerbose <= m_verboseLevel && CheckTag(LogTag::NOTAG))
		WriteToFile(LogTag::NOTAG, m_defaultVerbose, p_format, args);
	va_end (args);
}

void Logging::LogTextToConsole(LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel, const char* p_format, ... )
{
	va_list args;
	va_start (args, p_format);
	if(p_vLevel <= m_verboseLevel && CheckTag(p_tag))
		WriteToConsole(p_tag, p_vLevel, p_format, args);
	va_end (args);
}

void Logging::LogTextToConsole( const char * p_format, ... )
{
	va_list args;
	va_start (args, p_format);
	if(m_defaultVerbose <= m_verboseLevel && CheckTag(LogTag::NOTAG))
		WriteToConsole(LogTag::NOTAG, m_defaultVerbose, p_format, args);
	va_end (args);
}

void Logging::LogText( LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel, const char* p_format, ... )
{
	va_list args;
	va_start (args, p_format);
	if(p_vLevel <= m_verboseLevel && CheckTag(p_tag))
	{
		WriteToConsole(p_tag, p_vLevel, p_format, args);
		WriteToFile(p_tag, p_vLevel, p_format, args);
	}
	va_end (args);
}

void Logging::LogText( const char* p_format, ... )
{
	va_list args;
	va_start (args, p_format);
	if(m_defaultVerbose <= m_verboseLevel && CheckTag(LogTag::NOTAG))
	{
		WriteToConsole(LogTag::NOTAG, m_defaultVerbose, p_format, args);
		WriteToFile(LogTag::NOTAG, m_defaultVerbose, p_format, args);
	}
	va_end (args);
}

void Logging::SetVerboseLevel( LogLevel::LogLevel p_vLevel )
{
	m_verboseLevel = p_vLevel;
	LogTextToConsole(LogTag::NOTAG, LogLevel::DEBUG_PRINT, "Verbose level set to %d", (int)p_vLevel);
	LogTextToFile(LogTag::NOTAG, LogLevel::DEBUG_PRINT, "Verbose level set to %d", (int)p_vLevel);
}

void Logging::AddExclusiveTags( LogTag::LogTag p_tag)
{
	m_exTagList.push_back(p_tag);
}

void Logging::WriteToFile(LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel,std::string p_format, va_list p_args )
{
	
	std::string output = GetTimeFormatString() + "    " + GetStringFromTag(p_tag) + "    " + GetStringFromLevel(p_vLevel) +  "    " + p_format + "\n";

	vfprintf (m_logFile, output.c_str(), p_args);
	fflush(m_logFile);
}

void Logging::WriteToConsole(LogTag::LogTag p_tag, LogLevel::LogLevel p_vLevel, std::string p_format, va_list p_args )
{
	
	std::string output = GetTimeFormatString() + "    " + GetStringFromTag(p_tag) + "    " + GetStringFromLevel(p_vLevel) +  "    " + p_format + "\n";

	vprintf(output.c_str(), p_args);
}

std::string Logging::GetTimeFormatString()
{
	time_t currentTime = time(0);

	tm gmtm;

	gmtime_s(&gmtm, &currentTime);

	return GetTimeString(gmtm.tm_hour+1) + ":" + GetTimeString(gmtm.tm_min) + ":" + GetTimeString(gmtm.tm_sec);
}

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

std::string Logging::GetStringFromTag( LogTag::LogTag p_tag )
{
	return m_stringTagList.at(p_tag);
}

std::string Logging::GetStringFromLevel( LogLevel::LogLevel p_level )
{
	return m_stringLevelList.at(p_level);
}

//DEPRECATED FUNCTIONS, PLEASE USE LogLevel:: INSTEAD OF unsigned int
void Logging::LogTextToFile( LogTag::LogTag p_tag, unsigned int p_vLevel, const char* p_format, ... )
{
	va_list args;
	va_start (args, p_format);
	if((int)p_vLevel <= m_verboseLevel && CheckTag(p_tag))
		WriteToFile(p_tag, (LogLevel::LogLevel)p_vLevel, p_format, args);
	va_end (args);
}

void Logging::LogTextToConsole( LogTag::LogTag p_tag, unsigned int p_vLevel, const char* p_format, ... )
{
	va_list args;
	va_start (args, p_format);
	if((int)p_vLevel <= m_verboseLevel && CheckTag(p_tag))
		WriteToConsole(p_tag, (LogLevel::LogLevel)p_vLevel, p_format, args);
	va_end (args);
}

void Logging::LogText( LogTag::LogTag p_tag, unsigned int p_vLevel, const char* p_format, ... )
{
	va_list args;
	va_start (args, p_format);
	if((int)p_vLevel <= m_verboseLevel && CheckTag(p_tag))
	{
		WriteToConsole(p_tag, (LogLevel::LogLevel)p_vLevel, p_format, args);
		WriteToFile(p_tag, (LogLevel::LogLevel)p_vLevel, p_format, args);
	}
	va_end (args);
}



