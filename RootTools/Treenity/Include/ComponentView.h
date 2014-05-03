#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <RootTools/Treenity/Include/ComponentViewItem.h>

class ComponentView : public QWidget
{
public:
	ComponentView(QWidget* p_parent = 0);
	~ComponentView();

	void AddItem(ComponentViewItem* p_item);
	void RemoveItems();
private:
	QVBoxLayout* m_layout;
};