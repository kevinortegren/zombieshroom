#include "AbilityEditor.h"

#include <QtPropertyBrowser/QtVariantProperty>
#include <QtPropertyBrowser/QtVariantPropertyManager>
#include <QtPropertyBrowser/QtVariantEditorFactory>
#include <QtGui/QDrag>
#include <QtCore/QMimeData>
#include <QtGui/QPen>
#include "OnCreate.h"
#include "Exporter.h"
#include "Importer.h"
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
	AbilityEditorNameSpace::Exporter* exporter = new AbilityEditorNameSpace::Exporter();
	
	if(m_LastSelectedItem != nullptr)
	{

		if(m_LastSelectedTab == 0) //On Create list
			ui.treeOnCreate->SaveSelectedData(m_LastSelectedItem,m_propBrows);
		else if(m_LastSelectedTab == 1) //On Collide list
			ui.treeOnCollide->SaveSelectedData(m_LastSelectedItem,m_propBrows);
		else if(m_LastSelectedTab == 2) //On Destroy list
			ui.treeOnDestroy->SaveSelectedData(m_LastSelectedItem,m_propBrows);
	}
	exporter->Export("Test.ability", m_onCreate, m_onCollide, m_onDestroy);
}

void AbilityEditor::Init()
{
	ui.setupUi(this);
	m_onCreate = new AbilityEditorNameSpace::OnCreate();
	m_onCollide =new AbilityEditorNameSpace::OnCollide();
	m_onDestroy= new AbilityEditorNameSpace::OnDestroy();
	ui.treeOnCreate->SetOnEventClass(m_onCreate);
	ui.treeOnCollide->SetOnEventClass(m_onCollide);
	ui.treeOnDestroy->SetOnEventClass(m_onDestroy);
	this->setFixedSize(this->size());
	this->statusBar()->setSizeGripEnabled(false);
	
	//////////////////////////////////////////////////////////////////////////
	/*AbilityEditorNameSpace::OnCreate* test = new AbilityEditorNameSpace::OnCreate();
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

	connect(ui.treeOnCreate, SIGNAL(itemSelectionChanged()), this, SLOT(UpdatePropertyBrowser()));
	connect(ui.treeOnCollide, SIGNAL(itemSelectionChanged()), this, SLOT(UpdatePropertyBrowser()));
	connect(ui.treeOnDestroy, SIGNAL(itemSelectionChanged()), this, SLOT(UpdatePropertyBrowser()));
	connect(ui.abilityWidget, SIGNAL(currentChanged(int)), this, SLOT(ChangedTab()));
	ui.listComponents->addItems(m_compNames);
	ui.listAbilities->addItem("Empty Entity");
	m_propBrows = new QtTreePropertyBrowser;
	m_mainLayout = new QGridLayout();
	m_LastSelectedItem = nullptr;
	/*QPen pen(QColor("blue"));
	m_pixmap = new QPixmap(QSize(100, 20));
	m_painter = new QPainter(m_pixmap);
	m_painter->setPen(pen);*/
	//AbilityEditorNameSpace::Importer* importer = new AbilityEditorNameSpace::Importer();
#pragma warning (OY! Fixa så att saker faktikst laddas grafikst också när mna importar nåt)
//	importer->Import("test.ability", m_onCreate, m_onCollide, m_onDestroy);
}

