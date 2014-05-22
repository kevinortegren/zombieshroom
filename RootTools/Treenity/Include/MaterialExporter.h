#pragma once

#include <yaml-cpp/yaml.h>
#include <RootEngine/Render/Include/Material.h>

class MaterialExporter
{
public:
	MaterialExporter();

	void Export(Render::Material* p_material, const std::string& p_filepath);
};