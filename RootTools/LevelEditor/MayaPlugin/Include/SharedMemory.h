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
	int InitalizeSharedMemory();
	void UpdateSharedMaterials(int nrOfMaterials);
	
	//Variables
	Mesh meshList[g_maxMeshes];
	Light lightList[g_maxLights];
	Camera cameraList[g_maxCameras];
	int* NumberOfMeshes;
	int* NumberOfLights;
	int* NumberOfCameras;
	glm::vec2* MeshIdChange;
	glm::vec2* CameraIdChange;
	glm::vec2* LightIdChange;

	Material materialList[g_maxMeshes];
	int* NumberOfMaterials;

private:
	//Functions
	
	int shutdown();
	//Variables
	HANDLE MeshMutexHandle;
	HANDLE IdMutexHandle;
	HANDLE LightMutexHandle;
	HANDLE CameraMutexHandle;
	DWORD milliseconds;
	HANDLE shared_memory_handle;
	unsigned char* raw_data;
	Mesh* PmeshList[g_maxMeshes];
	Light* PlightList[g_maxLights];
	Camera* PcameraList[g_maxCameras];
	Material* PmaterialList[g_maxMeshes];

};