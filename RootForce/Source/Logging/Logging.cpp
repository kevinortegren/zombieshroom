#include "Logging/Logging.h"
#include <ctime>

Logging* Logging::s_loggingInstance = nullptr;

Logging::Logging(){}

Logging::~Logging()
{
	CloseLogStream();
}

Logging* Logging::GetInstance()
{
	if(!s_loggingInstance)
	{
		s_loggingInstance = new Logging();
	}

	return s_loggingInstance;
}

void Logging::LogTextToFile( const char * p_format, ... )
{
	va_list args;
	va_start (args, p_format);

	// current date/time based on current system
	time_t currentTime = time(0);

	tm gmtm;
	// convert now to tm struct for UTC
	gmtime_s(&gmtm, &currentTime);
	//Generate time string
	std::string UTC = GetTimeString(gmtm.tm_hour+1) + ":" + GetTimeString(gmtm.tm_min) + ":" + GetTimeString(gmtm.tm_sec);
	//Generate ouput string (time stamp + output text)
	std::string output = UTC + "    " + p_format + "\n";

	vfprintf (m_logFile, output.c_str(), args);
	fflush(m_logFile);
	va_end (args);
}

void Logging::LogTextToConsole( const char * p_format, ... )
{
	va_list args;
	va_start (args, p_format);
	// current date/time based on current system
	time_t currentTime = time(0);

	tm gmtm; 
	// convert now to tm struct for UTC
	gmtime_s(&gmtm, &currentTime);

	std::string UTC = GetTimeString(gmtm.tm_hour+1) + ":" + GetTimeString(gmtm.tm_min) + ":" + GetTimeString(gmtm.tm_sec);
	std::string output = UTC + "    " + p_format + "\n";

	vprintf(output.c_str(), args);
	va_end (args);
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

