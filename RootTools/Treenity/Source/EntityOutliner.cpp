#include <RootTools/Treenity/Include/EntityOutliner.h>

EntityOutlinerItem::EntityOutlinerItem(QTreeWidget* p_parent, ECS::Entity* p_entity, const QString& p_name)
	: QTreeWidgetItem(p_parent, 0), m_entity(p_entity)
{
	setText(0, p_name);
}

ECS::Entity* EntityOutlinerItem::GetEntity()
{
	return m_entity;
}


EntityOutliner::EntityOutliner(QWidget* p_parent)
	: QTreeWidget(p_parent)
{

}

void EntityOutliner::EntityCreated(ECS::Entity* p_entity, const QString& p_name)
{
	addTopLevelItem(new EntityOutlinerItem(this, p_entity, p_name));
}

void EntityOutliner::EntityDestroyed(ECS::Entity* p_entity)
{
	for (int i = 0; i < topLevelItemCount();)
	{
		EntityOutlinerItem* item = (EntityOutlinerItem*) topLevelItem(i);
		if (item->GetEntity() == p_entity)
		{
			takeTopLevelItem(i);
		}
		else
		{
			++i;
		}
	}
}

void EntityOutliner::EntityRenamed(ECS::Entity* p_entity, const QString& p_name)
{
	EntityOutlinerItem* item = FindItemWithEntity(p_entity);
	item->setText(0, p_name);
}

ECS::Entity* EntityOutliner::GetSelectedEntity()
{
	ECS::Entity* entity = nullptr;
	if (topLevelItemCount() > 0)
	{
		EntityOutlinerItem* item = (EntityOutlinerItem*) currentItem();
		entity = item->GetEntity();
	}
	
	return entity;
}

EntityOutlinerItem* EntityOutliner::FindItemWithEntity(ECS::Entity* p_entity)
{
	for (int i = 0; i < topLevelItemCount(); ++i)
	{
		EntityOutlinerItem* item = (EntityOutlinerItem*) topLevelItem(i);
		if (item->GetEntity() == p_entity)
		{
			return item;
		}
	}

	return nullptr;
}