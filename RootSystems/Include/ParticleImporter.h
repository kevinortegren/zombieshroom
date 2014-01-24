#pragma once

#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/Components.h>
#include <yaml-cpp/yaml.h>
#include <RootEngine/Include/GameSharedContext.h>

extern RootEngine::GameSharedContext g_engineContext;

static void ImportParticleEmitter(const std::string& p_filename, RootForce::ParticleEmitter* p_particleEmitter, RootForce::Transform* p_particleTransform, bool p_fullFilePath)
{
	std::string filePath;

	if(p_fullFilePath)
		filePath = p_filename;
	else
		filePath = g_engineContext.m_resourceManager->GetWorkingDirectory() + "Assets\\Particles\\" + p_filename + ".particle";

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
		//Name
		doc[i]["NAME"]			>> p_particleEmitter->m_particleSystems[i].m_name;

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
