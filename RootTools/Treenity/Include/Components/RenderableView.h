#pragma once

#include <RootTools/Treenity/Include/Components/AbstractComponentView.h>
#include <RootTools/Treenity/GeneratedFiles/ui_RenderableComponent.h>


class RenderableView : public AbstractComponentView
{
	Q_OBJECT
public:
	RenderableView(QWidget* p_parent = 0);

	const QString& GetComponentName() const;
	void DisplayEntity(ECS::Entity* p_entity);
private:
	QString m_name;
	Ui::Renderable ui;
private slots:
	void MaterialNameChanged();
	void ModelNameChanged();
};