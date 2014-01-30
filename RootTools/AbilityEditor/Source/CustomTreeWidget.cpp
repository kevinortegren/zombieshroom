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
	//Behövs ej?
	/*if(event->mimeData()->objectName().compare("Script") == 0)
	{
		QTreeWidgetItem* item = new QTreeWidgetItem;
		item->setText(0,event->mimeData()->text());
		item->setWhatsThis(0,"Script");
		item->setTextColor(0,QColor(0,255,0));
		
		this->addTopLevelItem(item);
		m_onEvent->AddScript(item->text(0));
	
	}*/
	if(event->mimeData()->objectName().compare("Entity") == 0) //Entity eller Entities?
	{
		QTreeWidgetItem* item = new QTreeWidgetItem;
		item->setText(0,event->mimeData()->text());
		item->setWhatsThis(0,"Entity");

		//Fel, ändra det här
		this->addTopLevelItem(item);
		m_onEvent->AddEntity(item->text(0));

	}
	else if(event->mimeData()->objectName().compare("Condition") == 0) //Kanske S
	{
		QTreeWidgetItem* item = new QTreeWidgetItem;
		item->setText(0,event->mimeData()->text());
		item->setWhatsThis(0,"Condition");

		//Ändra här
		this->addTopLevelItem(item);
		m_onEvent->AddEntity(item->text(0));

	}
	//Egen klass?
	else if(event->mimeData()->objectName().compare("Component") == 0) //Kanske S
	{

		QTreeWidgetItem* item = new QTreeWidgetItem;
		item->setText(0,event->mimeData()->text());
		item->setWhatsThis(0,event->mimeData()->objectName());
		//När man släpper på "entity"
		if((this->itemAt(event->pos())->whatsThis(0).compare("Entity") ==0))
		{
			if(m_onEvent->CheckForExistingComponents(this->indexOfTopLevelItem(this->itemAt(event->pos())), event->mimeData()->text()))
			{
				QMessageBox msgBox;
				msgBox.setText("An ability can only contain one of each type of condition");
				msgBox.exec();
				delete item;
				return;
			}
			this->itemAt(event->pos())->addChild(item);
		}
		//när man släpper utanför entity
		else
		{ 
			//STUFF!!!!!
			if(m_onEvent->CheckForExistingComponents(this->indexOfTopLevelItem(this->itemAt(event->pos())->parent()), event->mimeData()->text()))
			{
				QMessageBox msgBox;
				msgBox.setText("An ability can only contain one of each type of condition");
				msgBox.exec();
				return;
			}
			this->itemAt(event->pos())->parent()->addChild(item);
		}
		unsigned int index = this->indexOfTopLevelItem(item->parent());
		m_onEvent->AddComponent(index, item->text(0)); //Nope
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
	//Om Condition
	if(p_item->whatsThis(0).compare("Condition") == 0)
	{
		int test = this->indexOfTopLevelItem(p_item);
		delete p_item;
		//m_onEvent->RemoveEntity(this->indexOfTopLevelItem(p_item));
		m_onEvent->RemoveEntity(test); //snarare condition
		//delete this->takeTopLevelItem(this->indexOfTopLevelItem(p_item));
	}
	//Annars Entity
	else if (p_item->whatsThis(0).compare("Entity") == 0)
	{

		m_onEvent->RemoveComponent(this->indexOfTopLevelItem(p_item->parent()),p_item->text(0)); //Vadå component?
		this->removeItemWidget(p_item,0);
		delete p_item;
		
	}
}

void CustomTreeWidget::ViewSelectedData( QTreeWidgetItem* p_item, QtTreePropertyBrowser* p_propBrows, QtVariantPropertyManager* p_propMan ) //Ändra här vafan
{
	if(p_item->whatsThis(0).compare("Condition") == 0)
	{
		m_onEvent->ViewEntityData(this->indexOfTopLevelItem(p_item),p_propBrows, p_propMan );
	}
	else if (p_item->whatsThis(0).compare("Entity") == 0)
	{
		m_onEvent->ViewComponentData(this->indexOfTopLevelItem(p_item->parent()),p_propBrows, p_item->text(0), p_propMan);
	}
}

void CustomTreeWidget::SaveSelectedData( QTreeWidgetItem* p_item, QtTreePropertyBrowser* p_propBrows, QtVariantPropertyManager* p_propMan )
{
	
	if(p_item->whatsThis(0).compare("Condition") == 0)
	{
		m_onEvent->EditEntityData(this->indexOfTopLevelItem(p_item),p_propBrows, p_propMan);
		p_item->setText(0, m_onEvent->GetEntityName(this->indexOfTopLevelItem(p_item))); //Nej, conditions, inte entity
	}
	else if (p_item->whatsThis(0).compare("Entity") == 0)
	{
		m_onEvent->EditComponentData(this->indexOfTopLevelItem(p_item->parent()),p_propBrows ,p_item->text(0), p_propMan); //FELFELFEL
	}
}

void CustomTreeWidget::LoadData()
{
	for (unsigned int i = 0; i < m_onEvent->GetEntities()->size(); i++)
	{
		QTreeWidgetItem* item = new QTreeWidgetItem;
		item->setText(0,m_onEvent->GetEntities()->at(i)->m_name);
		item->setWhatsThis(0,"Condition");

		this->addTopLevelItem(item);

		for (unsigned int j = 0; j < m_onEvent->GetEntities()->at(i)->m_components->size(); j++) //ändra här för helvete
		{
			QTreeWidgetItem* itemComp = new QTreeWidgetItem;
			itemComp->setText(0,m_onEvent->GetEntities()->at(i)->GetComponentNameFromId(j));
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
