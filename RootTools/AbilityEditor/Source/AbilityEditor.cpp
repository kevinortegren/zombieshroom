#include "AbilityEditor.h"

#include <QtPropertyBrowser/QtVariantProperty>
#include <QtPropertyBrowser/QtVariantPropertyManager>
#include <QtPropertyBrowser/QtVariantEditorFactory>
#include <QtGui/QDrag>
#include <QtCore/QMimeData>
#include <QtGui/QPen>
#include <QtWidgets/QFileDialog>
#include "CustomTreeWidget.h"

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
	
	if(m_LastSelectedItem != nullptr)
	{

		if(m_LastSelectedTab == 0) //On Create list
			ui.treeOnCreate->SaveSelectedData(m_LastSelectedItem,m_propBrows, m_propMan);
		else if(m_LastSelectedTab == 1) //On Collide list
			ui.treeOnCollide->SaveSelectedData(m_LastSelectedItem,m_propBrows, m_propMan);
		else if(m_LastSelectedTab == 2) //On Destroy list
			ui.treeOnDestroy->SaveSelectedData(m_LastSelectedItem,m_propBrows, m_propMan);
	}
}

void AbilityEditor::Init()
{
	ui.setupUi(this);
	m_onCreate = new AbilityEditorNameSpace::OnCreate();
	m_onCollide = new AbilityEditorNameSpace::OnCollide();
	m_onDestroy = new AbilityEditorNameSpace::OnDestroy();

	m_scriptGenerator = new AbilityEditorNameSpace::ScriptGenerator();
	m_exporter = new AbilityEditorNameSpace::Exporter();
	m_importer = new AbilityEditorNameSpace::Importer();

	ui.listComponents->addItems(m_compNames);
	ui.listAbilities->addItem("Empty Entity");
	m_propBrows = new QtTreePropertyBrowser;
	m_propMan = new QtVariantPropertyManager;
	m_mainLayout = new QGridLayout();
	m_LastSelectedItem = nullptr;

	ui.treeOnCreate->SetOnEventClass(m_onCreate);
	ui.treeOnCollide->SetOnEventClass(m_onCollide);
	ui.treeOnDestroy->SetOnEventClass(m_onDestroy);
	this->setFixedSize(this->size());
	this->statusBar()->setSizeGripEnabled(false);
	

	connect(ui.treeOnCreate, SIGNAL(itemSelectionChanged()), this, SLOT(UpdatePropertyBrowser()));
	connect(ui.treeOnCollide, SIGNAL(itemSelectionChanged()), this, SLOT(UpdatePropertyBrowser()));
	connect(ui.treeOnDestroy, SIGNAL(itemSelectionChanged()), this, SLOT(UpdatePropertyBrowser()));
	connect(ui.abilityWidget, SIGNAL(currentChanged(int)), this, SLOT(ChangedTab()));
	//connect(m_propMan, SIGNAL(valueChanged(QtProperty*, const QVariant&)), this, SLOT(ChangedTab()));
	connect(ui.actionGenerate_Script, SIGNAL(triggered()), this, SLOT(GenerateScript()));
	connect(ui.actionSave_As, SIGNAL(triggered()), this, SLOT(SaveAs()));
	connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(Save()));
	connect(ui.actionLoad, SIGNAL(triggered()), this, SLOT(Load()));

	
}

void AbilityEditor::ChangedTab()
{
	//int test = ui.abilityWidget->currentIndex();
	if(m_LastSelectedItem != nullptr)
	{
		
		if(m_LastSelectedTab == 0) //On Create list
			ui.treeOnCreate->SaveSelectedData(m_LastSelectedItem,m_propBrows, m_propMan);
		else if(m_LastSelectedTab == 1) //On Collide list
			ui.treeOnCollide->SaveSelectedData(m_LastSelectedItem,m_propBrows, m_propMan);
		else if(m_LastSelectedTab == 2) //On Destroy list
			ui.treeOnDestroy->SaveSelectedData(m_LastSelectedItem,m_propBrows, m_propMan);
	}
	m_LastSelectedItem = nullptr;
	delete m_propBrows;
	m_propBrows = nullptr;
	//UpdatePropertyBrowser();
}

