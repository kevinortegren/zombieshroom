#include <RootEngine/Include/YamlParser.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <fstream>
#include <iostream>

extern Logging g_logger;

namespace RootEngine
{
	void YamlParser::Load(const std::string& filepath)
	{
		try
		{
			std::ifstream file(filepath, std::ifstream::in);
			if(!file.good())
			{
				g_logger.LogText(LogTag::GENERAL, LogLevel::FATAL_ERROR, "Failed to load yaml file: %s", filepath.c_str());
			}
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