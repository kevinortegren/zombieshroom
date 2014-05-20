#pragma once

#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/Components.h>

#include <yaml-cpp/yaml.h>
#include <RootEngine/Include/GameSharedContext.h>

#include <RootEngine/Physics/Include/RootPhysics.h>
#include <RootEngine/Script/Include/RootScript.h>

extern RootEngine::GameSharedContext g_engineContext;

static void Importer(ECS::World* p_world, int p_type, ECS::Entity* p_entity, const YAML::Node& p_node)
{
	switch(p_type)
	{
		case RootForce::ComponentType::RENDERABLE:
			{
				RootForce::Renderable* renderable = p_world->GetEntityManager()->CreateComponent<RootForce::Renderable>(p_entity);
				
				const YAML::Node* modelNode = p_node.FindValue("Model");
				if(modelNode != nullptr)
				{
					std::string model;
					p_node["Model"] >> model;					
					renderable->m_model = g_engineContext.m_resourceManager->LoadCollada(model);
					renderable->m_pass = RootForce::RenderPass::RENDERPASS_DEFAULT;

				}
				
				const YAML::Node* materialNode = p_node.FindValue("Material");
				if(materialNode != nullptr)
				{
					const YAML::Node* materialNameNode = materialNode->FindValue("Name");
					if(materialNameNode != nullptr)
					{
						std::string materialName;
						p_node["Material"]["Name"] >> materialName;
						renderable->m_material = g_engineContext.m_renderer->CreateMaterial(materialName);
					}

					const YAML::Node* effectNode = materialNode->FindValue("Effect");
					if(effectNode != nullptr)
					{
						// Load effect.
						std::string effect;
						p_node["Material"]["Effect"] >> effect;
						g_engineContext.m_resourceManager->LoadEffect(effect);
						if(renderable->m_material)
						{
							renderable->m_material->m_effect = g_engineContext.m_resourceManager->GetEffect(effect);
						}
						else
						{
							g_engineContext.m_logger->LogText(LogTag::RESOURCE, LogLevel::NON_FATAL_ERROR, "Trying to set effect on a renderable that doesn't have a material!");
						}
					}

					const YAML::Node* diffuseNode = materialNode->FindValue("Diffuse");
					if(diffuseNode != nullptr)
					{
						std::string diffuse;
						p_node["Material"]["Diffuse"] >> diffuse;
						if(renderable->m_material)
						{
							renderable->m_material->m_textures[Render::TextureSemantic::DIFFUSE] = g_engineContext.m_resourceManager->LoadTexture(diffuse, Render::TextureType::TEXTURE_2D);
						}
						else
						{
							g_engineContext.m_logger->LogText(LogTag::RESOURCE, LogLevel::NON_FATAL_ERROR, "Trying to set diffuse texture on a renderable that doesn't have a material!");
						}
					}
					const YAML::Node* specularNode = materialNode->FindValue("Specular");
					if(specularNode != nullptr)
					{
						std::string specular;
						p_node["Material"]["Specular"] >> specular;
						renderable->m_material->m_textures[Render::TextureSemantic::SPECULAR] = g_engineContext.m_resourceManager->LoadTexture(specular, Render::TextureType::TEXTURE_2D);
					}
					const YAML::Node* normalNode = materialNode->FindValue("Normal");
					if(normalNode != nullptr)
					{
						std::string normal;
						p_node["Material"]["Normal"] >> normal;
						renderable->m_material->m_textures[Render::TextureSemantic::NORMAL] = g_engineContext.m_resourceManager->LoadTexture(normal, Render::TextureType::TEXTURE_2D);
					}
					const YAML::Node* tmNode = materialNode->FindValue("TextureMap");
					if(tmNode != nullptr)
					{
						std::string texturemap;
						p_node["Material"]["TextureMap"] >> texturemap;
						renderable->m_material->m_textures[Render::TextureSemantic::TEXTUREMAP] = g_engineContext.m_resourceManager->LoadTexture(texturemap, Render::TextureType::TEXTURE_2D);
					}
					const YAML::Node* drNode = materialNode->FindValue("TextureRed");
					if(drNode != nullptr)
					{
						std::string diffuseR;
						p_node["Material"]["TextureRed"] >> diffuseR;
						renderable->m_material->m_textures[Render::TextureSemantic::TEXTURE_R] = g_engineContext.m_resourceManager->LoadTexture(diffuseR, Render::TextureType::TEXTURE_2D);
						renderable->m_material->m_textures[Render::TextureSemantic::TEXTURE_R]->SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
						renderable->m_material->m_textures[Render::TextureSemantic::TEXTURE_R]->SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

					}
					const YAML::Node* dgNode = materialNode->FindValue("TextureGreen");
					if(dgNode != nullptr)
					{
						std::string diffuseG;
						p_node["Material"]["TextureGreen"] >> diffuseG;
						renderable->m_material->m_textures[Render::TextureSemantic::TEXTURE_G] = g_engineContext.m_resourceManager->LoadTexture(diffuseG, Render::TextureType::TEXTURE_2D);
						renderable->m_material->m_textures[Render::TextureSemantic::TEXTURE_G]->SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
						renderable->m_material->m_textures[Render::TextureSemantic::TEXTURE_G]->SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
					}
					const YAML::Node* dbNode = materialNode->FindValue("TextureBlue");
					if(dbNode != nullptr)
					{
						std::string diffuseB;
						p_node["Material"]["TextureBlue"] >> diffuseB;
						renderable->m_material->m_textures[Render::TextureSemantic::TEXTURE_B] = g_engineContext.m_resourceManager->LoadTexture(diffuseB, Render::TextureType::TEXTURE_2D);
						renderable->m_material->m_textures[Render::TextureSemantic::TEXTURE_B]->SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
						renderable->m_material->m_textures[Render::TextureSemantic::TEXTURE_B]->SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
					}
					const YAML::Node* tileFactorNode = materialNode->FindValue("TileFactor");
					if(tileFactorNode != nullptr)
					{
						float tileFactor;
						p_node["Material"]["TileFactor"] >> tileFactor;
						renderable->m_material->m_tileFactor = tileFactor;
						renderable->m_params[Render::Semantic::SIZEMIN] = &renderable->m_material->m_tileFactor;
					}
					const YAML::Node* glowNode = materialNode->FindValue("Glow");
					if(glowNode != nullptr)
					{
						std::string glow;
						p_node["Material"]["Glow"] >> glow;
						renderable->m_material->m_textures[Render::TextureSemantic::GLOW] = g_engineContext.m_resourceManager->LoadTexture(glow, Render::TextureType::TEXTURE_2D);
					}
				}	
			}
			break;
		case RootForce::ComponentType::TRANSFORM:
			{
				RootForce::Transform* transform = p_world->GetEntityManager()->CreateComponent<RootForce::Transform>(p_entity);

				glm::vec3 position;
				p_node["Position"][0] >> position.x;
				p_node["Position"][1] >> position.y;
				p_node["Position"][2] >> position.z;

				transform->m_position = position;

				if(p_node["Rotation"].size() == 4)
				{
					glm::quat rotation;
					p_node["Rotation"][0] >> rotation.x;
					p_node["Rotation"][1] >> rotation.y;
					p_node["Rotation"][2] >> rotation.z;
					p_node["Rotation"][3] >> rotation.w;

					transform->m_orientation.SetOrientation(rotation);
				}
				else
				{
					glm::vec3 rotation;
					p_node["Rotation"][0] >> rotation.x;
					p_node["Rotation"][1] >> rotation.y;
					p_node["Rotation"][2] >> rotation.z;

					transform->m_orientation.SetOrientation(rotation.x, rotation.y, rotation.z);
				}

				glm::vec3 scale;
				p_node["Scale"][0] >> scale.x;
				p_node["Scale"][1] >> scale.y;
				p_node["Scale"][2] >> scale.z;

				transform->m_scale = scale;
			}
			break;
		case RootForce::ComponentType::POINTLIGHT:
			{
				RootForce::PointLight* pointLight = p_world->GetEntityManager()->CreateComponent<RootForce::PointLight>(p_entity);

				glm::vec3 attenuation;
				p_node["Attenuation"][0] >> attenuation.x;
				p_node["Attenuation"][1] >> attenuation.y;
				p_node["Attenuation"][2] >> attenuation.z;

				pointLight->m_attenuation = attenuation;

				glm::vec4 color;
				p_node["Color"][0] >> color.x;
				p_node["Color"][1] >> color.y;
				p_node["Color"][2] >> color.z;
				p_node["Color"][3] >> color.w;

				pointLight->m_color = color;

				float range;
				p_node["Range"] >> range;

				pointLight->m_range = range;
			}
			break;
		case RootForce::ComponentType::DIRECTIONALLIGHT:
			{
				RootForce::DirectionalLight* directionalLight = p_world->GetEntityManager()->CreateComponent<RootForce::DirectionalLight>(p_entity);

				glm::vec4 color;
				p_node["Color"][0] >> color.r;
				p_node["Color"][1] >> color.g;
				p_node["Color"][2] >> color.b;
				p_node["Color"][3] >> color.a;

				directionalLight->m_color = color;
			}
			break;
		case RootForce::ComponentType::PLAYERCONTROL:
			{
				RootForce::PlayerPhysics* playphys = p_world->GetEntityManager()->CreateComponent<RootForce::PlayerPhysics>(p_entity);
				p_node["Speed"] >> playphys->MovementSpeed;
			}
			break;
		case RootForce::ComponentType::PHYSICS:
			{
				/*RootForce::PhysicsAccessor* physaccessor = p_world->GetEntityManager()->CreateComponent<RootForce::PhysicsAccessor>(p_entity);
				float mass, maxSpeed, stepHeight, modelHeight;
				int type;
				std::string modelHandle;
				RootForce::Transform* temp = p_world->GetEntityManager()->GetComponent<RootForce::Transform>(p_entity);
				glm::quat rotation =  temp->m_orientation.GetQuaternion();
				p_node["Type"] >> type;
				p_node["Mass"] >> mass;
				p_node["ModelHandle"] >> modelHandle;
				if(type == RootEngine::Physics::PhysicsType::TYPE_PLAYER) 
				{
					p_node["StepHeight"] >> stepHeight;
					p_node["ModelHeight"] >> modelHeight;
					p_node["MaxSpeed"] >> maxSpeed;
					physaccessor->m_handle = g_engineContext.m_physics->AddPlayerObjectToWorld(modelHandle, p_entity->GetId(), temp->m_position, rotation, mass, maxSpeed, modelHeight, stepHeight, nullptr);
				}
				else if (type == RootEngine::Physics::PhysicsType::TYPE_DYNAMIC)
				{
					physaccessor->m_handle = g_engineContext.m_physics->AddDynamicObjectToWorld(modelHandle, p_entity->GetId(), temp->m_position, rotation, mass);
				}
				else if (type == RootEngine::Physics::PhysicsType::TYPE_STATIC)
				{
					physaccessor->m_handle = g_engineContext.m_physics->AddStaticObjectToWorld(p_entity->GetId());
					g_engineContext.m_physics->BindMeshShape(*(physaccessor->m_handle), modelHandle, temp->m_position, temp->m_orientation.GetQuaternion(), 0);
				}*/

			}
			break;
		case RootForce::ComponentType::COLLISION:
			{
				RootForce::Collision* collision = p_world->GetEntityManager()->CreateComponent<RootForce::Collision>(p_entity);
				RootForce::Physics* physics = p_world->GetEntityManager()->CreateComponent<RootForce::Physics>(p_entity);
				RootForce::Transform* trans = p_world->GetEntityManager()->GetComponent<RootForce::Transform>(p_entity);

				// Backwards compatibility - for static meshes.
				int type;
				if(!p_node.FindValue("PhysicsType"))
					type = RootEngine::Physics::PhysicsType::TYPE_STATIC;
				else
					p_node["PhysicsType"] >> type;

				//Create and store physics handle in collision component
				collision->m_handle = g_engineContext.m_physics->CreateHandle(p_entity, (RootEngine::Physics::PhysicsType::PhysicsType)type, false);

				//Create collision container
				//g_engineContext.m_physics->SetCollisionContainer(*collision->m_handle, &collisionResp->m_collisions);

				if (type == RootEngine::Physics::PhysicsType::TYPE_DYNAMIC)
				{
					//Set physics mass
					p_node["ShapeMass"] >> physics->m_mass;
					if (g_engineContext.m_physics->GetType(*collision->m_handle) == RootEngine::Physics::PhysicsType::TYPE_STATIC)
						physics->m_mass = 0;
				}

				//Create physics shape
				/*	
				if (type == RootEngine::Physics::PhysicsType::TYPE_STATIC)
				{
					std::string meshHandle;
					p_node["MeshHandle"] >> meshHandle;
					collision->m_meshHandle = meshHandle;

					g_engineContext.m_physics->BindMeshShape(*collision->m_handle, meshHandle, trans->m_position, trans->m_orientation.GetQuaternion(), trans->m_scale, 0.0f, true);

					break;
				}
				*/

				int shape;
				p_node["PhysicsShape"] >> shape;

				bool collideWithWorld;
				bool collideWithStatic;
				if (type == RootEngine::Physics::PhysicsType::TYPE_DYNAMIC)
				{
					p_node["CollideWithWorld"] >> collideWithWorld;
					p_node["CollideWithStatic"] >> collideWithStatic;
				}
				else
				{
					collideWithWorld = true;
					collideWithStatic = true;
				}

				RootEngine::Physics::PhysicsShape::PhysicsShape pshape = (RootEngine::Physics::PhysicsShape::PhysicsShape)shape;
				switch (pshape)
				{
				case RootEngine::Physics::PhysicsShape::SHAPE_SPHERE:
					{
						float shapeRadius;
						p_node["ShapeRadius"] >> shapeRadius;

						g_engineContext.m_physics->BindSphereShape(*collision->m_handle, trans->m_position, glm::quat(0,0,0,1), shapeRadius, physics->m_mass, collideWithWorld, collideWithStatic);
					}
					break;
				case RootEngine::Physics::PhysicsShape::SHAPE_CONE:
					{
						float shapeRadius;
						p_node["ShapeRadius"] >> shapeRadius;

						float shapeHeight;
						p_node["ShapeHeight"] >> shapeHeight;

						g_engineContext.m_physics->BindConeShape(*collision->m_handle, trans->m_position, glm::quat(0,0,0,1), shapeHeight, shapeRadius, physics->m_mass, collideWithWorld, collideWithStatic);
					}
					break;
				case RootEngine::Physics::PhysicsShape::SHAPE_CYLINDER:
					{
						float shapeRadius;
						p_node["ShapeRadius"] >> shapeRadius;

						float shapeHeight;
						p_node["ShapeHeight"] >> shapeHeight;

						g_engineContext.m_physics->BindCylinderShape(*collision->m_handle, trans->m_position, glm::quat(0,0,0,1), shapeHeight, shapeRadius, physics->m_mass, collideWithWorld, collideWithStatic);
					}
					break;
				case RootEngine::Physics::PhysicsShape::SHAPE_CUSTOM_MESH:
					{
						std::string meshHandle;
						p_node["MeshHandle"] >> meshHandle;
						collision->m_meshHandle = meshHandle;

						g_engineContext.m_physics->BindMeshShape(*collision->m_handle, meshHandle, trans->m_position, trans->m_orientation.GetQuaternion(), trans->m_scale, physics->m_mass, collideWithWorld, collideWithStatic);
					}
					break;
				case RootEngine::Physics::PhysicsShape::SHAPE_HULL:
					break;
				case RootEngine::Physics::PhysicsShape::SHAPE_NONE:
					break;
				default:
					break;
				}

				// If dynamic, set physics gravity (must be done after a shape has been created).
				if (type == RootEngine::Physics::PhysicsType::TYPE_DYNAMIC)
				{
					glm::vec3 gravity;
					p_node["ShapeGravity"][0] >> gravity.x;
					p_node["ShapeGravity"][1] >> gravity.y;
					p_node["ShapeGravity"][2] >> gravity.z;
					g_engineContext.m_physics->SetGravity(*collision->m_handle, gravity);
				}
			}
			break;
			case RootForce::ComponentType::COLLISIONRESPONDER:
			{
				RootForce::CollisionResponder* collisionResp = p_world->GetEntityManager()->CreateComponent<RootForce::CollisionResponder>(p_entity);
				RootForce::Collision* collision = p_world->GetEntityManager()->GetComponent<RootForce::Collision>(p_entity);
				if (collision != nullptr)
				{
					g_engineContext.m_physics->SetCollisionContainer(*collision->m_handle, &collisionResp->m_collisions);
				}
			}
			break;

		case RootForce::ComponentType::PARTICLE:
			{
				RootForce::ParticleEmitter* particleEmitter = p_world->GetEntityManager()->CreateComponent<RootForce::ParticleEmitter>(p_entity);
				RootForce::Transform* trans = p_world->GetEntityManager()->GetComponent<RootForce::Transform>(p_entity);
				std::string particleName;

				p_node["File"] >> particleName;

				particleEmitter->m_particleSystems = g_engineContext.m_resourceManager->LoadParticleEmitter(particleName, false);
				particleEmitter->m_name = particleName;

				for(unsigned i = 0; i < particleEmitter->m_particleSystems.size(); i++)
					particleEmitter->m_systems.push_back(g_engineContext.m_renderer->CreateParticleSystem());
			}
			break;
		case RootForce::ComponentType::SHADOWCASTER:
			{
				RootForce::Shadowcaster* shadow = p_world->GetEntityManager()->CreateComponent<RootForce::Shadowcaster>(p_entity);

				p_node["LightSlot"] >> shadow->m_directionalLightSlot;

			}
			break;

		case RootForce::ComponentType::SCRIPT:
			{
				RootForce::Script* script = p_world->GetEntityManager()->CreateComponent<RootForce::Script>(p_entity);
				
				p_node["ScriptName"] >> script->Name;
				g_engineContext.m_resourceManager->LoadScript(script->Name);
			}
			break;
		default:
			break;
	}
}