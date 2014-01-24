#pragma once
#include <Qt/QtWidgets/QListWidget>
#include <QtGui/QDrag>
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include <Qt/QtGui/QPixmap>
#include "CustomTreeWidget.h"
class CustomListWidget : public QListWidget
{
	Q_OBJECT
public:
	CustomListWidget(QWidget* parent = 0);
protected:
	void mousePressEvent( QMouseEvent* event );
private:

	QPen m_pen;
};