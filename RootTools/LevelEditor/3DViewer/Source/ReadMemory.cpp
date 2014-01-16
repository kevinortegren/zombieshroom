#include "ReadMemory.h"

ReadMemory::ReadMemory()
{
	NumberOfMeshes = nullptr;
	NumberOfLights = nullptr;
	NumberOfCameras = nullptr;
	export = nullptr;
	InitalizeSharedMemory();
}

ReadMemory::~ReadMemory()
{
	shutdown();
}

int ReadMemory::InitalizeSharedMemory()
{
	int total_memory_size = 0;
	
	total_memory_size += sizeof(Mesh) * g_maxMeshes;
	total_memory_size += sizeof(Light) * g_maxLights;
	total_memory_size += sizeof(Camera) * g_maxCameras;
	total_memory_size += sizeof(Material) * g_maxMeshes;
	total_memory_size += sizeof(Locator) * g_maxLocators;

	total_memory_size += sizeof(int) * 5;
	total_memory_size += sizeof(glm::vec2) * 4;
	total_memory_size += sizeof(int);
	


	shared_memory_handle = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		nullptr,
		PAGE_READWRITE,
		0,
		total_memory_size,
		L"MySharedMemory");

	bool first_process=(GetLastError() !=ERROR_ALREADY_EXISTS);

	raw_data = (unsigned char*)MapViewOfFile(
		shared_memory_handle,
		FILE_MAP_ALL_ACCESS,
		0,0,0);

	for(int i = 0; i < g_maxMeshes; i++)
	{
		PmeshList[i] = ((Mesh*)raw_data) + i ;
	}

	unsigned char* mem = (unsigned char*)(raw_data + sizeof(Mesh) * g_maxMeshes);
	NumberOfMeshes = (int*)(mem);
	mem = (unsigned char*)(NumberOfMeshes + sizeof(int));

	MeshIdChange = (glm::vec2*)(mem);
	mem = (unsigned char*)(MeshIdChange + sizeof(glm::vec2));

	for(int i = 0; i < g_maxLights; i++)
	{
		PlightList[i] = ((Light*)mem) + i ;
	}
	mem = (unsigned char*)(mem + sizeof(Light) * g_maxLights);

	NumberOfLights = (int*)(mem);
	mem = (unsigned char*)(mem + sizeof(int));

	LightIdChange = (glm::vec2*)(mem);
	mem = (unsigned char*)(mem + sizeof(glm::vec2));

	for(int i = 0; i < g_maxCameras; i++)
	{
		PcameraList[i] = ((Camera*)mem) + i ;
	}
	mem = (unsigned char*)(mem + sizeof(Camera) * g_maxCameras);

	CameraIdChange = (glm::vec2*)(mem);
	mem = (unsigned char*)(mem + sizeof(glm::vec2));

	NumberOfCameras = (int*)(mem);
	mem = (unsigned char*)(mem + sizeof(int));

	for(int i = 0; i < g_maxMeshes; i++)
	{
		PmaterialList[i] = ((Material*)mem) + i ;
	}
	mem = (unsigned char*)(mem + sizeof(Material) * g_maxMeshes);

	NumberOfMaterials = (int*)(mem);

	mem = (unsigned char*)(mem + sizeof(int));

	for(int i = 0; i < g_maxLocators; i++)
	{
		PlocatorList[i] = ((Locator*)mem) + i ;
	}
	mem = (unsigned char*)(mem + sizeof(Locator) * g_maxLocators);

	LocatorIdChange = (glm::vec2*)(mem);
	mem = (unsigned char*)(mem + sizeof(glm::vec2));

	NumberOfLocators = (int*)(mem);

	mem = (unsigned char*)(mem + sizeof(int));

	export = (int*)(mem);
	

	if(first_process)
	{
		memset(raw_data,0,total_memory_size);
	}
	return 0;
}

int ReadMemory::shutdown()
{
	UnmapViewOfFile(raw_data);
	CloseHandle(shared_memory_handle);
	CloseHandle(MeshMutexHandle);
	CloseHandle(IdMutexHandle);
	CloseHandle(LightMutexHandle);
	CloseHandle(CameraMutexHandle);
	return 0;
}
