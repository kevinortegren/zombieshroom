#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <RootTools/Treenity/Include/ComponentViewItem.h>

class ComponentView : public QWidget
{
public:
	ComponentView(QWidget* p_parent = 0);
	~ComponentView();

	//Add
	void AddItem(ComponentViewItem* p_item);

	//Remove
	void RemoveItems();
	void RemoveItem(const QString& p_name);

	//Utility
	int Count();

	//Getters
	ComponentViewItem* GetItemByName(const QString& p_name);

private:
	QVBoxLayout* m_layout;

};