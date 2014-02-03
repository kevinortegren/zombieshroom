#include "CustomTreeWidget.h"
#include <Qt/QtCore/QMimeData>
#include <Qt/QtWidgets/QMessageBox>
#include <QtGui/QDrag>
CustomTreeWidget::CustomTreeWidget( QWidget* parent /*= 0*/ ) : QTreeWidget(parent)
{
	setAcceptDrops(true);
}

void CustomTreeWidget::dropEvent( QDropEvent* event )
{
	
	if(event->mimeData()->objectName().compare("Conditions") == 0) 
	{
		QTreeWidgetItem* item = new QTreeWidgetItem;
		item->setText(0,event->mimeData()->text());
		item->setToolTip(0,event->mimeData()->html());
		item->setWhatsThis(0,"Conditions");

		this->addTopLevelItem(item);
		m_onEvent->AddCondition(item->text(0), item->toolTip(0));

	}

	if(event->mimeData()->objectName().compare("Entity") == 0) //Detta bestäms i AbilityEditor.cpp
	{
		if (this->itemAt(event->pos()) == nullptr)
		{
			return;
		}
		QTreeWidgetItem* item = new QTreeWidgetItem;
		item->setText(0,event->mimeData()->text());
		item->setWhatsThis(0,"Entity");

		
		//När man släpper på en condition
		if((this->itemAt(event->pos())->whatsThis(0).compare("Conditions") == 0))
			this->itemAt(event->pos())->addChild(item);
		//När man släpper "inuti" en condition
		else
			this->itemAt(event->pos())->parent()->addChild(item);
		unsigned int index = this->indexOfTopLevelItem(item->parent());
		m_onEvent->AddEntity(index, item->text(0));
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
	//kommer in hit någonsin?
	if(p_item == nullptr)
	{
		//m_onEvent->RemoveEntity(0);
		return;
	}
	//Om Condition
	if(p_item->whatsThis(0).compare("Conditions") == 0)
	{
		int test = this->indexOfTopLevelItem(p_item);
		delete p_item;
		//m_onEvent->RemoveEntity(this->indexOfTopLevelItem(p_item));
		m_onEvent->RemoveCondition(test); 
		//delete this->takeTopLevelItem(this->indexOfTopLevelItem(p_item));
	}
	//Annars Entity
	else if (p_item->whatsThis(0).compare("Entity") == 0)
	{
		m_onEvent->RemoveEntity(this->indexOfTopLevelItem(p_item->parent()),p_item->text(0));
		this->removeItemWidget(p_item,0);
		delete p_item;
		
	}
}

void CustomTreeWidget::ViewSelectedData( QTreeWidgetItem* p_item, QtTreePropertyBrowser* p_propBrows, QtVariantPropertyManager* p_propMan ) 
{
	if(p_item->whatsThis(0).compare("Condition") == 0)
	{
		m_onEvent->ViewConditionData(this->indexOfTopLevelItem(p_item),p_propBrows, p_propMan );
	}
	//No need?
// 	else if (p_item->whatsThis(0).compare("Entity") == 0)
// 	{
// 		m_onEvent->ViewComponentData(this->indexOfTopLevelItem(p_item->parent()),p_propBrows, p_item->text(0), p_propMan);
// 	}
}

void CustomTreeWidget::SaveSelectedData( QTreeWidgetItem* p_item, QtTreePropertyBrowser* p_propBrows, QtVariantPropertyManager* p_propMan )
{
	
	if(p_item->whatsThis(0).compare("Condition") == 0)
	{
		m_onEvent->EditConditionData(this->indexOfTopLevelItem(p_item),p_propBrows, p_propMan);
	}
}

void CustomTreeWidget::LoadData()
{
	for (unsigned int i = 0; i < m_onEvent->GetConditions()->size(); i++)
	{
		QTreeWidgetItem* item = new QTreeWidgetItem;
		item->setText(0,m_onEvent->GetConditions()->at(i)->GetName());
		item->setWhatsThis(0,"Condition");

		this->addTopLevelItem(item);

		for (unsigned int j = 0; j < m_onEvent->GetConditions()->at(i)->GetEntities().size(); j++)
		{
			QTreeWidgetItem* itemComp = new QTreeWidgetItem;
			itemComp->setText(0,m_onEvent->GetConditions()->at(i)->GetEntities().at(j));
			itemComp->setWhatsThis(0,"Entity");
			item->addChild(itemComp);
		}
	}
}

void CustomTreeWidget::Clear()
{
	this->clear();
	m_onEvent->Clear();
}
