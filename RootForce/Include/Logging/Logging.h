#pragma once

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class Logging
{
	public:
		static Logging* GetInstance();
		void LogTextToFile(string p_output);
		void LogTextToConsole(string p_output);
		bool OpenLogStream();
		bool CloseLogStream();
	protected:
	private:
		Logging();
		~Logging();

		static Logging* s_loggingInstance;

		ofstream m_logFileStream;

		string GetTimeString(int p_time);


};
