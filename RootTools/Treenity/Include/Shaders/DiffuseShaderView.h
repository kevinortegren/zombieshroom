#pragma once

#include <RootTools/Treenity/Include/Shaders/AbstractShaderView.h>
#include <RootTools/Treenity/GeneratedFiles/ui_DiffuseShaderView.h>

class DiffuseShaderView : public AbstractShaderView
{
	Q_OBJECT
	public:
		DiffuseShaderView(QWidget* p_parent = 0);

		void DisplayMaterial(Render::Material* p_material);

	private:
		Ui::diffuseShaderView ui;
};