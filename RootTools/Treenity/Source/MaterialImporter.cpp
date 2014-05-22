#include <RootTools/Treenity/Include/MaterialImporter.h>

#include <RootEngine/Include/GameSharedContext.h>
#include <RootEngine/Render/Include/Renderer.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>
extern RootEngine::GameSharedContext g_engineContext;

MaterialImporter::MaterialImporter()
{


}

Render::Material* MaterialImporter::Import(const std::string& p_filepath)
{
	std::ifstream file(p_filepath, std::ifstream::in);
	if(!file.is_open())
	{
		return nullptr;
	}

	YAML::Parser parser(file);

	YAML::Node doc;
	parser.GetNextDocument(doc);

	Render::Material* material;
	for(size_t i = 0; i < doc.size(); i++)
	{
		if(doc[i].FindValue("Name"))
		{
			std::string name;
			doc[i]["Name"] >> name; 

			material = g_engineContext.m_renderer->CreateMaterial(name);
		}
		else if(doc[i].FindValue("Effect"))
		{
			std::string effect;
			doc[i]["Effect"] >> effect;

			material->m_effect = g_engineContext.m_resourceManager->LoadEffect(effect);
		}
		else if(doc[i].FindValue("TileFactor"))
		{
			float factor;
			doc[i]["TileFactor"] >> factor;
			material->m_tileFactor = factor;
		}
		else if(doc[i].FindValue("Textures"))
		{
			const YAML::Node& textures = doc[i]["Textures"];
			for(size_t j = 0; j < textures.size(); j++)
			{
				int sem;
				textures[j]["Sem"] >> sem;

				std::string value;
				textures[j]["Texture"] >> value;

				Render::TextureInterface* texture = g_engineContext.m_resourceManager->LoadTexture(value, Render::TextureType::TEXTURE_2D);

				material->m_textures.insert(std::pair<Render::TextureSemantic::TextureSemantic, Render::TextureInterface*>((Render::TextureSemantic::TextureSemantic)sem, texture));
			}
		}
	}

	return material;
}