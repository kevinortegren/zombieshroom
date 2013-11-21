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
		char* str = "";

		DWORD error = ::GetLastError();
		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
			0,
			error,
			0,
			(LPSTR)&str,
			0,
			0);

		return str;
	}
}