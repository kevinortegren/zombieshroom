#pragma once

#include <QWidget>
#include <RootTools/Treenity/Include/EngineInterface.h>

class AbstractShaderView : public QWidget
{
	Q_OBJECT
public:
	AbstractShaderView(QWidget* p_parent = 0);
	virtual ~AbstractShaderView() {}

	virtual void SetMaterialName(const QString& p_name);
	virtual void SetMaterial(Render::Material* p_material);
	virtual void Save();
	virtual void SetEngineInterface(EngineInterface* p_engineInterface);
	virtual void Display() = 0;

protected:
	QString m_name;
	Render::Material* m_material;
	EngineInterface* m_engineInterface;
};