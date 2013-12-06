#include "SharedMemory.h"

SharedMemory::SharedMemory()
{
	NumberOfMeshes = nullptr;
	NumberOfLights = nullptr;	//STRUNTAT I HUR MÅNGA KAMEROR ATM KÖR PÅ KAMERA 0;
	NumberOfCameras = nullptr;
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
	total_memory_size += sizeof(Light) * g_maxLights;
	total_memory_size += sizeof(Camera) * g_maxCameras;
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
	for(int i = 0; i < g_maxLights; i++)			//ADDED
	{
		PlightList[i] = ((Light*)raw_data) + i ;
	}
	for(int i = 0; i < g_maxCameras; i++)			//ADDED
	{
		PcameraList[i] = ((Camera*)raw_data) + i ;
	}

	unsigned char* mem = (unsigned char*)raw_data;
	NumberOfMeshes = (int*)(mem + sizeof(Mesh) * g_maxMeshes);
	mem = (unsigned char*)NumberOfMeshes;
	MeshIdChange = (int*)(mem + sizeof(int));
	CameraIdChange = (int*)(MeshIdChange + sizeof(int));
	LightIdChange = (int*)(CameraIdChange + sizeof(int));
	unsigned char* mem2 = (unsigned char*)LightIdChange;
	NumberOfLights = (int*)(mem2 + sizeof(Light) * g_maxLights);
	mem2 = (unsigned char*)NumberOfLights;
	unsigned char* mem3 = (unsigned char*)CameraIdChange;
	NumberOfCameras = (int*)(mem3 + sizeof(Camera) * g_maxCameras);
	mem3 = (unsigned char*)NumberOfCameras;

	if(first_process)
	{
		memset(raw_data,0,total_memory_size);
	}
	return 0;
}

void SharedMemory::UpdateSharedLight(int index, int nrOfLights)	
{
	LightIdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
	WaitForSingleObject(LightIdMutexHandle, milliseconds);

	*NumberOfLights = nrOfLights;
	*LightIdChange = index;

	ReleaseMutex(LightIdMutexHandle);

	LightMutexHandle = CreateMutex(nullptr, false, L"LightMutex");
	WaitForSingleObject(LightMutexHandle, milliseconds);

		memcpy(PlightList[index]->transformation.name, lightList[index].transformation.name, 50);
		PlightList[index]->transformation.position = lightList[index].transformation.position;
		PlightList[index]->transformation.scale = lightList[index].transformation.scale;
		PlightList[index]->transformation.rotation = lightList[index].transformation.rotation;

	ReleaseMutex(LightMutexHandle);
}

void SharedMemory::UpdateSharedCamera(int index)		
{
	CameraIdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
	WaitForSingleObject(CameraIdMutexHandle, milliseconds);

	*CameraIdChange = index;

	ReleaseMutex(CameraIdMutexHandle);

	CameraMutexHandle = CreateMutex(nullptr, false, L"CameraMutex");
	WaitForSingleObject(CameraMutexHandle, milliseconds);

		memcpy(PcameraList[index]->transformation.name, cameraList[index].transformation.name, 50); 
		PcameraList[index]->transformation.position = cameraList[index].transformation.position;
		PcameraList[index]->transformation.rotation = cameraList[index].transformation.rotation;

	ReleaseMutex(CameraMutexHandle);
}

void SharedMemory::UpdateSharedMesh(int index, bool updateTransformation, bool updateVertex, int nrOfMeshes)
{
	MeshIdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
	WaitForSingleObject(MeshIdMutexHandle, milliseconds);
	*NumberOfMeshes = nrOfMeshes;
	*MeshIdChange = index;	//Moved this codeblock up, didnt change shit.

	ReleaseMutex(MeshIdMutexHandle);

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
	memcpy(PmeshList[index]->texturePath, meshList[index].texturePath, 100);		//Ligger pointlight här av någon annledning. DERRRP.
	memcpy(PmeshList[index]->normalPath, meshList[index].normalPath, 100);
	//PmeshList[index]->texturePath = meshList[index].texturePath;
	//PmeshList[index]->normalPath = meshList[index].normalPath;
	

	ReleaseMutex(MeshMutexHandle);
}

int SharedMemory::shutdown()
{
	UnmapViewOfFile(raw_data);
	CloseHandle(shared_memory_handle);
	CloseHandle(MeshMutexHandle);
	CloseHandle(MeshIdMutexHandle);
	CloseHandle(LightMutexHandle);
	CloseHandle(CameraMutexHandle);
	return 0;
}