void AbilityEditor::UpdatePropertyBrowser( )
{
	
	
	
	/*QTreeWidget* asdf = new QTreeWidget();
	asdf = ui.abilityWidget->widget(ui.abilityWidget->currentIndex());
	int bajs = asdf->selectedItems().count();*/
	if(m_propBrows != nullptr)
	{
		if(m_LastSelectedItem != nullptr)
		{
			if(ui.abilityWidget->currentIndex() == 0 && ui.treeOnCreate->hasFocus()) //On Create list
				ui.treeOnCreate->SaveSelectedData(m_LastSelectedItem,m_propBrows, m_propMan);
			else if(ui.abilityWidget->currentIndex() == 1 && ui.treeOnCollide->hasFocus()) //On Collide list
				ui.treeOnCollide->SaveSelectedData(m_LastSelectedItem,m_propBrows, m_propMan);
			else if(ui.abilityWidget->currentIndex() == 2 && ui.treeOnDestroy->hasFocus()) //On Destroy list
				ui.treeOnDestroy->SaveSelectedData(m_LastSelectedItem,m_propBrows, m_propMan);
		}
		delete m_propBrows;
		m_propBrows = nullptr;
		m_LastSelectedItem = nullptr;
	}
	
	/*QWidget* page = static_cast<QWidget*>(ui.abilityWidget->children().at(ui.abilityWidget->currentIndex()));
	QTreeWidget* tree = static_cast<QTreeWidget*>(page->children().at(0));
	QTreeWidgetItem* m_LastSelectedItem = tree->selectedItems().at(0);*/
	m_propBrows = new QtTreePropertyBrowser(ui.propertyWidget);
	m_propBrows->setGeometry(ui.propertyWidget->geometry());
	
	
	if(ui.abilityWidget->currentIndex() == 0 /*&& ui.treeOnCreate->hasFocus()*/) //On Create list
	{
		if(ui.treeOnCreate->selectedItems().count() == 0)
			return;
		if(ui.treeOnCreate->selectedItems().at(0) != nullptr)
		{
			m_LastSelectedItem = ui.treeOnCreate->selectedItems().at(0);
			ui.treeOnCreate->ViewSelectedData(m_LastSelectedItem, m_propBrows, m_propMan);
		}
	}
	else if(ui.abilityWidget->currentIndex() == 1/* && ui.treeOnCollide->hasFocus()*/) //On Collide list
	{
		if(ui.treeOnCollide->selectedItems().count() == 0)
			return;
		if(ui.treeOnCollide->selectedItems().at(0) != nullptr)
		{
			m_LastSelectedItem = ui.treeOnCollide->selectedItems().at(0);
			ui.treeOnCollide->ViewSelectedData(m_LastSelectedItem, m_propBrows, m_propMan);
		}
	}
	else if(ui.abilityWidget->currentIndex() == 2/* && ui.treeOnDestroy->hasFocus()*/) //On Destroy list
	{
		if(ui.treeOnDestroy->selectedItems().count() == 0)
			return;
		if(ui.treeOnDestroy->selectedItems().at(0) != 0)
		{
			m_LastSelectedItem = ui.treeOnDestroy->selectedItems().at(0);
			ui.treeOnDestroy->ViewSelectedData(m_LastSelectedItem, m_propBrows, m_propMan);
		}
	}
	
	m_LastSelectedTab = ui.abilityWidget->currentIndex();

	m_mainLayout->addWidget(m_propBrows);
	ui.propertyWidget->setLayout(m_mainLayout);


}


bool AbilityEditor::event( QEvent* event )
{
	
	if (event->type() == QEvent::KeyPress)
	{
		QKeyEvent* ke = static_cast<QKeyEvent*>(event);
		if (ke->key() == Qt::Key_Delete)
		{
		
			if(ui.abilityWidget->currentIndex() == 0 && ui.treeOnCreate->hasFocus()) //On Create list
			{
				if(ui.treeOnCreate->currentItem() != 0)
				{
					ui.treeOnCreate->RemoveSelected(ui.treeOnCreate->currentItem());		
					//delete ui.treeOnCreate->currentItem();	
					
				}
			}
			else if(ui.abilityWidget->currentIndex() == 1 && ui.treeOnCollide->hasFocus()) //On Collide list
			{
				if(ui.treeOnCollide->currentItem() != 0)
				{
					ui.treeOnCollide->RemoveSelected(ui.treeOnCollide->currentItem());	
					//delete ui.treeOnCollide->currentItem();
					
				}
			}
			else if(ui.abilityWidget->currentIndex() == 2 && ui.treeOnDestroy->hasFocus()) //On Destroy list
			{
				if(ui.treeOnDestroy->currentItem() != 0)
				{
					ui.treeOnDestroy->RemoveSelected(ui.treeOnDestroy->currentItem());
					//delete ui.treeOnDestroy->currentItem();
						
				}
			}
			m_LastSelectedItem = nullptr;
			//return false;
		}
	}
	if(event->type() == QEvent::Drop)
		return false;
	return QWidget::event(event);
	//if(event->type() == QEvent::Drop)
	//	return QWidget::event(event);
	//return QWidget::event(event);
//	return QWidget::event(event);
}

void AbilityEditor::GenerateScript()
{
	QFileDialog dial(this);
	dial.setFileMode(QFileDialog::AnyFile);
	dial.setAcceptMode(QFileDialog::AcceptSave);
	dial.setNameFilter("*.lua");
	QString path = dial.getSaveFileName();
	if(path.compare("") != 0)
	{
		QString name = &(path.toStdString().at(path.lastIndexOf("/")+1));
		m_scriptGenerator->GenerateScript(path, name, m_onCreate, m_onCollide, m_onDestroy);
	}
}

void AbilityEditor::SaveAs()
{
	QFileDialog dial(this);
	dial.setFileMode(QFileDialog::AnyFile);
	dial.setAcceptMode(QFileDialog::AcceptSave);
	m_currentSavePath = dial.getSaveFileName();
	if(m_currentSavePath.compare("") != 0)
		m_exporter->Export(m_currentSavePath.toStdString(), m_onCreate, m_onCollide, m_onDestroy);
}

void AbilityEditor::Save()
{
	if(m_currentSavePath.compare("") != 0)
		m_exporter->Export(m_currentSavePath.toStdString(), m_onCreate, m_onCollide, m_onDestroy);
}

void AbilityEditor::Load()
{
	QFileDialog dial(this);
	dial.setFileMode(QFileDialog::AnyFile);
	dial.setAcceptMode(QFileDialog::AcceptOpen);
	m_currentSavePath = dial.getOpenFileName();
	if(m_currentSavePath.compare("") != 0)
	{
		ui.treeOnCreate->Clear();
		ui.treeOnCollide->Clear();
		ui.treeOnDestroy->Clear();
		m_importer->Import(m_currentSavePath.toStdString(), m_onCreate, m_onCollide, m_onDestroy);
		ui.treeOnCreate->LoadData();
		ui.treeOnCollide->LoadData();
		ui.treeOnDestroy->LoadData();
	}
}
