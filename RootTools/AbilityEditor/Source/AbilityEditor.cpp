#include "AbilityEditor.h"
#include <QtPropertyBrowser/QtTreePropertyBrowser>
#include <QtPropertyBrowser/QtVariantProperty>
#include <QtPropertyBrowser/QtVariantPropertyManager>
#include <QtPropertyBrowser/QtVariantEditorFactory>
#include <QtWidgets/QCheckBox>
#include "OnCreate.h"
#include "Exporter.h"
#include "Importer.h"
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
	AbilityEditorNameSpace::OnCreate* test = new AbilityEditorNameSpace::OnCreate();
	AbilityEditorNameSpace::Exporter* exporter = new AbilityEditorNameSpace::Exporter();
	AbilityEditorNameSpace::Importer* importer = new AbilityEditorNameSpace::Importer();
	/*test->AddEntity("Testus totalius");
	test->AddComponent(0, AbilityEditorNameSpace::AbilityComponents::ComponentType::ABILITYMODEL);
	test->AddComponent(0, AbilityEditorNameSpace::AbilityComponents::ComponentType::ABILITYPARTICLE);
	test->AddComponent(0, AbilityEditorNameSpace::AbilityComponents::ComponentType::COLLISIONSHAPE);
	test->AddComponent(0, AbilityEditorNameSpace::AbilityComponents::ComponentType::COLLISION);
	test->AddComponent(0, AbilityEditorNameSpace::AbilityComponents::ComponentType::TRANSFORM);
	test->AddEntity("Testus totalius 2");
	test->AddComponent(1, AbilityEditorNameSpace::AbilityComponents::ComponentType::COLLISIONSHAPE);
	test->AddComponent(1, AbilityEditorNameSpace::AbilityComponents::ComponentType::COLLISION);
	test->AddComponent(1, AbilityEditorNameSpace::AbilityComponents::ComponentType::TRANSFORM);
	test->AddComponent(1, AbilityEditorNameSpace::AbilityComponents::ComponentType::ABILITYMODEL);
	test->AddComponent(1, AbilityEditorNameSpace::AbilityComponents::ComponentType::ABILITYPARTICLE);



	exporter->Export("Test.ability", test, nullptr, nullptr);*/
	
	importer->Import("Test.ability", test, nullptr, nullptr);
	exporter->Export("Test2.ability", test, nullptr, nullptr);
	//connect(ui.treeOnCollide, SIGNAL(ui.treeOnCollide->itemClicked()), this, SLOT(UpdatePropertyBrowser()));
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(UpdatePropertyBrowser()));
		
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

