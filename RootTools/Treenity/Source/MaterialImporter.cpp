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

	YAML::Parser parser(file);

	YAML::Node doc;
	parser.GetNextDocument(doc);

	Render::Material* material;
	if(doc[0].FindValue("Name"))
	{
		std::string name;
		doc[0]["Name"] >> name; 

		material = g_engineContext.m_renderer->CreateMaterial(name);

		if(doc[0].FindValue("Effect"))
		{
			std::string effect;
			doc[0]["Effect"] >> effect;

			material->m_effect = g_engineContext.m_resourceManager->LoadEffect(effect);
		}

		if(doc[0].FindValue("TileFactor"))
		{
			float factor;
			doc[0]["TileFactor"] >> factor;
			material->m_tileFactor = factor;
		}

		if(doc[0].FindValue("Textures"))
		{
			const YAML::Node& textures = doc[0]["Textures"];
			for(size_t i = 0; i < textures.size(); i++)
			{
				int key;
				textures[i]["Key"] >> key;

				std::string value;
				textures[i]["Texture"] >> value;

				Render::TextureInterface* texture = g_engineContext.m_resourceManager->LoadTexture(value, Render::TextureType::TEXTURE_2D);

				material->m_textures.insert(std::pair<Render::TextureSemantic::TextureSemantic, Render::TextureInterface*>((Render::TextureSemantic::TextureSemantic)key, texture));
			}
		}
	}

	return material;
}