#include <RootEngine/Include/ModelImporter.h>

#ifndef COMPILE_LEVEL_EDITOR

namespace RootEngine
{
	
	ModelImporter::ModelImporter(Logging* p_logger, Render::RendererInterface* p_renderer)
	{
		m_logger	= p_logger;
		m_renderer	= p_renderer;
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
		_splitpath_s(p_fileName.c_str(), NULL, 0, NULL, 0, fileName, 128, NULL, 0);

		m_logger->LogText(LogTag::RENDER, LogLevel::DEBUG_PRINT, "Starting to load mesh    '%s'", fileName);
		if (aiscene) 
		{
			InitFromScene(aiscene, p_fileName);
			m_logger->LogText(LogTag::RENDER, LogLevel::DEBUG_PRINT, "Successfully loaded mesh '%s'", fileName);
		}
		else 
		{
			m_logger->LogText(LogTag::RENDER, LogLevel::FATAL_ERROR, "Error parsing '%s': '%s'", fileName, importer.GetErrorString());
		}

		return m_model;
	}

	void ModelImporter::InitFromScene( const aiScene* p_scene, const std::string p_filename )
	{
	
		// Initialize the meshes in the scene one by one
		for (unsigned int i = 0 ; i < p_scene->mNumMeshes ; i++) 
		{
			const aiMesh* paiMesh = p_scene->mMeshes[i];
			InitMesh(i, paiMesh);
		}
		m_logger->LogText(LogTag::RENDER, LogLevel::DEBUG_PRINT, "Created %d meshes ",p_scene->mNumMeshes);
		InitMaterials(p_scene, p_filename);
	}

	void ModelImporter::InitMesh( unsigned int p_index, const aiMesh* p_aiMesh )
	{
		std::vector<Render::Vertex1P1N1UV> vertices;
		std::vector<unsigned int> indices;

		const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

		for (unsigned int i = 0 ; i < p_aiMesh->mNumVertices ; i++) {
			const aiVector3D* pPos      = &(p_aiMesh->mVertices[i]);
			const aiVector3D* pNormal   = &(p_aiMesh->mNormals[i]);
			const aiVector3D* pTexCoord = p_aiMesh->HasTextureCoords(0) ? &(p_aiMesh->mTextureCoords[0][i]) : &Zero3D;

			Render::Vertex1P1N1UV v;
			v.m_pos		= glm::vec3(pPos->x, pPos->y, pPos->z);
			v.m_normal	= glm::vec3(pNormal->x, pNormal->y, pNormal->z);
			v.m_UV		= glm::vec2(pTexCoord->x, pTexCoord->y);

			vertices.push_back(v);
		}

		for(unsigned int i = 0 ; i < p_aiMesh->mNumFaces ; i++)
		{
			const aiFace& Face = p_aiMesh->mFaces[i];
			if(Face.mNumIndices != 3)
			{
				m_logger->LogText(LogTag::RENDER, LogLevel::FATAL_ERROR, "Error: Mesh nr %d, face nr %d doesn't contain 3 indices!", p_index, i);
			}
			indices.push_back(Face.mIndices[0]);
			indices.push_back(Face.mIndices[1]);
			indices.push_back(Face.mIndices[2]);
		}
		
		m_logger->LogText(LogTag::RENDER, LogLevel::MASS_DATA_PRINT, "Mesh created with %d faces ", p_aiMesh->mNumFaces);

		Render::MeshInterface* tempmesh = m_renderer->CreateMesh();
		tempmesh->Init(&vertices[0], vertices.size(), &indices[0], indices.size());

		m_model->m_meshes.push_back(tempmesh);
		m_model->meshIndices = indices;
		m_model->meshPoints = GetMeshPoints(vertices);
		m_model->numberOfFaces = p_aiMesh->mNumFaces;
		m_model->numberOfIndices = indices.size();
		m_model->numberOfVertices = vertices.size();
	}

	void ModelImporter::InitMaterials( const aiScene* p_scene, const std::string p_filename )
	{
		
	}

	std::vector<glm::vec3> ModelImporter::GetMeshPoints( std::vector<Render::Vertex1P1N1UV> p_vertices )
	{
		std::vector<glm::vec3> returnVec;

		for (Render::Vertex1P1N1UV v : p_vertices ) 
		{
			returnVec.push_back(v.m_pos);
		}

		return returnVec;
	}

}

#endif