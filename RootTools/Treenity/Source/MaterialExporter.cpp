#include <RootTools/Treenity/Include/MaterialExporter.h>

#include <RootEngine/Include/GameSharedContext.h>
#include <RootEngine/Render/Include/Renderer.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>
extern RootEngine::GameSharedContext g_engineContext;

MaterialExporter::MaterialExporter()
{

}

void MaterialExporter::Export(Render::Material* p_material, const std::string& p_filepath)
{
	YAML::Emitter out;
	out << YAML::BeginSeq;

	out << YAML::BeginMap << YAML::Key << "Name" << YAML::Value << g_engineContext.m_renderer->GetStringFromMaterial(p_material) << YAML::EndMap;
	
	if(p_material->m_effect != nullptr)
	out << YAML::BeginMap << YAML::Key << "Effect" << YAML::Value << g_engineContext.m_resourceManager->ResolveStringFromEffect(p_material->m_effect) << YAML::EndMap;
	
	if(p_material->m_tileFactor != 0)
	out << YAML::BeginMap << YAML::Key << "TileFactor" << YAML::Value << p_material->m_tileFactor << YAML::EndMap;

	if(p_material->m_textures.size() > 0)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Textures";
		out << YAML::Value << YAML::BeginSeq;
		for(auto pair : p_material->m_textures)
		{
			if(pair.second != nullptr)
			{
				out << YAML::BeginMap;
				out << YAML::Key << "Sem" << YAML::Value << pair.first;
				out << YAML::Key << "Texture" << YAML::Value << g_engineContext.m_resourceManager->ResolveStringFromTexture(pair.second);
				out << YAML::EndMap;
			}

		}
		out << YAML::EndSeq;
		out << YAML::EndMap;
	}

	out << YAML::EndSeq;

	std::ofstream file;
	file.open(p_filepath);
	file << out.c_str();
}