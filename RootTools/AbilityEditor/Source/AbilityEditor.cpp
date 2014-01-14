#include "AbilityEditor.h"
#include <QtPropertyBrowser/QtTreePropertyBrowser>
#include <QtPropertyBrowser/QtVariantProperty>
#include <QtPropertyBrowser/QtVariantPropertyManager>
#include <QtPropertyBrowser/QtVariantEditorFactory>
#include <QtWidgets/QCheckBox>

AbilityEditor::AbilityEditor(QWidget *parent)
	: QMainWindow(parent)
{

}

AbilityEditor::~AbilityEditor()
{

}

void AbilityEditor::Init()
{
	ui.setupUi(this);

	//connect(ui.treeOnCollide, SIGNAL(ui.treeOnCollide->itemClicked()), this, SLOT(UpdatePropertyBrowser()));
	if(connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(UpdatePropertyBrowser())))
		int asdf = 1;
}

void AbilityEditor::UpdatePropertyBrowser(  )
{
	/*QTreeWidget* p_treeWidget = ui.treeOnCollide;
	QtTreePropertyBrowser* propBrows;
	propBrows = new QtTreePropertyBrowser();
	propBrows->setGeometry(ui.propertyWidget->geometry());
	this->layout()->addWidget(propBrows);

	QtVariantPropertyManager* hej;
	hej = new QtVariantPropertyManager;

	QtVariantProperty* prop;
	prop = hej->addProperty(QVariant::String, "Name");
	prop->setAttribute("name", p_treeWidget->currentItem()->text(0));
	
	QtVariantEditorFactory* propFact;
	propFact = new QtVariantEditorFactory;
	propBrows->setFactoryForManager(hej, propFact);*/

	QCheckBox* test;

	test = new QCheckBox();
	test->setObjectName(QStringLiteral("test"));
	test->setGeometry(ui.propertyWidget->geometry());
	test->setText(QApplication::translate("AbilityEditorClass", "BAJSBOX", 0));
	layout()->addWidget(test);
}


