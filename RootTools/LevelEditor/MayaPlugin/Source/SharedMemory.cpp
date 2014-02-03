#include "SharedMemory.h"
#include <string>

SharedMemory::SharedMemory()
{
	NumberOfMeshes = nullptr;
	NumberOfLights = nullptr;	//STRUNTAT I HUR MÅNGA KAMEROR ATM KÖR PÅ KAMERA 0;
	NumberOfCameras = nullptr;
	NumberOfMaterials = nullptr;
	NumberOfLocators = nullptr;
	NumberOfMessages = 0;
	export = 0;
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
	total_memory_size += sizeof(Locator) * g_maxLocators;
	total_memory_size += sizeof(UpdateMessage) * g_maxMessages;
	total_memory_size += sizeof(PaintTexture) * g_maxPaintTextures;
	total_memory_size += sizeof(int) * 7; //NumberOfStuffs
	total_memory_size += sizeof(WorldData);


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

	//if(first_process)
	//{
		memset(raw_data,0,total_memory_size);
	//}
	return 0;
}

void SharedMemory::AddUpdateMessage(string type, int index, bool updateTransform, bool updateShape, bool remove) //Valid types are "Mesh", "Camera", "Light", "Locator"
{
	IdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
	WaitForSingleObject(IdMutexHandle, milliseconds);

	int nrOfMessages = *NumberOfMessages;
	bool stop = false;

	//Check if last message is of same type and don't add new if true
	if(nrOfMessages > 0)
	{
		for(int i = 0; i < nrOfMessages; i++)
		{
			if(type == updateMessages[i]->name && ((index == updateMessages[i]->updateID && !remove) || (index == updateMessages[i]->removeID && remove)))
			{
				stop = true;
				break;
			}
		}
	}

	if(!stop && index >= 0)
	{
		//Maybe check last message for duplicate aswell
		if(nrOfMessages < g_maxMessages)
		{
			updateMessages[nrOfMessages]->updateTransform = updateTransform;
			updateMessages[nrOfMessages]->updateShape = updateShape;
			if(type != "")
			{
				memcpy(updateMessages[nrOfMessages]->name, type.c_str(), g_maxNameLength);
				int minusOne = -1;
				if(remove)
				{
					updateMessages[nrOfMessages]->updateID = minusOne;
					updateMessages[nrOfMessages]->removeID = index;
				}
				else
				{
					updateMessages[nrOfMessages]->updateID = index;
					updateMessages[nrOfMessages]->removeID = minusOne;
				}
			}
			nrOfMessages++;
			*NumberOfMessages = nrOfMessages;
		}
		else if(nrOfMessages > 0)
		{
			//Remove oldest / first message and try again
			//UpdateMessage tempCopy[g_maxMessages];

			for(int i = 0; i < nrOfMessages-1; i++)
			{
				*updateMessages[i] = *updateMessages[i+1];
			}

			updateMessages[g_maxMessages]->removeID = -1;
			updateMessages[g_maxMessages]->updateID = -1;
			updateMessages[g_maxMessages]->updateShape = false;
			updateMessages[g_maxMessages]->updateTransform = false;
			//*updateMessages = tempCopy;
			nrOfMessages--;
			*NumberOfMessages = nrOfMessages;
			AddUpdateMessage(type, index, updateTransform, updateShape, remove);
		}
	}
	ReleaseMutex(IdMutexHandle);
}

void SharedMemory::UpdateSharedLight(int index, int nrOfLights)	
{
	AddUpdateMessage("Light", index, true, true, false);

	LightMutexHandle = CreateMutex(nullptr, false, L"LightMutex");
	WaitForSingleObject(LightMutexHandle, milliseconds);

		*NumberOfLights = nrOfLights;
		memcpy(PlightList[index]->transformation.name, lightList[index].transformation.name, 50);
		PlightList[index]->transformation.position = lightList[index].transformation.position;
		PlightList[index]->transformation.scale = lightList[index].transformation.scale;
		PlightList[index]->transformation.rotation = lightList[index].transformation.rotation;
		PlightList[index]->color = lightList[index].color;
		PlightList[index]->Intensity = lightList[index].Intensity;
		memcpy(PlightList[index]->LightType, lightList[index].LightType,30);

	ReleaseMutex(LightMutexHandle);
}

