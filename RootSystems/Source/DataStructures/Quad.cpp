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
		ECS::GroupManager::GroupRange range = p_world->GetGroupManager()->GetEntitiesInGroup("Split");

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
				memcpy(&v, &data[i], sizeof(Render::Vertex1P1N1UV));

				v.m_pos.x = v.m_pos.x * transform->m_scale.x + transform->m_position.x;
				v.m_pos.y = v.m_pos.y * transform->m_scale.y + transform->m_position.y;
				v.m_pos.z = v.m_pos.z * transform->m_scale.z + transform->m_position.z;


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

				m_globalPolygonList.push_back(std::move(p));
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
				float tx = ps[i].m_pos.x;
				float ty = ps[i].m_pos.y;
				float tz = ps[i].m_pos.z;

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
		quadTreeBounds.m_minX = -RoundToPow2(maxX);

		quadTreeBounds.m_maxY = maxY;
		quadTreeBounds.m_minY = minY;

		quadTreeBounds.m_maxZ = RoundToPow2(maxZ);
		quadTreeBounds.m_minZ = -RoundToPow2(maxZ);

		m_root = new QuadNode(quadTreeBounds, numTriangles);

		m_context->m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "Begin subdividing %d", numTriangles);

		Subdivide(m_root, m_globalPolygonList);

		std::vector<Triangle> tris = Trianglulate(m_globalPolygonList);

		auto a = m_world->GetGroupManager()->GetEntitiesInGroup("Static");
		for(auto i = a.first; i != a.second; ++i)
		{
			m_world->GetEntityManager()->RemoveEntity(i->second);
		}

		CreateEntities(tris);
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
			static glm::vec3 splittingNormals[2] = { glm::vec3(0, 0, -1), glm::vec3(-1, 0, 0) };

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
			std::vector<Polygon> polygonsAfterZSplit;

			// Split all polygons with the x-plane.
			for(int i = 0; i < p_polygons.size(); ++i)
			{
				PolygonSplit result = SplitPolygon(zp, p_polygons[i]);

				if(result.m_back.m_indices.size() > 0)
					polygonsAfterZSplit.push_back(result.m_back);

				if(result.m_front.m_indices.size() > 0)
					polygonsAfterZSplit.push_back(result.m_front);
			}

			m_context->m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "Created %d new Polygons.", polygonsAfterZSplit.size() - p_polygons.size());
			m_context->m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "Polygons after ZSplit: %d", polygonsAfterZSplit.size());

			std::vector<Polygon> polygonsAfterXSplit;

			// Split all polygons with the z-plane.
			for(int i = 0; i < polygonsAfterZSplit.size(); ++i)
			{
				PolygonSplit result = SplitPolygon(xp, polygonsAfterZSplit[i]);

				if(result.m_back.m_indices.size() > 0)
					polygonsAfterXSplit.push_back(result.m_back);

				if(result.m_front.m_indices.size() > 0)
					polygonsAfterXSplit.push_back(result.m_front);
			}

			m_context->m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "Created %d new Polygons.", polygonsAfterXSplit.size() - polygonsAfterZSplit.size());
			m_context->m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "Polygons after XSplit: %d", polygonsAfterXSplit.size());

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

			std::vector<Polygon> bl = DividePolygons(bottomLeft, polygonsAfterZSplit);
			std::vector<Polygon> br = DividePolygons(bottomRight, polygonsAfterZSplit);
			std::vector<Polygon> tl = DividePolygons(topLeft, polygonsAfterZSplit);
			std::vector<Polygon> tr = DividePolygons(topRight, polygonsAfterZSplit);

			m_context->m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "Bottom Left Polygons: %d", bl.size());
			m_context->m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "Bottom Right Polygons: %d", br.size());
			m_context->m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "Top Left Polygons: %d", tl.size());
			m_context->m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "Top Right Polygons: %d", tr.size());

			m_context->m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "Polycount: %d", bl.size() + br.size() + tl.size() + tr.size());

			QuadNode* bottomLeftChild = new QuadNode(bottomLeftAABB, bl.size());
			QuadNode* bottomRightChild = new QuadNode(bottomRightAABB, br.size());
			QuadNode* topLeftChild = new QuadNode(topLeftAABB, tl.size());
			QuadNode* topRightChild = new QuadNode(topRightAABB, tr.size());

			p_node->AddChild(bottomLeftChild);
			p_node->AddChild(bottomRightChild);
			p_node->AddChild(topLeftChild);
			p_node->AddChild(topRightChild);

			//Subdivide( bottomLeftChild, bl);
			//Subdivide( bottomRightChild, br );
			//Subdivide( topLeftChild, tl );
			//Subdivide( topRightChild, tr );

			m_globalPolygonList = polygonsAfterZSplit;
		}
		else
		{
			m_context->m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "Adding data node. - Polygons: %d", p_polygons.size());



		}
	}

	std::vector<Polygon> QuadTree::DividePolygons( Rectangle p_rect, std::vector<Polygon> p_polygons )
	{
		std::vector<Polygon> polygons;

		for(auto polygon = p_polygons.begin(); polygon != p_polygons.end(); ++polygon)
		{
			glm::vec3 center = CalcCenter((*polygon));

			if(p_rect.IsPointContained(glm::vec2(center.x, center.z)))
			{
				polygons.push_back((*polygon));
			}
		}

		return polygons;
	}

	std::vector<Polygon> QuadTree::DividePolygonsEx( Rectangle p_rect, std::vector<Polygon> p_polygons )
	{
		std::vector<Polygon> polygons;

		float bias = 0.1f;
		for(int i = 0; i < p_polygons.size(); ++i)
		{
			bool add = true;
			for(int j = 0; j < p_polygons[i].m_indices.size(); ++j)
			{
				glm::vec3 p = m_vertices[p_polygons[i].m_indices[j]].m_pos;

				if((p.x-bias >= p_rect.m_x && p.x <= p_rect.m_x + p_rect.m_width+bias) &&
					( p.z-bias >= p_rect.m_y && p.z <= p_rect.m_y + p_rect.m_height+bias ))
				{

				}
				else
				{
					add = false;
					break;
				}

			}

			if(add)
			{
				polygons.push_back(p_polygons[i]);
			}	
		}

		return polygons;
	}

	glm::vec3 QuadTree::CalcCenter(Polygon& p_polygon)
	{
		float area = 0;
		float cx = 0;
		float cy = 0;
		float x0 = 0;
		float y0 = 0;
		float x1 = 0;
		float y1 = 0;
		float a = 0;

		int i;
		for(i = 0; i < p_polygon.m_indices.size()-1; ++i)
		{
			x0 = m_vertices[p_polygon.m_indices[i]].m_pos.x;
			y0 = m_vertices[p_polygon.m_indices[i]].m_pos.z;
			x1 = m_vertices[p_polygon.m_indices[i+1]].m_pos.x;
			y1 = m_vertices[p_polygon.m_indices[i+1]].m_pos.z;

			a = x0 * y1 - x1 * y0;
			area += a;

			cx += (x0 + x1) * a;
			cy += (y0 + y1) * a;
		}

		x0 = m_vertices[p_polygon.m_indices[i]].m_pos.x;
		y0 = m_vertices[p_polygon.m_indices[i]].m_pos.z;
		x1 = m_vertices[p_polygon.m_indices[0]].m_pos.x;
		y1 = m_vertices[p_polygon.m_indices[0]].m_pos.z;

		a = x0 * y1 - x1 * y0;
		area += a;

		cx += (x0 + x1) * a;
		cy += (y0 + y1) * a;

		area *= 0.5f;
		cx /= (6.0f * area);
		cy /= (6.0f * area);
		
		return glm::vec3(cx, 0, cy);
	}

	PolygonSplit QuadTree::SplitPolygon(PlaneEx& p_divider, Polygon& p_polygon)
	{
		PolygonSplit split;

		split.m_back.m_scale = p_polygon.m_scale;
		split.m_back.m_translation = p_polygon.m_translation;
		split.m_back.m_material = p_polygon.m_material;

		split.m_front.m_scale = p_polygon.m_scale;
		split.m_front.m_translation = p_polygon.m_translation;
		split.m_front.m_material = p_polygon.m_material;

		Render::Vertex1P1N1UV pa = m_vertices[p_polygon.m_indices[p_polygon.m_indices.size()-1]];

		int sa = ClassifyPoint(p_divider, pa.m_pos);

		for(int i = 0; i < p_polygon.m_indices.size(); ++i)
		{
			Render::Vertex1P1N1UV pb = m_vertices[p_polygon.m_indices[i]];

			int sb = ClassifyPoint(p_divider, pb.m_pos);

			if(sb == Side::BACK) // Behind
			{
				if(sa == Side::FRONT) // Infront
				{
					int index = SplitVertex(p_divider, pa, pb);

					split.m_front.m_indices.push_back(index);
					split.m_back.m_indices.push_back(index);			
				}

				split.m_back.m_indices.push_back(p_polygon.m_indices[i]);

			}
			else if(sb == Side::FRONT)// Infront
			{
				if(sa == Side::BACK) // Behind
				{
					int index = SplitVertex(p_divider, pa, pb);

					split.m_front.m_indices.push_back(index);
					split.m_back.m_indices.push_back(index);
				}

				split.m_front.m_indices.push_back(p_polygon.m_indices[i]);
			}
			else
			{
				split.m_front.m_indices.push_back(p_polygon.m_indices[i]);
				split.m_back.m_indices.push_back(p_polygon.m_indices[i]);
			}

			pa = pb;
			sa = sb;
		}

		return split;
	}

	int QuadTree::SplitVertex(PlaneEx& p_divider, Render::Vertex1P1N1UV& p_p0, Render::Vertex1P1N1UV& p_p1)
	{

		/*Line l;
		l.m_origin = p_p0.m_pos;
		l.m_direction = p_p1.m_pos - p_p0.m_pos;

		float t = PlaneIntersectLine(p_divider, l);

		Render::Vertex1P1N1UV as;
		as.m_pos = l.m_origin + l.m_direction * t;*/

		glm::vec3 i = PlaneIntersectLineA(p_divider, p_p0.m_pos, p_p1.m_pos);

		Render::Vertex1P1N1UV newVertex;
		newVertex.m_pos = i;
		newVertex.m_normal = p_p0.m_normal;
		newVertex.m_UV = glm::vec2(1,1);

		int index = m_vertices.size();
		m_vertices.push_back(newVertex);

		return index;
	}

	Side::Side QuadTree::ClassifyPoint(PlaneEx& p_divider, glm::vec3 p_position)
	{
		glm::vec4 plane = glm::vec4(p_divider.a, p_divider.b, p_divider.c, p_divider.d);

		float result = glm::dot(plane, glm::vec4(p_position, 1.0f));

		if(result < 0.0f)
		{
			return Side::BACK;
		}
		else if(result == 0.0f)
		{
			return Side::NONE;
		}
		else
		{
			return Side::FRONT;
		}
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

	std::vector<Triangle> QuadTree::Trianglulate(std::vector<Polygon> p_polygons)
	{
		std::vector<Triangle> triangles;

		for(int i = 0; i < p_polygons.size(); ++i)
		{
			Polygon p = p_polygons[i];

			for(int j = 2; j < p.m_indices.size(); j++)
			{
				Triangle t;
				t.m_indices[0] = p.m_indices[0];
				t.m_indices[1] = p.m_indices[j-1];
				t.m_indices[2] = p.m_indices[j];

				triangles.push_back(t);
			}
		}

		return triangles;
	}

	void QuadTree::CreateEntities(std::vector<Triangle> p_triangles)
	{
		std::sort(p_triangles.begin(), p_triangles.end(), [](Triangle& a, Triangle& b)->bool{ return a.m_material > b.m_material; });

		std::vector<unsigned int> indices;
		std::vector<Render::Vertex1P1N1UV> vertices;

		Render::Material* wp = p_triangles[0].m_material;
		for(int i = 0; i < p_triangles.size(); i++)
		{
			if(p_triangles[i].m_material != wp)
			{
				wp = p_triangles[i].m_material;
				
				ECS::Entity* newE = m_world->GetEntityManager()->CreateEntity();
				RootForce::Renderable* r = m_world->GetEntityManager()->CreateComponent<RootForce::Renderable>(newE);
				r->m_model = m_context->m_resourceManager->CreateModel("asda");
				r->m_model->m_meshes[0]->CreateVertexBuffer1P1N1UV(&m_vertices[0], m_vertices.size());
				r->m_model->m_meshes[0]->CreateIndexBuffer(&indices[0], indices.size());
				r->m_material = wp;
				
				RootForce::Transform* t = m_world->GetEntityManager()->CreateComponent<RootForce::Transform>(newE);
				t->m_position = p_triangles[0].m_translation;
				t->m_scale = p_triangles[0].m_scale;
			}

			// Push triangle face.
			indices.push_back(p_triangles[i].m_indices[0]);
			indices.push_back(p_triangles[i].m_indices[1]);
			indices.push_back(p_triangles[i].m_indices[2]);
		}
	}

	void CreateEntity();

}