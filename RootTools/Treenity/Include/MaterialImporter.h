#pragma once

#include <RootEngine/Render/Include/Material.h>

class MaterialImporter
{
public:
	MaterialImporter();

	Render::Material* Import(const std::string& p_filepath);

};