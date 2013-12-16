#include "SharedMemory.h"
#include <string>

SharedMemory::SharedMemory()
{
	NumberOfMeshes = nullptr;
	NumberOfLights = nullptr;	//STRUNTAT I HUR MÅNGA KAMEROR ATM KÖR PÅ KAMERA 0;
	NumberOfCameras = nullptr;
	NumberOfMaterials = nullptr;
	InitalizeSharedMemory();
	//export = nullptr;
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
	total_memory_size += sizeof(Material) * g_maxMeshes;
	total_memory_size += sizeof(int) * 4;
	total_memory_size += sizeof(glm::vec2) * 3;
	total_memory_size += sizeof(bool);
	


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

	export = (bool*)(mem);
	

	//if(first_process)
	//{
		memset(raw_data,0,total_memory_size);
	//}
	return 0;
}

void SharedMemory::UpdateSharedLight(int index, int nrOfLights)	
{
	IdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
	WaitForSingleObject(IdMutexHandle, milliseconds);

	*NumberOfLights = nrOfLights;
	*LightIdChange = glm::vec2(index,-1);

	ReleaseMutex(IdMutexHandle);

	LightMutexHandle = CreateMutex(nullptr, false, L"LightMutex");
	WaitForSingleObject(LightMutexHandle, milliseconds);

		memcpy(PlightList[index]->transformation.name, lightList[index].transformation.name, 50);
		PlightList[index]->transformation.position = lightList[index].transformation.position;
		PlightList[index]->transformation.scale = lightList[index].transformation.scale;
		PlightList[index]->transformation.rotation = lightList[index].transformation.rotation;
		PlightList[index]->color = lightList[index].color;
		PlightList[index]->Intensity = lightList[index].Intensity;

	ReleaseMutex(LightMutexHandle);
}

void SharedMemory::UpdateSharedCamera(int index)		
{
	IdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
	WaitForSingleObject(IdMutexHandle, milliseconds);

	*CameraIdChange = glm::vec2(index,-1);

	ReleaseMutex(IdMutexHandle);

	CameraMutexHandle = CreateMutex(nullptr, false, L"CameraMutex");
	WaitForSingleObject(CameraMutexHandle, milliseconds);

		memcpy(PcameraList[index]->transformation.name, cameraList[index].transformation.name, 50); 
		PcameraList[index]->transformation.position = cameraList[index].transformation.position;
		PcameraList[index]->transformation.rotation = cameraList[index].transformation.rotation;
		PcameraList[index]->horizontalFieldOfView = cameraList[index].horizontalFieldOfView;
		PcameraList[index]->verticalFieldOfView = cameraList[index].verticalFieldOfView;
		PcameraList[index]->nearClippingPlane = cameraList[index].nearClippingPlane;
		PcameraList[index]->farClippingPlane = cameraList[index].farClippingPlane;

	ReleaseMutex(CameraMutexHandle);
}

void SharedMemory::UpdateSharedMesh(int index, bool updateTransformation, bool updateVertex, int nrOfMeshes)
{
	IdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
	WaitForSingleObject(IdMutexHandle, milliseconds);
	*NumberOfMeshes = nrOfMeshes;
	MeshIdChange->x = index;	//Moved this codeblock up, didnt change shit.

	ReleaseMutex(IdMutexHandle);

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
	memcpy(PmeshList[index]->transformation.name, meshList[index].transformation.name, 30);
	if(updateTransformation)
	{
		//memcpy(PmeshList[index]->transformation.name, meshList[index].transformation.name, 30); // DONT HAVE THE CORRECT LENGHT
		PmeshList[index]->transformation.position = meshList[index].transformation.position;
		PmeshList[index]->transformation.scale = meshList[index].transformation.scale;
		PmeshList[index]->transformation.rotation = meshList[index].transformation.rotation;
	}


	//memcpy(PmeshList[index]->texturePath, meshList[index].texturePath, 100);		//Ligger pointlight här av någon annledning. DERRRP.
	//memcpy(PmeshList[index]->normalPath, meshList[index].normalPath, 100);
	
	//PmeshList[index]->texturePath = meshList[index].texturePath;
	//PmeshList[index]->normalPath = meshList[index].normalPath;
	

	ReleaseMutex(MeshMutexHandle);
}

void SharedMemory::UpdateSharedMaterials(int nrOfMaterials, int materialID, int meshID)
{
	MeshMutexHandle = CreateMutex(nullptr, false, L"MeshMutex");
	WaitForSingleObject(MeshMutexHandle, milliseconds);

	*NumberOfMaterials = nrOfMaterials;
	PmeshList[meshID]->MaterialID = materialID;

	for(int i = 0; i < nrOfMaterials; i++)
	{
		memcpy(PmaterialList[i]->materialName, materialList[i].materialName, 30);
		memcpy(PmaterialList[i]->texturePath, materialList[i].texturePath, 100);
		memcpy(PmaterialList[i]->normalPath, materialList[i].normalPath, 100);
	}

	ReleaseMutex(MeshMutexHandle);
}

void SharedMemory::RemoveMesh(int id, int nrOfMeshes)
{
	IdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
	WaitForSingleObject(IdMutexHandle, milliseconds);

	MeshIdChange->y = id;
	*NumberOfMeshes = nrOfMeshes;

	ReleaseMutex(IdMutexHandle);
}

void SharedMemory::RemoveLight(int id, int nrOfLights)
{
	IdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
	WaitForSingleObject(IdMutexHandle, milliseconds);

	LightIdChange->y = id;
	*NumberOfLights = nrOfLights;

	ReleaseMutex(IdMutexHandle);
}

int SharedMemory::shutdown()
{
	UnmapViewOfFile(raw_data);
	CloseHandle(shared_memory_handle);
	CloseHandle(MeshMutexHandle);
	CloseHandle(IdMutexHandle);
	CloseHandle(LightMutexHandle);
	CloseHandle(CameraMutexHandle);
	return 0;
}