void SharedMemory::UpdateSharedCamera(int index)		
{
	AddUpdateMessage("Camera", index, true, true, false);

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
	AddUpdateMessage("Mesh",index, updateTransformation, updateVertex, false);

	MeshMutexHandle = CreateMutex(nullptr, false, L"MeshMutex");
	WaitForSingleObject(MeshMutexHandle, milliseconds);

	*NumberOfMeshes = nrOfMeshes;
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
		PmeshList[index]->tangent[i] = meshList[index].tangent[i];
		PmeshList[index]->binormal[i] = meshList[index].binormal[i];
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
		PmeshList[index]->transformation.nrOfFlags = meshList[index].transformation.nrOfFlags;

		for(int i = 0; i < g_maxNrOfFlags; i++)
		{
			memset(PmeshList[index]->transformation.flags[i], NULL, sizeof(PmeshList[index]->transformation.flags[i]));
			memcpy(PmeshList[index]->transformation.flags[i], meshList[index].transformation.flags[i], g_shortMaxNameLength);
		}
		
	}

	ReleaseMutex(MeshMutexHandle);
}

void SharedMemory::UpdateSharedLocator(int index, int nrOfLocators)
{
	AddUpdateMessage("Locator", index, true, true, false);

	LocatorMutexHandle = CreateMutex(nullptr, false, L"LocatorMutex");
	WaitForSingleObject(IdMutexHandle, milliseconds);
	
	*NumberOfLocators = nrOfLocators;
	memcpy(PlocatorList[index]->transformation.name, locatorList[index].transformation.name, g_maxNameLength);
	PlocatorList[index]->transformation.position = locatorList[index].transformation.position;
	PlocatorList[index]->transformation.scale = locatorList[index].transformation.scale;
	PlocatorList[index]->transformation.rotation = locatorList[index].transformation.rotation;
	PlocatorList[index]->transformation.nrOfFlags = locatorList[index].transformation.nrOfFlags;

	for(int i = 0; i < locatorList[index].transformation.nrOfFlags; i++)
	{
		memset(PlocatorList[index]->transformation.flags[i], NULL, sizeof(PlocatorList[index]->transformation.flags[i]));
		memcpy(PlocatorList[index]->transformation.flags[i], locatorList[index].transformation.flags[i], g_shortMaxNameLength);
	}

	ReleaseMutex(LocatorMutexHandle);
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
		memcpy(PmaterialList[i]->specularPath, materialList[i].specularPath, g_maxPathLength);
		memcpy(PmaterialList[i]->glowPath, materialList[i].glowPath, g_maxPathLength);
	}

	*NumberOfMaterials = nrOfMaterials;		

	if(meshID != -1)
	{
		memcpy(PmeshList[meshID]->materialName, meshList[meshID].materialName, g_maxNameLength);

		IdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
		WaitForSingleObject(IdMutexHandle, milliseconds);
		AddUpdateMessage("Mesh", meshID, true, true, false);
		PmeshList[meshID]->MaterialID = meshList[meshID].MaterialID;
		PmeshList[meshID]->paintIndex = meshList[meshID].paintIndex;
		ReleaseMutex(IdMutexHandle);

		AddUpdateMessage("Mesh", meshID, true, true, false);
	}

	ReleaseMutex(MeshMutexHandle);
}

void SharedMemory::RemoveMesh(int id, int nrOfMeshes)
{
	MeshMutexHandle = CreateMutex(nullptr, false, L"MeshMutex");
	WaitForSingleObject(MeshMutexHandle, milliseconds);

	*NumberOfMeshes = nrOfMeshes;
	//meshList[id] = Mesh();

	ReleaseMutex(MeshMutexHandle);

	AddUpdateMessage("Mesh", id, false, false, true);
}

void SharedMemory::RemoveLight(int id, int nrOfLights)
{
	LightMutexHandle = CreateMutex(nullptr, false, L"LightMutex");
	WaitForSingleObject(LightMutexHandle, milliseconds);
	*NumberOfLights = nrOfLights;
	ReleaseMutex(LightMutexHandle);

	AddUpdateMessage("Light", id, false, false, true);
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
