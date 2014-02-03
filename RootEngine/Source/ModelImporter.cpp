#include <RootEngine/Include/ModelImporter.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootEngine/Physics/Include/PhysicsMesh.h>
#include <RootEngine/Physics/Include/RootPhysics.h>

#ifndef COMPILE_LEVEL_EDITOR

namespace RootEngine
{
	ModelImporter::ModelImporter(GameSharedContext* p_context)
		: m_context(p_context)
	{
		m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::INIT_PRINT, "Model importer initialized!");
	}

	ModelImporter::~ModelImporter()
	{
		
	}

	Model* ModelImporter::LoadModel(const std::string p_fileName)
	{
		m_model = new Model(); //Owned by ResourceManager

		std::shared_ptr<Assimp::Importer> m_importer = std::shared_ptr<Assimp::Importer>(new Assimp::Importer);
		const aiScene* aiscene = m_importer->ReadFile(p_fileName.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
		if(!aiscene)
		{
			m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::DEBUG_PRINT, "Error parsing scene %s: %s", p_fileName, m_importer->GetErrorString());
		}

		if(aiscene->HasAnimations())
		{
			m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::DEBUG_PRINT, "Scene contains %d animations", aiscene->mNumAnimations);
			m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::DEBUG_PRINT, "Animation 0 is played at  %f tick per second", (float)aiscene->mAnimations[0]->mTicksPerSecond);
			m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::DEBUG_PRINT, "Animation 0 duration is %f",(float)aiscene->mAnimations[0]->mDuration);
			/*for(unsigned int i = 0; i < aiscene->mNumAnimations; i++)
			{
				m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::DEBUG_PRINT, "animation name: '%s'", aiscene->mAnimations[i]->mName.C_Str());

				for(unsigned int j = 0; j < aiscene->mAnimations[i]->mNumChannels; j++)
				{
					m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::DEBUG_PRINT, "Channel name: '%s'", aiscene->mAnimations[i]->mChannels[j]->mNodeName.C_Str());
					m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::DEBUG_PRINT, "Channel number of key frames: %d", aiscene->mAnimations[i]->mChannels[j]->mNumPositionKeys);
				}
			}*/	
		}
		
		char fileName[128];
		_splitpath_s(p_fileName.c_str(), NULL, 0, NULL, 0, fileName, p_fileName.size(), NULL, 0);

		m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::DEBUG_PRINT, "Starting to load mesh '%s'", fileName);
		if (aiscene) 
		{
			TraverseSceneHierarchy(aiscene, p_fileName);

			m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::DEBUG_PRINT, "Successfully loaded mesh '%s'", fileName);
		}
		else 
		{
			m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::FATAL_ERROR, "Error parsing '%s': '%s'", fileName, m_importer->GetErrorString());
		}

		if(aiscene->HasAnimations())
		{
			m_model->m_animation->SetAiImporter(m_importer);
			m_model->m_animation->SplitAnimation();
		}

		return m_model;
	}

	void ModelImporter::TraverseSceneHierarchy(const aiScene* p_scene, const std::string p_filename )
	{
		Traverse(p_scene->mRootNode, p_scene,p_filename);
	}

	void ModelImporter::Traverse(const aiNode* p_node, const aiScene* p_scene, const std::string p_filename )
	{
		for(unsigned i = 0; i < p_node->mNumMeshes; ++i)
		{
			InitMesh(i, p_scene->mMeshes[i], p_filename);

			memcpy(&m_model->m_transform, &p_node->mTransformation, sizeof(aiMatrix4x4));
			m_model->m_transform = glm::transpose(m_model->m_transform);
		}

		for(unsigned i = 0; i < p_node->mNumChildren; ++i)
		{
			Traverse(p_node->mChildren[i], p_scene, p_filename);
		}
	}

	void ModelImporter::InitFromScene( const aiScene* p_scene, const std::string p_filename )
	{
		// Initialize the meshes in the scene one by one
		for (unsigned int i = 0 ; i < p_scene->mNumMeshes ; i++) 
		{
			const aiMesh* paiMesh = p_scene->mMeshes[i];
			//InitMesh(i, paiMesh, p_filename);
		}
		m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::DEBUG_PRINT, "Created %d meshes",p_scene->mNumMeshes);

		InitMaterials(p_scene, p_filename);	
	}

	void ModelImporter::InitMesh( unsigned int p_index, const aiMesh* p_aiMesh, const std::string p_filename )
	{
		static const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

		std::string handle = GetNameFromPath(p_filename) + std::to_string(p_index);

		if(!p_aiMesh->HasPositions())
		{
			m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::FATAL_ERROR, "Error: Mesh nr %s, missing vertex position data.", handle.c_str());
			return;
		}

		

		Render::MeshInterface* mesh = m_context->m_renderer->CreateMesh();
		mesh->SetVertexBuffer(m_context->m_renderer->CreateBuffer(GL_ARRAY_BUFFER));	
		mesh->SetVertexAttribute(m_context->m_renderer->CreateVertexAttributes());

		std::vector<glm::vec3> positions;
		if(p_aiMesh->HasBones())
		{
			LoadBones(p_aiMesh);
			std::vector<Render::Vertex1P1N1UV1T1BT1BID1W> vertices;

			for (unsigned int i = 0 ; i < p_aiMesh->mNumVertices ; i++) {
				const aiVector3D* pPos      = &(p_aiMesh->mVertices[i]);
				const aiVector3D* pNormal   = &(p_aiMesh->mNormals[i]);
				const aiVector3D* pTangent   = &(p_aiMesh->mTangents[i]);
				const aiVector3D* pBitangent   = &(p_aiMesh->mBitangents[i]);
				const aiVector3D* pTexCoord = p_aiMesh->HasTextureCoords(0) ? &(p_aiMesh->mTextureCoords[0][i]) : &Zero3D;


				Render::Vertex1P1N1UV1T1BT1BID1W v;
				v.m_pos			= glm::vec3(pPos->x, pPos->y, pPos->z);
				v.m_normal		= glm::vec3(pNormal->x, pNormal->y, pNormal->z);
				v.m_UV			= glm::vec2(pTexCoord->x, pTexCoord->y);
				v.m_tangent		= glm::vec3(pTangent->x, pTangent->y, pTangent->z);
				v.m_bitangent	= glm::vec3(pBitangent->x, pBitangent->y, pBitangent->z);
				v.m_boneIDs		= glm::uvec4(m_boneData[i].m_IDList[0], m_boneData[i].m_IDList[1], m_boneData[i].m_IDList[2], m_boneData[i].m_IDList[3]);
				v.m_weights		= glm::vec4(m_boneData[i].m_weightList[0], m_boneData[i].m_weightList[1], m_boneData[i].m_weightList[2], m_boneData[i].m_weightList[3]);

				vertices.push_back(v);
				positions.push_back(v.m_pos);
			}

			mesh->CreateVertexBuffer1P1N1UV1T1BT1BID1W(&vertices[0], vertices.size());	

		}
		else if(p_aiMesh->HasTangentsAndBitangents())
		{
			std::vector<Render::Vertex1P1N1UV1T1BT> vertices;

			for (unsigned int i = 0 ; i < p_aiMesh->mNumVertices ; i++) {
				const aiVector3D* pPos      = &(p_aiMesh->mVertices[i]);
				const aiVector3D* pNormal   = &(p_aiMesh->mNormals[i]);
				const aiVector3D* pTangent   = &(p_aiMesh->mTangents[i]);
				const aiVector3D* pBitangent   = &(p_aiMesh->mBitangents[i]);
				const aiVector3D* pTexCoord = p_aiMesh->HasTextureCoords(0) ? &(p_aiMesh->mTextureCoords[0][i]) : &Zero3D;

				Render::Vertex1P1N1UV1T1BT v;
				v.m_pos		= glm::vec3(pPos->x, pPos->y, pPos->z);
				v.m_normal	= glm::vec3(pNormal->x, pNormal->y, pNormal->z);
				v.m_UV		= glm::vec2(pTexCoord->x, pTexCoord->y);
				v.m_tangent = glm::vec3(pTangent->x, pTangent->y, pTangent->z);
				v.m_bitangent = glm::vec3(pBitangent->x, pBitangent->y, pBitangent->z);

				vertices.push_back(v);
				positions.push_back(v.m_pos);
			}

			mesh->CreateVertexBuffer1P1N1UV1T1BT(&vertices[0], vertices.size());	
		}
		else 
		{
			std::vector<Render::Vertex1P1N1UV> vertices;

			for (unsigned int i = 0 ; i < p_aiMesh->mNumVertices ; i++) {
				const aiVector3D* pPos      = &(p_aiMesh->mVertices[i]);
				const aiVector3D* pNormal   = &(p_aiMesh->mNormals[i]);
				const aiVector3D* pTexCoord = p_aiMesh->HasTextureCoords(0) ? &(p_aiMesh->mTextureCoords[0][i]) : &Zero3D;

				Render::Vertex1P1N1UV v;
				v.m_pos		= glm::vec3(pPos->x, pPos->y, pPos->z);
				v.m_normal	= glm::vec3(pNormal->x, pNormal->y, pNormal->z);
				v.m_UV		= glm::vec2(pTexCoord->x, pTexCoord->y);

				vertices.push_back(v);
				positions.push_back(v.m_pos);
			}

			mesh->CreateVertexBuffer1P1N1UV(&vertices[0], vertices.size());	
		}

		
		

		if(p_aiMesh->HasFaces())
		{
			mesh->SetElementBuffer(m_context->m_renderer->CreateBuffer(GL_ELEMENT_ARRAY_BUFFER));

			std::vector<unsigned int> indices;
			for(unsigned int i = 0 ; i < p_aiMesh->mNumFaces ; i++)
			{
				const aiFace& Face = p_aiMesh->mFaces[i];
				if(Face.mNumIndices != 3)
				{
					m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::FATAL_ERROR, "Error: Mesh nr %d, face nr %d doesn't contain 3 indices!", p_index, i);
				}
				indices.push_back(Face.mIndices[0]);
				indices.push_back(Face.mIndices[1]);
				indices.push_back(Face.mIndices[2]);
			}
		
			mesh->CreateIndexBuffer(&indices[0], indices.size());

			// Create physics mesh.
			if(m_context->m_physics)
			{
				std::shared_ptr<Physics::PhysicsMeshInterface> pmesh = m_context->m_physics->CreatePhysicsMesh();

				pmesh->Init(positions, (int)positions.size(), indices, (int)indices.size(), p_aiMesh->mNumFaces);

				m_context->m_resourceManager->m_physicMeshes[handle] = pmesh;
				m_model->m_physicsMeshes.push_back(pmesh.get());
			}
		}

		mesh->SetPrimitiveType(GL_TRIANGLES);

		m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::MASS_DATA_PRINT, "Mesh created with %d faces ", p_aiMesh->mNumFaces);

		m_context->m_resourceManager->m_meshes[handle] = mesh;

		m_model->m_meshes[0] = mesh;
		m_model->m_meshes[1] = nullptr;
	}

	void ModelImporter::InitMaterials( const aiScene* p_scene, const std::string p_filename )
	{
		m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::DEBUG_PRINT, "Starting to load %d materials to model", p_scene->mNumMaterials);
		// Initialize the materials
		for (unsigned int i = 0 ; i < p_scene->mNumMaterials ; i++) 
		{
			const aiMaterial* pMaterial = p_scene->mMaterials[i];
			m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::DEBUG_PRINT, "Starting to load %d diffuse textures, %d specular textures and %d normal textures from material %d", pMaterial->GetTextureCount(aiTextureType_DIFFUSE), pMaterial->GetTextureCount(aiTextureType_SPECULAR), pMaterial->GetTextureCount(aiTextureType_NORMALS), i);

			aiString path;
			std::string texName;

			if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) 
			{
				if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) 
				{
					texName = GetNameFromPath(path.data);
					//if load successfull -> add texture handle to model
					if(m_context->m_resourceManager->LoadTexture(texName, Render::TextureType::TEXTURE_2D))
					{
						m_model->m_textureHandles[0] = texName;
					}
				}
			}
			if (pMaterial->GetTextureCount(aiTextureType_SPECULAR) > 0) 
			{
				if (pMaterial->GetTexture(aiTextureType_SPECULAR, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) 
				{
					texName = GetNameFromPath(path.data);
					//if load successfull -> add texture handle to model
					if(m_context->m_resourceManager->LoadTexture(texName, Render::TextureType::TEXTURE_2D))
					{
						m_model->m_textureHandles[1] = texName;
					}
				}
			}
			if (pMaterial->GetTextureCount(aiTextureType_NORMALS) > 0) 
			{
				if (pMaterial->GetTexture(aiTextureType_NORMALS, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) 
				{
					texName = GetNameFromPath(path.data);
					//if load successfull -> add texture handle to model
					if(m_context->m_resourceManager->LoadTexture(texName, Render::TextureType::TEXTURE_2D))
					{
						m_model->m_textureHandles[2] = texName;
					}
				}
			}
		}
	}

	void ModelImporter::LoadBones( const aiMesh* p_aiMesh )
	{
		m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::DEBUG_PRINT, "Loading animation data with %d bones", p_aiMesh->mNumBones);

		RootEngine::RootAnimation::AnimationInterface* animation = new RootEngine::RootAnimation::Animation(m_context->m_logger);
		m_boneData.resize(p_aiMesh->mNumVertices);
		//Loop through all the bones in the mesh
		for(unsigned int i = 0; i < p_aiMesh->mNumBones; i++)
		{
			unsigned int boneIndex = 0;
			//Store bone name
			std::string boneName = p_aiMesh->mBones[i]->mName.data;

			//If bone doesn't exist, add a new bone to the end of the list
			if(!animation->BoneExists(boneName))
			{
				boneIndex = animation->GetNumBones();
				animation->SetNumBones(boneIndex + 1);
				RootEngine::RootAnimation::BoneInfo bi;
				animation->PushBoneInfo(bi);
			}
			else
			{
				//Get bone index if it exists
				boneIndex = animation->GetIndexFromBoneName(boneName);
			}
			
			//Great code for converting atMatrix4x4 to glm::mat4x4!
			aiMatrix4x4 am = p_aiMesh->mBones[i]->mOffsetMatrix;
			glm::mat4x4 gm = glm::mat4x4();
			memcpy(&gm[0][0], &am[0][0], sizeof(aiMatrix4x4));
			animation->MapBone(boneName, boneIndex);
			animation->GetBoneInfo(boneIndex)->m_boneOffset = glm::transpose(gm);

			//Loop through all weights in the bone and add weights and bone data to vertex slot
			for(unsigned int j = 0; j < p_aiMesh->mBones[i]->mNumWeights; j++)
			{
				unsigned int vertexID =  p_aiMesh->mBones[i]->mWeights[j].mVertexId;
				float vweight  = p_aiMesh->mBones[i]->mWeights[j].mWeight;                   
				m_boneData[vertexID].AddBoneData(boneIndex, vweight);
				//m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::DEBUG_PRINT, "AddBoneData -> VertexID:  %d, BoneIndex: %d, Weight: %f", vertexID, boneIndex, vweight);
			}
			//m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::SUCCESS, "Added bone: %s", boneName.c_str());
		}

		m_model->m_animation = animation;
		m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::SUCCESS, "Loaded animation data!");
	}

	std::string ModelImporter::GetNameFromPath( std::string p_path )
	{
		std::string cutPath;
		std::string::size_type slashIndex, dotIndex;

		// Extract the file name
		cutPath		= p_path;
		slashIndex	= cutPath.find_last_of("/")+1;
		if(slashIndex == 0)
			slashIndex	= cutPath.find_last_of("\\")+1;
		cutPath		= cutPath.substr(slashIndex, cutPath.size());
		dotIndex	= cutPath.find_last_of(".");
		cutPath		= cutPath.substr(0, dotIndex);
		return cutPath;
	}

	

}

#endif
