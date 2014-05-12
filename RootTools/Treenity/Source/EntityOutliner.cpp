#include <RootTools/Treenity/Include/EntityOutliner.h>
#include <algorithm>

EntityOutlinerItem::EntityOutlinerItem(QTreeWidget* p_parent, ECS::Entity* p_entity, const QString& p_name)
	: QTreeWidgetItem(p_parent, 0), m_entity(p_entity)
{
	m_name = p_name;
	UpdateLabel();
}

void EntityOutlinerItem::EntityRenamed(const QString& p_name)
{
	m_name = p_name;
	UpdateLabel();
}

void EntityOutlinerItem::TagAdded(const std::string& p_tag)
{
	m_tags.insert(QString(p_tag.c_str()));
	UpdateLabel();
}

void EntityOutlinerItem::TagRemoved(const std::string& p_tag)
{
	m_tags.erase(QString(p_tag.c_str()));
	UpdateLabel();
}

void EntityOutlinerItem::EntityAddedToGroup(const std::string& p_group)
{
	m_groups.insert(QString::fromStdString(p_group));
	UpdateLabel();
}

void EntityOutlinerItem::EntityRemovedFromGroup(const std::string& p_group)
{
	m_groups.erase(QString(p_group.c_str()));
	UpdateLabel();
}


ECS::Entity* EntityOutlinerItem::GetEntity()
{
	return m_entity;
}

void EntityOutlinerItem::UpdateLabel()
{
	QString label = m_name;
	
	// Add tags
	if (m_tags.size() > 0)
	{
		label += " (";
		
		for (auto it = m_tags.begin(); it != m_tags.end(); ++it)
		{
			if (it != --m_tags.end())
			{
				label += (*it) + ", ";
			}
			else
			{
				label += (*it);
			}
		}
		
		label += ") ";
	}

	// Add groups
	if (m_groups.size() > 0)
	{
		label += " [";
		
		for (auto it = m_groups.begin(); it != m_groups.end(); ++it)
		{
			if (it != --m_groups.end())
			{
				label += (*it) + ", ";
			}
			else
			{
				label += (*it);
			}
		}
		
		label += "] ";
	}

	label += " " + QString::number(m_entity->GetId());

	setText(0, label);
}


EntityOutliner::EntityOutliner(QWidget* p_parent)
	: QTreeWidget(p_parent), m_engineInterface(nullptr), m_editorInterface(nullptr)
{
	connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(TargetEntity(QTreeWidgetItem*, int)));
	connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(SelectionChanged()));
}

void EntityOutliner::SetEngineInterface(EngineInterface* p_engineInterface)
{
	m_engineInterface = p_engineInterface;
}

void EntityOutliner::SetEditorInterface(EditorInterface* p_editorInterface)
{
	m_editorInterface = p_editorInterface;
}

void EntityOutliner::EntityCreated(ECS::Entity* p_entity, const QString& p_name)
{
	addTopLevelItem(new EntityOutlinerItem(this, p_entity, p_name));
}

void EntityOutliner::EntityRemoved(ECS::Entity* p_entity)
{
	for (int i = 0; i < topLevelItemCount();)
	{
		EntityOutlinerItem* item = (EntityOutlinerItem*) topLevelItem(i);
		if (item->GetEntity() == p_entity)
		{
			delete takeTopLevelItem(i);
		}
		else
		{
			++i;
		}
	}
}

void EntityOutliner::TagAdded(ECS::Entity* p_entity, const std::string& p_tag)
{
	EntityOutlinerItem* item = FindItemWithEntity(p_entity);
	item->TagAdded(p_tag);
}

void EntityOutliner::TagRemoved(ECS::Entity* p_entity, const std::string& p_tag)
{
	EntityOutlinerItem* item = FindItemWithEntity(p_entity);
	item->TagRemoved(p_tag);
}

void EntityOutliner::EntityAddedToGroup(ECS::Entity* p_entity, const std::string& p_group)
{
	EntityOutlinerItem* item = FindItemWithEntity(p_entity);
	item->EntityAddedToGroup(p_group);
}

void EntityOutliner::EntityRemovedFromGroup(ECS::Entity* p_entity, const std::string& p_group)
{
	EntityOutlinerItem* item = FindItemWithEntity(p_entity);
	item->EntityRemovedFromGroup(p_group);
}


void EntityOutliner::EntityRenamed(ECS::Entity* p_entity, const QString& p_name)
{
	EntityOutlinerItem* item = FindItemWithEntity(p_entity);
	item->EntityRenamed(p_name);
}

ECS::Entity* EntityOutliner::GetSelectedEntity()
{
	ECS::Entity* entity = nullptr;
	EntityOutlinerItem* item = nullptr;
	if ( (item = (EntityOutlinerItem*) currentItem()) != nullptr)
	{
		entity = item->GetEntity();
	}
	
	return entity;
}

void EntityOutliner::SetCurrentItems(const std::set<ECS::Entity*> p_selected)
{
	blockSignals(true);

	for(ECS::Entity* entity : p_selected)
	{
		FindItemWithEntity(entity)->setSelected(true);
	}

	blockSignals(false);
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

void EntityOutliner::TargetEntity(QTreeWidgetItem* item, int column )
{
	ECS::Entity* entity = ((EntityOutlinerItem*)item)->GetEntity();

	m_engineInterface->TargetEntity(entity);
}

void EntityOutliner::SelectionChanged()
{
	QList<QTreeWidgetItem *> selected = selectedItems();

	std::set<ECS::Entity*> entities;
	for(auto item : selected)
	{
		entities.insert(((EntityOutlinerItem*)item)->GetEntity());
	}

	m_editorInterface->Select(entities);
}


