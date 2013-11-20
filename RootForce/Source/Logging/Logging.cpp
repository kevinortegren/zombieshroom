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

void Logging::LogTextToFile( string p_output )
{
	// current date/time based on current system
	time_t currentTime = time(0);

	// convert now to tm struct for UTC
	tm* gmtm = gmtime(&currentTime);
	
	string UTC = GetTimeString(gmtm->tm_hour+1) + ":" + GetTimeString(gmtm->tm_min) + ":" + GetTimeString(gmtm->tm_sec);
	string output = UTC + "    " + p_output + "\n";

	if(m_logFileStream.is_open() || m_logFileStream.bad())
	{
		m_logFileStream << output;
	}
	
}

bool Logging::OpenLogStream()
{
	// current date/time based on current system
	time_t currentTime = time(0);

	// convert now to tm struct for UTC
	tm* gmtm = gmtime(&currentTime);
	string fileName = to_string(gmtm->tm_year+1900) + to_string(gmtm->tm_mon+1) + to_string(gmtm->tm_mday) + "_" + GetTimeString(gmtm->tm_hour+1) + "-" + GetTimeString(gmtm->tm_min) + "-" + GetTimeString(gmtm->tm_sec) + ".rlog";
	m_logFileStream.open(fileName);

	return true;
}

bool Logging::CloseLogStream()
{
	if(m_logFileStream.is_open())
	{
		m_logFileStream.close();
		return true;
	}
	return false;
}

std::string Logging::GetTimeString( int p_time )
{
	if(p_time < 10)
		return "0" + to_string(p_time);
	else
		return to_string(p_time);
}

