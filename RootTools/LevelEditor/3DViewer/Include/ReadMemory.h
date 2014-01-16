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

	glm::vec2* MeshIdChange;
	glm::vec2* CameraIdChange;
	glm::vec2* LightIdChange;
	glm::vec2* LocatorIdChange;

	Mesh* PmeshList[g_maxMeshes];
	Light* PlightList[g_maxLights];
	Camera* PcameraList[g_maxCameras];
	Material* PmaterialList[g_maxMeshes];
	Locator* PlocatorList[g_maxLocators];

	int* export;

	HANDLE MeshMutexHandle;
	HANDLE IdMutexHandle;
	HANDLE LightMutexHandle;
	HANDLE CameraMutexHandle;
	HANDLE LocatorMutexHandle;
	DWORD milliseconds;
	
private:
	//Functions
	int shutdown();
	void ReadMesh(int);
	void ReadCamera(int);
	void ReadLight(int);

	//Variables

	HANDLE shared_memory_handle;
	unsigned char* raw_data;	
};