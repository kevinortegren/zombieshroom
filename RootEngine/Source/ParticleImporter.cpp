#include <RootEngine/Include/ParticleImporter.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>
namespace RootEngine
{

	ParticleImporter::ParticleImporter( RootEngine::GameSharedContext* p_context)
	{
		m_context = p_context;
	}

	ParticleImporter::~ParticleImporter()
	{

	}

	std::vector<ParticleSystemStruct*>* ParticleImporter::LoadParticleSystem( const std::string p_fileName )
	{
		std::vector<ParticleSystemStruct*>* outStruct = new std::vector<ParticleSystemStruct*>();

		std::ifstream file(p_fileName, std::ifstream::in);
		if(!file.good())
		{
			m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::FATAL_ERROR, "Failed to open particle file: %s", p_fileName.c_str());
			return outStruct;
		}
		YAML::Parser parser(file);

		YAML::Node doc;
		parser.GetNextDocument(doc);

		for (unsigned i = 0; i < doc.size(); i++)
		{
			outStruct->push_back(new ParticleSystemStruct());
		}

		int materialIndex = 0;
		for (unsigned int i = 0; i < doc.size(); i++)
		{
			//Resources
			std::string effectName;
			std::string textureName;
			doc[i]["EFFECT"]		>> effectName;
			doc[i]["TEXTURE"]		>> textureName;
			
			outStruct->at(i)->m_material					= m_context->m_renderer->CreateMaterial(p_fileName + std::to_string(materialIndex++));
			outStruct->at(i)->m_material->m_textures[Render::TextureSemantic::DIFFUSE]	= m_context->m_resourceManager->LoadTexture(textureName, Render::TextureType::TEXTURE_2D);
			outStruct->at(i)->m_material->m_effect		= m_context->m_resourceManager->LoadEffect(effectName);

			//Position
			doc[i]["POSITION"][0]	>> outStruct->at(i)->m_position.x;
			doc[i]["POSITION"][1]	>> outStruct->at(i)->m_position.y;
			doc[i]["POSITION"][2]	>> outStruct->at(i)->m_position.z;
			//Life time					
			doc[i]["LIFETIMEMIN"]	>> outStruct->at(i)->m_lifeTimeMin;
			doc[i]["LIFETIMEMAX"]	>> outStruct->at(i)->m_lifeTimeMax;
			//Speed						
			doc[i]["SPEEDMIN"]		>> outStruct->at(i)->m_speedMin;
			doc[i]["SPEEDMAX"]		>> outStruct->at(i)->m_speedMax;
			//Size						
			doc[i]["SIZEMIN"][0]	>> outStruct->at(i)->m_sizeMin.x;
			doc[i]["SIZEMIN"][1]	>> outStruct->at(i)->m_sizeMin.y;
			doc[i]["SIZEMAX"][0]	>> outStruct->at(i)->m_sizeMax.x;
			doc[i]["SIZEMAX"][1]	>> outStruct->at(i)->m_sizeMax.y;
			doc[i]["SIZEEND"][0]	>> outStruct->at(i)->m_sizeEnd.x;
			doc[i]["SIZEEND"][1]	>> outStruct->at(i)->m_sizeEnd.y;
			//Color						
			doc[i]["COLOR"][0]		>> outStruct->at(i)->m_color.r;
			doc[i]["COLOR"][1]		>> outStruct->at(i)->m_color.g;
			doc[i]["COLOR"][2]		>> outStruct->at(i)->m_color.b;
			doc[i]["COLOR"][3]		>> outStruct->at(i)->m_color.a;
			doc[i]["COLOREND"][0]	>> outStruct->at(i)->m_colorEnd.r;
			doc[i]["COLOREND"][1]	>> outStruct->at(i)->m_colorEnd.g;
			doc[i]["COLOREND"][2]	>> outStruct->at(i)->m_colorEnd.b;
			doc[i]["COLOREND"][3]	>> outStruct->at(i)->m_colorEnd.a;
			//Gravity					
			doc[i]["GRAVITY"][0]	>> outStruct->at(i)->m_gravity.x;
			doc[i]["GRAVITY"][1]	>> outStruct->at(i)->m_gravity.y;
			doc[i]["GRAVITY"][2]	>> outStruct->at(i)->m_gravity.z;
			//Direction					
			doc[i]["DIRECTION"][0]	>> outStruct->at(i)->m_direction.x;
			doc[i]["DIRECTION"][1]	>> outStruct->at(i)->m_direction.y;
			doc[i]["DIRECTION"][2]	>> outStruct->at(i)->m_direction.z;
			//Spread					
			doc[i]["SPREAD"]		>> outStruct->at(i)->m_spread;
			//Spawn time				
			doc[i]["SPAWNTIME"]		>> outStruct->at(i)->m_spawnTime;
			//Name						
			doc[i]["NAME"]			>> outStruct->at(i)->m_name;

			//Set params
			outStruct->at(i)->m_params[Render::Semantic::POSITION]			= &outStruct->at(i)->m_position;
			outStruct->at(i)->m_params[Render::Semantic::LIFETIMEMIN]		= &outStruct->at(i)->m_lifeTimeMin;
			outStruct->at(i)->m_params[Render::Semantic::LIFETIMEMAX]		= &outStruct->at(i)->m_lifeTimeMax;
			outStruct->at(i)->m_params[Render::Semantic::SPEEDMIN]			= &outStruct->at(i)->m_speedMin;
			outStruct->at(i)->m_params[Render::Semantic::SPEEDMAX]			= &outStruct->at(i)->m_speedMax;
			outStruct->at(i)->m_params[Render::Semantic::SIZEMIN]			= &outStruct->at(i)->m_sizeMin;
			outStruct->at(i)->m_params[Render::Semantic::SIZEMAX]			= &outStruct->at(i)->m_sizeMax;
			outStruct->at(i)->m_params[Render::Semantic::SIZEEND]			= &outStruct->at(i)->m_sizeEnd;
			outStruct->at(i)->m_params[Render::Semantic::COLOR]				= &outStruct->at(i)->m_color;
			outStruct->at(i)->m_params[Render::Semantic::COLOREND]			= &outStruct->at(i)->m_colorEnd;
			outStruct->at(i)->m_params[Render::Semantic::GRAVITY]			= &outStruct->at(i)->m_gravity;
			outStruct->at(i)->m_params[Render::Semantic::DIRECTION]			= &outStruct->at(i)->m_direction;
			outStruct->at(i)->m_params[Render::Semantic::SPREAD]			= &outStruct->at(i)->m_spread;
			outStruct->at(i)->m_params[Render::Semantic::SPAWNTIME]			= &outStruct->at(i)->m_spawnTime;
		}
		return outStruct;
	}

}
