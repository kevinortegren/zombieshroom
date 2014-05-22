#include <RootTools/Treenity/Include/Shaders/DiffuseShaderView.h>

DiffuseShaderView::DiffuseShaderView(QWidget* p_parent)
	: AbstractShaderView(p_parent)
{
	ui.setupUi(this);
	ui.diffuse_textureDrop->SetName("Diffuse");

	connect(ui.diffuse_textureDrop, SIGNAL(textureChanged(Render::TextureInterface*)), this, SLOT(DiffuseTextureChanged(Render::TextureInterface*)));
}

void DiffuseShaderView::SetEngineInterface(EngineInterface* p_engineInterface)
{
	ui.diffuse_textureDrop->SetEngineInterface(p_engineInterface);

	AbstractShaderView::SetEngineInterface(p_engineInterface);
}

void DiffuseShaderView::Display()
{
	auto itr = m_material->m_textures.find(Render::TextureSemantic::DIFFUSE);
	if(itr != m_material->m_textures.end())
	{
		ui.diffuse_textureDrop->SetTexture(itr->second);
	}
}

void DiffuseShaderView::DiffuseTextureChanged(Render::TextureInterface* p_texture)
{
	m_material->m_textures[Render::TextureSemantic::DIFFUSE] = p_texture;
	Save();
}