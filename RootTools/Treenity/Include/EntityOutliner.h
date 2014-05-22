#pragma once

#include <set>
#include <QtWidgets/qtreewidget.h>
#include <Utility/ECS/Include/Entity.h>
#include <RootTools/Treenity/Include/EngineInterface.h>
#include <RootTools/Treenity/Include/EditorInterface.h>

class EntityOutlinerItem : public QTreeWidgetItem
{
public:
	EntityOutlinerItem(QTreeWidget* p_parent, ECS::Entity* p_entity, const QString& p_name);

	void EntityRenamed(const QString& p_name);
	void TagAdded(const std::string& p_tag);
	void TagRemoved(const std::string& p_tag);
	void EntityAddedToGroup(const std::string& p_group);
	void EntityRemovedFromGroup(const std::string& p_group);

	ECS::Entity* GetEntity();

private:
	ECS::Entity* m_entity;
	QString m_name;
	std::set<QString> m_tags;
	std::set<QString> m_groups;

	void UpdateLabel();
};

class EntityOutliner : public QTreeWidget
{
	Q_OBJECT

public:
	EntityOutliner(QWidget* p_parent);

	void SetEngineInterface(EngineInterface* p_engineInterface);
	void SetEditorInterface(EditorInterface* p_editorInterface);

	void EntityCreated(ECS::Entity* p_entity, const QString& p_name);
	void EntityRemoved(ECS::Entity* p_entity);

	void TagAdded(ECS::Entity* p_entity, const std::string& p_tag);
	void TagRemoved(ECS::Entity* p_entity, const std::string& p_tag);
	void EntityAddedToGroup(ECS::Entity* p_entity, const std::string& p_group);
	void EntityRemovedFromGroup(ECS::Entity* p_entity, const std::string& p_group);

	void EntityRenamed(ECS::Entity* p_entity, const QString& p_name);

	ECS::Entity* GetSelectedEntity();

	void SetCurrentItems(const std::set<ECS::Entity*> p_selected);

private:
	EntityOutlinerItem* FindItemWithEntity(ECS::Entity* p_entity);
	EngineInterface* m_engineInterface;
	EditorInterface* m_editorInterface;

private slots:
	void TargetEntity(QTreeWidgetItem* item, int column);
	void SelectionChanged();
};