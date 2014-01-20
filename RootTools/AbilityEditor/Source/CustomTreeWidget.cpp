#include "CustomTreeWidget.h"
#include <Qt/QtCore/QMimeData>
#include <Qt/QtWidgets/QMessageBox>
CustomTreeWidget::CustomTreeWidget( QWidget* parent /*= 0*/ ) : QTreeWidget(parent)
{
	setAcceptDrops(true);
}

void CustomTreeWidget::dropEvent( QDropEvent* event )
{
	if(event->mimeData()->objectName().compare("Abilities") == 0)
	{
		QTreeWidgetItem* item = new QTreeWidgetItem;
		item->setText(0,event->mimeData()->text());
		item->setWhatsThis(0,"Entity");
		
		this->addTopLevelItem(item);
		m_onEvent->AddEntity(item->text(0));
	
	}
	else if(this->itemAt(event->pos()) != nullptr )
	{
		
		QTreeWidgetItem* item = new QTreeWidgetItem;
		item->setText(0,event->mimeData()->text());
		item->setWhatsThis(0,event->mimeData()->objectName());
		if((this->itemAt(event->pos())->whatsThis(0).compare("Entity") ==0))
		{
			if(m_onEvent->CheckForExistingComponents(this->indexOfTopLevelItem(this->itemAt(event->pos())), event->mimeData()->text()))
			{
				QMessageBox msgBox;
				msgBox.setText("An entity can only contain one of each type of component");
				msgBox.exec();
				delete item;
				return;
			}
			this->itemAt(event->pos())->addChild(item);
		}
		else
		{
			if(m_onEvent->CheckForExistingComponents(this->indexOfTopLevelItem(this->itemAt(event->pos())->parent()), event->mimeData()->text()))
			{
				QMessageBox msgBox;
				msgBox.setText("An entity can only contain one of each type of component");
				msgBox.exec();
				return;
			}
			this->itemAt(event->pos())->parent()->addChild(item);
		}
		unsigned int index = this->indexOfTopLevelItem(item->parent());
		m_onEvent->AddComponent(index, item->text(0));
	}
}

void CustomTreeWidget::dragEnterEvent( QDragEnterEvent* event )
{
	event->acceptProposedAction();
}

void CustomTreeWidget::dragMoveEvent( QDragMoveEvent *event )
{

	event->acceptProposedAction();
}

void CustomTreeWidget::SetOnEventClass(AbilityEditorNameSpace::MainOnEvent* p_onEvent )
{
	m_onEvent = p_onEvent;
}

void CustomTreeWidget::RemoveSelected( QTreeWidgetItem* p_item )
{
	if(p_item == nullptr)
	{
		m_onEvent->RemoveEntity(0);
		return;
	}
	if(p_item->whatsThis(0).compare("Entity") == 0)
	{
		m_onEvent->RemoveEntity(this->indexOfTopLevelItem(p_item));
	}
	else if (p_item->whatsThis(0).compare("Components") == 0)
	{
		m_onEvent->RemoveComponent(this->indexOfTopLevelItem(p_item->parent()),p_item->text(0));
	}
}

void CustomTreeWidget::ViewSelectedData( QTreeWidgetItem* p_item, QtTreePropertyBrowser* p_propBrows, QtVariantPropertyManager* p_propMan )
{
	if(p_item->whatsThis(0).compare("Entity") == 0)
	{
		m_onEvent->ViewEntityData(this->indexOfTopLevelItem(p_item),p_propBrows, p_propMan );
	}
	else if (p_item->whatsThis(0).compare("Components") == 0)
	{
		m_onEvent->ViewComponentData(this->indexOfTopLevelItem(p_item->parent()),p_propBrows, p_item->text(0), p_propMan);
	}
}

void CustomTreeWidget::SaveSelectedData( QTreeWidgetItem* p_item, QtTreePropertyBrowser* p_propBrows, QtVariantPropertyManager* p_propMan )
{
	
	if(p_item->whatsThis(0).compare("Entity") == 0)
	{
		m_onEvent->EditEntityData(this->indexOfTopLevelItem(p_item),p_propBrows, p_propMan);
	}
	else if (p_item->whatsThis(0).compare("Components") == 0)
	{
		m_onEvent->EditComponentData(this->indexOfTopLevelItem(p_item->parent()),p_propBrows ,p_item->text(0), p_propMan);
	}
}

void CustomTreeWidget::LoadData()
{
	for (unsigned int i = 0; i < m_onEvent->GetEntities()->size(); i++)
	{
		QTreeWidgetItem* item = new QTreeWidgetItem;
		item->setText(0,m_onEvent->GetEntities()->at(i)->m_name);
		item->setWhatsThis(0,"Entity");

		this->addTopLevelItem(item);

		for (unsigned int j = 0; j < m_onEvent->GetEntities()->at(i)->m_components->size(); j++)
		{
			QTreeWidgetItem* itemComp = new QTreeWidgetItem;
			itemComp->setText(0,m_onEvent->GetEntities()->at(i)->GetComponentNameFromId(j));
			itemComp->setWhatsThis(0,"Components");
			item->addChild(itemComp);
		}
	}
}

void CustomTreeWidget::Clear()
{
	this->clear();
	m_onEvent->Clear();
}
