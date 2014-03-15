#include <RootSystems/Include/DataStructures/Quad.h>
#include <RootSystems/Include/RenderingSystem.h>
#include <RootSystems/Include/Shapes/Plane.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>
#include <glm/gtx/transform.hpp>
#include <RootSystems/Include/Shapes/OBB.h>

#include <RootEngine/Include/GameSharedContext.h>
extern RootEngine::GameSharedContext g_engineContext;

namespace RootForce
{
	void QuadTree::Initialize(RootEngine::GameSharedContext* p_context, ECS::World* p_world, const std::string& p_groupName, const std::string& p_newName, bool p_clear)
	{
		if(m_root != nullptr)
			delete m_root;

		m_idCounter = 0;
		m_groupName = p_groupName;
		m_vertices.clear();
		m_materials.clear();
		m_boundsPolygons.clear();
		m_sizes.clear();
		m_context = p_context;
		m_world = p_world;
		m_newGroupName = p_newName;

		// Get working set.
		ECS::GroupManager::GroupRange range = p_world->GetGroupManager()->GetEntitiesInGroup(p_groupName);

		unsigned indexOffset = 0;
		unsigned matPtr = 0;
		for(auto itr = range.first; itr != range.second; ++itr)
		{
			ECS::Entity* entity = (*itr).second;

			RootForce::Renderable* renderable = p_world->GetEntityManager()->GetComponent<RootForce::Renderable>(entity);
			auto material = renderable->m_material;
			auto mesh = renderable->m_model->m_meshes[0];

			unsigned materialIndex = 0;

			auto materialItr = std::find(m_materials.begin(), m_materials.end(), material);

			if(materialItr == m_materials.end())
			{
				materialIndex = m_materials.size();
				m_materials.push_back(material);
			}
			else
			{
				materialIndex = materialItr - m_materials.begin();
			}
			
			m_sizes.push_back(mesh->GetVertexBuffer()->GetElementSize());

			RootForce::Transform* transform = p_world->GetEntityManager()->GetComponent<RootForce::Transform>(entity);

			glm::mat4x4 transformMatrix;
			transformMatrix = glm::translate(glm::mat4(1.0f), transform->m_position);
			transformMatrix = glm::rotate(transformMatrix, transform->m_orientation.GetAngle(), transform->m_orientation.GetAxis());
			transformMatrix = glm::scale(transformMatrix, transform->m_scale);

			glm::mat4x4 normalMatrix;
			normalMatrix = glm::mat4(glm::transpose(glm::inverse(glm::mat3(transformMatrix))));

			// Parse vertex data.
			glBindBuffer(GL_ARRAY_BUFFER, mesh->GetVertexBuffer()->GetBufferId());
			unsigned char* data = (unsigned char*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);

			unsigned offset = m_vertices.size();
			for(unsigned i = 0; i < mesh->GetVertexBuffer()->GetBufferSize(); i += mesh->GetVertexBuffer()->GetElementSize())
			{
				Render::Vertex1P1N1UV1T1BT v;
				v.m_tangent = glm::vec3(0);
				v.m_bitangent = glm::vec3(0);

				memcpy(&v, &data[i], mesh->GetVertexBuffer()->GetElementSize());

				// Transform vertex to world space.
				glm::vec4 tf = transformMatrix * glm::vec4(v.m_pos, 1.0f);
				v.m_pos = glm::vec3(tf.x, tf.y, tf.z);

				glm::vec4 tfn = normalMatrix * glm::vec4(v.m_normal, 1.0f);
				v.m_normal = glm::vec3(tfn.x, tfn.y, tfn.z);

				glm::vec4 tfb = normalMatrix * glm::vec4(v.m_bitangent, 1.0f);
				v.m_bitangent = glm::vec3(tfb.x, tfb.y, tfb.z);

				glm::vec4 tbt = normalMatrix * glm::vec4(v.m_tangent, 1.0f);
				v.m_tangent = glm::vec3(tbt.x, tbt.y, tbt.z);

				m_vertices.push_back(std::move(v));
			}

			if(mesh->GetElementBuffer() == nullptr)
			{
				for(unsigned i = 0; i < mesh->GetVertexBuffer()->GetNumElements(); i += 3)
				{		
					Polygon p;

					p.m_indices.push_back(i + offset);
					p.m_indices.push_back(i + 1 + offset);
					p.m_indices.push_back(i + 2 + offset);

					/*std::cout << "P0: " << i + indexOffset << std::endl;
					std::cout << "P1: " << i + 1 + indexOffset << std::endl;
					std::cout << "P2: " << i + 2 + indexOffset << std::endl;
					std::cout << "----" << std::endl;*/

					p.m_materialIndex = materialIndex;

					m_boundsPolygons.push_back(std::move(p));
				}
			}

			glUnmapBuffer(GL_ARRAY_BUFFER);

			if(mesh->GetElementBuffer() != nullptr)
			{
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

					p.m_indices.push_back(i0 + offset);
					p.m_indices.push_back(i1 + offset);
					p.m_indices.push_back(i2 + offset);

					/*std::cout << "P0: " << i0 + indexOffset << std::endl;
					std::cout << "P1: " << i1 + indexOffset << std::endl;
					std::cout << "P2: " << i2 + indexOffset << std::endl;
					std::cout << "----" << std::endl;*/

					p.m_materialIndex = materialIndex;

					m_boundsPolygons.push_back(std::move(p));
				}

				glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
			}
		}
		
