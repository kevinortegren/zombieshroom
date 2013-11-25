#pragma once
#include <string>
#include <vector>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <glm/glm.hpp>
#include <RootEngine/Render/Include/Vertex.h>
#include <RootEngine/Render/Include/Mesh.h>
#include <RootEngine/Include/Logging/Logging.h>

namespace RootEngine
{
	class ModelImporter
	{
	public:
		ModelImporter(Logging* p_logger) : m_logger(p_logger){}
		~ModelImporter();

		void LoadMesh(const std::string p_fileName);
		void InitFromScene(const aiScene* p_scene, const std::string p_filename);
		void InitMesh(unsigned int p_index, const aiMesh* p_aiMesh);
		void InitMaterials(const aiScene* p_scene, const std::string p_filename);
	private:
		Logging* m_logger;
	};
}
