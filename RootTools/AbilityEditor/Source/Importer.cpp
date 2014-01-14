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

	void Importer::Import( std::string &p_filename, OnCreate* p_onCreate, OnCollide* p_onCollide, OnDestroy* p_onDestroy )
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
			const YAML::Node& onCreate = doc[0]["onCreate"];
			for(unsigned int i = 0; i < onCreate.size() ; i++)
			{

			}
			
		}
		catch(YAML::ParserException& e)
		{
			std::cout << e.what() << "\n";
		}
	}

	AbilityEntity::Entity* Importer::ImportEntity( YAML::Emitter& p_emitter )
	{
		return nullptr;
	}

}