#pragma once
#include <string>
#include <vector>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <glm/glm.hpp>
#include <RootEngine/Render/Include/Vertex.h>
#include <RootEngine/Render/Include/Mesh.h>

namespace Render
{
	class MeshImporter
	{
	public:
		MeshImporter();
		~MeshImporter();

		void LoadMesh(const std::string p_fileName);
		void InitFromScene(const aiScene* p_scene, const std::string p_filename);
		void InitMesh(unsigned int p_index, const aiMesh* p_aiMesh);
		void InitMaterials(const aiScene* p_scene, const std::string p_filename);

		std::vector<Mesh> m_entries;
		std::vector<int> m_textures;
	};
}
