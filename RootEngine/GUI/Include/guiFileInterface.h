#pragma once

#include <Rocket\Core\String.h>
#include <Rocket\Core\FileInterface.h>

class guiFileInterface : public Rocket::Core::FileInterface
{
public:
	guiFileInterface(void);
	~guiFileInterface(void);

	/// Opens a file.		
	virtual Rocket::Core::FileHandle Open(const Rocket::Core::String& path);

	/// Closes a previously opened file.		
	virtual void Close(Rocket::Core::FileHandle file);

	/// Reads data from a previously opened file.		
	virtual size_t Read(void* buffer, size_t size, Rocket::Core::FileHandle file);

	/// Seeks to a point in a previously opened file.		
	virtual bool Seek(Rocket::Core::FileHandle file, long offset, int origin);

	/// Returns the current position of the file pointer.		
	virtual size_t Tell(Rocket::Core::FileHandle file);

private:
	Rocket::Core::String root;
};


