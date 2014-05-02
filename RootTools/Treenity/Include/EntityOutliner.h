#pragma once

#include <QtWidgets/qtreewidget.h>
#include <Utility/ECS/Include/Entity.h>

class EntityOutlinerItem
{
public:
	EntityOutlinerItem(QTreeWidget* p_parent, ECS::Entity* p_entity, QString p_name);

	ECS::Entity* GetEntity();
private:
	ECS::Entity* m_entity;
};

class EntityOutliner : public QTreeWidget
{
public:
	EntityOutliner(QWidget *parent);
};