#pragma once

#include <QtWidgets/qtreewidget.h>
#include <Utility/ECS/Include/Entity.h>

class EntityOutlinerItem : public QTreeWidgetItem
{
public:
	EntityOutlinerItem(QTreeWidget* p_parent, ECS::Entity* p_entity, const QString& p_name);

	ECS::Entity* GetEntity();
private:
	ECS::Entity* m_entity;
};

class EntityOutliner : public QTreeWidget
{
public:
	EntityOutliner(QWidget* p_parent);

	void EntityCreated(ECS::Entity* p_entity, const QString& p_name);
	void EntityDestroyed(ECS::Entity* p_entity);
	void EntityRenamed(ECS::Entity* p_entity, const QString& p_name);
	ECS::Entity* GetSelectedEntity();
private:
	EntityOutlinerItem* FindItemWithEntity(ECS::Entity* p_entity);
};