		// Create an AABB for the quad tree structure.
		AABB quadTreeBounds;
		int maxX, maxY, maxZ, minX, minY, minZ;
		maxX = maxY = maxZ = -99999;
		minX = minY = minZ = 99999;

		for(auto itr = m_boundsPolygons.begin(); itr != m_boundsPolygons.end(); ++itr)
		{
			Render::Vertex1P1N1UV1T1BT ps[3];

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

		quadTreeBounds.m_maxX = (float)RoundToPow2(maxX);
		quadTreeBounds.m_minX = (float)-RoundToPow2(abs(minX));

		quadTreeBounds.m_maxY = (float)maxY;
		quadTreeBounds.m_minY = (float)minY;

		quadTreeBounds.m_maxZ = (float)RoundToPow2(maxZ);
		quadTreeBounds.m_minZ = (float)-RoundToPow2(abs(minZ));

		m_root = new QuadNode(quadTreeBounds);

		if(p_clear)
		{
			m_vertices.clear();
			m_boundsPolygons.clear();
			m_sizes.clear();
		}

	}

	void QuadTree::BeginDivide(unsigned int p_polygonsPerNode, bool p_splitPolygons, bool p_removeOrigionalEntities)
	{		
		for(auto itr = m_entities.begin(); itr != m_entities.end(); ++itr)
		{
			m_world->GetEntityManager()->RemoveEntity((*itr));
		}

		m_polygonsPerNode = p_polygonsPerNode;
		m_splitPolygons = p_splitPolygons;


		if(p_removeOrigionalEntities)
		{
			auto a = m_world->GetGroupManager()->GetEntitiesInGroup(m_groupName);
			for(auto b = a.first; b != a.second; ++b)
			{
				RootForce::Renderable* renderable = m_world->GetEntityManager()->GetComponent<RootForce::Renderable>((*b).second);

				g_engineContext.m_resourceManager->RemoveRenderingMeshesFromModel(renderable->m_model);

				m_world->GetEntityManager()->RemoveComponent<RootForce::Renderable>((*b).second);
			}
		}

		m_context->m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "Begin subdividing %d", m_boundsPolygons.size());
		Subdivide(m_root, m_boundsPolygons);

