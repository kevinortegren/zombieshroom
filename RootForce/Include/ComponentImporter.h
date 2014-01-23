#pragma once

#include <Utility\ECS\Include\World.h>
#include <RootSystems\Include\Components.h>

#include <yaml-cpp\yaml.h>

#include <RootEngine/Include/GameSharedContext.h>

extern RootEngine::GameSharedContext g_engineContext;

static void ImportParticleEmitter(const std::string& p_filename, RootForce::ParticleEmitter* p_particleEmitter, RootForce::Transform* p_particleTransform)
{
	std::string filePath = g_engineContext.m_resourceManager->GetWorkingDirectory() + "Assets\\Particles\\" + p_filename + ".particle";

	std::ifstream file(filePath, std::ifstream::in);
	if(!file.good())
	{
		g_engineContext.m_logger->LogText(LogTag::RESOURCE, LogLevel::FATAL_ERROR, "Failed to load particle file: %s", p_filename.c_str());
	}
	YAML::Parser parser(file);

	YAML::Node doc;
	parser.GetNextDocument(doc);
	p_particleEmitter->m_particleSystems.resize(doc.size());
	int materialIndex = 0;
	for (unsigned int i = 0; i < doc.size(); i++)
	{
		//Resources
		std::string effectName;
		std::string textureName;
		doc[i]["EFFECT"]		>> effectName;
		doc[i]["TEXTURE"]		>> textureName;

		p_particleEmitter->m_particleSystems[i].m_system					= g_engineContext.m_renderer->CreateParticleSystem();	
		p_particleEmitter->m_particleSystems[i].m_material					= g_engineContext.m_resourceManager->GetMaterial(p_filename + std::to_string(materialIndex++));
		p_particleEmitter->m_particleSystems[i].m_material->m_textures[Render::TextureSemantic::DIFFUSE]	= g_engineContext.m_resourceManager->LoadTexture(textureName, Render::TextureType::TEXTURE_2D);
		p_particleEmitter->m_particleSystems[i].m_material->m_effect		= g_engineContext.m_resourceManager->LoadEffect(effectName);

		//Position
		doc[i]["POSITION"][0]	>> p_particleEmitter->m_particleSystems[i].m_position.x;
		doc[i]["POSITION"][1]	>> p_particleEmitter->m_particleSystems[i].m_position.y;
		doc[i]["POSITION"][2]	>> p_particleEmitter->m_particleSystems[i].m_position.z;
		//Life time
		doc[i]["LIFETIMEMIN"]	>> p_particleEmitter->m_particleSystems[i].m_lifeTimeMin;
		doc[i]["LIFETIMEMAX"]	>> p_particleEmitter->m_particleSystems[i].m_lifeTimeMax;
		//Speed
		doc[i]["SPEEDMIN"]		>> p_particleEmitter->m_particleSystems[i].m_speedMin;
		doc[i]["SPEEDMAX"]		>> p_particleEmitter->m_particleSystems[i].m_speedMax;
		//Size
		doc[i]["SIZEMIN"][0]	>> p_particleEmitter->m_particleSystems[i].m_sizeMin.x;
		doc[i]["SIZEMIN"][1]	>> p_particleEmitter->m_particleSystems[i].m_sizeMin.y;
		doc[i]["SIZEMAX"][0]	>> p_particleEmitter->m_particleSystems[i].m_sizeMax.x;
		doc[i]["SIZEMAX"][1]	>> p_particleEmitter->m_particleSystems[i].m_sizeMax.y;
		doc[i]["SIZEEND"][0]	>> p_particleEmitter->m_particleSystems[i].m_sizeEnd.x;
		doc[i]["SIZEEND"][1]	>> p_particleEmitter->m_particleSystems[i].m_sizeEnd.y;
		//Color
		doc[i]["COLOR"][0]		>> p_particleEmitter->m_particleSystems[i].m_color.r;
		doc[i]["COLOR"][1]		>> p_particleEmitter->m_particleSystems[i].m_color.g;
		doc[i]["COLOR"][2]		>> p_particleEmitter->m_particleSystems[i].m_color.b;
		doc[i]["COLOR"][3]		>> p_particleEmitter->m_particleSystems[i].m_color.a;
		doc[i]["COLOREND"][0]	>> p_particleEmitter->m_particleSystems[i].m_colorEnd.r;
		doc[i]["COLOREND"][1]	>> p_particleEmitter->m_particleSystems[i].m_colorEnd.g;
		doc[i]["COLOREND"][2]	>> p_particleEmitter->m_particleSystems[i].m_colorEnd.b;
		doc[i]["COLOREND"][3]	>> p_particleEmitter->m_particleSystems[i].m_colorEnd.a;
		//Gravity
		doc[i]["GRAVITY"][0]	>> p_particleEmitter->m_particleSystems[i].m_gravity.x;
		doc[i]["GRAVITY"][1]	>> p_particleEmitter->m_particleSystems[i].m_gravity.y;
		doc[i]["GRAVITY"][2]	>> p_particleEmitter->m_particleSystems[i].m_gravity.z;
		//Direction
		doc[i]["DIRECTION"][0]	>> p_particleEmitter->m_particleSystems[i].m_direction.x;
		doc[i]["DIRECTION"][1]	>> p_particleEmitter->m_particleSystems[i].m_direction.y;
		doc[i]["DIRECTION"][2]	>> p_particleEmitter->m_particleSystems[i].m_direction.z;
		//Spread
		doc[i]["SPREAD"]		>> p_particleEmitter->m_particleSystems[i].m_spread;
		//Spawn time
		doc[i]["SPAWNTIME"]		>> p_particleEmitter->m_particleSystems[i].m_spawnTime;

		//Set params
		p_particleEmitter->m_particleSystems[i].m_params[Render::Semantic::POSITION]		= &p_particleEmitter->m_particleSystems[i].m_position;
		p_particleEmitter->m_particleSystems[i].m_params[Render::Semantic::LIFETIMEMIN]		= &p_particleEmitter->m_particleSystems[i].m_lifeTimeMin;
		p_particleEmitter->m_particleSystems[i].m_params[Render::Semantic::LIFETIMEMAX]		= &p_particleEmitter->m_particleSystems[i].m_lifeTimeMax;
		p_particleEmitter->m_particleSystems[i].m_params[Render::Semantic::SPEEDMIN]		= &p_particleEmitter->m_particleSystems[i].m_speedMin;
		p_particleEmitter->m_particleSystems[i].m_params[Render::Semantic::SPEEDMAX]		= &p_particleEmitter->m_particleSystems[i].m_speedMax;
		p_particleEmitter->m_particleSystems[i].m_params[Render::Semantic::SIZEMIN]			= &p_particleEmitter->m_particleSystems[i].m_sizeMin;
		p_particleEmitter->m_particleSystems[i].m_params[Render::Semantic::SIZEMAX]			= &p_particleEmitter->m_particleSystems[i].m_sizeMax;
		p_particleEmitter->m_particleSystems[i].m_params[Render::Semantic::SIZEEND]			= &p_particleEmitter->m_particleSystems[i].m_sizeEnd;
		p_particleEmitter->m_particleSystems[i].m_params[Render::Semantic::COLOR]			= &p_particleEmitter->m_particleSystems[i].m_color;
		p_particleEmitter->m_particleSystems[i].m_params[Render::Semantic::COLOREND]		= &p_particleEmitter->m_particleSystems[i].m_colorEnd;
		p_particleEmitter->m_particleSystems[i].m_params[Render::Semantic::GRAVITY]			= &p_particleEmitter->m_particleSystems[i].m_gravity;
		p_particleEmitter->m_particleSystems[i].m_params[Render::Semantic::DIRECTION]		= &p_particleEmitter->m_particleSystems[i].m_direction;
		p_particleEmitter->m_particleSystems[i].m_params[Render::Semantic::SPREAD]			= &p_particleEmitter->m_particleSystems[i].m_spread;
		p_particleEmitter->m_particleSystems[i].m_params[Render::Semantic::SPAWNTIME]		= &p_particleEmitter->m_particleSystems[i].m_spawnTime;
	}
	g_engineContext.m_logger->LogText(LogTag::RESOURCE, LogLevel::SUCCESS, "Particle emitter %s loaded!", filePath.c_str());
}

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
					g_engineContext.m_resourceManager->LoadCollada(model);
					renderable->m_model = g_engineContext.m_resourceManager->GetModel(model);
					renderable->m_pass = RootForce::RenderPass::RENDERPASS_TERRAIN;

				}

				const YAML::Node* materialNode = p_node.FindValue("Material");
				if(materialNode != nullptr)
				{
					const YAML::Node* materialNameNode = materialNode->FindValue("Name");
					if(materialNameNode != nullptr)
					{
						std::string materialName;
						p_node["Material"]["Name"] >> materialName;
						renderable->m_material = g_engineContext.m_resourceManager->GetMaterial(materialName);
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

							// Allocate memory for this renderable's uniforms.
							//renderable->m_material->m_params = g_engineContext.m_renderer->CreateEffectParams();
							//renderable->m_material->m_params->AllocateParams(renderable->m_material->m_effect);
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
					const YAML::Node* drNode = materialNode->FindValue("DiffuseR");
					if(drNode != nullptr)
					{
						std::string diffuseR;
						p_node["Material"]["DiffuseR"] >> diffuseR;
						renderable->m_material->m_textures[Render::TextureSemantic::TEXTURE_R] = g_engineContext.m_resourceManager->LoadTexture(diffuseR, Render::TextureType::TEXTURE_2D);
						renderable->m_material->m_textures[Render::TextureSemantic::TEXTURE_R]->SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
						renderable->m_material->m_textures[Render::TextureSemantic::TEXTURE_R]->SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

					}
					const YAML::Node* dgNode = materialNode->FindValue("DiffuseG");
					if(dgNode != nullptr)
					{
						std::string diffuseG;
						p_node["Material"]["DiffuseG"] >> diffuseG;
						renderable->m_material->m_textures[Render::TextureSemantic::TEXTURE_G] = g_engineContext.m_resourceManager->LoadTexture(diffuseG, Render::TextureType::TEXTURE_2D);
						renderable->m_material->m_textures[Render::TextureSemantic::TEXTURE_G]->SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
						renderable->m_material->m_textures[Render::TextureSemantic::TEXTURE_G]->SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
					}
					const YAML::Node* dbNode = materialNode->FindValue("DiffuseB");
					if(dbNode != nullptr)
					{
						std::string diffuseB;
						p_node["Material"]["DiffuseB"] >> diffuseB;
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

				glm::vec3 rotation;
				p_node["Rotation"][0] >> rotation.x;
				p_node["Rotation"][1] >> rotation.y;
				p_node["Rotation"][2] >> rotation.z;

				transform->m_orientation.SetOrientation(rotation.x, rotation.y, rotation.z);

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
				std::string meshHandle;
				
				p_node["MeshHandle"] >> meshHandle;
				collision->m_meshHandle = meshHandle;
			}
			break;
		case RootForce::ComponentType::PARTICLE:
			{
				RootForce::ParticleEmitter* particleEmitter = p_world->GetEntityManager()->CreateComponent<RootForce::ParticleEmitter>(p_entity);
				RootForce::Transform* trans = p_world->GetEntityManager()->GetComponent<RootForce::Transform>(p_entity);
				std::string particleName;

				p_node["File"] >> particleName;

				ImportParticleEmitter(particleName, particleEmitter, trans);
			}
			break;
		default:
			break;
	}
}