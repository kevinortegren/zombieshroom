#pragma once
#include <WinSock2.h>
#include <windows.h>
#include "Constants.h"

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>

class ReadMemory
{
public:
	ReadMemory(void);
	~ReadMemory(void);
	int InitalizeSharedMemory();
	void Read();

	//Variables
	Mesh meshList[g_maxMeshes];
	Light lightList[g_maxLights];
	Camera cameraList[g_maxCameras];
	Material materialList[g_maxMeshes];
	Locator locatorList[g_maxLocators];

	int* NumberOfMeshes;
	int* NumberOfLights;
	int* NumberOfCameras;
	int* NumberOfMaterials;
	int* NumberOfLocators;

	int* NumberOfMessages;
	int* NumberOfPaintTextures;
	int* NumberOfMegaMeshes;

	UpdateMessage* updateMessages[g_maxMessages];

	Mesh* PmeshList[g_maxMeshes];
	Light* PlightList[g_maxLights];
	Camera* PcameraList[g_maxCameras];
	Material* PmaterialList[g_maxMeshes];
	Locator* PlocatorList[g_maxLocators];
	PaintTexture* PpaintList[g_maxPaintTextures];
	WorldData* worldData;
	MegaMesh* PmegaMeshes[g_maxMegaMeshes];

	int* export;

	void ReadMessage(string &out_type, int &out_updateIndex, int &out_removeIndex, bool &out_updateTransform, bool &out_updateShape);
	UpdateMessage PeekMessageAt(int index);
	void ClearAllMessages();
	WorldData getWorldData();
	Mesh getMesh(int index);
	Light getLight(int index);
	Camera getCamera(int index);
	Locator getLocator(int index);
	Material getMaterial(int index);
	Counters getCounters();
	PaintTexture getPaintTexture(int index);
	void LockMutex(string mutexName);
	void UnlockMutex(string mutexName);
	
private:
	//Functions
	int shutdown();
	//void ReadMesh(int);
	//void ReadCamera(int);
	//void ReadLight(int);

	//Variables

	HANDLE shared_memory_handle;
	HANDLE MeshMutexHandle;
	HANDLE IdMutexHandle;
	HANDLE LightMutexHandle;
	HANDLE CameraMutexHandle;
	HANDLE LocatorMutexHandle;
	HANDLE TextureMutexHandle;
	DWORD milliseconds;
	unsigned char* raw_data;	
};