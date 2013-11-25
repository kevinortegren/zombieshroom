#pragma once

namespace DynamicLoader
{
	/** Load a dynamic library. */
	void* LoadSharedLibrary(const char* filename);

	/** Free a loaded dynamic library. */
	void FreeSharedLibrary(void* library);

	/** Load a process from a loaded library. */
	void* LoadProcess(void* library, char* name);

	/** Get the last error produced. */
	const char* GetLastError();
}