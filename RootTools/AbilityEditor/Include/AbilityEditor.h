#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QBoxLayout>
#include <QtCore/QStringList>
#include <QtPropertyBrowser/QtTreePropertyBrowser>
#include "GeneratedFiles/ui_AbilityEditor.h"
#include "RootTools/AbilityEditor/GeneratedFiles/ui_AbilityEditor.h"

class AbilityEditor : public QMainWindow
{
	Q_OBJECT

public:
	AbilityEditor(QWidget *parent = 0);
	~AbilityEditor();
	void Init();

	QStringList m_compNames;

	public slots:
		void UpdatePropertyBrowser(QTreeWidgetItem* p_item);

private:
	Ui::AbilityEditorClass ui;

	QtTreePropertyBrowser* m_propBrows;
	QLayout* m_mainLayout;


};
