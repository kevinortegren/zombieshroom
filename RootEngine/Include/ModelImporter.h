#pragma once

#include <string>
#include <vector>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <glm/glm.hpp>
#include <RootEngine/Physics/Include/PhysicsMesh.h>
#include <RootEngine/Render/Include/Vertex.h>
#include <RootEngine/Render/Include/Renderer.h>
#include <RootEngine/Render/Include/Mesh.h>
#include <RootEngine/Render/Include/Texture.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <memory>

namespace RootEngine
{
	struct GameSharedContext;

	struct Model
	{
		std::vector<Render::MeshInterface*> m_meshes;
		std::vector<Physics::PhysicsMeshInterface*> m_physicsMeshes;

		std::string m_textureHandles[3];
		//vector<AnimationData*> m_animations;
	};

	class ModelImporter
	{
	public:
		
		ModelImporter(GameSharedContext* p_context);
		~ModelImporter();

		Model* LoadModel(const std::string p_fileName);
		
	private:

		void InitFromScene(const aiScene* p_scene, const std::string p_filename);
		void InitMesh(unsigned int p_index, const aiMesh* p_aiMesh, const std::string p_filename );
		void InitMaterials(const aiScene* p_scene, const std::string p_filename);
		std::string GetNameFromPath(std::string p_path);

		GameSharedContext* m_context;
		Model*		m_model; 
	};
}
