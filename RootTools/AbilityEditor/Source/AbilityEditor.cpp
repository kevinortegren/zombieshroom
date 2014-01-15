#include "AbilityEditor.h"

#include <QtPropertyBrowser/QtVariantProperty>
#include <QtPropertyBrowser/QtVariantPropertyManager>
#include <QtPropertyBrowser/QtVariantEditorFactory>

AbilityEditor::AbilityEditor(QWidget *parent)
	: QMainWindow(parent)
{
	m_compNames.append("Transform");
	m_compNames.append("Collision");
	m_compNames.append("Ability Model");
	m_compNames.append("Collision Shape");
	m_compNames.append("Ability Particle");
	m_compNames.append("Physics Controlled");
	m_compNames.append("Offensive Ability");
	m_compNames.append("Explosive");

	
	
}

AbilityEditor::~AbilityEditor()
{

}

void AbilityEditor::Init()
{
	ui.setupUi(this);

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

