#pragma once

#include <RootTools/Treenity/Include/Shaders/AbstractShaderView.h>
#include <RootTools/Treenity/GeneratedFiles/ui_BlendShaderView.h>
#include <vector>

class BlendShaderView : public AbstractShaderView
{
	Q_OBJECT
	public:
		BlendShaderView(QWidget* p_parent = 0);

		void SetEngineInterface(EngineInterface* p_engineInterface);
		void Display();

	private:
		Ui::meshBlendedShaderView ui;
		std::vector<TextureDrop*> m_textureDropList;

	private slots:
		
		void TextureChanged(Render::TextureInterface* p_texture, Render::TextureSemantic::TextureSemantic p_texSem);
		void TileFactorChanged(int p_val);
};