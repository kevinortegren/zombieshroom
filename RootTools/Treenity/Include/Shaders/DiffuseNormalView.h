#pragma once

#include <RootTools/Treenity/Include/Shaders/AbstractShaderView.h>
#include <RootTools/Treenity/GeneratedFiles/ui_DiffuseNormalView.h>
#include <vector>

class DiffuseNormalShaderView : public AbstractShaderView
{
	Q_OBJECT
	public:
		DiffuseNormalShaderView(QWidget* p_parent = 0);

		void SetEngineInterface(EngineInterface* p_engineInterface);
		void Display();

	private:
		Ui::diffuseNormalView ui;
		std::vector<TextureDrop*> m_textureDropList;

	private slots:
		void TextureChanged(Render::TextureInterface* p_texture, Render::TextureSemantic::TextureSemantic p_sem);
};