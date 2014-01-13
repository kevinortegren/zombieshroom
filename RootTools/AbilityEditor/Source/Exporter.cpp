#include "Exporter.h"
#include "Components.h"
#include <iostream>
#include <fstream>
#include <sstream>


Exporter::Exporter::Exporter()
{

}

Exporter::Exporter::~Exporter()
{

}

void Exporter::Exporter::Export(const std::string& p_filepath)
{
	YAML::Emitter out;
	/*We have the classes onCreate, onCollide and onDestroy.
	Each of these will hold the entitys and components used be them.
	So we export them in the format:
	-Class
		--Entitys
			---Components
	-Class
		--Entitys
			---Components
	*/
}
