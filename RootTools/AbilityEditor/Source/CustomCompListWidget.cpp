#include "CustomCompListWidget.h"
#include <Qt/QtCore/QMimeData>
#include <Qt/QtWidgets/QMessageBox>
#include <QtGui/QDrag>
CustomCompListWidget::CustomCompListWidget( QWidget* parent /*= 0*/ ) : QListWidget(parent)
{
	setAcceptDrops(true);

	m_propFactory = new QtVariantEditorFactory;
	m_propManager = new QtVariantPropertyManager;
}

void CustomCompListWidget::dropEvent( QDropEvent* event )
{
	//Om objektet man släpper är en component
	if(event->mimeData()->objectName().compare("Components") == 0)
	{
		//QString asdf = event->mimeData()->text();
		if(!m_entity->DoesComponentExist(event->mimeData()->text()))
		{
			//QListWidgetItem* item = new QListWidgetItem(event->mimeData()->text(), this);
			QListWidgetItem* item = new QListWidgetItem;
			item->setText(event->mimeData()->text());
			item->setWhatsThis("Components");

			this->addItem(item);
			m_entity->AddComponent(item->text());
		}
	}
}

void CustomCompListWidget::dragEnterEvent( QDragEnterEvent* event )
{
	event->acceptProposedAction();
}

void CustomCompListWidget::dragMoveEvent( QDragMoveEvent *event )
{

	event->acceptProposedAction();
}



void CustomCompListWidget::SetEntity(AbilityEditorNameSpace::Entity* p_entity )
{
	m_entity = p_entity;
}

void CustomCompListWidget::RemoveSelected( QListWidgetItem* p_item )
{
	//kommer in hit någonsin?
	if(p_item == nullptr)
	{
		//m_onEvent->RemoveEntity(0);
		return;
	}
	if (p_item->whatsThis().compare("Components") == 0)
	{
		m_entity->RemoveComponent(p_item->text());
		this->removeItemWidget(p_item);
		delete p_item;
	}
}

void CustomCompListWidget::ViewSelectedData( QListWidgetItem* p_item, QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows) 
{	
	if (p_item->whatsThis().compare("Components") == 0)
	{
		m_entity->ViewComponentData(p_propMan, p_propBrows, m_propFactory, p_item->text());
	}
}

void CustomCompListWidget::SaveSelectedData( QListWidgetItem* p_item, QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows)
{
	if(p_item->whatsThis().compare("Components") == 0)
	{
		m_entity->SaveComponentData(p_propMan, p_propBrows, m_propFactory, p_item->text());
	}
}

void CustomCompListWidget::LoadData()
{
	for (unsigned int i = 0; i < m_entity->GetComponents()->size(); i++)
	{
		QListWidgetItem* itemComp = new QListWidgetItem;
		itemComp->setText(m_entity->GetComponentNameFromId(i));
		itemComp->setWhatsThis("Components");
		this->addItem(itemComp);
	}
}

void CustomCompListWidget::Clear()
{
	this->clear();
	m_entity->RemoveAllComponents();
}
