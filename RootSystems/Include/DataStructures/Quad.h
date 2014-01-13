#pragma once

#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/Shapes/AABB.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <vector>

#define QUADTREE_NODE_HEIGHT 100
#define QUADTREE_TRIANGLES_PER_NODE 100

#define QUAD_MAX_CHILDS 4

namespace RootForce
{
	class QuadNode
	{
	public:

		friend class QuadTree;

		//TODO: Skip user data and use an entity array.

		QuadNode(AABB& p_bounds, unsigned p_numTriangles, void* p_userData);
		~QuadNode();

		void AddChild(QuadNode* p_child);

	private:

		AABB m_bounds;
		unsigned m_numTriangles;


		void* m_userData;

		std::vector<QuadNode*> m_childs;
		unsigned m_numChilds;
	};

	class QuadTree
	{
	public:
		void Init(RootEngine::GameSharedContext* p_context, ECS::World* p_world);

	private:
		void Subdivide( QuadNode* p_node );
		int CountTriangles( glm::ivec4 p_rect );
		bool IsTriangleContained( glm::vec3 p_vertices[3], glm::ivec4 p_rect ); 

		RootEngine::GameSharedContext* m_context;
		QuadNode* m_root;

		std::vector<std::pair<ECS::Entity*, std::vector<glm::vec3>>> m_globalEntityList;
	};
}