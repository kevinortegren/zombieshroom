#pragma once

#include <RootTools/Treenity/Include/Components/AbstractComponentView.h>
#include <RootTools/Treenity/GeneratedFiles/ui_ScriptComponent.h>


class ScriptView : public AbstractComponentView
{
	Q_OBJECT
public:
	ScriptView(QWidget* p_parent = 0);

	const QString& GetComponentName() const;
	void DisplayEntity(ECS::Entity* p_entity);
private:
	QString m_name;
	Ui::Script ui;
private slots:
	void NameChanged();
	void BrowseScript();
};