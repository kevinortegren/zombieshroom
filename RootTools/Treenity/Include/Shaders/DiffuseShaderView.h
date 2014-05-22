#pragma once

#include <RootTools/Treenity/Include/Shaders/AbstractShaderView.h>
#include <RootTools/Treenity/GeneratedFiles/ui_DiffuseShaderView.h>

class DiffuseShaderView : public AbstractShaderView
{
	Q_OBJECT
	public:
		DiffuseShaderView(QWidget* p_parent = 0);

		void SetEngineInterface(EngineInterface* p_engineInterface);
		void Display();

	private:
		Ui::diffuseShaderView ui;

	private slots:
		void DiffuseTextureChanged(Render::TextureInterface* p_texture);
};