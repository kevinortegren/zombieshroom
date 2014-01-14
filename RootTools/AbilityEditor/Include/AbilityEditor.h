#ifndef ABILITYEDITOR_H
#define ABILITYEDITOR_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QBoxLayout>
//#include "ui_AbilityEditor.h"
#include "RootTools/AbilityEditor/GeneratedFiles/ui_AbilityEditor.h"
class AbilityEditor : public QMainWindow
{
	Q_OBJECT

public:
	AbilityEditor(QWidget *parent = 0);
	~AbilityEditor();
	void Init();

public slots:
	void UpdatePropertyBrowser();

private:
	Ui::AbilityEditorClass ui;

	
};
#endif // ABILITYEDITOR_H
