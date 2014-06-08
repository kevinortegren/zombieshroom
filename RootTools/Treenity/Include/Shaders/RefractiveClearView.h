#pragma once

#include <RootTools/Treenity/Include/Shaders/AbstractShaderView.h>
#include <RootTools/Treenity/GeneratedFiles/ui_RefractiveClearView.h>
#include <vector>

class RefractiveClearShaderView : public AbstractShaderView
{
	Q_OBJECT
	public:
		RefractiveClearShaderView(QWidget* p_parent = 0);

		void SetEngineInterface(EngineInterface* p_engineInterface);
		void Display();

	private:
		Ui::refractiveClearView ui;
};