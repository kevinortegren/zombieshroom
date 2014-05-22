#pragma once

#include <QScrollArea>
 
class VerticalScrollArea : public QScrollArea
{
 Q_OBJECT
public:
 explicit VerticalScrollArea(QWidget *parent = 0);
 
 virtual bool eventFilter(QObject *o, QEvent *e);
 
};