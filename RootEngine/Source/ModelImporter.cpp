#include <RootEngine/Include/ModelImporter.h>
#include <RootEngine/Include/GameSharedContext.h>

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
		Assimp::Importer importer;

		const aiScene* aiscene = importer.ReadFile(p_fileName.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

		char fileName[128];
		_splitpath_s(p_fileName.c_str(), NULL, 0, NULL, 0, fileName, p_fileName.size(), NULL, 0);

		m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::DEBUG_PRINT, "Starting to load mesh '%s'", fileName);
		if (aiscene) 
		{
			InitFromScene(aiscene, p_fileName);
			m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::DEBUG_PRINT, "Successfully loaded mesh '%s'", fileName);
		}
		else 
		{
			m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::FATAL_ERROR, "Error parsing '%s': '%s'", fileName, importer.GetErrorString());
		}

		return m_model;
	}

	void ModelImporter::InitFromScene( const aiScene* p_scene, const std::string p_filename )
	{
		// Initialize the meshes in the scene one by one
		for (unsigned int i = 0 ; i < p_scene->mNumMeshes ; i++) 
		{
			const aiMesh* paiMesh = p_scene->mMeshes[i];
			InitMesh(i, paiMesh, p_filename);
		}
		m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::DEBUG_PRINT, "Created %d meshes",p_scene->mNumMeshes);
		m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::DEBUG_PRINT, "Starting to load  %d textures to model", p_scene->mNumMaterials);

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

		std::shared_ptr<Render::MeshInterface> mesh	= m_context->m_renderer->CreateMesh();
		mesh->SetVertexBuffer(m_context->m_renderer->CreateBuffer());	
		mesh->SetVertexAttribute(m_context->m_renderer->CreateVertexAttributes());

		std::vector<glm::vec3> positions;

		if(p_aiMesh->HasTangentsAndBitangents())
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
			mesh->SetElementBuffer(m_context->m_renderer->CreateBuffer());

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
			std::shared_ptr<Physics::PhysicsMeshInterface> pmesh = m_context->m_physics->CreatePhysicsMesh();

			pmesh->Init(positions, (int)positions.size(), indices, (int)indices.size(), p_aiMesh->mNumFaces);

			m_context->m_resourceManager->m_physicMeshes[handle] = pmesh;
			m_model->m_physicsMeshes.push_back(pmesh.get());
		}

		mesh->SetPrimitiveType(GL_TRIANGLES);

		m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::MASS_DATA_PRINT, "Mesh created with %d faces ", p_aiMesh->mNumFaces);

		m_context->m_resourceManager->m_meshes[handle] = mesh;

		m_model->m_meshes.push_back(mesh.get());
	}

	void ModelImporter::InitMaterials( const aiScene* p_scene, const std::string p_filename )
	{
		// Initialize the materials
		for (unsigned int i = 0 ; i < p_scene->mNumMaterials ; i++) 
		{
			const aiMaterial* pMaterial = p_scene->mMaterials[i];

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