		// ???
		m_culledEntities.reserve(m_entities.size() * 4);
	}


	void QuadTree::RenderDebug(const glm::vec3& p_color)
	{
		RenderNodeDebug(m_root, p_color);
	}

	void QuadTree::RenderNodeDebug(QuadNode* p_node, const glm::vec3& p_color)
	{
		p_node->m_bounds.DebugDraw(m_context->m_renderer, p_color, glm::mat4(1.0f));

		for(unsigned i = 0; i < p_node->m_childs.size(); ++i)
		{
			RenderNodeDebug(p_node->m_childs[i], p_color);
		}
	}

	QuadNode* QuadTree::GetRoot() const
	{
		return m_root;
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

	void QuadTree::Cull(RootForce::Frustum* p_frustrum, QuadNode* p_node)
	{
		if(p_frustrum->CheckBoxEx(p_node->GetBounds()))
		{
			if(p_node->GetChilds().size() == 0)
			{
				// Insert range.
				m_culledNodes.push_back(p_node);
				m_culledEntities.insert(m_culledEntities.end(), p_node->m_indices.begin(), p_node->m_indices.end());
#ifdef QUADTREE_DRAWLINES
				p_node->GetBounds().DebugDraw(m_context->m_renderer, glm::vec3(0,1,1), glm::mat4(1.0f));
#endif
			}
			else
			{
				for(unsigned i = 0; i < p_node->GetChilds().size(); ++i)
				{
					Cull( p_frustrum, p_node->GetChilds().at(i)); 
				}
			}
		}
	}

	void QuadTree::Cull(std::vector<glm::vec4>& p_points, QuadNode* p_node)
	{
		if(Intersect(p_points, p_node->GetBounds()))
		{
			if(p_node->GetChilds().size() == 0)
			{
				// Insert range.
				m_culledNodes.push_back(p_node);
				m_culledEntities.insert(m_culledEntities.end(), p_node->m_indices.begin(), p_node->m_indices.end());
			}
			else
			{
				for(unsigned i = 0; i < p_node->GetChilds().size(); ++i)
				{
					Cull( p_points, p_node->GetChilds().at(i)); 
				}
			}
		}

	}

	int QuadTree::RoundToPow2(int p_value) const
	{
		return (int)pow(2, ceil(log(p_value)/log(2)));
	}

	void QuadTree::Subdivide( QuadNode* p_node,  std::vector<Polygon> p_polygons )
	{
		if(p_polygons.size() == 0)
		{
			return;
		}
		else if(p_polygons.size() > m_polygonsPerNode)
		{
			const AABB* aabb = &p_node->m_bounds;

			// Calculate center of the aabb.
			glm::vec3 center = aabb->GetCenter();

			#ifdef QUADTREE_VERBOSE
				m_context->m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "Generating 4 children. - Polygons: %d", p_polygons.size());
			#endif
			
			if(m_splitPolygons)
			{
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
				for(unsigned i = 0; i < p_polygons.size(); ++i)
				{
					PolygonSplit result = SplitPolygon(zp, p_polygons[i]);

					if(result.m_back.m_indices.size() > 0)
						polygonsAfterZSplit.push_back(result.m_back);

					if(result.m_front.m_indices.size() > 0)
						polygonsAfterZSplit.push_back(result.m_front);
				}
#ifdef QUADTREE_VERBOSE
				m_context->m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "Polygons after ZSplit: %d", polygonsAfterZSplit.size());
#endif
				std::vector<Polygon> polygonsAfterXSplit;

				// Split all polygons with the z-plane.
				for(unsigned i = 0; i < polygonsAfterZSplit.size(); ++i)
				{
					PolygonSplit result = SplitPolygon(xp, polygonsAfterZSplit[i]);

					if(result.m_back.m_indices.size() > 0)
						polygonsAfterXSplit.push_back(result.m_back);

					if(result.m_front.m_indices.size() > 0)
						polygonsAfterXSplit.push_back(result.m_front);
				}

#ifdef QUADTREE_VERBOSE
			m_context->m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "Polygons after XSplit: %d", polygonsAfterXSplit.size());
#endif
				// Set the splitted polygon list.
				p_polygons = polygonsAfterXSplit;
			}

			float halfwidth = aabb->GetLengthX() / 2;
			float halfheight = aabb->GetLengthZ() / 2;

			Rectangle bottomLeft = Rectangle(aabb->m_minX, aabb->m_minZ, halfwidth, halfheight);
			Rectangle bottomRight = Rectangle(aabb->m_minX + halfwidth, aabb->m_minZ, halfwidth, halfheight);
			Rectangle topLeft = Rectangle(aabb->m_minX, aabb->m_minZ + halfheight, halfwidth, halfheight);
			Rectangle topRight = Rectangle(aabb->m_minX + halfwidth, aabb->m_minZ + halfheight, halfwidth, halfheight);

			AABB bottomLeftAABB = AABB(bottomLeft.m_x, bottomLeft.m_x + halfwidth, (float)m_minY, (float)m_maxY, bottomLeft.m_y, bottomLeft.m_y + halfheight);
			AABB bottomRightAABB = AABB(bottomRight.m_x, bottomRight.m_x + halfwidth, (float)m_minY, (float)m_maxY, bottomRight.m_y, bottomRight.m_y + halfheight);
			AABB topLeftAABB = AABB(topLeft.m_x, topLeft.m_x + halfwidth, (float)m_minY, (float)m_maxY, topLeft.m_y, topLeft.m_y + halfheight);
			AABB topRightAABB = AABB(topRight.m_x, topRight.m_x + halfwidth, (float)m_minY, (float)m_maxY, topRight.m_y, topRight.m_y + halfheight);

			std::vector<Polygon> tr;
			std::vector<Polygon> tl;
			std::vector<Polygon> br;
			std::vector<Polygon> bl;

			for(unsigned i = 0; i < p_polygons.size(); ++i)
			{
				glm::vec2 polygonCenter = CalcXZCenter(p_polygons[i]);

				if(polygonCenter.x >= center.x && polygonCenter.y >= center.z)
				{
					tr.push_back(p_polygons[i]);
				}
				else if(polygonCenter.x >= center.x && polygonCenter.y <= center.z)
				{
					br.push_back(p_polygons[i]);
				}
				else if(polygonCenter.x <= center.x && polygonCenter.y <= center.z)
				{
					bl.push_back(p_polygons[i]);
				}
				else if(polygonCenter.x <= center.x && polygonCenter.y >= center.z)
				{
					tl.push_back(p_polygons[i]);
				}
			}

			QuadNode* bottomLeftChild = new QuadNode(bottomLeftAABB);
			QuadNode* bottomRightChild = new QuadNode(bottomRightAABB);
			QuadNode* topLeftChild = new QuadNode(topLeftAABB);
			QuadNode* topRightChild = new QuadNode(topRightAABB);

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
			p_node->m_id = m_idCounter++;

			std::vector<Triangle> triangles = Trianglulate(p_polygons);
			p_node->m_indices = CreateEntities(triangles);

