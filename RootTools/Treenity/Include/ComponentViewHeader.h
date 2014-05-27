#pragma once

#include <QWidget.h>
#include <RootTools/Treenity/GeneratedFiles/ui_ComponentViewHeader.h>

class ComponentViewHeader : public QWidget
{
	Q_OBJECT
public:
	ComponentViewHeader(const QString& p_name, QWidget* p_parent);
	virtual ~ComponentViewHeader() {}

private:
	Ui::Form_componentHeader ui;

private slots:
	void CollapseButtonPress();
	void RemoveButtonPress();

signals:
	void show();
	void hide();
	void remove();
};