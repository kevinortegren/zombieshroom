#include "guiFileInterface.h"
#include <stdio.h>

guiFileInterface::guiFileInterface(void)
{
}


guiFileInterface::~guiFileInterface(void)
{

}

// Opens a file.
Rocket::Core::FileHandle guiFileInterface::Open(const Rocket::Core::String& path)
{
	// Attempt to open the file relative to the application's root.
	FILE* fp = fopen((root + path).CString(), "rb");
	if (fp != NULL)
		return (Rocket::Core::FileHandle) fp;

	// Attempt to open the file relative to the current working directory.
	fp = fopen(path.CString(), "rb");
	return (Rocket::Core::FileHandle) fp;
}

// Closes a previously opened file.
void guiFileInterface::Close(Rocket::Core::FileHandle file)
{
	fclose((FILE*) file);
}

// Reads data from a previously opened file.
size_t guiFileInterface::Read(void* buffer, size_t size, Rocket::Core::FileHandle file)
{
	return fread(buffer, 1, size, (FILE*) file);
}

// Seeks to a point in a previously opened file.
bool guiFileInterface::Seek(Rocket::Core::FileHandle file, long offset, int origin)
{
	return fseek((FILE*) file, offset, origin) == 0;
}

// Returns the current position of the file pointer.
size_t guiFileInterface::Tell(Rocket::Core::FileHandle file)
{
	return ftell((FILE*) file);
}