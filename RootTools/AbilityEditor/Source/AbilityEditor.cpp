#include "AbilityEditor.h"
#include <QtPropertyBrowser/QtTreePropertyBrowser>
#include <QtWidgets/QCheckBox>

AbilityEditor::AbilityEditor(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	
	QtTreePropertyBrowser* hej;
	hej = new QtTreePropertyBrowser();
	hej->setGeometry(ui.propertyWidget->geometry());
	
	this->layout()->addWidget(hej);

// 	QCheckBox* test;
// 	
// 	test = new QCheckBox();
// 	test->setObjectName(QStringLiteral("test"));
// 	test->setGeometry(ui.propertyWidget->geometry());
// 	test->setText(QApplication::translate("AbilityEditorClass", "BAJSBOX", 0));
// 	layout()->addWidget(test);
	
}

AbilityEditor::~AbilityEditor()
{

}
