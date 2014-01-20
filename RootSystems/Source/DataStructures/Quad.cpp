#include <RootSystems/Include/DataStructures/Quad.h>
#include <RootSystems/Include/RenderingSystem.h>
#include <RootSystems/Include/Shapes/Plane.h>

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

	const std::vector<QuadNode*>& QuadNode::GetChilds()
	{
		return m_childs;
	}

	const std::vector<ECS::Entity*>& QuadNode::GetEntities()
	{
		return m_entities;
	}

	QuadTree::QuadTree()
		: m_split(0)
	{

	}

	void QuadTree::Init(RootEngine::GameSharedContext* p_context, ECS::World* p_world)
	{
		m_context = p_context;
		m_world = p_world;

		// Find Static entities.
		ECS::GroupManager::GroupRange range = p_world->GetGroupManager()->GetEntitiesInGroup("Static");

		for(auto itr = range.first; itr != range.second; ++itr)
		{
			ECS::Entity* entity = (*itr).second;

			RootForce::Renderable* renderable = p_world->GetEntityManager()->GetComponent<RootForce::Renderable>(entity);
			auto material = renderable->m_material;
			auto mesh = renderable->m_model->m_meshes[0];

			RootForce::Transform* transform = p_world->GetEntityManager()->GetComponent<RootForce::Transform>(entity);
			auto scale = transform->m_scale;
			auto translation = transform->m_position;

			// Parse vertex data.
			glBindBuffer(GL_ARRAY_BUFFER, mesh->GetVertexBuffer()->GetBufferId());
			unsigned char* data = (unsigned char*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);

			for(unsigned i = 0; i < mesh->GetVertexBuffer()->GetBufferSize(); i += mesh->GetVertexBuffer()->GetElementSize())
			{
				Render::Vertex1P1N1UV v;

				memcpy(&v.m_pos, &data[i], sizeof(glm::vec3));
				memcpy(&v.m_normal, &data[i + 12], sizeof(glm::vec3));
				memcpy(&v.m_UV, &data[i + 24], sizeof(glm::vec2));

				m_vertices.push_back(v);
			}

			glUnmapBuffer(GL_ARRAY_BUFFER);

			// Parse index data.
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->GetElementBuffer()->GetBufferId());
			data = (unsigned char*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_READ_ONLY);

			for(unsigned i = 0; i < mesh->GetElementBuffer()->GetBufferSize(); i += mesh->GetElementBuffer()->GetElementSize() * 3)
			{
				Polygon p;
				int i0, i1, i2;

				memcpy(&i0, &data[i], sizeof(int));
				memcpy(&i1, &data[i + 4], sizeof(int));
				memcpy(&i2, &data[i + 8], sizeof(int));

				p.m_indices.push_back(i0);
				p.m_indices.push_back(i1);
				p.m_indices.push_back(i2);

				p.m_material = material;
				p.m_translation = translation;
				p.m_scale = scale;

				m_globalPolygonList.push_back(p);
			}

			glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
		}
		
		// Create an AABB for the quad tree structure.

		AABB quadTreeBounds;
		int maxX, maxY, maxZ, minX, minY, minZ;
		maxX = maxY = maxZ = -99999;
		minX = minY = minZ = 99999;

		unsigned numTriangles = m_globalPolygonList.size();

		for(auto itr = m_globalPolygonList.begin(); itr != m_globalPolygonList.end(); ++itr)
		{
			Render::Vertex1P1N1UV ps[3];

			ps[0] = m_vertices[(*itr).m_indices[0]];
			ps[1] = m_vertices[(*itr).m_indices[1]];
			ps[2] = m_vertices[(*itr).m_indices[2]];

			for(int i = 0; i < 3; i++)
			{
				float tx = ps[i].m_pos.x * (*itr).m_scale.x + (*itr).m_translation.x;
				float ty = ps[i].m_pos.y * (*itr).m_scale.y + (*itr).m_translation.y;
				float tz = ps[i].m_pos.z * (*itr).m_scale.z + (*itr).m_translation.z;

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

		m_root = new QuadNode(quadTreeBounds, numTriangles);

		m_context->m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "Begin subdividing %d", numTriangles);

		Subdivide(m_root, m_globalPolygonList);
	}

	int QuadTree::RoundToPow2(int p_value)
	{
		return pow(2, ceil(log(p_value)/log(2)));
	}

	void QuadTree::Subdivide( QuadNode* p_node,  std::vector<Polygon> p_polygons  )
	{
		if(p_polygons.size() == 0)
		{
			return;
		}
		else if(p_polygons.size() > QUADTREE_POLYGONS_PER_NODE)
		{
			m_context->m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "Generating 4 children. - Polygons: %d", p_polygons.size());

			AABB* aabb = &p_node->m_bounds;

			// Calculate center of the aabb.
			glm::vec3 center = aabb->GetCenter();

			// Define splitting planes.
			static glm::vec3 splittingNormals[2] = { glm::vec3(0, 0, 1), glm::vec3(1, 0, 0) };

			PlaneEx zp;
			zp.a = splittingNormals[0].x;
			zp.b = splittingNormals[0].y;
			zp.c = splittingNormals[0].z;
			zp.d = -glm::dot(center, splittingNormals[0]);

			PlaneEx xp;
			xp.a = splittingNormals[1].x;
			xp.b = splittingNormals[1].y;
			xp.c = splittingNormals[1].z;
			xp.d = -glm::dot(center, splittingNormals[1]);
			
			// Create a new polygon list.
			std::vector<Polygon> polygons;

			// Split all polygons with the x-plane.
			for(auto itr = p_polygons.begin(); itr != p_polygons.end(); ++itr)
			{
				PolygonSplit result = SplitPolygon(zp, (*itr));

				if(result.m_back.m_indices.size() > 0)
					polygons.push_back(result.m_back);

				if(result.m_front.m_indices.size() > 0)
					polygons.push_back(result.m_front);
			}

			// Split all polygons with the z-plane.
			for(auto itr = polygons.begin(); itr != p_polygons.end(); ++itr)
			{
				PolygonSplit result = SplitPolygon(xp, (*itr));

				if(result.m_back.m_indices.size() > 0)
					polygons.push_back(result.m_back);

				if(result.m_front.m_indices.size() > 0)
					polygons.push_back(result.m_front);
			}

			m_context->m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "Polygons: %d", p_polygons.size());

			std::vector<Polygon> bl;
			std::vector<Polygon> br;
			std::vector<Polygon> tl;
			std::vector<Polygon> tr;



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

			Subdivide( bottomLeftChild, p_polygons);
			Subdivide( bottomRightChild, p_polygons );
			Subdivide( topLeftChild, p_polygons );
			Subdivide( topRightChild, p_polygons );
		}
		else
		{
			m_context->m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "Adding data node. - Triangles: %d", p_node->m_numTriangles);
		}
	}

	int QuadTree::CountTriangles( Rectangle p_rect )
	{
		/*int numTriangles = 0;

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

		return numTriangles;*/

		return 0;
	}

	PolygonSplit QuadTree::SplitPolygon(PlaneEx& p_divider, Polygon& p_polygon)
	{
		PolygonSplit split;
		
		Render::Vertex1P1N1UV pa = m_vertices[p_polygon.m_indices[p_polygon.m_indices.size()-1]];
		int sa = ClassifyPoint(p_divider, pa.m_pos);

		for(int i = 0; i < p_polygon.m_indices.size(); ++i)
		{
			Render::Vertex1P1N1UV pb = m_vertices[p_polygon.m_indices[i]];
			int sb = ClassifyPoint(p_divider, pb.m_pos);

			if(sb == 0) // Behind
			{
				if(sa == 1) // Infront
				{
					int index = SplitVertex(p_divider, pa, pb);

					split.m_back.m_indices.push_back(index);
					split.m_front.m_indices.push_back(index);
				}

				split.m_back.m_indices.push_back(p_polygon.m_indices[i]);

			}
			else // Infront
			{
				if(sa == 0) // Behind
				{
					int index = SplitVertex(p_divider, pa, pb);

					split.m_front.m_indices.push_back(index);
					split.m_back.m_indices.push_back(index);
				}

				split.m_front.m_indices.push_back(p_polygon.m_indices[i]);
			}

			pa = pb;
			sa = sb;
		}

		return split;
	}

	int QuadTree::SplitVertex(PlaneEx& p_divider, Render::Vertex1P1N1UV& p_p0, Render::Vertex1P1N1UV& p_p1)
	{
		Line l;
		l.m_origin = p_p0.m_pos;
		l.m_direction = glm::normalize(p_p1.m_pos - p_p0.m_pos);

		float t = PlaneIntersectLine(p_divider, l);

		Render::Vertex1P1N1UV newVertex;
		newVertex.m_pos = p_p0.m_pos + t * l.m_direction;
		newVertex.m_normal = p_p0.m_normal;
		newVertex.m_UV = p_p0.m_UV + (p_p1.m_UV - p_p0.m_UV) * t;

		int index = m_vertices.size();
		m_vertices.push_back(newVertex);

		return index;
	}

	bool QuadTree::ClassifyPoint(PlaneEx& p_divider, glm::vec3 p_position)
	{
		glm::vec4 plane = glm::vec4(p_divider.a, p_divider.b, p_divider.c, p_divider.d);

		if(glm::dot(plane, glm::vec4(p_position, 1.0f)) < 0.0f)
		{
			return true;
		}

		return false;
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

	QuadNode* QuadTree::GetRoot() const
	{
		return m_root;
	}
}