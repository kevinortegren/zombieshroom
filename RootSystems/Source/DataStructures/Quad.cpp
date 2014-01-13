#include <RootSystems/Include/DataStructures/Quad.h>
#include <RootSystems/Include/RenderingSystem.h>

namespace RootForce
{
	QuadNode::QuadNode(AABB& p_bounds, unsigned p_numTriangles, void* p_userData)
		: m_numChilds(0), m_bounds(p_bounds), m_numTriangles(p_numTriangles), m_userData(p_userData)
	{

	}

	QuadNode::~QuadNode()
	{
		if(m_userData != nullptr)
		{
			delete m_userData;
			m_userData = nullptr;
		}
	}

	void QuadNode::AddChild(QuadNode* p_child)
	{
		assert(m_numChilds >= QUAD_MAX_CHILDS);

		m_childs.push_back(p_child);
		m_numChilds++;
	}

	void QuadTree::Init(RootEngine::GameSharedContext* p_context, ECS::World* p_world)
	{
		m_context = p_context;

		// Fill the global entity list with entities.

		ECS::GroupManager::GroupRange range = p_world->GetGroupManager()->GetEntitiesInGroup("Split");

		for(auto itr = range.first; itr != range.second; ++itr)
		{
			std::pair<ECS::Entity*, std::vector<glm::vec3>> entry;

			entry.first = (*itr).second;

			RootForce::Renderable* renderable = p_world->GetEntityManager()->GetComponent<RootForce::Renderable>(entry.first);

			for(auto itr = renderable->m_model->m_meshes.begin(); itr != renderable->m_model->m_meshes.end(); ++itr)
			{
				std::shared_ptr<Render::BufferInterface> buffer = (*itr)->GetVertexBuffer();

				char* data = (char*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);

				for(int i = 0; i < buffer->GetBufferSize(); i += buffer->GetElementSize())
				{
					float x = (float)data[i];
					float y = (float)data[i + sizeof(float)];
					float z = (float)data[i + 2 * sizeof(float)];

					entry.second.push_back(glm::vec3(x,y,z));
				}

				glUnmapBuffer(GL_ARRAY_BUFFER);
			}

			m_globalEntityList.push_back(entry);
		}
		
		// Create an AABB for the quad tree structure.

		AABB quadTreeBounds;
		int maxX, maxY, maxZ, minX, minY, minZ;
		maxX = maxY = maxZ = -99999;
		minX = minY = minZ = 99999;

		unsigned numTriangles = 0;

		for(auto itr = m_globalEntityList.begin(); itr != m_globalEntityList.end(); ++itr)
		{
			numTriangles += (*itr).second.size();

			for(auto pos = (*itr).second.begin(); pos != (*itr).second.end(); ++pos)
			{
				if(pos->x > maxX) maxX = pos->x;
				if(pos->x < minX) minX = pos->x;
				
				if(pos->y > maxY) maxY = pos->y;
				if(pos->y < minY) minY = pos->y;

				if(pos->z > maxZ) maxZ = pos->z;
				if(pos->z < minZ) minZ = pos->z;
			}
		}
		
		quadTreeBounds.m_maxX = maxX;
		quadTreeBounds.m_maxY = maxY;
		quadTreeBounds.m_maxZ = maxZ;
		quadTreeBounds.m_minX = minX;
		quadTreeBounds.m_minY = minY;
		quadTreeBounds.m_minZ = minZ;

		m_root = new QuadNode(quadTreeBounds, numTriangles, nullptr);

		m_context->m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "Begin subdividing %d", numTriangles);

