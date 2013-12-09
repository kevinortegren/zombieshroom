#include <RootEngine/Include/TextureImporter.h>

namespace RootEngine
{
	TextureImporter::TextureImporter(Logging* p_logger, Render::RendererInterface* p_renderer)
	{
		m_logger	= p_logger;
		m_renderer  = p_renderer;
		m_logger->LogText(LogTag::RESOURCE, LogLevel::INIT_PRINT, "Texture importer initialized!");
	}

	TextureImporter::~TextureImporter()
	{

	}

	std::shared_ptr<Render::TextureInterface> TextureImporter::LoadTexture( const std::string p_fileName )
	{
		std::shared_ptr<Render::TextureInterface> texture = m_renderer->CreateTexture();

		if (texture->Load(p_fileName)) 
			return texture;
		else
			return nullptr;
	}

	std::shared_ptr<Render::TextureInterface> TextureImporter::LoadCubeTexture( const std::string p_fileName )
	{
		std::shared_ptr<Render::TextureInterface> texture = m_renderer->CreateTexture();

		if (texture->LoadCubeMap(p_fileName)) 
			return texture;
		else
			return nullptr;
	}
}
