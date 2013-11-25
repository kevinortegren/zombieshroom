#include <RootEngine/Include/ModelImporter.h>
#include <RootEngine/Render/Include/RenderExtern.h>

namespace RootEngine
{
	
	ModelImporter::~ModelImporter()
	{
	}

	void ModelImporter::LoadMesh( const std::string p_fileName )
	{

		Assimp::Importer importer;

		const aiScene* aiscene = importer.ReadFile(p_fileName.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
		m_logger->LogText(LogTag::RENDER, LogLevel::DEBUG_PRINT, "Starting to load mesh    '%s'", p_fileName.c_str());
		if (aiscene) 
		{
			InitFromScene(aiscene, p_fileName);
			m_logger->LogText(LogTag::RENDER, LogLevel::DEBUG_PRINT, "Successfully loaded mesh '%s'", p_fileName.c_str());
		}
		else 
		{
			m_logger->LogText(LogTag::RENDER, LogLevel::FATAL_ERROR, "Error parsing '%s': '%s'", p_fileName.c_str(), importer.GetErrorString());
		}
	}

	void ModelImporter::InitFromScene( const aiScene* p_scene, const std::string p_filename )
	{
	//	m_entries.resize(p_scene->mNumMeshes);
		//m_textures.resize(p_scene->mNumMaterials);

		// Initialize the meshes in the scene one by one
		/*for (unsigned int i = 0 ; i < m_entries.size() ; i++) 
		{
			const aiMesh* paiMesh = p_scene->mMeshes[i];
			InitMesh(i, paiMesh);
		}
		InitMaterials(p_scene, p_filename);*/
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
	}

	void ModelImporter::InitMaterials( const aiScene* p_scene, const std::string p_filename )
	{
		
	}
}