		Subdivide(m_root);
	}

	void QuadTree::Subdivide( QuadNode* p_node )
	{
		if(p_node->m_numTriangles == 0)
		{
			m_context->m_logger->LogText(LogTag::GAME, LogLevel::FATAL_ERROR, "Attemt to subdivide node with 0 triangles.");
		}
		else if(p_node->m_numTriangles > QUADTREE_TRIANGLES_PER_NODE)
		{
			m_context->m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "Generating 4 children.");

			AABB* aabb = &p_node->m_bounds;

			int halfwidth = aabb->GetLengthX() / 2;
			int halfheight = aabb->GetLengthZ() / 2;

			glm::ivec4 bottomLeft = glm::ivec4(aabb->m_minX, aabb->m_minZ, halfwidth, halfheight);
			glm::ivec4 bottomRight = glm::ivec4(aabb->m_minX + halfwidth, aabb->m_minZ, halfwidth, halfheight);
			glm::ivec4 topLeft = glm::ivec4(aabb->m_minX, aabb->m_minZ + halfheight, halfwidth, halfheight);
			glm::ivec4 topRight = glm::ivec4(aabb->m_minX + halfwidth, aabb->m_minZ + halfheight, halfwidth, halfheight);

			AABB bottomLeftAABB = AABB(bottomLeft.x, bottomLeft.x + halfwidth, 0, QUADTREE_NODE_HEIGHT, bottomLeft.y, bottomLeft.y + halfheight);
			AABB bottomRightAABB = AABB(bottomRight.x, bottomRight.x + halfwidth, 0, QUADTREE_NODE_HEIGHT, bottomRight.z, bottomRight.w + halfheight);
			AABB topLeftAABB = AABB(topLeft.x, topLeft.x + halfwidth, 0, QUADTREE_NODE_HEIGHT, topLeft.z, topLeft.w + halfheight);
			AABB topRightAABB = AABB(topRight.x, topRight.x + halfwidth, 0, QUADTREE_NODE_HEIGHT, topRight.z, topRight.w + halfheight);

			// Count triangles for each child and send new triangles.

			QuadNode* bottomLeftChild = new QuadNode(bottomLeftAABB, CountTriangles( bottomLeft ), nullptr);
			QuadNode* bottomRightChild = new QuadNode(bottomRightAABB, CountTriangles( bottomRight ), nullptr);
			QuadNode* topLeftChild = new QuadNode(topLeftAABB, CountTriangles( topLeft ), nullptr);
			QuadNode* topRightChild = new QuadNode(topRightAABB, CountTriangles( topRight ), nullptr);

			Subdivide( bottomLeftChild );
			Subdivide( bottomRightChild );
			Subdivide( topLeftChild );
			Subdivide( topRightChild );
		}
		else
		{
			m_context->m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "Adding data node.");

			//TODO: Create new entities by checking if entities are contained in the regions.

			//MESSY: Modify the 

		}
	}

	int QuadTree::CountTriangles( glm::ivec4 p_rect )
	{
		int numTriangles = 0;

		// TODO: Foreach e 
		// foreach pos
		// check contained.
		
		for(int i = 0; i < 0; i+= 3)
		{
			glm::vec3 verts[3];

			if( IsTriangleContained(verts, p_rect) )
			{
				numTriangles++;
			}
		}

		return numTriangles;
	}

	bool QuadTree::IsTriangleContained( glm::vec3 p_vertices[3], glm::ivec4 p_rect )
	{
		float x1, x2, x3, z1, z2, z3;

		x1 = p_vertices[0].x;
		z1 = p_vertices[0].z;

		x2 = p_vertices[1].x;
		z2 = p_vertices[1].z;

		x3 = p_vertices[2].x;
		z3 = p_vertices[2].z;

		float minX = glm::min(x1, glm::min(x2, x3) );
		if( minX > p_rect.x + p_rect.z )
		{
			return false;
		}

		float maxX = glm::max(x1, glm::max(x2, x3) );
		if( maxX < p_rect.x )
		{
			return false;
		}

		float minZ = glm::min(z1, glm::min(z2, z3) );
		if( minZ > p_rect.y + p_rect.w )
		{
			return false;
		}

		float maxZ = glm::max(z1, glm::max(z2, z3) );
		if( maxZ < p_rect.y )
		{
			return false;
		}

		return true;
	}
}