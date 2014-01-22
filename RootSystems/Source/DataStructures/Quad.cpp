#include <RootSystems/Include/DataStructures/Quad.h>
#include <RootSystems/Include/RenderingSystem.h>
#include <RootSystems/Include/Shapes/Plane.h>
#include <glm/gtx/transform.hpp>

namespace RootForce
{
	QuadNode::QuadNode(AABB& p_bounds)
		: m_bounds(p_bounds) {}

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

	void QuadTree::Init(RootEngine::GameSharedContext* p_context, ECS::World* p_world)
	{
		m_vertices.clear();
		m_globalPolygonList.clear();
		m_materials.clear();

		m_context = p_context;
		m_world = p_world;

		// Find Static entities.
		ECS::GroupManager::GroupRange range = p_world->GetGroupManager()->GetEntitiesInGroup("Static");

		unsigned int indexOffset = 0;

		int matPtr = 0;

		for(auto itr = range.first; itr != range.second; ++itr)
		{
			ECS::Entity* entity = (*itr).second;

			RootForce::Renderable* renderable = p_world->GetEntityManager()->GetComponent<RootForce::Renderable>(entity);
			auto material = renderable->m_material;
			auto mesh = renderable->m_model->m_meshes[0];

			unsigned int materialIndex = m_materials.size();

			m_materials.push_back(material);

			RootForce::Transform* transform = p_world->GetEntityManager()->GetComponent<RootForce::Transform>(entity);

			glm::mat4x4 transformMatrix;
			transformMatrix = glm::translate(glm::mat4(1.0f), transform->m_position);
			transformMatrix = glm::rotate(transformMatrix, transform->m_orientation.GetAngle(), transform->m_orientation.GetAxis());
			transformMatrix = glm::scale(transformMatrix, transform->m_scale);

			// Parse vertex data.
			glBindBuffer(GL_ARRAY_BUFFER, mesh->GetVertexBuffer()->GetBufferId());
			unsigned char* data = (unsigned char*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);

			int offset = 0;

			for(unsigned i = 0; i < mesh->GetVertexBuffer()->GetBufferSize(); i += mesh->GetVertexBuffer()->GetElementSize())
			{
				Render::Vertex1P1N1UV v;
				memcpy(&v, &data[i], sizeof(Render::Vertex1P1N1UV));

				glm::vec4 tf = transformMatrix * glm::vec4(v.m_pos, 1.0f);
				v.m_pos = glm::vec3(tf.x, tf.y, tf.z);

				m_vertices.push_back(std::move(v));

				offset ++;
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

				p.m_indices.push_back(i0 + indexOffset);
				p.m_indices.push_back(i1 + indexOffset);
				p.m_indices.push_back(i2 + indexOffset);

				p.m_materialIndex = materialIndex;

				m_globalPolygonList.push_back(std::move(p));
			}

			glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

			indexOffset += offset;
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
		quadTreeBounds.m_minX = -RoundToPow2(abs(minX));

		quadTreeBounds.m_maxY = maxY;
		quadTreeBounds.m_minY = minY;

		quadTreeBounds.m_maxZ = RoundToPow2(maxZ);
		quadTreeBounds.m_minZ = -RoundToPow2(abs(minZ));

		m_root = new QuadNode(quadTreeBounds);

		m_context->m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "Begin subdividing %d", numTriangles);

		Subdivide(m_root, m_globalPolygonList);

		auto a = m_world->GetGroupManager()->GetEntitiesInGroup("Static");
		for(auto b = a.first; b != a.second; ++b)
		{
			m_world->GetEntityManager()->RemoveAllComponents(b->second);
			m_world->GetEntityManager()->RemoveEntity(b->second);
		}
	}

	int QuadTree::RoundToPow2(int p_value)
	{
		return (int)pow(2, ceil(log(p_value)/log(2)));
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

			std::vector<Polygon> tr;
			std::vector<Polygon> tl;
			std::vector<Polygon> br;
			std::vector<Polygon> bl;

			for(int i = 0; i < polygonsAfterXSplit.size(); ++i)
			{
				glm::vec3 polygonCenter = CalcCenterEx(polygonsAfterXSplit[i]);

				if(polygonCenter.x >= center.x && polygonCenter.z >= center.z)
				{
					tr.push_back(polygonsAfterXSplit[i]);
				}

				else if(polygonCenter.x >= center.x && polygonCenter.z <= center.z)
				{
					br.push_back(polygonsAfterXSplit[i]);
				}

				else if(polygonCenter.x <= center.x && polygonCenter.z <= center.z)
				{
					bl.push_back(polygonsAfterXSplit[i]);
				}
				else if(polygonCenter.x <= center.x && polygonCenter.z >= center.z)
				{
					tl.push_back(polygonsAfterXSplit[i]);
				}
			}

			int size = bl.size() + br.size() + tl.size() + tr.size();

			int diff = polygonsAfterXSplit.size() - size;

			m_context->m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "Polygon Diff: %d", diff);

