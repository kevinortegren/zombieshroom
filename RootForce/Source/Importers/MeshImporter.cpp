#include "Importers/MeshImporter.h"
#include "Logging/Logging.h"


MeshImporter::MeshImporter()
{
}


MeshImporter::~MeshImporter()
{
}

void MeshImporter::LoadMesh( const std::string p_fileName )
{
	
	Assimp::Importer importer;

	const aiScene* aiscene = importer.ReadFile(p_fileName.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
	Logging::GetInstance()->LogText(Logging::DEBUG, 2,		"Starting to load mesh    '%s'", p_fileName.c_str());
	if (aiscene) 
	{
		InitFromScene(aiscene, p_fileName);
		Logging::GetInstance()->LogText(Logging::DEBUG, 2,	"Successfully loaded mesh '%s'", p_fileName.c_str());
	}
	else 
	{
		Logging::GetInstance()->LogText(Logging::ERR, 1, "Error parsing '%s': '%s'", p_fileName.c_str(), importer.GetErrorString());
	}
}

void MeshImporter::InitFromScene( const aiScene* p_scene, const std::string p_filename )
{
	m_entries.resize(p_scene->mNumMeshes);
	m_textures.resize(p_scene->mNumMaterials);

	// Initialize the meshes in the scene one by one
	for (unsigned int i = 0 ; i < m_entries.size() ; i++) 
	{
		const aiMesh* paiMesh = p_scene->mMeshes[i];
		InitMesh(i, paiMesh);
	}
	InitMaterials(p_scene, p_filename);
}

void MeshImporter::InitMesh( unsigned int p_index, const aiMesh* p_aiMesh )
{
	m_entries[p_index].MaterialIndex = p_aiMesh->mMaterialIndex;

	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;

	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	for (unsigned int i = 0 ; i < p_aiMesh->mNumVertices ; i++) {
		const aiVector3D* pPos      = &(p_aiMesh->mVertices[i]);
		const aiVector3D* pNormal   = &(p_aiMesh->mNormals[i]);
		const aiVector3D* pTexCoord = p_aiMesh->HasTextureCoords(0) ? &(p_aiMesh->mTextureCoords[0][i]) : &Zero3D;

		Vertex v(Vector3f(pPos->x, pPos->y, pPos->z),
				Vector2f(pTexCoord->x, pTexCoord->y),
				Vector3f(pNormal->x, pNormal->y, pNormal->z));

		Vertices.push_back(v);
	}

	for(unsigned int i = 0 ; i < p_aiMesh->mNumFaces ; i++)
	{
		const aiFace& Face = p_aiMesh->mFaces[i];
		if(Face.mNumIndices != 3)
			Logging::GetInstance()->LogText(Logging::ERR, 1, "Error: Mesh nr %d, face nr %d doesn't contain 3 indices!", p_index, i);
		Indices.push_back(Face.mIndices[0]);
		Indices.push_back(Face.mIndices[1]);
		Indices.push_back(Face.mIndices[2]);
	}

	m_entries[p_index].Init(Vertices, Indices);
}

void MeshImporter::InitMaterials( const aiScene* p_scene, const std::string p_filename )
{
	/*
	// Extract the directory part from the file name
	std::string::size_type SlashIndex = p_filename.find_last_of("/");
	std::string Dir;

	if (SlashIndex == std::string::npos) {
		Dir = ".";
	}
	else if (SlashIndex == 0) {
		Dir = "/";
	}
	else {
		Dir = p_filename.substr(0, SlashIndex);
	}


	// Initialize the materials
	for (unsigned int i = 0 ; i < p_scene->mNumMaterials ; i++) {
		const aiMaterial* pMaterial = p_scene->mMaterials[i];

		m_textures[i] = NULL;

		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString Path;

			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				std::string FullPath = Dir + "/" + Path.data;
				m_textures[i] = new Texture(GL_TEXTURE_2D, FullPath.c_str());

				if (!m_textures[i]->Load()) {
					Logging::GetInstance()->LogText(Logging::ERROR, 1, "Error loading texture '%s'\n", FullPath.c_str());
					delete m_Textures[i];
					m_textures[i] = NULL;
					Ret = false;
				}
				else {
					Logging::GetInstance()->LogText(Logging::DEBUG, 2, "Loaded texture '%s'\n", FullPath.c_str()); 
				}
			}
		}

		// Load a white texture in case the model does not include its own texture
		if (!m_textures[i]) {
			m_textures[i] = new Texture(GL_TEXTURE_2D, "./white.png");

			m_textures[i]->Load();
		}
	}
	*/
}
