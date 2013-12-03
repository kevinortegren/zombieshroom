#include "SharedMemory.h"

SharedMemory::SharedMemory()
{
	NumberOfMeshes = nullptr;
	InitalizeSharedMemory();
}

SharedMemory::~SharedMemory()
{
	shutdown();
}

int SharedMemory::InitalizeSharedMemory()
{
	int total_memory_size = 0;
	
	total_memory_size += sizeof(Mesh) * g_maxMeshes;
	//total_memory_size += sizeof(Camera) * g_maxCameras;
	//total_memory_size += sizeof(Light) * g_maxLights;
	total_memory_size += sizeof(int) * 3;


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

	unsigned char* mem = (unsigned char*)raw_data;
	NumberOfMeshes = (int*)(mem + sizeof(Mesh) * g_maxMeshes);
	mem = (unsigned char*)NumberOfMeshes;
	MeshIdChange = (int*)(mem + sizeof(int));
	CameraIdChange = (int*)(MeshIdChange + sizeof(int));
	LightIdChange = (int*)(CameraIdChange + sizeof(int));

	if(first_process)
	{
		memset(raw_data,0,total_memory_size);
	}
	return 0;
}

void SharedMemory::UpdateSharedMesh(int index, bool updateTransformation, bool updateVertex, int nrOfMeshes)			//AINT CALLING FOR SHIET
{
	MeshMutexHandle = CreateMutex(nullptr, false, L"MeshMutex");
	WaitForSingleObject(MeshMutexHandle, milliseconds);

	if(updateVertex)
	{
		for(int i = 0; i < meshList[index].nrOfVertices; i ++)
		{
		PmeshList[index]->vertex[i] = meshList[index].vertex[i];
		PmeshList[index]->normal[i] = meshList[index].normal[i];
		PmeshList[index]->UV[i] = meshList[index].UV[i];
		}

		PmeshList[index]->nrOfVertices = meshList[index].nrOfVertices;
	}	

	if(updateTransformation)
	{
		memcpy(PmeshList[index]->transformation.name, meshList[index].transformation.name, 30); // DONT HAVE THE CORRECT LENGHT
		PmeshList[index]->transformation.position = meshList[index].transformation.position;
		PmeshList[index]->transformation.scale = meshList[index].transformation.scale;
		PmeshList[index]->transformation.rotation = meshList[index].transformation.rotation;
	}
	
	PmeshList[index]->texturePath = meshList[index].texturePath;
	PmeshList[index]->normalPath = meshList[index].normalPath;
	

	ReleaseMutex(MeshMutexHandle);

	IdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
	WaitForSingleObject(IdMutexHandle, milliseconds);
	*NumberOfMeshes = nrOfMeshes;
	*MeshIdChange = index;

	ReleaseMutex(IdMutexHandle);
}

int SharedMemory::shutdown()
{
	UnmapViewOfFile(raw_data);
	CloseHandle(shared_memory_handle);
	//CloseHandle(MeshMutexHandle);
	//CloseHandle(IdMutexHandle);
	return 0;
}
