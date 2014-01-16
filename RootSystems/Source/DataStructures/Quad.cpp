#include <RootSystems/Include/DataStructures/Quad.h>
#include <RootSystems/Include/RenderingSystem.h>

namespace RootForce
{
	QuadNode::QuadNode(AABB& p_bounds, unsigned p_numTriangles)
		: m_bounds(p_bounds), m_numTriangles(p_numTriangles)
	{

	}

	QuadNode::~QuadNode()
	{

	}

	void QuadNode::AddChild(QuadNode* p_child)
	{
		assert(m_childs.size() < QUAD_MAX_CHILDS);

		m_childs.push_back(p_child);
	}

	const AABB& QuadNode::GetBounds() const
	{
		return m_bounds;
	}

	void QuadTree::Init(RootEngine::GameSharedContext* p_context, ECS::World* p_world)
	{
		m_globalEntityList.clear();

		m_context = p_context;
		m_world = p_world;

		// Fill the global entity list with entities.

		ECS::GroupManager::GroupRange range = p_world->GetGroupManager()->GetEntitiesInGroup("Static");

		for(auto itr = range.first; itr != range.second; ++itr)
		{
			std::pair<ECS::Entity*, MeshData> entry;

			entry.first = (*itr).second;

			RootForce::Renderable* renderable = p_world->GetEntityManager()->GetComponent<RootForce::Renderable>(entry.first);
			auto mesh = renderable->m_model->m_meshes[0];
			
			MeshData meshData;

			// Parse vertex data.
			glBindBuffer(GL_ARRAY_BUFFER, mesh->GetVertexBuffer()->GetBufferId());
			unsigned char* data = (unsigned char*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);

			for(unsigned i = 0; i < mesh->GetVertexBuffer()->GetBufferSize(); i += mesh->GetVertexBuffer()->GetElementSize())
			{
				glm::vec3 pos;
				memcpy(&pos, &data[i], sizeof(glm::vec3));

				meshData.m_positions.push_back(pos);
			}
			glUnmapBuffer(GL_ARRAY_BUFFER);

			// Parse index data.
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->GetElementBuffer()->GetBufferId());
			data = (unsigned char*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_READ_ONLY);

			for(unsigned i = 0; i < mesh->GetElementBuffer()->GetBufferSize(); i += mesh->GetElementBuffer()->GetElementSize())
			{
				int index = 0;
				memcpy(&index, &data[i], sizeof(int));

				meshData.m_indices.push_back(index);
			}

			glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

			entry.second = meshData;

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
			numTriangles += (*itr).second.m_indices.size() / 3;

			RootForce::Transform* transform = p_world->GetEntityManager()->GetComponent<RootForce::Transform>((*itr).first);
		
			for(auto pos = (*itr).second.m_positions.begin(); pos != (*itr).second.m_positions.end(); ++pos)
			{
				float tx = pos->x * transform->m_scale.x + transform->m_position.x;
				float ty = pos->y * transform->m_scale.y + transform->m_position.y;
				float tz = pos->z * transform->m_scale.z + transform->m_position.z;

				if(tx > maxX) maxX = (int)tx;
				if(tx < minX) minX = (int)tx;
				
				if(ty > maxY) maxY = (int)ty;
				if(ty < minY) minY = (int)ty;

				if(tz > maxZ) maxZ = (int)tz;
				if(tz < minZ) minZ = (int)tz;
			}
		}
		
		m_maxY = maxY;
		m_minY = minY;

		quadTreeBounds.m_maxX = RoundToPow2(maxX);
		quadTreeBounds.m_minX = -RoundToPow2(abs(minX));

		quadTreeBounds.m_maxY = maxY;
		quadTreeBounds.m_minY = minY;

		quadTreeBounds.m_maxZ = RoundToPow2(maxZ);
		quadTreeBounds.m_minZ = -RoundToPow2(abs(minZ));

		Rectangle bottomLeft = Rectangle(quadTreeBounds.m_minX, quadTreeBounds.m_minZ, 2048, 2048);

		int a = CountTriangles(bottomLeft);

		m_root = new QuadNode(quadTreeBounds, numTriangles);

