#include "Importer.h"
#include <iostream>
#include <fstream>
#include <sstream>
namespace AbilityEditorNameSpace
{
	Importer::Importer()
	{

	}

	Importer::~Importer()
	{

	}

	void Importer::Import(const std::string &p_filename, OnCreate* p_onCreate, OnCollide* p_onCollide, OnDestroy* p_onDestroy )
	{
		try
		{
			std::ifstream file(p_filename, std::ifstream::in);
			if(!file.good())
			{
				//TODO, add a real error message
				throw("File is bad, get another file");
			}
			YAML::Parser parser(file);

			YAML::Node doc;
			parser.GetNextDocument(doc);

			//OnCreate
			const YAML::Node& onCreate = doc[0]["OnCreate"];
			for(unsigned int i = 0; i < onCreate.size() ; i++)
			{
				std::string entityName;
				onCreate[i]["EntityName"] >> entityName;
				AbilityEntity::Entity* entity = new AbilityEntity::Entity(entityName);
				const YAML::Node& comps = onCreate[0]["Components"];
				for(unsigned int j = 0; j < comps.size(); j++)
				{
					entity->AddComponent(ImportComponents(comps, j));
				}
			}
			
		}
		catch(YAML::ParserException& e)
		{
			std::cout << e.what() << "\n";
		}
	}

	AbilityComponents::MainComponent* Importer::ImportComponents(const YAML::Node& p_node, unsigned int p_index)
	{
		Todo you are here!
		//Todo, to make it slightly more simple, aka easier to put data into the components, fix their constructors to take arguments and that cool stuff

		int type;
		p_node[p_index]["Type"] >> type;
		return nullptr;
	}

}