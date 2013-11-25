#include <RootEngine/Include/YamlParser.h>
#include <fstream>
#include <iostream>

namespace RootEngine
{
	void YamlParser::Load(const std::string& filepath)
	{
		try
		{
			std::ifstream file(filepath, std::ifstream::in);
			YAML::Parser parser(file);

			YAML::Node doc;
			parser.GetNextDocument(doc);
	
			for(unsigned i=0; i < doc.size(); i++) {	
				Process(doc[i]);
			}
		}
		catch(YAML::ParserException& e) {
			 std::cout << e.what() << "\n";
		}

		return;
	}
}