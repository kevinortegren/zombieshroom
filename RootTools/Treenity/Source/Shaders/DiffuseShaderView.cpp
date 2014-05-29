#include <RootTools/Treenity/Include/Shaders/DiffuseShaderView.h>

extern RootEngine::GameSharedContext g_engineContext;

DiffuseShaderView::DiffuseShaderView(QWidget* p_parent)
	: AbstractShaderView(p_parent)
{
	ui.setupUi(this);
	ui.textureDrop_diffuse->SetName("Diffuse");
	ui.textureDrop_specular->SetName("Specular");
	ui.textureDrop_glow->SetName("Glow");

	connect(ui.textureDrop_diffuse, SIGNAL(textureChanged(Render::TextureInterface*)), this, SLOT(DiffuseTextureChanged(Render::TextureInterface*)));
	connect(ui.textureDrop_specular, SIGNAL(textureChanged(Render::TextureInterface*)), this, SLOT(SpecularTextureChanged(Render::TextureInterface*)));
	connect(ui.textureDrop_glow, SIGNAL(textureChanged(Render::TextureInterface*)), this, SLOT(GlowTextureChanged(Render::TextureInterface*)));

}

void DiffuseShaderView::SetEngineInterface(EngineInterface* p_engineInterface)
{
	ui.textureDrop_diffuse->SetEngineInterface(p_engineInterface);
	ui.textureDrop_specular->SetEngineInterface(p_engineInterface);
	ui.textureDrop_glow->SetEngineInterface(p_engineInterface);

	AbstractShaderView::SetEngineInterface(p_engineInterface);
}

void DiffuseShaderView::Display()
{
	auto itr = m_material->m_textures.find(Render::TextureSemantic::DIFFUSE);

	if(itr != m_material->m_textures.end())
	{
		if(itr->second == nullptr)
		{
			g_engineContext.m_logger->LogText(LogTag::GUI, LogLevel::NON_FATAL_ERROR, "DiffuseShaderView received nullptr diffuse texture");
		}
		else
		{
			ui.textureDrop_diffuse->SetTexture(itr->second);
        }
	}
	else
	{
		ui.textureDrop_diffuse->Clear();
	}

	itr = m_material->m_textures.find(Render::TextureSemantic::SPECULAR);
	if(itr != m_material->m_textures.end())
	{
		if(itr->second == nullptr)
		{
			g_engineContext.m_logger->LogText(LogTag::GUI, LogLevel::NON_FATAL_ERROR, "DiffuseShaderView received nullptr specular texture");
		}
		else
		{
			ui.textureDrop_specular->SetTexture(itr->second);
		}
	}
	else
	{
		ui.textureDrop_specular->Clear();
	}

	itr = m_material->m_textures.find(Render::TextureSemantic::GLOW);
	if(itr != m_material->m_textures.end())
	{
		if(itr->second == nullptr)
		{
			g_engineContext.m_logger->LogText(LogTag::GUI, LogLevel::NON_FATAL_ERROR, "DiffuseShaderView received nullptr glow texture");
		}
		else
		{
			ui.textureDrop_glow->SetTexture(itr->second);
		}
	}
	else
	{
		ui.textureDrop_glow->Clear();
	}
}

void DiffuseShaderView::DiffuseTextureChanged(Render::TextureInterface* p_texture)
{
	m_material->m_textures[Render::TextureSemantic::DIFFUSE] = p_texture;
	Save();
}

void DiffuseShaderView::SpecularTextureChanged(Render::TextureInterface* p_texture)
{
	m_material->m_textures[Render::TextureSemantic::SPECULAR] = p_texture;
	Save();
}

void DiffuseShaderView::GlowTextureChanged(Render::TextureInterface* p_texture)
{
	m_material->m_textures[Render::TextureSemantic::GLOW] = p_texture;
	Save();
}
