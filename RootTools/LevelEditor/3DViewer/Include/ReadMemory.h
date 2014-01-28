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

	UpdateMessage* updateMessages[g_maxMessages];

	Mesh* PmeshList[g_maxMeshes];
	Light* PlightList[g_maxLights];
	Camera* PcameraList[g_maxCameras];
	Material* PmaterialList[g_maxMeshes];
	Locator* PlocatorList[g_maxLocators];
	PaintTexture* PpaintList[g_maxPaintTextures];

	int* export;

	HANDLE MeshMutexHandle;
	HANDLE IdMutexHandle;
	HANDLE LightMutexHandle;
	HANDLE CameraMutexHandle;
	HANDLE LocatorMutexHandle;
	HANDLE TextureMutexHandle;
	DWORD milliseconds;

	void ReadMessage(string &out_type, int &out_updateIndex, int &out_removeIndex, bool &out_updateTransform, bool &out_updateShape);
	UpdateMessage PeekMessageAt(int index);
	void ClearAllMessages();
	
private:
	//Functions
	int shutdown();
	//void ReadMesh(int);
	//void ReadCamera(int);
	//void ReadLight(int);

	//Variables

	HANDLE shared_memory_handle;
	unsigned char* raw_data;	
};