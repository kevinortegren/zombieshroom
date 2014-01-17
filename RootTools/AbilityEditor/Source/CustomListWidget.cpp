#include "CustomListWidget.h"
#include <Qt/QtCore/QMimeData>
#include "GeneratedFiles/ui_AbilityEditor.h"
#include <QtGui/QPen>
CustomListWidget::CustomListWidget( QWidget* parent /*= 0*/ ) : QListWidget(parent)
{
	m_pen.setColor(QColor("blue"));
	
}



void CustomListWidget::mousePressEvent( QMouseEvent* event )
{
	QListWidget::mousePressEvent(event);
	if(event->button() == Qt::LeftButton && this->geometry().contains(event->pos()))
	{
		if(currentItem() != nullptr && (currentItem() == this->itemAt(event->pos())))
		{	
			//Remove list from the objectname to get the name of the object types in this list, assumes the list name starts with "list"
			QString type = &(this->objectName().toStdString().at(4)); //TODO : Maybe change this to a less hacky solution
			QPixmap	pixmap(QSize(100, 20));
			QPainter painter(&pixmap);
			painter.setPen(m_pen);
			QListWidgetItem* item = currentItem();
			QDrag* drag = new QDrag(this);
			QMimeData* data = new QMimeData;
			if(type.compare("Abilities"))
				data->setText(item->text());
			else
				data->setText("Entity");
			data->setObjectName(type);
			drag->setMimeData(data);
		//	m_painter->fillRect(QRect(0,0,100,20), QBrush(QColor("orange")));
			painter.drawText(QRect(10, 0, 170, 20), item->text() );
			drag->setPixmap(pixmap);
			Qt::DropAction dropAction = drag->exec();		
		}

	}
	
}
