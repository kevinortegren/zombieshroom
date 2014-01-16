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
	exporter->Export("Test.ability", m_onCreate, m_onCollide, m_onDestroy);
}

void AbilityEditor::Init()
{
	ui.setupUi(this);
	m_onCreate = new AbilityEditorNameSpace::OnCreate();
	m_onCollide =new AbilityEditorNameSpace::OnCollide();
	m_onDestroy= new AbilityEditorNameSpace::OnDestroy();
	ui.treeOnCollide->SetOnEventClass(m_onCreate);
	ui.treeOnCreate->SetOnEventClass(m_onCollide);
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

	connect(ui.treeOnCollide, SIGNAL(itemSelectionChanged()), this, SLOT(UpdatePropertyBrowser()));
	
	
	ui.listComponents->addItems(m_compNames);
	ui.listAbilities->addItem("Empty Entity");
	m_propBrows = new QtTreePropertyBrowser;
	m_mainLayout = new QGridLayout();


	/*QPen pen(QColor("blue"));
	m_pixmap = new QPixmap(QSize(100, 20));
	m_painter = new QPainter(m_pixmap);
	m_painter->setPen(pen);*/

}

void AbilityEditor::UpdatePropertyBrowser( )
{
	
	
	
	/*QTreeWidget* asdf = new QTreeWidget();
	asdf = ui.abilityWidget->widget(ui.abilityWidget->currentIndex());
	int bajs = asdf->selectedItems().count();*/
	if(m_propBrows != nullptr)
	{
		delete m_propBrows;
		m_propBrows = nullptr;
	}
	
	/*QWidget* page = static_cast<QWidget*>(ui.abilityWidget->children().at(ui.abilityWidget->currentIndex()));
	QTreeWidget* tree = static_cast<QTreeWidget*>(page->children().at(0));
	QTreeWidgetItem* p_item = tree->selectedItems().at(0);*/

	QTreeWidgetItem* p_item;

	if(ui.abilityWidget->currentIndex() == 0 && ui.treeOnCreate->hasFocus()) //On Create list
	{
		if(ui.treeOnCreate->selectedItems().count() == 0)
			return;
		if(ui.treeOnCreate->selectedItems().at(0) != nullptr)
			p_item = ui.treeOnCreate->selectedItems().at(0);
	}
	else if(ui.abilityWidget->currentIndex() == 1 && ui.treeOnCollide->hasFocus()) //On Collide list
	{
		if(ui.treeOnCollide->selectedItems().count() == 0)
			return;
		if(ui.treeOnCollide->selectedItems().at(0) != nullptr)
			p_item = ui.treeOnCollide->selectedItems().at(0);
	}
	else if(ui.abilityWidget->currentIndex() == 2 && ui.treeOnDestroy->hasFocus()) //On Destroy list
	{
		if(ui.treeOnDestroy->selectedItems().count() == 0)
			return;
		if(ui.treeOnDestroy->selectedItems().at(0) != 0)
			p_item = ui.treeOnDestroy->selectedItems().at(0);
	}
	
	m_propBrows = new QtTreePropertyBrowser(ui.propertyWidget);
	m_propBrows->setGeometry(ui.propertyWidget->geometry());

	m_mainLayout->addWidget(m_propBrows);
	ui.propertyWidget->setLayout(m_mainLayout);

	QtVariantPropertyManager* hej;
	hej = new QtVariantPropertyManager;

	QString temp = p_item->text(p_item->columnCount()-1);
	QtVariantProperty* prop, *prop1;
	prop = hej->addProperty(QVariant::String, "fafaf");
	prop1 = hej->addProperty(QVariant::String, "test");
	
	//prop->setPropertyName("test");
	hej->setValue(prop, temp);
	hej->setValue(prop1, temp);
	QtVariantEditorFactory* propFact;
	propFact = new QtVariantEditorFactory;
	m_propBrows->setFactoryForManager(hej, propFact);
	m_propBrows->addProperty(prop);
	m_propBrows->addProperty(prop1);
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
					delete ui.treeOnCreate->currentItem();
			}
			else if(ui.abilityWidget->currentIndex() == 1 && ui.treeOnCollide->hasFocus()) //On Collide list
			{
				if(ui.treeOnCollide->currentItem() != 0)
					delete ui.treeOnCollide->currentItem();
			}
			else if(ui.abilityWidget->currentIndex() == 2 && ui.treeOnDestroy->hasFocus()) //On Destroy list
			{
				if(ui.treeOnDestroy->currentItem() != 0)
					delete ui.treeOnDestroy->currentItem();
			}
		}
	}
	if(event->type() == QEvent::Drop)
		return false;
	return QWidget::event(event);
}



bool AbilityEditor::eventFilter( QObject * p_object, QEvent * p_event )
{
	if(p_object->objectName().compare(ui.tabOnCollide->objectName()) == 0 && p_event->type() == QEvent::Drop)
	{
		int test = 0;
		test*=2;
		//qDebug() << "Winning";
	}
	return true;
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
