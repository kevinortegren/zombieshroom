#include "Logging.h"

Logging* Logging::s_loggingInstance = nullptr;

Logging::Logging(){}

Logging::~Logging(){}

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

}
