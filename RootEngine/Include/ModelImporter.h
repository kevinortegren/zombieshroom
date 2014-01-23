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

	#define NUM_BONES_PER_VERTEX 4

	struct Model
	{
		Render::MeshInterface* m_meshes[2];
		std::vector<Physics::PhysicsMeshInterface*> m_physicsMeshes;
		RootEngine::RootAnimation::AnimationInterface* m_animation;
		std::string m_textureHandles[3];
		glm::mat4x4 m_transform;
	};

	struct VertexBoneData
	{        
		unsigned int m_IDList[NUM_BONES_PER_VERTEX];
		float m_weightList[NUM_BONES_PER_VERTEX];

		VertexBoneData()
		{
			Reset();
		};

		void Reset()
		{
			std::fill_n(m_IDList, NUM_BONES_PER_VERTEX, 0);
			std::fill_n(m_weightList, NUM_BONES_PER_VERTEX, 0.0f);
		}

		void AddBoneData(unsigned int p_boneID, float p_weight)
		{
			for (unsigned int i = 0 ; i < NUM_BONES_PER_VERTEX ; i++) 
			{
				if (m_weightList[i] == 0.0f) 
				{
					m_IDList[i]     = p_boneID;
					m_weightList[i] = p_weight;
					return;
				}        
			}

			// Should never get here! If it does, there are more bones per vertex than allowed.
			assert(0);
		}
	};

#ifndef COMPILE_LEVEL_EDITOR
	class ModelImporter
	{
	public:
		
		ModelImporter(GameSharedContext* p_context);
		~ModelImporter();


		Model* LoadModel(const std::string p_fileName);
		
	private:

		void TraverseSceneHierarchy(const aiScene* p_scene, const std::string p_filename );
		void Traverse(const aiNode* p_node, const aiScene* p_scene, const std::string p_filename );


		void InitFromScene(const aiScene* p_scene, const std::string p_filename);
		void InitMesh(unsigned int p_index, const aiMesh* p_aiMesh, const std::string p_filename );
		void InitMaterials(const aiScene* p_scene, const std::string p_filename);
		void LoadBones(const aiMesh* p_aiMesh);
		void LoadAnimation(unsigned int p_index, const aiScene* p_scene);
		std::string GetNameFromPath(std::string p_path);

		std::vector<VertexBoneData> m_boneData;
		GameSharedContext*			m_context;
		Model*						m_model; 
	
	};
#endif
}


