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

	const QString& GetComponentName() const;
	void DisplayEntity(ECS::Entity* p_entity);
private:
	
	QPixmap GetPixmap(Render::TextureInterface* p_texture);

	void DisplayDiffuse(Render::Material* p_material);
	void DisplayDiffuseNormal(Render::Material* p_material);

	QString m_name;
	Ui::Renderable ui;

	ECS::Entity* m_currentEntity;
	Render::Material* m_currentMaterial;

	std::vector<AbstractShaderView*> m_shaderViews;

private slots:
	void NewMaterial();
	void MaterialNameChanged();
	void ModelNameChanged();
	void ShaderChanged(int index);

	void DiffuseTextureDropped(const QString& p_path);
	void SpecularTextureDropped(const QString& p_path);
	void GlowTextureDropped(const QString& p_path);
	void NormalTextureDropped(const QString& p_path);

};