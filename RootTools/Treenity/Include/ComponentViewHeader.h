#pragma once

#include <QWidget.h>
#include <QMenu>
#include <RootTools/Treenity/GeneratedFiles/ui_ComponentViewHeader.h>

class ComponentViewHeader : public QWidget
{
	Q_OBJECT
public:
	ComponentViewHeader(const QString& p_name, QWidget* p_parent);
	virtual ~ComponentViewHeader() {}

	void SetArrowShape(Qt::ArrowType p_type);

private:
	Ui::Form_componentHeader ui;
	QMenu* m_contextMenu;

private slots:
	void CollapseButtonPress();
	void RemoveButtonPress();

	//void Populate();
	void MenuActionRemove();

signals:
	void showView();
	void hideView();
	void remove();
};