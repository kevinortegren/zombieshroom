#pragma once

#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/Shapes/AABB.h>
#include <RootSystems/Include/Shapes/Plane.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootEngine/Render/Include/Mesh.h>
#include <RootEngine/Render/Include/Vertex.h>
#include <vector>

#define QUADTREE_POLYGONS_PER_NODE 10000
#define QUAD_MAX_CHILDS 4

namespace RootForce
{
	class QuadNode
	{
	public:

		friend class QuadTree;

		QuadNode(AABB& p_bounds);

		void AddChild(QuadNode* p_child);
		const std::vector<QuadNode*>& GetChilds();
		const AABB& GetBounds() const;
		
		std::string m_key;

	private:
		AABB m_bounds;
		std::vector<QuadNode*> m_childs;
	};

	struct Triangle
	{
		int m_indices[3];
		unsigned int m_materialIndex;
	};

	struct Polygon
	{
		std::vector<int> m_indices;
		unsigned int m_materialIndex;
	};

	namespace Side
	{
		enum Side
		{
			FRONT,
			BACK,
			NONE
		};
	}

	// Resulting polygons from a split with plane.
	struct PolygonSplit
	{
		Polygon m_front;
		Polygon m_back;
	};

	struct PolygonRecord
	{
		int m_list;
		int count;
	};

	class QuadTree
	{
	public:
		void Init(RootEngine::GameSharedContext* p_context, ECS::World* p_world);

		QuadNode* PickRoot(glm::vec2 p_position);
		void RenderDebug();
		QuadNode* GetRoot() const;

	private:
		
		QuadNode* PickNode(QuadNode* p_node, glm::vec2 p_position);
		void RenderNode(QuadNode* p_node);	
		int RoundToPow2(int p_value);
		void Subdivide( QuadNode* p_node, std::vector<Polygon> p_polygons );

		std::vector<Polygon> DividePolygons( Rectangle p_rect, std::vector<Polygon> p_polygons );
		std::vector<Polygon> DividePolygonsEx( Rectangle p_rect, std::vector<Polygon> p_polygons );

		std::vector<Triangle> Trianglulate(std::vector<Polygon> p_polygons);

		void CreateEntities(std::vector<Triangle> p_triangles, std::string p_key);
		void CreateEntitiesEx(std::vector<Triangle> p_triangles, std::string p_key); 


		PolygonSplit SplitPolygon(PlaneEx& p_divider, Polygon& p_polygon);
		Side::Side ClassifyPoint(PlaneEx& p_divider, glm::vec3 p_position);
		int SplitVertex(PlaneEx& p_divider, Render::Vertex1P1N1UV& p_p0, Render::Vertex1P1N1UV& p_p1);

		glm::vec3 CalcCenter(Polygon& p_polygon);
		glm::vec3 CalcCenterEx(Polygon& p_polygon);


		RootEngine::GameSharedContext* m_context;
		ECS::World* m_world;
		int m_minY, m_maxY;

		QuadNode* m_root;

		std::vector<Polygon> m_globalPolygonList;

		std::vector<Render::Vertex1P1N1UV> m_vertices;
		std::vector<Render::Material*> m_materials;
	};
}