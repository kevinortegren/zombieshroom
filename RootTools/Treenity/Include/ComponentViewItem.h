#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QLabel>
#include <QVBoxLayout>

class ComponentViewItem : public QWidget
{
public:
	ComponentViewItem(const QString& p_title, QWidget* p_item, QWidget* p_parent = 0);
	~ComponentViewItem();
protected:
	void mousePressEvent(QMouseEvent* event);

private:
	
	QWidget*		m_item;
	QLabel*			m_label;
	QVBoxLayout*	m_layout;
};