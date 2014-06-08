#include <RootTools/Treenity/Include/Shaders/BlendShaderView.h>

extern RootEngine::GameSharedContext g_engineContext;

BlendShaderView::BlendShaderView(QWidget* p_parent)
	: AbstractShaderView(p_parent)
{
	ui.setupUi(this);

	//////////////////////////This is the only part that needs changing if creating a new shader view////////
	//Init alltexture drops with Texture semantic and a name
	ui.textureDrop_specular->Init(Render::TextureSemantic::SPECULAR, "Specular");
	ui.textureDrop_glow->Init(Render::TextureSemantic::GLOW, "Glow");
	ui.textureDrop_blendmap->Init(Render::TextureSemantic::TEXTUREMAP, "Blend map");
	ui.textureDrop_diffuseR->Init(Render::TextureSemantic::TEXTURE_R, "DiffuseR");
	ui.textureDrop_diffuseG->Init(Render::TextureSemantic::TEXTURE_G, "DiffuseG");
	ui.textureDrop_diffuseB->Init(Render::TextureSemantic::TEXTURE_B, "DiffuseB");

	//Push them to the texture drop list, if they are not in this list they will not work
	m_textureDropList.push_back(ui.textureDrop_specular);
	m_textureDropList.push_back(ui.textureDrop_glow);
	m_textureDropList.push_back(ui.textureDrop_blendmap);
	m_textureDropList.push_back(ui.textureDrop_diffuseR);
	m_textureDropList.push_back(ui.textureDrop_diffuseG);
	m_textureDropList.push_back(ui.textureDrop_diffuseB);
	
	//Connect other things
	connect(ui.spinBox_tileFactor, SIGNAL(valueChanged(int)), this, SLOT(TileFactorChanged(int)));
	//////////////////////////This is the only part that needs changing if creating a new shader view////////

	//Connect all texture drops to the texture changed slot
	for(auto texDrop : m_textureDropList)
		connect(texDrop, SIGNAL(textureChanged(Render::TextureInterface*, Render::TextureSemantic::TextureSemantic)), this, SLOT(TextureChanged(Render::TextureInterface*, Render::TextureSemantic::TextureSemantic)));
}

void BlendShaderView::SetEngineInterface(EngineInterface* p_engineInterface)
{
	//Set Engine interface for all texture drops
	for(auto tex : m_textureDropList)
	{
		tex->SetEngineInterface(p_engineInterface);
	}

	AbstractShaderView::SetEngineInterface(p_engineInterface);
}

void BlendShaderView::Display()
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

	ui.spinBox_tileFactor->blockSignals(true);
	ui.horizontalSlider_tileFactor->blockSignals(true);

	ui.spinBox_tileFactor->setValue((int)m_material->m_tileFactor);
	ui.horizontalSlider_tileFactor->setValue((int)m_material->m_tileFactor);

	ui.spinBox_tileFactor->blockSignals(false);
	ui.horizontalSlider_tileFactor->blockSignals(false);
}

void BlendShaderView::TextureChanged( Render::TextureInterface* p_texture, Render::TextureSemantic::TextureSemantic p_texSem )
{
	//All texture drops trigger this when a texture is dropped on it
	p_texture->SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	p_texture->SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	m_material->m_textures[p_texSem] = p_texture;
	Save();
}

void BlendShaderView::TileFactorChanged( int p_val )
{
	m_material->m_tileFactor = (float)p_val;
	Save();
}
