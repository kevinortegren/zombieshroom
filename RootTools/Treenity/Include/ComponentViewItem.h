#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QLabel>
#include <QVBoxLayout>
#include <RootTools/Treenity/Include/Components/AbstractComponentView.h>

class ComponentViewItem : public QWidget
{
public:
	ComponentViewItem(AbstractComponentView* p_item, QWidget* p_parent = 0);
	~ComponentViewItem();

	AbstractComponentView* GetItem();
protected:
	void mousePressEvent(QMouseEvent* event);

private:
	AbstractComponentView*		m_item;
	QLabel*						m_label;
	QVBoxLayout*				m_layout;
};