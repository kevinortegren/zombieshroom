#include <Utility\ECS\Include\EntityImporter.h>
#include <iostream>
#include <fstream>
#include <yaml-cpp\yaml.h>
#include <Utility\ECS\Include\World.h>
#include <map>

void ECS::EntityImporter::SetImporter(COMPIMPORT p_importer)
{
	m_importer = p_importer;
}

void ECS::EntityImporter::Import(const std::string& p_filename)
{
	try
	{
		std::ifstream file(p_filename, std::ifstream::in);
		if(!file.good())
		{
			//g_logger.LogText(LogTag::GENERAL, LogLevel::FATAL_ERROR, "Failed to load yaml file: %s", filepath.c_str());
		}
		YAML::Parser parser(file);

		YAML::Node doc;
		parser.GetNextDocument(doc);
	
		std::map<int, ECS::Entity*> entitiesMap;

		const YAML::Node& entities = doc[0]["Entities"];
		for(unsigned int j = 0; j < entities.size(); j++)
		{
			int id;
			entities[j] >> id;

			entitiesMap[id] = m_world->GetEntityManager()->CreateEntity();
		}

		const YAML::Node& comps = doc[1]["Components"];
		for(unsigned int j = 0; j < comps.size(); j++)
		{
			int type;
			comps[j]["Type"] >> type;

			const YAML::Node& data = comps[j]["Data"];
			for(unsigned int k = 0; k < data.size(); k++)
			{
				int id;
				data[k]["Entity"] >> id;
				std::cout << id;

				m_importer(m_world, type, entitiesMap[id], data[k]);
			}
		}
	}
	
	catch(YAML::ParserException& e) {
			std::cout << e.what() << "\n";
	}
}