void AbilityEditor::ChangedTab()
{
	int test = ui.abilityWidget->currentIndex();
	if(m_LastSelectedItem != nullptr)
	{
		
		if(m_LastSelectedTab == 0) //On Create list
			ui.treeOnCreate->SaveSelectedData(m_LastSelectedItem,m_propBrows);
		else if(m_LastSelectedTab == 1) //On Collide list
			ui.treeOnCollide->SaveSelectedData(m_LastSelectedItem,m_propBrows);
		else if(m_LastSelectedTab == 2) //On Destroy list
			ui.treeOnDestroy->SaveSelectedData(m_LastSelectedItem,m_propBrows);
	}
	m_LastSelectedItem = nullptr;
	delete m_propBrows;
	m_propBrows = nullptr;
	UpdatePropertyBrowser();
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
				ui.treeOnCreate->SaveSelectedData(m_LastSelectedItem,m_propBrows);
			else if(ui.abilityWidget->currentIndex() == 1 && ui.treeOnCollide->hasFocus()) //On Collide list
				ui.treeOnCollide->SaveSelectedData(m_LastSelectedItem,m_propBrows);
			else if(ui.abilityWidget->currentIndex() == 2 && ui.treeOnDestroy->hasFocus()) //On Destroy list
				ui.treeOnDestroy->SaveSelectedData(m_LastSelectedItem,m_propBrows);
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
			ui.treeOnCreate->ViewSelectedData(m_LastSelectedItem, m_propBrows);
		}
	}
	else if(ui.abilityWidget->currentIndex() == 1/* && ui.treeOnCollide->hasFocus()*/) //On Collide list
	{
		if(ui.treeOnCollide->selectedItems().count() == 0)
			return;
		if(ui.treeOnCollide->selectedItems().at(0) != nullptr)
		{
			m_LastSelectedItem = ui.treeOnCollide->selectedItems().at(0);
			ui.treeOnCollide->ViewSelectedData(m_LastSelectedItem, m_propBrows);
		}
	}
	else if(ui.abilityWidget->currentIndex() == 2/* && ui.treeOnDestroy->hasFocus()*/) //On Destroy list
	{
		if(ui.treeOnDestroy->selectedItems().count() == 0)
			return;
		if(ui.treeOnDestroy->selectedItems().at(0) != 0)
		{
			m_LastSelectedItem = ui.treeOnDestroy->selectedItems().at(0);
			ui.treeOnDestroy->ViewSelectedData(m_LastSelectedItem, m_propBrows);
		}
	}
	
	m_LastSelectedTab = ui.abilityWidget->currentIndex();

	m_mainLayout->addWidget(m_propBrows);
	ui.propertyWidget->setLayout(m_mainLayout);


}


/*
void AbilityEditor::mousePressEvent( QMouseEvent* event )
{
	if (event->button() == Qt::LeftButton && ui.listComponents->geometry().contains(event->pos())) 
	{
		QPoint hotSpot = event->pos() - childAt(event->pos())->pos();
		QDrag *drag = new QDrag(this);
		QMimeData *mimeData = new QMimeData;
		QString text = ui.listComponents->itemAt(event->pos())->text();
		mimeData->setText(text);
		drag->setMimeData(mimeData);
		m_painter->fillRect(QRect(0, 0, 100, 20), QBrush(QColor("orange")));
		m_painter->drawText(QRect(10, 0, 170, 20), text );
		drag->setPixmap(*m_pixmap);
		drag->setHotSpot(hotSpot);

		Qt::DropAction dropAction = drag->exec();
	}
}*/

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
									
					delete ui.treeOnCreate->currentItem();	
					ui.treeOnCreate->RemoveSelected(m_LastSelectedItem);
				}
			}
			else if(ui.abilityWidget->currentIndex() == 1 && ui.treeOnCollide->hasFocus()) //On Collide list
			{
				if(ui.treeOnCollide->currentItem() != 0)
				{
					
					delete ui.treeOnCollide->currentItem();
					ui.treeOnCollide->RemoveSelected(m_LastSelectedItem);	
				}
			}
			else if(ui.abilityWidget->currentIndex() == 2 && ui.treeOnDestroy->hasFocus()) //On Destroy list
			{
				if(ui.treeOnDestroy->currentItem() != 0)
				{
					delete ui.treeOnDestroy->currentItem();
					ui.treeOnDestroy->RemoveSelected(m_LastSelectedItem);	
				}
			}
			m_LastSelectedItem = nullptr;
		}
	}
	if(event->type() == QEvent::Drop)
		return false;
	return QWidget::event(event);
}
