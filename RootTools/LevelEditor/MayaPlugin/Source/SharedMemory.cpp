#include "SharedMemory.h"
#include <string>

SharedMemory::SharedMemory()
{
	NumberOfMeshes = nullptr;
	NumberOfLights = nullptr;	//STRUNTAT I HUR MÅNGA KAMEROR ATM KÖR PÅ KAMERA 0;
	NumberOfCameras = nullptr;
	NumberOfMaterials = nullptr;
	export = nullptr;
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
	total_memory_size += sizeof(Material) * g_maxMeshes;
	total_memory_size += sizeof(int) * 4;
	total_memory_size += sizeof(glm::vec2) * 3;
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

	export = (int*)(mem);
	

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
	MeshIdChange->x = index;
	ReleaseMutex(IdMutexHandle);

	MeshMutexHandle = CreateMutex(nullptr, false, L"MeshMutex");
	WaitForSingleObject(MeshMutexHandle, milliseconds);

	memset(PmeshList[index]->transformation.name, NULL, sizeof(PmeshList[index]->transformation.name));
	memset(PmeshList[index]->modelName, NULL, sizeof(PmeshList[index]->modelName));
	//memset(PmeshList[index]->materialName, NULL, sizeof(PmeshList[index]->materialName));

	memcpy(PmeshList[index]->transformation.name, meshList[index].transformation.name, g_maxNameLength);
	memcpy(PmeshList[index]->modelName, meshList[index].modelName, g_maxNameLength);
	//memcpy(PmeshList[index]->materialName, meshList[index].materialName, g_maxNameLength);

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
		PmeshList[index]->transformation.position = meshList[index].transformation.position;
		PmeshList[index]->transformation.scale = meshList[index].transformation.scale;
		PmeshList[index]->transformation.rotation = meshList[index].transformation.rotation;
		PmeshList[index]->transformation.rotPivot = meshList[index].transformation.rotPivot;
		PmeshList[index]->transformation.scalePivot = meshList[index].transformation.scalePivot;
	}

	ReleaseMutex(MeshMutexHandle);
}

void SharedMemory::UpdateSharedMaterials(int nrOfMaterials, int meshID)
{
	MeshMutexHandle = CreateMutex(nullptr, false, L"MeshMutex");
	WaitForSingleObject(MeshMutexHandle, milliseconds);

	for(int i = 0; i < nrOfMaterials; i++)
	{
		memcpy(PmaterialList[i]->materialName, materialList[i].materialName, g_maxNameLength);
		memcpy(PmaterialList[i]->texturePath, materialList[i].texturePath, g_maxPathLength);
		memcpy(PmaterialList[i]->normalPath, materialList[i].normalPath, g_maxPathLength);
	}

	*NumberOfMaterials = nrOfMaterials;		

	if(meshID != -1)
	{
		memcpy(PmeshList[meshID]->materialName, meshList[meshID].materialName, g_maxNameLength);

		IdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
		WaitForSingleObject(IdMutexHandle, milliseconds);
		MeshIdChange->x = meshID;
		PmeshList[meshID]->MaterialID = meshList[meshID].MaterialID;
		ReleaseMutex(IdMutexHandle);
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
