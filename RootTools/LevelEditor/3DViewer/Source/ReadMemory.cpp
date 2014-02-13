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
	total_memory_size += sizeof(UpdateMessage) * g_maxMessages;
	total_memory_size += sizeof(PaintTexture) * g_maxPaintTextures;
	total_memory_size += sizeof(int) * 8; //NumberOfStuffs
	total_memory_size += sizeof(WorldData);
	total_memory_size += sizeof(MegaMesh)*g_maxMegaMeshes;


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

	for(int i = 0; i < g_maxLights; i++)
	{
		PlightList[i] = ((Light*)mem) + i ;
	}
	mem = (unsigned char*)(mem + sizeof(Light) * g_maxLights);

	NumberOfLights = (int*)(mem);
	mem = (unsigned char*)(mem + sizeof(int));

	for(int i = 0; i < g_maxCameras; i++)
	{
		PcameraList[i] = ((Camera*)mem) + i ;
	}
	mem = (unsigned char*)(mem + sizeof(Camera) * g_maxCameras);


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

	NumberOfLocators = (int*)(mem);

	mem = (unsigned char*)(mem + sizeof(int));

	export = (int*)(mem);

	mem = (unsigned char*)(mem + sizeof(int));

	for(int i = 0; i < g_maxMessages; i++)
	{
		updateMessages[i] = ((UpdateMessage*)mem) + i ;
	}

	mem = (unsigned char*)(mem + sizeof(UpdateMessage) * g_maxMessages);

	NumberOfMessages = (int*)(mem);

	mem = (unsigned char*)(mem + sizeof(int));

	for(int i = 0; i < g_maxPaintTextures; i++)
	{
		PpaintList[i] = ((PaintTexture*)mem) + i ;
	}

	mem = (unsigned char*)(mem + sizeof(PaintTexture) * g_maxPaintTextures);

	NumberOfPaintTextures = (int*)(mem);

	mem = (unsigned char*)(mem + sizeof(int));

	worldData = (WorldData*)mem;

	mem = (unsigned char*)(mem + sizeof(WorldData));

	for(int i = 0; i < g_maxMegaMeshes; i++)
	{
		PmegaMeshes[i] = ((MegaMesh*)mem) + i ;
	}

	mem = (unsigned char*)(mem + sizeof(MegaMesh) * g_maxMegaMeshes);

	NumberOfMegaMeshes = (int*)mem;

	if(first_process)
	{
		memset(raw_data,0,total_memory_size);
	}
	return 0;
}

WorldData ReadMemory::getWorldData()
{
	LightMutexHandle = CreateMutex(nullptr, false, L"LightMutex");
	WaitForSingleObject(LightMutexHandle, milliseconds);

	return *worldData;

	ReleaseMutex(LightMutexHandle);
}

Mesh ReadMemory::getMesh(int index)
{
	Mesh mesh;
	MeshMutexHandle = CreateMutex(nullptr, false, L"MeshMutex");
	WaitForSingleObject(MeshMutexHandle, milliseconds);
	mesh = *PmeshList[index];
	ReleaseMutex(MeshMutexHandle);
	return mesh;
}

Counters ReadMemory::getCounters()
{
	Counters count;
	IdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
	WaitForSingleObject(IdMutexHandle, milliseconds);
	count.NumberOfCameras = *NumberOfCameras;
	count.NumberOfLights = *NumberOfLights;
	count.NumberOfLocators = *NumberOfLocators;
	count.NumberOfMaterials = *NumberOfMaterials;
	count.NumberOfMeshes = *NumberOfMeshes;
	count.NumberOfMessages = *NumberOfMessages;
	count.NumberOfPaintTextures = *NumberOfPaintTextures;
	count.export = *export;
	ReleaseMutex(IdMutexHandle);

	return count;
}

Camera ReadMemory::getCamera(int index)
{
	Camera cam;
	CameraMutexHandle = CreateMutex(nullptr, false, L"CameraMutex");
	WaitForSingleObject(CameraMutexHandle, milliseconds);
	cam = *PcameraList[index];
	ReleaseMutex(CameraMutexHandle);
	return cam;
}

Light ReadMemory::getLight(int index)
{
	Light light;
	LightMutexHandle = CreateMutex(nullptr, false, L"LightMutex");
	WaitForSingleObject(LightMutexHandle, milliseconds);
	light = *PlightList[index];
	ReleaseMutex(CameraMutexHandle);
	return light;
}

Locator ReadMemory::getLocator(int index)
{
	Locator loc;
	LocatorMutexHandle = CreateMutex(nullptr, false, L"LocatorMutex");
	WaitForSingleObject(LocatorMutexHandle, milliseconds);
	loc = *PlocatorList[index];
	ReleaseMutex(LocatorMutexHandle);
	return loc;
}

Material ReadMemory::getMaterial(int index)
{
	Material mat;
	MeshMutexHandle = CreateMutex(nullptr, false, L"MeshMutex");
	WaitForSingleObject(MeshMutexHandle, milliseconds);
	mat = *PmaterialList[index];
	ReleaseMutex(LocatorMutexHandle);
	return mat;
}

PaintTexture ReadMemory::getPaintTexture(int index)
{
	PaintTexture paint;
	TextureMutexHandle = CreateMutex(nullptr, false, L"TextureMutex");
	WaitForSingleObject(TextureMutexHandle, milliseconds);
	cout << PpaintList[index]->heigth << endl;
	cout << PpaintList[index]->width << endl;
	paint = *PpaintList[index];
	ReleaseMutex(TextureMutexHandle);
	return paint;
}

void ReadMemory::ReadMessage(string &out_type, int &out_updateIndex, int &out_removeIndex, bool &out_updateTransform, bool &out_updateShape)
{
	IdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
	WaitForSingleObject(IdMutexHandle, milliseconds);

	if(*NumberOfMessages > 0)
	{
		out_type = updateMessages[0]->name;
		out_updateIndex = updateMessages[0]->updateID;
		out_removeIndex = updateMessages[0]->removeID;
		out_updateTransform = updateMessages[0]->updateTransform;
		out_updateShape = updateMessages[0]->updateShape;

		for(int i = 0; i < *NumberOfMessages-1; i++)
		{
			*updateMessages[i] = *updateMessages[i+1];
		}

		*NumberOfMessages = *NumberOfMessages - 1;
	}else
	{
		out_type = "";
		out_updateIndex = -1;
		out_removeIndex = -1;
		out_updateTransform = false;
		out_updateShape = false;
	}

	ReleaseMutex(IdMutexHandle);
}

UpdateMessage ReadMemory::PeekMessageAt(int index)
{
	IdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
	WaitForSingleObject(IdMutexHandle, milliseconds);

	return *updateMessages[index];

	ReleaseMutex(IdMutexHandle);
}

void ReadMemory::ClearAllMessages()
{
	IdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
	WaitForSingleObject(IdMutexHandle, milliseconds);
	UpdateMessage mess;
	string nada = "";
	memcpy(mess.name, nada.c_str(), g_maxNameLength);
	*NumberOfMessages = 0;

	for(int i = 0; i < g_maxMessages; i++)
	{		
		*updateMessages[i] = mess;
	}

	ReleaseMutex(IdMutexHandle);
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
