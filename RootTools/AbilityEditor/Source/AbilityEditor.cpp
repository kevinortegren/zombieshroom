#include "AbilityEditor.h"

AbilityEditor::AbilityEditor(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//QHBoxLayout* test = new QHBoxLayout(this);
	QPushButton* testus = new QPushButton();
//	test->addWidget(testus);
	//centralWidget()->setLayout(test);
	
	this->layout()->addWidget(testus);
}

AbilityEditor::~AbilityEditor()
{

}
