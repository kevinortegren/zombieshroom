#include <RootEngine/Physics/Include/PhysicsMesh.h>
#include <iostream>

namespace RootEngine
{
namespace Physics
{
	PhysicsMesh::PhysicsMesh()
	{

	}

	PhysicsMesh::~PhysicsMesh()
	{
		//std::cout << "Clear meshes." << std::endl;
		delete[] m_indices;
		delete[] m_points;
	}

	void PhysicsMesh::Init(std::vector<glm::vec3> p_pointData, int p_nrOfPoints, std::vector<unsigned int> p_indexData, int p_nrOfIndices, int p_faces)
	{
		m_points = new float[p_nrOfPoints*3];
		for(int i = 0; i < p_nrOfPoints; i++)
		{
			m_points[(i*3)]		= p_pointData[i].x;
			m_points[(i*3)+1]	= p_pointData[i].y;
			m_points[(i*3)+2]	= p_pointData[i].z;
		}

		m_indices = new int[p_nrOfIndices];
		for(int i = 0; i < p_nrOfIndices; i++)
		{
			m_indices[i] = (int)p_indexData[i];
		}

		m_nrOfFaces		= p_faces;
		m_nrOfIndices	= p_nrOfIndices;
		m_nrOfPoints	= p_nrOfPoints;
	}

	float* PhysicsMesh::GetMeshPoints()
	{
		return m_points;
	}

	int* PhysicsMesh::GetIndices()
	{
		return m_indices;
	}

	int PhysicsMesh::GetNrOfIndices()
	{
		return m_nrOfIndices;
	}

	int PhysicsMesh::GetNrOfPoints()
	{
		return m_nrOfPoints;
	}

	int PhysicsMesh::GetNrOfFaces()
	{
		return m_nrOfFaces;
	}

}
}


