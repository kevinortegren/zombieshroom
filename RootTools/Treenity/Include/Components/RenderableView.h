#pragma once

#include <RootTools/Treenity/Include/Components/AbstractComponentView.h>
#include <RootTools/Treenity/GeneratedFiles/ui_RenderableComponent.h>

#include <RootTools/Treenity/Include/Shaders/DiffuseShaderView.h>
#include <vector>

class RenderableView : public AbstractComponentView
{
	Q_OBJECT
public:
	RenderableView(QWidget* p_parent = 0);

	void SetEngineInterface(EngineInterface* p_engineInterface);

	const QString& GetComponentName() const;
	void DisplayEntity(ECS::Entity* p_entity);
private:

	void Clear();

	QString m_name;
	Ui::Renderable ui;
	Render::Material* m_currentMaterial;
	AbstractShaderView* m_currentView;
	std::vector<AbstractShaderView*> m_shaderViews;
	std::map<const std::string, int> m_effectToShaderIndex;

private slots:
	void NewMaterial();
	void MaterialNameChanged();
	void ModelNameChanged();
	void ShaderChanged(int index);
};