#include <RootTools/Treenity/Include/EntityOutliner.h>

EntityOutlinerItem::EntityOutlinerItem(QTreeWidget* p_parent, ECS::Entity* p_entity, QString p_name)
{

}

EntityOutliner::EntityOutliner(QWidget *parent)
	: QTreeWidget(parent)
{

}

void EntityOutliner::EntityCreated(ECS::Entity* p_entity, const QString& p_name)
{

}

void EntityOutliner::EntityDestroyed(ECS::Entity* p_entity)
{

}

void EntityOutliner::EntityRenamed(ECS::Entity* p_entity, const QString& p_name)
{

}

ECS::Entity* EntityOutliner::GetSelectedEntity()
{
	return nullptr;
}