		m_context->m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "Begin subdividing %d", numTriangles);

		Subdivide(m_root);
	}

	int QuadTree::RoundToPow2(int p_value)
	{
		return pow(2, ceil(log(p_value)/log(2)));
	}

	void QuadTree::Subdivide( QuadNode* p_node )
	{
		if(p_node->m_numTriangles == 0)
		{
			return;
		}
		else if(p_node->m_numTriangles > QUADTREE_TRIANGLES_PER_NODE)
		{
			m_context->m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "Generating 4 children. - Triangles: %d", p_node->m_numTriangles);

			AABB* aabb = &p_node->m_bounds;

			int halfwidth = aabb->GetLengthX() / 2;
			int halfheight = aabb->GetLengthZ() / 2;

			Rectangle bottomLeft = Rectangle(aabb->m_minX, aabb->m_minZ, halfwidth, halfheight);
			Rectangle bottomRight = Rectangle(aabb->m_minX + halfwidth, aabb->m_minZ, halfwidth, halfheight);
			Rectangle topLeft = Rectangle(aabb->m_minX, aabb->m_minZ + halfheight, halfwidth, halfheight);
			Rectangle topRight = Rectangle(aabb->m_minX + halfwidth, aabb->m_minZ + halfheight, halfwidth, halfheight);

			AABB bottomLeftAABB = AABB(bottomLeft.m_x, bottomLeft.m_x + halfwidth, m_minY, m_maxY, bottomLeft.m_y, bottomLeft.m_y + halfheight);
			AABB bottomRightAABB = AABB(bottomRight.m_x, bottomRight.m_x + halfwidth, m_minY, m_maxY, bottomRight.m_y, bottomRight.m_y + halfheight);
			AABB topLeftAABB = AABB(topLeft.m_x, topLeft.m_x + halfwidth, m_minY, m_maxY, topLeft.m_y, topLeft.m_y + halfheight);
			AABB topRightAABB = AABB(topRight.m_x, topRight.m_x + halfwidth, m_minY, m_maxY, topRight.m_y, topRight.m_y + halfheight);

			int bottomLeftTrisCount = CountTriangles( bottomLeft );
			int bottomRightTrisCount = CountTriangles( bottomRight );
			int topLeftTrisCount = CountTriangles( topLeft );
			int topRightTrisCount = CountTriangles( topRight );

			QuadNode* bottomLeftChild = new QuadNode(bottomLeftAABB, bottomLeftTrisCount);
			QuadNode* bottomRightChild = new QuadNode(bottomRightAABB, bottomRightTrisCount);
			QuadNode* topLeftChild = new QuadNode(topLeftAABB, topLeftTrisCount);
			QuadNode* topRightChild = new QuadNode(topRightAABB, topRightTrisCount);

			p_node->AddChild(bottomLeftChild);
			p_node->AddChild(bottomRightChild);
			p_node->AddChild(topLeftChild);
			p_node->AddChild(topRightChild);

			Subdivide( bottomLeftChild );
			Subdivide( bottomRightChild );
			Subdivide( topLeftChild );
			Subdivide( topRightChild );
		}
		else
		{
			m_context->m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "Adding data node. - Triangles: %d", p_node->m_numTriangles);

			bool createNewEntity = false;
			
			for(auto entity = m_globalEntityList.begin(); entity != m_globalEntityList.end(); ++entity)
			{
				for(unsigned i = 0; i < entity->second.m_indices.size(); i += 3)
				{
					unsigned v0 = entity->second.m_indices[i];
					unsigned v1 = entity->second.m_indices[i+1];
					unsigned v2 = entity->second.m_indices[i+2];

					glm::vec3 verts[3];
					verts[0] = entity->second.m_positions[v0];
					verts[1] = entity->second.m_positions[v1];
					verts[2] = entity->second.m_positions[v2];
		
					if( IsTriangleContained(verts, p_node->m_bounds.GetXZRect()) )
					{
						//TODO: Create new entity.
						// Strip triangles not contained in the node.
					}
				}
			}
		}
	}

	int QuadTree::CountTriangles( Rectangle p_rect )
	{
		int numTriangles = 0;

		for(auto entity = m_globalEntityList.begin(); entity != m_globalEntityList.end(); ++entity)
		{
			RootForce::Transform* transform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>((*entity).first);

			for(unsigned i = 0; i < entity->second.m_indices.size(); i += 3)
			{
				unsigned v0 = entity->second.m_indices[i];
				unsigned v1 = entity->second.m_indices[i+1];
				unsigned v2 = entity->second.m_indices[i+2];

				glm::vec3 p0 = entity->second.m_positions[v0];
				glm::vec3 p1 = entity->second.m_positions[v0];
				glm::vec3 p2 = entity->second.m_positions[v0];

				glm::vec3 verts[3];
				verts[0] = p0 * transform->m_scale + transform->m_position;
				verts[1] = p1 * transform->m_scale + transform->m_position;
				verts[2] = p2 * transform->m_scale + transform->m_position;
		
				if( IsTriangleContained(verts, p_rect) )
				{
					numTriangles++;
				}
			}
		}

		return numTriangles;
	}

	bool QuadTree::IsTriangleContained( glm::vec3 p_vertices[3], Rectangle p_rect )
	{
		float x1, x2, x3, z1, z2, z3;

		x1 = p_vertices[0].x;
		z1 = p_vertices[0].z;

		x2 = p_vertices[1].x;
		z2 = p_vertices[1].z;

		x3 = p_vertices[2].x;
		z3 = p_vertices[2].z;

		float minX = glm::min(x1, glm::min(x2, x3) );
		if( minX > p_rect.m_x + p_rect.m_width )
		{
			return false;
		}

		float maxX = glm::max(x1, glm::max(x2, x3) );
		if( maxX < p_rect.m_x )
		{
			return false;
		}

		float minZ = glm::min(z1, glm::min(z2, z3) );
		if( minZ > p_rect.m_y + p_rect.m_height )
		{
			return false;
		}

		float maxZ = glm::max(z1, glm::max(z2, z3) );
		if( maxZ < p_rect.m_y )
		{
			return false;
		}

		return true;
	}

	QuadNode* QuadTree::PickRoot(glm::vec2 p_position)
	{
		return PickNode(m_root, p_position);
	}

	QuadNode* QuadTree::PickNode(QuadNode* p_node, glm::vec2 p_position)
	{
		QuadNode* currentNode = p_node;
		if( currentNode->m_bounds.GetXZRect().IsPointContained(p_position) )
		{
			if(currentNode->m_childs.size() == 0)
			{
				return currentNode;
			}

			for( unsigned i = 0; i < currentNode->m_childs.size(); ++i)
			{
				QuadNode* r = PickNode(currentNode->m_childs[i], p_position);
				if(r != nullptr)
				{
					return r;
				}
			}
		}

		return nullptr;
	}

	void QuadTree::RenderDebug()
	{
		RenderNode(m_root);
	}

	void QuadTree::RenderNode(QuadNode* p_node)
	{
		p_node->m_bounds.DebugDraw(m_context->m_renderer, glm::vec3(1,0,0));

		for(unsigned i = 0; i < p_node->m_childs.size(); ++i)
		{
			RenderNode(p_node->m_childs[i]);
		}
	}
}