#pragma once

#include <RootSystems/Include/QuadNode.h>

#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/Shapes/AABB.h>
#include <RootSystems/Include/Shapes/Plane.h>
#include <RootSystems/Include/Frustum.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootEngine/Render/Include/Mesh.h>
#include <RootEngine/Render/Include/Vertex.h>
#include <vector>

//#define QUADTREE_VERBOSE
#define QUADTREE_DRAWLINES

namespace RootForce
{
	// Represent a triangle polygon with material index.
	struct Triangle
	{
		unsigned m_indices[3];
		unsigned m_materialIndex;
	};

	// Represent a n-polygon with material index.
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
		
		// Create a quad tree bound by an entity group.
		void Initialize(RootEngine::GameSharedContext* p_context, ECS::World* p_world, const std::string& p_groupName);

		// Divide the polygons and create the tree.
		void BeginDivide(unsigned int p_polygonsPerNode, bool p_splitPolygons, bool p_removeOrigionalEntities);

		// Debug draw nodes of the tree.
		void RenderDebug(const glm::vec3& p_color);
		void RenderNodeDebug(QuadNode* p_node, const glm::vec3& p_color);	

		QuadNode* GetRoot() const;
		QuadNode* PickRoot(glm::vec2 p_position);
		QuadNode* PickNode(QuadNode* p_node, glm::vec2 p_position);

		void Cull(RootForce::Frustum* p_frustrum, QuadNode* p_node);
		void Cull(std::vector<glm::vec4>& p_points, QuadNode* p_node);

		// Culling results.
		std::vector<ECS::Entity*> m_entities;

		std::vector<QuadNode*> m_culledNodes;
		std::vector<int> m_culledEntities;

	private:	

		// Operations for splitting a distrubuting polygons in the tree.
		int RoundToPow2(int p_value) const;
		void Subdivide( QuadNode* p_node, std::vector<Polygon> p_polygons );	
		PolygonSplit SplitPolygon(PlaneEx& p_divider, Polygon& p_polygon);
		Side::Side ClassifyPoint(PlaneEx& p_divider, glm::vec3 p_position) const;
		unsigned SplitVertex(PlaneEx& p_divider, Render::Vertex1P1N1UV1T1BT& p_p0, Render::Vertex1P1N1UV1T1BT& p_p1);
		glm::vec2 CalcXZCenter(const Polygon& p_polygon) const;
		std::vector<Triangle> Trianglulate(const std::vector<Polygon>& p_polygons) const;

		// Constructs new entities from a triangle set.
		std::vector<unsigned> CreateEntities(std::vector<Triangle>& p_triangles);

		// Entity creation.
		void CreatePNUVEntity(unsigned int p_entityId, unsigned int p_materialIndex, std::vector<Render::Vertex1P1N1UV>& p_vertices, std::vector<unsigned>& p_indices);
		void CreatePNUVTBTEntity(unsigned int p_entityId, unsigned int p_materialIndex, std::vector<Render::Vertex1P1N1UV1T1BT>& p_vertices, std::vector<unsigned>& p_indices);
		
		// Id counter.
		int m_idCounter;

		// Name of entity group contained.
		std::string m_groupName;

		// Polygon bound by the tree.
		std::vector<Polygon> m_boundsPolygons;

		// Polygons per node.
		unsigned int m_polygonsPerNode;

		// Split polygons.
		bool m_splitPolygons;
		
		// Vertex/Material pool.
		std::vector<Render::Vertex1P1N1UV1T1BT> m_vertices;
		std::vector<Render::Material*> m_materials;

		// TODO: Use type.
		// Sizes of polygons attributes.
		std::vector<int> m_sizes;

		// Height of tree.
		int m_minY;
		int m_maxY;	

		RootEngine::GameSharedContext* m_context;
		ECS::World* m_world;
		QuadNode* m_root;
	};
}