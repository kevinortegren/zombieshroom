#pragma once
#include <RootEngine/Render/Include/Vertex.h>
#include <vector>

namespace Physics
{
	class PhysicsMeshInterface abstract
	{
	public:
		virtual void Init(std::vector<glm::vec3> p_pointData, int p_nrOfPoints, std::vector<unsigned int> p_indexData, int p_nrOfIndices, int p_faces) = 0;

		virtual float*	GetMeshPoints() = 0;
		virtual int*	GetIndices() = 0;
		virtual int		GetNrOfIndices() = 0;
		virtual int		GetNrOfPoints() = 0;
		virtual int		GetNrOfFaces() = 0;
	};
	class PhysicsMesh : public PhysicsMeshInterface
	{
	public:
		PhysicsMesh();
		~PhysicsMesh();
		void Init(std::vector<glm::vec3> p_pointData, int p_nrOfPoints, std::vector<unsigned int> p_indexData, int p_nrOfIndices, int p_faces);

		float*	GetMeshPoints();
		int*	GetIndices();
		int		GetNrOfIndices();
		int		GetNrOfPoints();
		int		GetNrOfFaces();

	private:
		float*	m_points;
		int*	m_indices;

		int		m_nrOfIndices;
		int		m_nrOfPoints;
		int		m_nrOfFaces;
	};
}
