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
	int* NumberOfMeshes;
	int* NumberOfLights;
	int* NumberOfCameras;
	Mesh* PmeshList[g_maxMeshes];
	Light* PlightList[g_maxLights];
	Camera* PcameraList[g_maxCameras];
	int* MeshIdChange;
	int* CameraIdChange;
	int* LightIdChange;
	HANDLE MeshMutexHandle;
	HANDLE IdMutexHandle;
	HANDLE LightMutexHandle;
	HANDLE CameraMutexHandle;
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