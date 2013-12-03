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

private:
	//Functions
	int shutdown();
	void ReadMesh(int);
	void ReadCamera(int);
	void ReadLight(int);

	//Variables
	HANDLE MutexHandle;
	DWORD milliseconds;
	HANDLE shared_memory_handle;
	unsigned char* raw_data;
	Mesh* PmeshList[g_maxMeshes];
	Light* PlightList[g_maxLights];
	Camera* PcameraList[g_maxCameras];
	int* MeshIdChange;
	int* CameraIdChange;
	int* LightIdChange;
};