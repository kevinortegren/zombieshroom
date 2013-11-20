#pragma once

#include <string>
using namespace std;

class Logging
{
	public:
		static Logging* GetInstance();
		void LogTextToFile(string p_output);
	protected:
	private:
		Logging();
		~Logging();

		static Logging* s_loggingInstance;
};
