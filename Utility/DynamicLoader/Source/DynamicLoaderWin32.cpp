#include <DynamicLoader.h>
#include <Windows.h>

/** Windows Implementation */
namespace DynamicLoader
{
	void* LoadSharedLibrary(const char* filename)
	{
		return (void*) LoadLibraryA(filename);
	}

	void FreeSharedLibrary(void* library)
	{
		FreeLibrary((HMODULE)library);
	}

	void* LoadProcess(void* library, char* name)
	{
		return GetProcAddress((HMODULE)library, name);
	}

	const char* GetLastError()
	{
		return nullptr;
	}
}