#include "ReadMemory.h"

ReadMemory::ReadMemory()
{
	NumberOfMeshes = nullptr;
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
	//total_memory_size += sizeof(Camera) * g_maxCameras;
	//total_memory_size += sizeof(Light) * g_maxLights;
	total_memory_size += sizeof(int) * 3;


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

	unsigned char* mem = (unsigned char*)raw_data;
	NumberOfMeshes = (int*)(mem + sizeof(Mesh) * g_maxMeshes);
	mem = (unsigned char*)NumberOfMeshes;
	MeshIdChange = (int*)(mem + sizeof(int));
	CameraIdChange = (int*)(MeshIdChange + sizeof(int));
	LightIdChange = (int*)(CameraIdChange + sizeof(int));

	if(first_process)
	{
		memset(raw_data,0,total_memory_size);
	}
	return 0;
}



void ReadMemory::Read()			//read if the active object have changed.
{
	IdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
	WaitForSingleObject(IdMutexHandle, milliseconds);

	if(*MeshIdChange != -1)
	{
		system("cls");
		cout << "ID: " + to_string(*MeshIdChange) << endl;
		ReadMesh(*MeshIdChange);
		*MeshIdChange = -1;
	}

	ReleaseMutex(IdMutexHandle);
}

void ReadMemory::ReadMesh(int i)
{
	MeshMutexHandle = CreateMutex(nullptr, false, L"MeshMutex");
	WaitForSingleObject(MeshMutexHandle, milliseconds);

	cout << PmeshList[i]->transformation.name << endl;
	cout << "NrOfVertices " + to_string(PmeshList[i]->nrOfVertices) << endl;
	cout << "TransPosXYZ " + to_string(PmeshList[i]->transformation.position.x) + " "
							+ to_string(PmeshList[i]->transformation.position.y) + " "
							+ to_string(PmeshList[i]->transformation.position.z)<< endl;

	cout << "ScaleXYZ " + to_string(PmeshList[i]->transformation.scale.x) + " "
							+ to_string(PmeshList[i]->transformation.scale.y) + " "
							+ to_string(PmeshList[i]->transformation.scale.z)<< endl;
	
	cout << "RotationXYZ " + to_string(PmeshList[i]->transformation.rotation.x) + " "
							+ to_string(PmeshList[i]->transformation.rotation.y) + " "
							+ to_string(PmeshList[i]->transformation.rotation.z)<< endl;
	for(int k = 0; k < PmeshList[i]->nrOfVertices; k++)
	{
	cout << "Vertex x,y,z" + to_string(PmeshList[i]->vertex[k].x) + " " + to_string(PmeshList[i]->vertex[k].y) + " " + to_string(PmeshList[i]->vertex[k].z) << endl;
	}

	ReleaseMutex(MeshMutexHandle);
}

int ReadMemory::shutdown()
{
	UnmapViewOfFile(raw_data);
	CloseHandle(shared_memory_handle);
	CloseHandle(MeshMutexHandle);
	CloseHandle(IdMutexHandle);
	return 0;
}
