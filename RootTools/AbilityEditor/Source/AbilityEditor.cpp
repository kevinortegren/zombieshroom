#include "AbilityEditor.h"

#include <QtPropertyBrowser/QtVariantProperty>
#include <QtPropertyBrowser/QtVariantPropertyManager>
#include <QtPropertyBrowser/QtVariantEditorFactory>
#include "OnCreate.h"
#include "Exporter.h"
#include "Importer.h"
AbilityEditor::AbilityEditor(QWidget *parent)
	: QMainWindow(parent)
{
	for(unsigned i = 0; i < AbilityEditorNameSpace::AbilityComponents::ComponentType::END_OF_ENUM; i++)
	{
		m_compNames.append(AbilityEditorNameSpace::AbilityComponents::g_componentNameList.m_compNames.at(i));
	}
	
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
	//////////////////////////////////////////////////////////////////////////
	//AbilityEditorNameSpace::OnCreate* test = new AbilityEditorNameSpace::OnCreate();
	//AbilityEditorNameSpace::Exporter* exporter = new AbilityEditorNameSpace::Exporter();
	//AbilityEditorNameSpace::Importer* importer = new AbilityEditorNameSpace::Importer();

	//test->AddEntity("Testus totalius");
	//test->AddComponent(0, AbilityEditorNameSpace::AbilityComponents::ComponentType::ABILITYMODEL);
	//test->AddComponent(0, AbilityEditorNameSpace::AbilityComponents::ComponentType::ABILITYPARTICLE);
	//test->AddComponent(0, AbilityEditorNameSpace::AbilityComponents::ComponentType::COLLISIONSHAPE);
	//test->AddComponent(0, AbilityEditorNameSpace::AbilityComponents::ComponentType::COLLISION);
	//test->AddComponent(0, AbilityEditorNameSpace::AbilityComponents::ComponentType::TRANSFORM);
	//test->AddEntity("Testus totalius 2");
	//test->AddComponent(1, AbilityEditorNameSpace::AbilityComponents::ComponentType::COLLISIONSHAPE);
	//test->AddComponent(1, AbilityEditorNameSpace::AbilityComponents::ComponentType::COLLISION);
	//test->AddComponent(1, AbilityEditorNameSpace::AbilityComponents::ComponentType::TRANSFORM);
	//test->AddComponent(1, AbilityEditorNameSpace::AbilityComponents::ComponentType::ABILITYMODEL);
	//test->AddComponent(1, AbilityEditorNameSpace::AbilityComponents::ComponentType::ABILITYPARTICLE);

	//exporter->Export("Test.ability", test, nullptr, nullptr);

	/*importer->Import("Test.ability", test, nullptr, nullptr);
	exporter->Export("Test2.ability", test, nullptr, nullptr);*/
	//////////////////////////////////////////////////////////////////////////

	importer->Import("Test.ability", test, nullptr, nullptr);
	test->RemoveComponent(0, AbilityEditorNameSpace::AbilityComponents::ComponentType::ABILITYMODEL );
	exporter->Export("Test2.ability", test, nullptr, nullptr);
	connect(ui.treeOnCollide, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(UpdatePropertyBrowser(QTreeWidgetItem*)));
	
	ui.listComponents->addItems(m_compNames);
	ui.listAbilities->addItem("Empty Entity");
	m_propBrows = new QtTreePropertyBrowser;
	m_mainLayout = new QGridLayout();

	
	

}

void AbilityEditor::UpdatePropertyBrowser( QTreeWidgetItem* p_item )
{

	if (m_propBrows != 0)
	{
		delete m_propBrows;
	}

	m_propBrows = new QtTreePropertyBrowser(ui.propertyWidget);
	m_propBrows->setGeometry(ui.propertyWidget->geometry());

	m_mainLayout->addWidget(m_propBrows);
	ui.propertyWidget->setLayout(m_mainLayout);

	QtVariantPropertyManager* hej;
	hej = new QtVariantPropertyManager;

	QString temp = p_item->text(p_item->columnCount()-1);
	QtVariantProperty* prop;
	prop = hej->addProperty(QVariant::String, "Name");
	hej->setValue(prop, temp);

	

	QtVariantEditorFactory* propFact;
	propFact = new QtVariantEditorFactory;
	m_propBrows->setFactoryForManager(hej, propFact);
	m_propBrows->addProperty(prop);
	
}

