#pragma once
#include <WinSock2.h>
#include <windows.h>
#include "Constants.h"

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>

class SharedMemory
{
public:
	SharedMemory(void);
	~SharedMemory(void);
	void UpdateSharedMesh(int index, bool updateTransformation, bool updateVertex, int nrOfMeshes);
	void UpdateSharedCamera(int index);
	void UpdateSharedLight(int index, int nrOfLights);
	void RemoveMesh(int id, int nrOfMeshes);
	void RemoveLight(int id, int nrOfLights);
	int InitalizeSharedMemory();
	void UpdateSharedMaterials(int nrOfMaterials, int meshID, bool itsAmegaMesh);
	void UpdateSharedLocator(int index, int nrOfLocators);
	void AddUpdateMessage(string type, int index, bool updateTransform, bool updateShape, bool remove); //Valid types are "Mesh", "Camera", "Light", "Locator"
	void LockMeshMutex();
	void UnLockMeshMutex();
	
	//Variables
	Mesh meshList[g_maxMeshes];
	Light lightList[g_maxLights];
	Camera cameraList[g_maxCameras];
	Material materialList[g_maxMeshes];
	Locator locatorList[g_maxLocators];

	PaintTexture* PpaintList[g_maxPaintTextures];

	int* NumberOfMeshes;
	int* NumberOfLights;
	int* NumberOfCameras;
	int* NumberOfMaterials;
	int* NumberOfLocators;
	int* NumberOfMessages;
	int* NumberOfPaintTextures;
	int* NumberOfMegaMeshes;

	UpdateMessage* updateMessages[g_maxMessages];
	WorldData* worldData;

	int* export;
	
	DWORD milliseconds;

	HANDLE IdMutexHandle;
	HANDLE LightMutexHandle;

	MegaMesh* PmegaMeshes[g_maxMegaMeshes];
private:
	//Functions
	
	int shutdown();
	//Variables
	HANDLE MeshMutexHandle;

	HANDLE CameraMutexHandle;
	HANDLE shared_memory_handle;
	HANDLE LocatorMutexHandle;
	unsigned char* raw_data;
	Mesh* PmeshList[g_maxMeshes];
	Light* PlightList[g_maxLights];
	Camera* PcameraList[g_maxCameras];
	Material* PmaterialList[g_maxMeshes];
	Locator* PlocatorList[g_maxLocators];


};