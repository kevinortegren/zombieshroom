#include <Utility\ECS\Include\EntityImporter.h>
#include <iostream>
#include <fstream>
#include <yaml-cpp\yaml.h>

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
	
		for(unsigned int i = 0; i < doc.size(); i++) {	

			// TODO: Create entities with the following ids.
			const YAML::Node& entities = doc[i]["Entities"];
			for(unsigned int j = 0; j < entities.size(); j++)
			{
				int id;
				entities[j] >> id;
			}

			const YAML::Node& comp1s = doc[i]["Component0"];
			for(unsigned int j = 0; j < comp1s.size(); j++)
			{
				int id;
				comp1s[j]["Entity"] >> id;
				std::cout << id;
			}
		}
	}
	catch(YAML::ParserException& e) {
			std::cout << e.what() << "\n";
	}
}