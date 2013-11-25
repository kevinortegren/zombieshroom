#pragma once
#include <string>
#include <vector>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

class MeshImporter
{
public:
	MeshImporter();
	~MeshImporter();

	void LoadMesh(const std::string p_fileName);

private:
	struct Vector3f
	{
		Vector3f(){}
		Vector3f(float p_x, float p_y, float p_z)
		{
			x = p_x;
			y = p_y;
			z = p_z;
		}

		float x, y, z;
	};

	struct Vector2f
	{
		Vector2f(){}
		Vector2f(float p_x, float p_y)
		{
			x = p_x;
			y = p_y;
		}

		float x, y;
	};

	struct Vertex
	{
		Vector3f m_pos;
		Vector2f m_tex;
		Vector3f m_normal;

		Vertex() {}

		Vertex(const Vector3f& pos, const Vector2f& tex, const Vector3f& normal)
		{
			m_pos    = pos;
			m_tex    = tex;
			m_normal = normal;
		}
	};

	struct MeshEntry 
	{
		MeshEntry(){};

		void Init(const std::vector<Vertex> p_vertices,
			const std::vector<unsigned int> p_indices)
		{
			vertices	= p_vertices;
			indices		= p_indices;
		}

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		unsigned int NumIndices;
		unsigned int MaterialIndex;
	};

	void InitFromScene(const aiScene* p_scene, const std::string p_filename);
	void InitMesh(unsigned int p_index, const aiMesh* p_aiMesh);
	void InitMaterials(const aiScene* p_scene, const std::string p_filename);

	std::vector<MeshEntry> m_entries;
	std::vector<int> m_textures;
};

