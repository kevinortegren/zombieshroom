#pragma once
#include <string>
#include <vector>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <glm/glm.hpp>
#include <RootEngine/Render/Include/Vertex.h>
#include <RootEngine/Render/Include/Renderer.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <memory>

namespace RootEngine
{
	struct Model
	{
		std::vector<Render::Mesh*> m_meshes;
		//vector<Texture*> m_textures;
		//vector<AnimationData*> m_animations;
	};

	class ModelImporter
	{
	public:
		

		ModelImporter(Logging* p_logger);
		~ModelImporter();

		Model* LoadModel(const std::string p_fileName);
		
	private:

		void InitFromScene(const aiScene* p_scene, const std::string p_filename);
		void InitMesh(unsigned int p_index, const aiMesh* p_aiMesh);
		void InitMaterials(const aiScene* p_scene, const std::string p_filename);

		Logging*	m_logger;
		Model*		m_model; 
	};
}
