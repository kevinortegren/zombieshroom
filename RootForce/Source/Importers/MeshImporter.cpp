#include "Importers/MeshImporter.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

MeshImporter::MeshImporter()
{
}


MeshImporter::~MeshImporter()
{
}

void MeshImporter::LoadMesh( const std::string p_fileName )
{
	
	Assimp::Importer importer;

	const aiScene* aiscene = importer.ReadFile(p_fileName.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

	if (aiscene) {
		//InitFromScene(aiscene, p_fileName);
	}
	else {
		printf("Error parsing '%s': '%s'\n", p_fileName.c_str(), importer.GetErrorString());
	}
}
