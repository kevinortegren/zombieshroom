#pragma once

#include <RootTools/Treenity/Include/Shaders/AbstractShaderView.h>
#include <RootTools/Treenity/GeneratedFiles/ui_DiffuseNormalTransView.h>
#include <vector>

class DiffuseNormalTransShaderView : public AbstractShaderView
{
	Q_OBJECT
	public:
		DiffuseNormalTransShaderView(QWidget* p_parent = 0);

		void SetEngineInterface(EngineInterface* p_engineInterface);
		void Display();

	private:
		Ui::diffuseNormalTransView ui;
		std::vector<TextureDrop*> m_textureDropList;

	private slots:
		void TextureChanged(Render::TextureInterface* p_texture, Render::TextureSemantic::TextureSemantic p_sem);
};