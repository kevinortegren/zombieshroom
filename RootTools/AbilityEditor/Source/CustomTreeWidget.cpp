#include "CustomTreeWidget.h"
#include <Qt/QtCore/QMimeData>
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
		QString test = item->whatsThis(0);
		this->addTopLevelItem(item);
		m_onEvent->AddEntity(item->text(0).toStdString());
	}
	else if(this->itemAt(event->pos()) != nullptr )
	{
		QTreeWidgetItem* item = new QTreeWidgetItem;
		item->setText(0,event->mimeData()->text());
		item->setWhatsThis(5,event->mimeData()->text());
		if((this->itemAt(event->pos())->whatsThis(0).compare("Entity") ==0))	
			this->itemAt(event->pos())->addChild(item);
		else
			this->itemAt(event->pos())->parent()->addChild(item);
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
	event->accept();
}

void CustomTreeWidget::SetOnEventClass(AbilityEditorNameSpace::MainOnEvent* p_onEvent )
{
	m_onEvent = p_onEvent;
}
