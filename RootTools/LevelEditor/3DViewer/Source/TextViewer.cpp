#include "stdafx.h"
#include "ReadMemory.h"

ReadMemory RM;
bool DERP = false;
int main()
{
	RM.InitalizeSharedMemory();

	while (DERP == false)
	{
		RM.Read();
	}
	return 0;
}

