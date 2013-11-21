#pragma once
#include <string>

class MeshImporter
{
public:
	MeshImporter();
	~MeshImporter();

	void LoadMesh(const std::string p_fileName);
};

