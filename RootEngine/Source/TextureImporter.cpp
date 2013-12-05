#include <RootEngine/Include/TextureImporter.h>

namespace RootEngine
{
	
	TextureImporter::TextureImporter(Logging* p_logger, Render::RendererInterface* p_renderer)
	{
		m_logger	= p_logger;
		if(p_renderer)
			m_texture   = p_renderer->CreateTexture();
		m_logger->LogText(LogTag::RESOURCE, LogLevel::INIT_PRINT, "Texture importer initialized!");
	}

	TextureImporter::~TextureImporter()
	{
		if(m_texture)
			delete m_texture;
	}

	bool TextureImporter::LoadTexture( const std::string p_fileName )
	{
		if (!m_texture->Load(p_fileName)) 
			return false;
		else 
			return true;
	}

	Render::TextureInterface* TextureImporter::GetTexture()
	{
		return m_texture;
	}



}
