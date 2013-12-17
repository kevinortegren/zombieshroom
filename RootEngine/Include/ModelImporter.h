#pragma once

#ifndef COMPILE_LEVEL_EDITOR
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#endif

#include <RootEngine/Physics/Include/PhysicsMesh.h>
#include <RootEngine/Render/Include/Vertex.h>
#include <RootEngine/Render/Include/Renderer.h>
#include <RootEngine/Render/Include/Texture.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Render/Include/Mesh.h>
#include <RootEngine/Include/Animation.h>
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

namespace RootEngine
{
	struct GameSharedContext;

	struct Model
	{
		std::vector<Render::MeshInterface*> m_meshes;
		std::vector<Physics::PhysicsMeshInterface*> m_physicsMeshes;
		std::vector<RootEngine::RootAnimation::AnimationInterface*> m_animations;

		std::string m_textureHandles[3];
		
	};

#ifndef COMPILE_LEVEL_EDITOR
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
		void LoadBones(unsigned int p_index, const aiMesh* p_aiMesh);
		std::string GetNameFromPath(std::string p_path);

		GameSharedContext*			m_context;
		Model*						m_model; 
		
	};
#endif
}


