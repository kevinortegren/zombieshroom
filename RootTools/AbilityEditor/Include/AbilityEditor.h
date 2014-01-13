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

private:
	Ui::AbilityEditorClass ui;
};

#endif // ABILITYEDITOR_H
