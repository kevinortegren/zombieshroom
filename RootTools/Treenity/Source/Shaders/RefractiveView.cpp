#include <RootTools/Treenity/Include/Shaders/RefractiveView.h>

extern RootEngine::GameSharedContext g_engineContext;

RefractiveShaderView::RefractiveShaderView(QWidget* p_parent)
	: AbstractShaderView(p_parent)
{
	ui.setupUi(this);
	//Init alltexture drops with Texture semantic and a name
	ui.textureDrop_diffuse->Init(Render::TextureSemantic::DIFFUSE, "Diffuse");

	//Push them to the texture drop list, if they are not in this list they will not work
	m_textureDropList.push_back(ui.textureDrop_diffuse);
	
	//Connect all texture drops to the texture changed slot
	for(auto texDrop : m_textureDropList)
		connect(texDrop, SIGNAL(textureChanged(Render::TextureInterface*, Render::TextureSemantic::TextureSemantic)), this, SLOT(TextureChanged(Render::TextureInterface*, Render::TextureSemantic::TextureSemantic)));

}

void RefractiveShaderView::SetEngineInterface(EngineInterface* p_engineInterface)
{
	//Set Engine interface for all texture drops
	for(auto tex : m_textureDropList)
	{
		tex->SetEngineInterface(p_engineInterface);
	}

	AbstractShaderView::SetEngineInterface(p_engineInterface);
}

void RefractiveShaderView::Display()
{
	//This will loop through all texture drops and display an image if there is one
	for(auto tex : m_textureDropList)
	{
		auto itr = m_material->m_textures.find(tex->GetTextureSemantic());
		if(itr != m_material->m_textures.end())
		{
			if(itr->second == nullptr)
			{
				g_engineContext.m_logger->LogText(LogTag::GUI, LogLevel::NON_FATAL_ERROR, "Received nullptr %s texture", tex->GetName().toStdString().c_str());
			}
			else
			{
				tex->SetTexture(itr->second);
			}
		}
		else
		{
			tex->Clear();
		}
	}
}

void RefractiveShaderView::TextureChanged( Render::TextureInterface* p_texture, Render::TextureSemantic::TextureSemantic p_sem )
{
	m_material->m_textures[p_sem] = p_texture;
	Save();
}
