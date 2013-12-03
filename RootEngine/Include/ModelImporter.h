#pragma once

#include <string>
#include <vector>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <glm/glm.hpp>
#include <RootEngine/Render/Include/Vertex.h>
#include <RootEngine/Render/Include/Renderer.h>
#include <RootEngine/Render/Include/Mesh.h>
#include <RootEngine/Render/Include/Texture.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <memory>

namespace RootEngine
{
	class ResourceManager;

	struct Model
	{
		std::vector<std::shared_ptr<Render::Mesh>> m_meshes;
		std::string m_textureHandles[3];
		//vector<AnimationData*> m_animations;
		unsigned int numberOfIndices;
		unsigned int numberOfVertices;
		unsigned int numberOfFaces;
		std::vector<glm::vec3> meshPoints;
		std::vector<unsigned int> meshIndices;
	};

	class ModelImporter
	{
	public:
		
		

		ModelImporter(Logging* p_logger, Render::RendererInterface* p_renderer, ResourceManager* p_resourceManager);
		~ModelImporter();

		Model* LoadModel(const std::string p_fileName);
		
	private:

		void InitFromScene(const aiScene* p_scene, const std::string p_filename);
		void InitMesh(unsigned int p_index, const aiMesh* p_aiMesh);
		void InitMaterials(const aiScene* p_scene, const std::string p_filename);
		std::vector<glm::vec3> GetMeshPoints(std::vector<Render::Vertex1P1N1UV> p_vertices);
		std::string GetNameFromPath(std::string p_path);

		ResourceManager* m_resourceManager;
		Logging*	m_logger;
		Model*		m_model; 
		Render::RendererInterface* m_renderer;
	};
}