#ifdef QUADTREE_VERBOSE
			m_context->m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "Adding data node. - Polygons: %d", p_polygons.size());
#endif
		}
	}

	PolygonSplit QuadTree::SplitPolygon(PlaneEx& p_divider, Polygon& p_polygon)
	{
		PolygonSplit split;

		split.m_back.m_materialIndex = p_polygon.m_materialIndex;
		split.m_front.m_materialIndex = p_polygon.m_materialIndex;

		Render::Vertex1P1N1UV1T1BT vertexA = m_vertices[p_polygon.m_indices[p_polygon.m_indices.size()-1]];

		Side::Side sideA = ClassifyPoint(p_divider, vertexA.m_pos);

		for(unsigned int i = 0; i < p_polygon.m_indices.size(); ++i)
		{
			Render::Vertex1P1N1UV1T1BT vertexB = m_vertices[p_polygon.m_indices[i]];

			Side::Side sideB = ClassifyPoint(p_divider, vertexB.m_pos);

			if(sideB == Side::BACK) // Behind
			{
				if(sideA == Side::FRONT) // Infront
				{
					int index = SplitVertex(p_divider, vertexA, vertexB);

					split.m_front.m_indices.push_back(index);
					split.m_back.m_indices.push_back(index);			
				}

				split.m_back.m_indices.push_back(p_polygon.m_indices[i]);

			}
			else if(sideB == Side::FRONT)// Infront
			{
				if(sideA == Side::BACK) // Behind
				{
					int index = SplitVertex(p_divider, vertexA, vertexB);

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

			vertexA = vertexB;
			sideA = sideB;
		}

		return split;
	}

	Side::Side QuadTree::ClassifyPoint(PlaneEx& p_divider, glm::vec3 p_position) const
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

	unsigned QuadTree::SplitVertex(PlaneEx& p_divider, Render::Vertex1P1N1UV1T1BT& p_p0, Render::Vertex1P1N1UV1T1BT& p_p1)
	{
		Line line;
		line.m_origin = p_p0.m_pos;
		line.m_direction = p_p1.m_pos - p_p0.m_pos;

		float t = PlaneIntersectLine(p_divider, line);

		// Create new vertex.
		Render::Vertex1P1N1UV1T1BT newVertex;
		newVertex.m_pos = line.m_origin + line.m_direction * t;
		newVertex.m_normal = p_p0.m_normal + (p_p1.m_normal - p_p0.m_normal) * t;
		newVertex.m_UV = p_p0.m_UV + (p_p1.m_UV - p_p0.m_UV) * t;
		newVertex.m_bitangent = p_p0.m_bitangent + (p_p1.m_bitangent - p_p0.m_bitangent) * t;
		newVertex.m_tangent = p_p0.m_tangent + (p_p1.m_tangent - p_p0.m_tangent) * t;

		unsigned index = m_vertices.size();
		m_vertices.push_back(newVertex);

		return index;
	}

	glm::vec2 QuadTree::CalcXZCenter(const Polygon& p_polygon) const
	{
		float sumX = 0;
		float sumZ = 0;

		float invPolygonSize = 1.0f / p_polygon.m_indices.size();

		for(unsigned i = 0; i < p_polygon.m_indices.size(); ++i)
		{
			sumX += m_vertices[p_polygon.m_indices[i]].m_pos.x;
			sumZ += m_vertices[p_polygon.m_indices[i]].m_pos.z;
		}

		sumX *= invPolygonSize;
		sumZ *= invPolygonSize;

		return glm::vec2(sumX, sumZ);
	}

	std::vector<Triangle> QuadTree::Trianglulate(const std::vector<Polygon>& p_polygons) const
	{
		std::vector<Triangle> triangles;
		for(unsigned i = 0; i < p_polygons.size(); ++i)
		{
			for(unsigned j = 2; j < p_polygons[i].m_indices.size(); j++)
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

	void QuadTree::CreatePNUVEntity(unsigned int p_entityId, unsigned int p_materialIndex, std::vector<Render::Vertex1P1N1UV>& p_vertices, std::vector<unsigned>& p_indices)
	{
		
#ifdef QUADTREE_VERBOSE
		m_context->m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "Creating PNUV entity %d triangles", p_indices.size() / 3);
#endif
		// Create entity.
		ECS::Entity* entity = m_world->GetEntityManager()->CreateEntity();

		m_entities.push_back(entity);

		std::stringstream modelNameStringStream;
		modelNameStringStream << m_newGroupName << p_entityId;

		// Create renderable.
		RootForce::Renderable* renderable = m_world->GetEntityManager()->CreateComponent<RootForce::Renderable>(entity);
		renderable->m_model = m_context->m_resourceManager->CreateModel(modelNameStringStream.str());
		renderable->m_model->m_meshes[0]->SetVertexBuffer(m_context->m_renderer->CreateBuffer(GL_ARRAY_BUFFER));
		renderable->m_model->m_meshes[0]->SetElementBuffer(m_context->m_renderer->CreateBuffer(GL_ELEMENT_ARRAY_BUFFER));
		renderable->m_model->m_meshes[0]->SetVertexAttribute(m_context->m_renderer->CreateVertexAttributes());	
		renderable->m_model->m_meshes[0]->CreateVertexBuffer1P1N1UV(&p_vertices[0], p_vertices.size());

		renderable->m_material = m_materials[p_materialIndex];

		// Add tile factor for blended meshes.
		if(renderable->m_material->m_tileFactor != 0)
		{
			renderable->m_material->m_effect = m_context->m_resourceManager->LoadEffect("Mesh_Static_Blend");
			renderable->m_params[Render::Semantic::SIZEMIN] = &renderable->m_material->m_tileFactor;
		}
		else
		{
			renderable->m_material->m_effect = m_context->m_resourceManager->LoadEffect("Mesh_Static");
		}
	
		m_world->GetGroupManager()->RegisterEntity(m_newGroupName, entity);

		renderable->m_model->m_meshes[0]->CreateIndexBuffer(&p_indices[0], p_indices.size());		
	}

	void QuadTree::CreatePNUVTBTEntity(unsigned int p_entityId, unsigned int p_materialIndex, std::vector<Render::Vertex1P1N1UV1T1BT>& p_vertices, std::vector<unsigned>& p_indices)
	{
		#ifdef QUADTREE_VERBOSE
		m_context->m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "Creating PNUVTBT entity %d triangles id: %d", p_indices.size() / 3, p_entityId);
#endif
		// Create entity.
		ECS::Entity* entity = m_world->GetEntityManager()->CreateEntity();

		m_entities.push_back(entity);

		// Model name.
		std::stringstream modelNameStringStream;
		modelNameStringStream << m_newGroupName << p_entityId;

		// Create renderable.
		RootForce::Renderable* renderable = m_world->GetEntityManager()->CreateComponent<RootForce::Renderable>(entity);

		renderable->m_model = m_context->m_resourceManager->CreateModel(modelNameStringStream.str());
		renderable->m_model->m_meshes[0]->SetVertexBuffer(m_context->m_renderer->CreateBuffer(GL_ARRAY_BUFFER));
		renderable->m_model->m_meshes[0]->SetElementBuffer(m_context->m_renderer->CreateBuffer(GL_ELEMENT_ARRAY_BUFFER));
		renderable->m_model->m_meshes[0]->SetVertexAttribute(m_context->m_renderer->CreateVertexAttributes());	
		renderable->m_model->m_meshes[0]->CreateVertexBuffer1P1N1UV1T1BT(&p_vertices[0], p_vertices.size());

		std::stringstream materialStringStream;
		materialStringStream << m_newGroupName << p_materialIndex;
		
		renderable->m_material = g_engineContext.m_renderer->CreateMaterial(materialStringStream.str());
		renderable->m_material->m_textures = m_materials[p_materialIndex]->m_textures;
		renderable->m_material->m_tileFactor = m_materials[p_materialIndex]->m_tileFactor;
		renderable->m_material->m_flipped = m_materials[p_materialIndex]->m_flipped;

		// Add tile factor for blended meshes.
		if(renderable->m_material->m_tileFactor != 0)
		{
			renderable->m_material->m_effect = m_context->m_resourceManager->LoadEffect("Mesh_Static_Blend");
			renderable->m_params[Render::Semantic::SIZEMIN] = &renderable->m_material->m_tileFactor;
		}
		else
		{
			renderable->m_material->m_effect = m_context->m_resourceManager->LoadEffect("Mesh_Static_NormalMap");
		}
	
		m_world->GetGroupManager()->RegisterEntity(m_newGroupName, entity);

		renderable->m_model->m_meshes[0]->CreateIndexBuffer(&p_indices[0], p_indices.size());		
	}

	std::vector<unsigned> QuadTree::CreateEntities(std::vector<Triangle>& p_triangles)
	{
		// Sort by material.
		std::sort(p_triangles.begin(), p_triangles.end(), [](Triangle& a, Triangle& b)->bool{ return a.m_materialIndex < b.m_materialIndex; });

		std::vector<unsigned> indices;
		std::vector<unsigned> entitiesIds;

		std::vector<Render::Vertex1P1N1UV> verticesPNUV;
		std::vector<Render::Vertex1P1N1UV1T1BT> verticesPNUVTBT;
		
		unsigned currentMaterialIndex = p_triangles[0].m_materialIndex;
		unsigned entityId = 0;
		for(unsigned i = 0; i < p_triangles.size(); i++)
		{
			if(p_triangles[i].m_materialIndex != currentMaterialIndex)
			{
				entityId = m_entities.size();

				if(m_sizes[currentMaterialIndex] == 32)
				{
					// Create PNUV-entity.
					CreatePNUVEntity(entityId, currentMaterialIndex, verticesPNUV, indices);
				}
				else if(m_sizes[currentMaterialIndex] == 56)
				{
					// Create PNUVTBT-entity.
					CreatePNUVTBTEntity(entityId, currentMaterialIndex, verticesPNUVTBT, indices);
				}

				currentMaterialIndex = p_triangles[i].m_materialIndex;
				indices.clear();
				entitiesIds.push_back(entityId);
			}

			if(m_sizes[currentMaterialIndex] == 32)
			{
				Render::Vertex1P1N1UV vertex;
				int index;
				for(unsigned j = 0; j < 3; ++j)
				{
					vertex = m_vertices[p_triangles[i].m_indices[j]];

					index = -1;
					for(auto itr = verticesPNUV.begin(); itr != verticesPNUV.end(); ++itr)
					{
						if((*itr).m_pos == vertex.m_pos && (*itr).m_UV == vertex.m_UV && (*itr).m_normal == vertex.m_normal)
						{
							index = itr - verticesPNUV.begin();
						}
					}
					if(index == -1)
					{
						verticesPNUV.push_back(vertex);
						indices.push_back(verticesPNUV.size()-1);
					}
					else
					{
						indices.push_back(index);
					}
				}
			}
			else if(m_sizes[currentMaterialIndex] == 56)
			{
				Render::Vertex1P1N1UV1T1BT vertex;
				int index;
				for(unsigned j = 0; j < 3; ++j)
				{
					vertex = m_vertices[p_triangles[i].m_indices[j]];

					index = -1;
					for(auto itr = verticesPNUVTBT.begin(); itr != verticesPNUVTBT.end(); ++itr)
					{
						if((*itr).m_pos == vertex.m_pos && (*itr).m_UV == vertex.m_UV && (*itr).m_normal == vertex.m_normal)
						{
							index = itr - verticesPNUVTBT.begin();
						}
					}
					if(index == -1)
					{
						verticesPNUVTBT.push_back(vertex);
						indices.push_back(verticesPNUVTBT.size()-1);
					}
					else
					{
						indices.push_back(index);
					}
				}
			}
		}

		entityId = m_entities.size();

		if(m_sizes[currentMaterialIndex] == 32)
		{
			// Create PNUV-entity.
			CreatePNUVEntity(entityId, currentMaterialIndex, verticesPNUV, indices);
		}
		else if(m_sizes[currentMaterialIndex] == 56)
		{
			// Create PNUVTBT-entity.
			CreatePNUVTBTEntity(entityId, currentMaterialIndex, verticesPNUVTBT, indices);
		}

		indices.clear();
		entitiesIds.push_back(entityId);

		return entitiesIds;
	}

	QuadTree::QuadTree() : m_root(nullptr)
	{

	}

	QuadTree::~QuadTree()
	{
		delete m_root;
	}

}