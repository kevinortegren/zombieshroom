#pragma once

#include <yaml-cpp\yaml.h>
#include <RootSystems\Include\Components.h>

#include <RootEngine/Include/GameSharedContext.h>
extern RootEngine::GameSharedContext g_engineContext;

static void Exporter(YAML::Emitter& p_emitter, ECS::ComponentInterface* p_component, int p_type)
{
	switch(p_type)
	{
		case RootForce::ComponentType::RENDERABLE:
			{
				RootForce::Renderable* renderable = static_cast<RootForce::Renderable*>(p_component);	
				if(renderable->m_model != nullptr)
				{
					std::string s = g_engineContext.m_resourceManager->ResolveStringFromModel(renderable->m_model);
					p_emitter << YAML::Key << "Model" << YAML::Value << s;
				}
				if(renderable->m_material != nullptr)
				{
					std::string s = g_engineContext.m_renderer->GetStringFromMaterial(renderable->m_material);
					p_emitter << YAML::Key << "Material" << YAML::Value << YAML::BeginMap;
					p_emitter << YAML::Key << "Name" << YAML::Value << s;
					if(renderable->m_material->m_effect != nullptr)
					{
						std::string s = g_engineContext.m_resourceManager->ResolveStringFromEffect(renderable->m_material->m_effect);
						p_emitter << YAML::Key << "Effect" << YAML::Value << s;
					}			
					if(renderable->m_material->m_textures[Render::TextureSemantic::DIFFUSE] != nullptr)
					{				
						std::string s = g_engineContext.m_resourceManager->ResolveStringFromTexture(renderable->m_material->m_textures[Render::TextureSemantic::DIFFUSE]);
						p_emitter << YAML::Key << "Diffuse" << YAML::Value << s;
					}	
					if(renderable->m_material->m_textures[Render::TextureSemantic::SPECULAR] != nullptr)
					{
						std::string s = g_engineContext.m_resourceManager->ResolveStringFromTexture(renderable->m_material->m_textures[Render::TextureSemantic::SPECULAR]);
						p_emitter << YAML::Key << "Specular" << YAML::Value << s;
					}
					if(renderable->m_material->m_textures[Render::TextureSemantic::NORMAL] != nullptr)
					{
						std::string s = g_engineContext.m_resourceManager->ResolveStringFromTexture(renderable->m_material->m_textures[Render::TextureSemantic::NORMAL]);
						p_emitter << YAML::Key << "Normal" << YAML::Value << s;
					}

					if(renderable->m_material->m_textures[Render::TextureSemantic::GLOW] != nullptr)
					{
						std::string s = g_engineContext.m_resourceManager->ResolveStringFromTexture(renderable->m_material->m_textures[Render::TextureSemantic::GLOW]);
						p_emitter << YAML::Key << "Glow" << YAML::Value << s;
					}

					if(g_engineContext.m_resourceManager->ResolveStringFromEffect(renderable->m_material->m_effect) == "Mesh_Blend" )
					{
					if(renderable->m_material->m_textures[Render::TextureSemantic::TEXTUREMAP] != nullptr)
					{
						std::string s = g_engineContext.m_resourceManager->ResolveStringFromTexture(renderable->m_material->m_textures[Render::TextureSemantic::TEXTUREMAP]);
						p_emitter << YAML::Key << "TextureMap" << YAML::Value << s;
					}

					if(renderable->m_material->m_textures[Render::TextureSemantic::TEXTURE_R] != nullptr)
					{
						std::string s = g_engineContext.m_resourceManager->ResolveStringFromTexture(renderable->m_material->m_textures[Render::TextureSemantic::TEXTURE_R]);
						p_emitter << YAML::Key << "TextureRed" << YAML::Value << s;
					}

					if(renderable->m_material->m_textures[Render::TextureSemantic::TEXTURE_G] != nullptr)
					{
						std::string s = g_engineContext.m_resourceManager->ResolveStringFromTexture(renderable->m_material->m_textures[Render::TextureSemantic::TEXTURE_G]);
						p_emitter << YAML::Key << "TextureGreen" << YAML::Value << s;
					}
					if(renderable->m_material->m_textures[Render::TextureSemantic::TEXTURE_B] != nullptr)
					{
						std::string s = g_engineContext.m_resourceManager->ResolveStringFromTexture(renderable->m_material->m_textures[Render::TextureSemantic::TEXTURE_B]);
						p_emitter << YAML::Key << "TextureBlue" << YAML::Value << s;
					}
					if(renderable->m_material->m_textures[Render::TextureSemantic::TEXTURE_RN] != nullptr)
					{
						std::string s = g_engineContext.m_resourceManager->ResolveStringFromTexture(renderable->m_material->m_textures[Render::TextureSemantic::TEXTURE_RN]);
						p_emitter << YAML::Key << "TextureRedNormal" << YAML::Value << s;
					}
					if(renderable->m_material->m_textures[Render::TextureSemantic::TEXTURE_GN] != nullptr)
					{
						std::string s = g_engineContext.m_resourceManager->ResolveStringFromTexture(renderable->m_material->m_textures[Render::TextureSemantic::TEXTURE_GN]);
						p_emitter << YAML::Key << "TextureGreenNormal" << YAML::Value << s;
					}
					if(renderable->m_material->m_textures[Render::TextureSemantic::TEXTURE_BN] != nullptr)
					{
						std::string s = g_engineContext.m_resourceManager->ResolveStringFromTexture(renderable->m_material->m_textures[Render::TextureSemantic::TEXTURE_BN]);
						p_emitter << YAML::Key << "TextureBlueNormal" << YAML::Value << s;
					}
					if(renderable->m_material->m_tileFactor != 0)
					{
						std::ostringstream oss;
						oss << renderable->m_material->m_tileFactor;
						
						std::string s = oss.str();
						p_emitter << YAML::Key << "TileFactor" << YAML::Value << s;
					}
					}
					p_emitter << YAML::EndMap;
				}
			}
			break;
		case RootForce::ComponentType::TRANSFORM:
			{
				RootForce::Transform* transform = static_cast<RootForce::Transform*>(p_component);
				glm::vec3 position = transform->m_position;
				glm::vec3 scale = transform->m_scale;
				
				/*(*glm::mat3 rotation = transform->m_orientation.GetMatrix();

				float x,y,z;
				x = atan2(rotation[2][1], rotation[2][2]);
				y = atan2(-rotation[2][0], sqrt(pow(rotation[2][1],2) + pow(rotation[2][2],2)));
				z = atan2(rotation[1][0], rotation[0][0]);*/

				glm::quat q = transform->m_orientation.GetQuaternion();

				p_emitter << YAML::Key << "Position" << YAML::Value << YAML::Flow << YAML::BeginSeq << position.x << position.y << position.z << YAML::EndSeq;
				//p_emitter << YAML::Key << "Rotation" << YAML::Value << YAML::Flow << YAML::BeginSeq << -glm::degrees(x) << -glm::degrees(y) << -glm::degrees(z) << YAML::EndSeq;
				p_emitter << YAML::Key << "Rotation" << YAML::Value << YAML::Flow << YAML::BeginSeq << q.x << q.y << q.z << q.w << YAML::EndSeq;
				p_emitter << YAML::Key << "Scale" << YAML::Value << YAML::Flow << YAML::BeginSeq << scale.x << scale.y << scale.z << YAML::EndSeq;
			}
			break;
		case RootForce::ComponentType::POINTLIGHT:
			{
				RootForce::PointLight* pointLight = static_cast<RootForce::PointLight*>(p_component);
				glm::vec3 attenuation = pointLight->m_attenuation;
				glm::vec4 color = pointLight->m_color;
				float range = pointLight->m_range;

				p_emitter << YAML::Key << "Attenuation" << YAML::Value << YAML::Flow << YAML::BeginSeq << attenuation.x << attenuation.y << attenuation.z << YAML::EndSeq;
				p_emitter << YAML::Key << "Color" << YAML::Value << YAML::Flow << YAML::BeginSeq << color.x << color.y << color.z << color.w << YAML::EndSeq;
				p_emitter << YAML::Key << "Range" << YAML::Value << range;
			}
			break;
		case RootForce::ComponentType::DIRECTIONALLIGHT:
			{
				RootForce::DirectionalLight* directionalLight = static_cast<RootForce::DirectionalLight*>(p_component);
				glm::vec4 color = directionalLight->m_color;

				p_emitter << YAML::Key << "Color" << YAML::Value << YAML::Flow << YAML::BeginSeq << color.x << color.y << color.z << color.w << YAML::EndSeq;
			}
			break;
		case RootForce::ComponentType::PHYSICS:
			{
				/*RootForce::Physics* physics = static_cast<RootForce::Physics*>(p_component);
				p_emitter << YAML::Key << "Mass" << YAML::Value << physics->m_mass;*/
			}
			break;
		case RootForce::ComponentType::COLLISION:
			{
				RootForce::Collision* collision = static_cast<RootForce::Collision*>(p_component);

				
				p_emitter << YAML::Key << "PhysicsType" << YAML::Value << g_engineContext.m_physics->GetType(*collision->m_handle);

				// If the entity is dynamic and has a physics component.
				if (g_engineContext.m_physics->GetType(*collision->m_handle) == RootEngine::Physics::PhysicsType::TYPE_DYNAMIC)
				{
					p_emitter << YAML::Key << "ShapeMass" << YAML::Value << g_engineContext.m_physics->GetMass(*collision->m_handle);

					glm::vec3 gravity = g_engineContext.m_physics->GetGravity(*collision->m_handle);
					p_emitter << YAML::Key << "ShapeGravity" << YAML::Value << YAML::Flow << YAML::BeginSeq << gravity.x << gravity.y << gravity.z << YAML::EndSeq;
				}

				RootEngine::Physics::PhysicsShape::PhysicsShape shape = g_engineContext.m_physics->GetShape(*collision->m_handle);

				p_emitter << YAML::Key << "PhysicsShape" << YAML::Value << (int)shape;
				p_emitter << YAML::Key << "CollideWithWorld"	<< YAML::Value << g_engineContext.m_physics->GetCollideWithWorld(*collision->m_handle);
				p_emitter << YAML::Key << "CollideWithStatic"	<< YAML::Value << g_engineContext.m_physics->GetCollideWithStatic(*collision->m_handle);

				switch (shape)
				{
				case RootEngine::Physics::PhysicsShape::SHAPE_SPHERE:
					{

						p_emitter << YAML::Key << "ShapeRadius"			<< YAML::Value << g_engineContext.m_physics->GetRadius(*collision->m_handle);
					}
					break;
				case RootEngine::Physics::PhysicsShape::SHAPE_CONE:
					{
						p_emitter << YAML::Key << "ShapeRadius"			<< YAML::Value << g_engineContext.m_physics->GetRadius(*collision->m_handle);
						p_emitter << YAML::Key << "ShapeHeight"			<< YAML::Value << g_engineContext.m_physics->GetHeight(*collision->m_handle);
					}
					break;
				case RootEngine::Physics::PhysicsShape::SHAPE_CYLINDER:
					{
						p_emitter << YAML::Key << "ShapeRadius"			<< YAML::Value << g_engineContext.m_physics->GetRadius(*collision->m_handle);
						p_emitter << YAML::Key << "ShapeHeight"			<< YAML::Value << g_engineContext.m_physics->GetHeight(*collision->m_handle);
					}
					break;
				case RootEngine::Physics::PhysicsShape::SHAPE_CUSTOM_MESH:
					{
						p_emitter << YAML::Key << "MeshHandle" << YAML::Value << g_engineContext.m_physics->GetPhysicsModelHandle(*collision->m_handle);
					}
					break;
				case RootEngine::Physics::PhysicsShape::SHAPE_HULL:
					{
						// Possibly not needed to export?
					}
					break;
				case RootEngine::Physics::PhysicsShape::SHAPE_NONE:
					break;
				default:
					break;
				}
			}
			break;
		case RootForce::ComponentType::PARTICLE:
			{
				RootForce::ParticleEmitter* particle = static_cast<RootForce::ParticleEmitter*>(p_component);

				p_emitter << YAML::Key << "File" << YAML::Value << particle->m_name;
			}
			break;
		case RootForce::ComponentType::SHADOWCASTER:
			{
				RootForce::Shadowcaster* shadow = static_cast<RootForce::Shadowcaster*>(p_component);

				p_emitter << YAML::Key << "LightSlot" << YAML::Value << shadow->m_directionalLightSlot;
			}
			break;
		default:
			break;
	}
}