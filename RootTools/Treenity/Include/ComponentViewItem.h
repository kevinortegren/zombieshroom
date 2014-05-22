#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <RootTools/Treenity/Include/Components/AbstractComponentView.h>

class ComponentViewItem : public QWidget
{
	Q_OBJECT
public:
	ComponentViewItem(AbstractComponentView* p_item, QWidget* p_parent = 0);
	~ComponentViewItem();

	AbstractComponentView* GetItem();

private:
	AbstractComponentView*		m_item;
	QPushButton*				m_headerButton;
	QVBoxLayout*				m_layout;

private slots:
	void HeaderClicked();
};