			QuadNode* bottomLeftChild = new QuadNode(bottomLeftAABB);
			QuadNode* bottomRightChild = new QuadNode(bottomRightAABB);
			QuadNode* topLeftChild = new QuadNode(topLeftAABB);
			QuadNode* topRightChild = new QuadNode(topRightAABB);

			bottomLeftChild->m_key = p_node->m_key + "BL";
			bottomRightChild->m_key = p_node->m_key + "BR";
			topLeftChild->m_key = p_node->m_key + "TL";
			topRightChild->m_key = p_node->m_key + "TR";

			p_node->AddChild(bottomLeftChild);
			p_node->AddChild(bottomRightChild);
			p_node->AddChild(topLeftChild);
			p_node->AddChild(topRightChild);

			Subdivide( topLeftChild, tl );
			Subdivide( topRightChild, tr );
			Subdivide( bottomLeftChild, bl );
			Subdivide( bottomRightChild, br );

		}
		else
		{
			m_context->m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "Adding data node. - Polygons: %d", p_polygons.size());

			auto a = Trianglulate(p_polygons);

			CreateEntities(a, p_node->m_key);
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

				if((p.x+bias >= p_rect.m_x && p.x <= p_rect.m_x + p_rect.m_width-bias) &&
					( p.z+bias >= p_rect.m_y && p.z <= p_rect.m_y + p_rect.m_height-bias ))
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

	glm::vec3 QuadTree::CalcCenterEx(Polygon& p_polygon)
	{
		float sumX = 0;
		float sumZ = 0;

		for(int i = 0; i < p_polygon.m_indices.size(); ++i)
		{
			sumX += m_vertices[p_polygon.m_indices[i]].m_pos.x;
			sumZ += m_vertices[p_polygon.m_indices[i]].m_pos.z;
		}

		sumX /= p_polygon.m_indices.size();
		sumZ /= p_polygon.m_indices.size();

		return glm::vec3(sumX, 0, sumZ);
	}

	glm::vec3 QuadTree::CalcCenter(Polygon& p_polygon)
	{
		if(p_polygon.m_indices.size() == 3)
		{
			float centerX = (m_vertices[p_polygon.m_indices[0]].m_pos.x + m_vertices[p_polygon.m_indices[1]].m_pos.x + m_vertices[p_polygon.m_indices[2]].m_pos.x) / 3;
			float centerz = (m_vertices[p_polygon.m_indices[0]].m_pos.z + m_vertices[p_polygon.m_indices[1]].m_pos.z + m_vertices[p_polygon.m_indices[2]].m_pos.z) / 3;

			return glm::vec3(centerX, 0, centerz);
		}
		else if(p_polygon.m_indices.size() == 4)
		{
			float centerX = (m_vertices[p_polygon.m_indices[0]].m_pos.x + m_vertices[p_polygon.m_indices[1]].m_pos.x + m_vertices[p_polygon.m_indices[2]].m_pos.x + m_vertices[p_polygon.m_indices[3]].m_pos.x) / 4;
			float centerz = (m_vertices[p_polygon.m_indices[0]].m_pos.z + m_vertices[p_polygon.m_indices[1]].m_pos.z + m_vertices[p_polygon.m_indices[2]].m_pos.z + m_vertices[p_polygon.m_indices[3]].m_pos.x) / 4;

			return glm::vec3(centerX, 0, centerz);
		}

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
		cx *= 1.0f / (6.0f * area);
		cy *= 1.0f / (6.0f * area);
		
		return glm::vec3(cx, 0, cy);
	}

	PolygonSplit QuadTree::SplitPolygon(PlaneEx& p_divider, Polygon& p_polygon)
	{
		PolygonSplit split;

		split.m_back.m_materialIndex = p_polygon.m_materialIndex;
		split.m_front.m_materialIndex = p_polygon.m_materialIndex;

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

		Line l;
		l.m_origin = p_p0.m_pos;
		l.m_direction = p_p1.m_pos - p_p0.m_pos;

		float t = PlaneIntersectLine(p_divider, l);

		Render::Vertex1P1N1UV newVertex;
		newVertex.m_pos = l.m_origin + l.m_direction * t;
		newVertex.m_normal = p_p0.m_normal + (p_p1.m_normal - p_p0.m_normal) * t;
		newVertex.m_UV = p_p0.m_UV + (p_p1.m_UV - p_p0.m_UV) * t;

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
			for(int j = 2; j < p_polygons[i].m_indices.size(); j++)
			{
				Triangle t;
				t.m_indices[0] = p_polygons[i].m_indices[0];
				t.m_indices[1] = p_polygons[i].m_indices[j-1];
				t.m_indices[2] = p_polygons[i].m_indices[j];
				t.m_materialIndex = p_polygons[i].m_materialIndex;

				triangles.push_back(t);
			}
		}

		return triangles;
	}

	void QuadTree::CreateEntities(std::vector<Triangle> p_triangles, std::string p_key)
	{
		std::sort(p_triangles.begin(), p_triangles.end(), [](Triangle& a, Triangle& b)->bool{ return a.m_materialIndex < b.m_materialIndex; });

		std::vector<unsigned int> indices;
		std::vector<Render::Vertex1P1N1UV> vertices;

		unsigned int currentMaterialIndex = p_triangles[0].m_materialIndex;

		int entityId = 0;

		for(int i = 0; i < p_triangles.size(); i++)
		{
			if(p_triangles[i].m_materialIndex != currentMaterialIndex)
			{
				// Create entity.
				ECS::Entity* e1 = m_world->GetEntityManager()->CreateEntity();
				RootForce::Renderable* r = m_world->GetEntityManager()->CreateComponent<RootForce::Renderable>(e1);

				std::stringstream ss;
				ss << p_key << entityId;

				r->m_model = m_context->m_resourceManager->CreateModel(ss.str());
				r->m_model->m_meshes[0]->SetVertexBuffer(m_context->m_renderer->CreateBuffer());
				r->m_model->m_meshes[0]->SetElementBuffer(m_context->m_renderer->CreateBuffer());
				r->m_model->m_meshes[0]->SetVertexAttribute(m_context->m_renderer->CreateVertexAttributes());
				r->m_model->m_meshes[0]->CreateVertexBuffer1P1N1UV(&vertices[0], vertices.size());
				r->m_model->m_meshes[0]->CreateIndexBuffer(&indices[0], indices.size());
				
				r->m_material = m_materials[currentMaterialIndex];

				// TODO: Remove this for static meshes.
				RootForce::Transform* t = m_world->GetEntityManager()->CreateComponent<RootForce::Transform>(e1);
				t->m_position = glm::vec3(0.0f, 0.0f, 0.0f);
				t->m_scale = glm::vec3(1.0f, 1.0f, 1.0f);

				currentMaterialIndex = p_triangles[i].m_materialIndex;
				entityId++;

				indices.clear();
				vertices.clear();
			}

			Render::Vertex1P1N1UV p0 = m_vertices[p_triangles[i].m_indices[0]];
			
			int index = -1;
			for(auto itr = vertices.begin(); itr != vertices.end(); ++itr)
			{
				if((*itr).m_pos == p0.m_pos && (*itr).m_UV == p0.m_UV && (*itr).m_normal == p0.m_normal)
				{
					index = itr - vertices.begin();
				}
			}
			if(index == -1)
			{
				vertices.push_back(p0);
				indices.push_back(vertices.size()-1);
			}
			else
			{
				indices.push_back(index);

			}

			Render::Vertex1P1N1UV p1 = m_vertices[p_triangles[i].m_indices[1]];

			index = -1;
			for(auto itr = vertices.begin(); itr != vertices.end(); ++itr)
			{
				if((*itr).m_pos == p1.m_pos && (*itr).m_UV == p1.m_UV && (*itr).m_normal == p1.m_normal)
				{
					index = itr - vertices.begin();
				}
			}
			if(index == -1)
			{
				vertices.push_back(p1);
				indices.push_back(vertices.size()-1);
			}
			else
			{
				indices.push_back(index);

			}


			Render::Vertex1P1N1UV p2 = m_vertices[p_triangles[i].m_indices[2]];

			index = -1;
			for(auto itr = vertices.begin(); itr != vertices.end(); ++itr)
			{
				if((*itr).m_pos == p2.m_pos && (*itr).m_UV == p2.m_UV && (*itr).m_normal == p2.m_normal)
				{
					index = itr - vertices.begin();
				}
			}
			if(index == -1)
			{
				vertices.push_back(p2);
				indices.push_back(vertices.size()-1);
			}
			else
			{
				indices.push_back(index);

			}


			// Push triangle face.
			/*indices.push_back(p_triangles[i].m_indices[0]);
			indices.push_back(p_triangles[i].m_indices[1]);
			indices.push_back(p_triangles[i].m_indices[2]);*/
		}

		ECS::Entity* e2 = m_world->GetEntityManager()->CreateEntity();
		RootForce::Renderable* r = m_world->GetEntityManager()->CreateComponent<RootForce::Renderable>(e2);

		std::stringstream ss;
		ss << p_key << entityId;

		r->m_model = m_context->m_resourceManager->CreateModel(ss.str());
		r->m_model->m_meshes[0]->SetVertexBuffer(m_context->m_renderer->CreateBuffer());
		r->m_model->m_meshes[0]->SetElementBuffer(m_context->m_renderer->CreateBuffer());
		r->m_model->m_meshes[0]->SetVertexAttribute(m_context->m_renderer->CreateVertexAttributes());
		r->m_model->m_meshes[0]->CreateVertexBuffer1P1N1UV(&vertices[0], vertices.size());
		r->m_model->m_meshes[0]->CreateIndexBuffer(&indices[0], indices.size());
		r->m_material = m_materials[currentMaterialIndex];
		
		// TODO: Remove this for static meshes.
		RootForce::Transform* t = m_world->GetEntityManager()->CreateComponent<RootForce::Transform>(e2);
		t->m_position = glm::vec3(0.0f, 0.0f, 0.0f);
		t->m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
	}

	void QuadTree::CreateEntitiesEx(std::vector<Triangle> p_triangles, std::string p_key)
	{
		std::sort(p_triangles.begin(), p_triangles.end(), [](Triangle& a, Triangle& b)->bool{ return a.m_materialIndex < b.m_materialIndex; });

		std::vector<unsigned int> indices;

		unsigned int currentMaterialIndex = p_triangles[0].m_materialIndex;

		int entityId = 0;

		for(int i = 0; i < p_triangles.size(); i++)
		{
			if(p_triangles[i].m_materialIndex != currentMaterialIndex)
			{
				// Create entity.
				ECS::Entity* e1 = m_world->GetEntityManager()->CreateEntity();
				RootForce::Renderable* r = m_world->GetEntityManager()->CreateComponent<RootForce::Renderable>(e1);

				std::stringstream ss;
				ss << p_key << entityId;

				r->m_model = m_context->m_resourceManager->CreateModel(ss.str());
				r->m_model->m_meshes[0]->SetVertexBuffer(m_context->m_renderer->CreateBuffer());
				r->m_model->m_meshes[0]->SetElementBuffer(m_context->m_renderer->CreateBuffer());
				r->m_model->m_meshes[0]->SetVertexAttribute(m_context->m_renderer->CreateVertexAttributes());
				r->m_model->m_meshes[0]->CreateVertexBuffer1P1N1UV(&m_vertices[0], m_vertices.size());
				r->m_model->m_meshes[0]->CreateIndexBuffer(&indices[0], indices.size());
				
				r->m_material = m_materials[currentMaterialIndex];

				// TODO: Remove this for static meshes.
				RootForce::Transform* t = m_world->GetEntityManager()->CreateComponent<RootForce::Transform>(e1);
				t->m_position = glm::vec3(0.0f, 0.0f, 0.0f);
				t->m_scale = glm::vec3(1.0f, 1.0f, 1.0f);

				currentMaterialIndex = p_triangles[i].m_materialIndex;
				entityId++;

				indices.clear();
			}

			// Push triangle face.
			indices.push_back(p_triangles[i].m_indices[0]);
			indices.push_back(p_triangles[i].m_indices[1]);
			indices.push_back(p_triangles[i].m_indices[2]);
		}

		ECS::Entity* e2 = m_world->GetEntityManager()->CreateEntity();
		RootForce::Renderable* r = m_world->GetEntityManager()->CreateComponent<RootForce::Renderable>(e2);

		std::stringstream ss;
		ss << p_key << entityId;

		r->m_model = m_context->m_resourceManager->CreateModel(ss.str());
		r->m_model->m_meshes[0]->SetVertexBuffer(m_context->m_renderer->CreateBuffer());
		r->m_model->m_meshes[0]->SetElementBuffer(m_context->m_renderer->CreateBuffer());
		r->m_model->m_meshes[0]->SetVertexAttribute(m_context->m_renderer->CreateVertexAttributes());
		r->m_model->m_meshes[0]->CreateVertexBuffer1P1N1UV(&m_vertices[0], m_vertices.size());
		r->m_model->m_meshes[0]->CreateIndexBuffer(&indices[0], indices.size());
		r->m_material = m_materials[currentMaterialIndex];
		
		// TODO: Remove this for static meshes.
		RootForce::Transform* t = m_world->GetEntityManager()->CreateComponent<RootForce::Transform>(e2);
		t->m_position = glm::vec3(0.0f, 0.0f, 0.0f);
		t->m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
	}
}