#pragma once

#include <QWidget>
//#include <RootTools/Treenity/Include/EngineInterface.h>
//#include <RootTools/Treenity/Include/EditorInterface.h>
#include <RootEngine/Render/Include/Material.h>

class AbstractShaderView : public QWidget
{
	Q_OBJECT
public:
	AbstractShaderView(QWidget* p_parent = 0);
	virtual ~AbstractShaderView();

	virtual void DisplayMaterial(Render::Material* p_material) = 0;
};