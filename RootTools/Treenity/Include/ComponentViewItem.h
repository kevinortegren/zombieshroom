#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMenu>
#include <RootTools/Treenity/Include/Components/AbstractComponentView.h>
#include <RootTools/Treenity/Include/ComponentViewHeader.h>

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
	ComponentViewHeader*		m_header;

private slots:
	void ShowComponentView();
	void HideComponentView();
	void RemoveComponent();
};