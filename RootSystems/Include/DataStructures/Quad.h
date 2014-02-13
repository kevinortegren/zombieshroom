#pragma once

#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/Shapes/AABB.h>
#include <RootSystems/Include/Shapes/Plane.h>
#include <RootSystems/Include/Frustum.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootEngine/Render/Include/Mesh.h>
#include <RootEngine/Render/Include/Vertex.h>
#include <vector>

#define QUADTREE_POLYGONS_PER_NODE 1000
#define QUADTREE_MAX_CHILDS 4
#define QUADTREE_SUBDIVIDE
//#define QUADTREE_VERBOSE
#define QUADTREE_DRAWLINES

namespace RootForce
{
	class QuadNode
	{
	public:
		friend class QuadTree;
		QuadNode(const AABB& p_bounds);

		void AddChild(QuadNode* p_child);
		const std::vector<QuadNode*>& GetChilds();
		const AABB& GetBounds() const;

		// Indices to entity vector.
		std::vector<unsigned> m_entityIndices;
		std::vector<unsigned> m_terrainEntityIndices;


	private:
		const AABB m_bounds;
		std::vector<QuadNode*> m_childs;	
	};

	struct Triangle
	{
		unsigned m_indices[3];
		unsigned m_materialIndex;
	};

	struct Polygon
	{
		std::vector<unsigned> m_indices;
		unsigned m_materialIndex;
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
		
		void RenderDebug();
		void RenderNode(QuadNode* p_node);	

		QuadNode* GetRoot() const;
		QuadNode* PickRoot(glm::vec2 p_position);
		QuadNode* PickNode(QuadNode* p_node, glm::vec2 p_position);

		void CullNodes(RootForce::Frustum* p_frustrum, QuadNode* p_node);
		void CullEntities(RootForce::Frustum* p_frustrum, QuadNode* p_node);
		void CullEntities(std::vector<glm::vec4>& p_points, QuadNode* p_node);

		void Init(RootEngine::GameSharedContext* p_context, ECS::World* p_world);

		std::vector<ECS::Entity*> m_entities;
		std::vector<int> m_culledEntities;
		std::vector<QuadNode*> m_culledNodes;

	private:
		
		int RoundToPow2(int p_value) const;
		void Subdivide( QuadNode* p_node, std::vector<Polygon> p_polygons );	
		PolygonSplit SplitPolygon(PlaneEx& p_divider, Polygon& p_polygon);
		Side::Side ClassifyPoint(PlaneEx& p_divider, glm::vec3 p_position) const;

		unsigned SplitVertex(PlaneEx& p_divider, Render::Vertex1P1N1UV1T1BT& p_p0, Render::Vertex1P1N1UV1T1BT& p_p1);

		glm::vec2 CalcXZCenter(const Polygon& p_polygon) const;
		std::vector<Triangle> Trianglulate(const std::vector<Polygon>& p_polygons) const;
		std::vector<unsigned> CreateEntities(std::vector<Triangle>& p_triangles, QuadNode* p_node);

		QuadNode* m_root;
		RootEngine::GameSharedContext* m_context;
		ECS::World* m_world;

		std::vector<Render::Vertex1P1N1UV1T1BT> m_vertices;
		
		std::vector<Render::Material*> m_materials;
		std::vector<int> m_sizes;

		int m_minY, m_maxY;	
	};
}