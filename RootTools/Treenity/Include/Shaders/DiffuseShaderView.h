#pragma once

#include <RootTools/Treenity/Include/Shaders/AbstractShaderView.h>
#include <RootTools/Treenity/GeneratedFiles/ui_DiffuseShaderView.h>
#include <vector>
class DiffuseShaderView : public AbstractShaderView
{
	Q_OBJECT
	public:
		DiffuseShaderView(QWidget* p_parent = 0);

		void SetEngineInterface(EngineInterface* p_engineInterface);
		void Display();

	private:
		Ui::diffuseShaderView ui;
		std::vector<TextureDrop*> m_textureDropList;

	private slots:
		void TextureChanged(Render::TextureInterface* p_texture, Render::TextureSemantic::TextureSemantic p_sem);
};