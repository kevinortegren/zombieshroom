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
	
	m_conditions.push_back(new AbilityEditorNameSpace::Condition("if collide with enemy player", "if type == PhysicsType.TYPE_PLAYER and abilityOwnerPlayerComponent:GetTeamId() ~= targetPlayerComponent:GetTeamId() then"));
	m_conditions.push_back(new AbilityEditorNameSpace::Condition("if collide with friendly player", "if type == PhysicsType.TYPE_PLAYER and abilityOwnerPlayerComponent:GetTeamId() == targetPlayerComponent:GetTeamId() then"));
}

AbilityEditor::~AbilityEditor()
{

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

	m_entity = new AbilityEditorNameSpace::Entity("NewAbility");

	ui.listComponents->addItems(m_compNames);
	for (unsigned int i = 0; i < m_conditions.size(); i++)
	{
		QListWidgetItem* item = new QListWidgetItem();
		item->setText(m_conditions.at(i)->GetName());
		item->setToolTip(m_conditions.at(i)->GetCode());
		ui.listConditions->addItem(item);
	}
	
	//ui.listAbilities->addItem("Empty Entity");
	m_propBrows = new QtTreePropertyBrowser;
	m_propMan = new QtVariantPropertyManager;
	m_mainLayout = new QGridLayout();
	m_LastSelectedItem = nullptr;


	// File View Tree Test
	QStringList filters;
	filters << "*.lua";
	QString path = QDir::currentPath()+"/../../Debug/Assets/Scripts";
	m_fileViewModel = new QFileSystemModel;
	m_fileViewModel->setRootPath(path);
	m_fileViewModel->setNameFilters(filters);
	ui.listEntities->setModel(m_fileViewModel);
	ui.listEntities->setRootIndex(m_fileViewModel->index(path));
	ui.listEntities->setColumnHidden(1, true);
	ui.listEntities->setColumnHidden(2, true);
	ui.listEntities->setColumnHidden(3, true);
	
	//End Test

	ui.treeOnCreate->SetOnEventClass(m_onCreate);
	ui.treeOnCollide->SetOnEventClass(m_onCollide);
	ui.treeOnDestroy->SetOnEventClass(m_onDestroy);
	ui.listAbilityComponents->SetEntity(m_entity);
	this->setFixedSize(this->size());
	this->statusBar()->setSizeGripEnabled(false);
	
	connect(ui.listAbilityComponents, SIGNAL(itemSelectionChanged()), this, SLOT(UpdatePropertyBrowser()));
	//connect(ui.treeOnCreate, SIGNAL(itemSelectionChanged()), this, SLOT(UpdatePropertyBrowser()));
	//connect(ui.treeOnCollide, SIGNAL(itemSelectionChanged()), this, SLOT(UpdatePropertyBrowser()));
	//connect(ui.treeOnDestroy, SIGNAL(itemSelectionChanged()), this, SLOT(UpdatePropertyBrowser()));

	//connect(ui.OnTabWidget, SIGNAL(currentChanged(int)), this, SLOT(ChangedTab()));
	//connect(m_propMan, SIGNAL(valueChanged(QtProperty*, const QVariant&)), this, SLOT(ChangedTab()));
	connect(ui.actionGenerate_Script, SIGNAL(triggered()), this, SLOT(GenerateScript()));
	connect(ui.actionSave_As, SIGNAL(triggered()), this, SLOT(SaveAs()));
	connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(Save()));
	connect(ui.actionLoad, SIGNAL(triggered()), this, SLOT(Load()));
	connect(ui.listEntities, SIGNAL(pressed(const QModelIndex&)), this, SLOT(FileViewDrag(const QModelIndex&)));
	//connect(ui.actionEntity, SIGNAL(triggered()), this, SLOT(AddNewEntity()));
	connect(ui.buttonNameBrowser, SIGNAL(pressed()), this, SLOT(BrowseName()));
	connect(ui.abilityNameEdit, SIGNAL(editingFinished()), this, SLOT(ChangeAbilityName()));
	
	connect(ui.cooldownSpinbox, SIGNAL(valueChanged(double)), this, SLOT(SetCooldown(double)));
	connect(ui.chargesSpinbox, SIGNAL(valueChanged(int)), this, SLOT(SetCharges(int)));
	connect(ui.chargingSpinbox, SIGNAL(valueChanged(double)), this, SLOT(SetChargeTime(double)));
	connect(ui.channelingSpingbox, SIGNAL(valueChanged(double)), this, SLOT(SetChannelingTime(double)));
	connect(ui.durationSpinbox, SIGNAL(valueChanged(double)), this, SLOT(SetDuration(double)));

}


void AbilityEditor::UpdatePropertyBrowser( )
{
	//QTreeWidget* asdf = new QTreeWidget();
	//asdf = ui.abilityWidget->widget(ui.abilityWidget->currentIndex());
	//int bajs = asdf->selectedItems().count();
	if(m_propBrows != nullptr)
	{
		if(m_LastSelectedItem != nullptr)
		{
			ui.listAbilityComponents->SaveSelectedData(m_LastSelectedItem, m_propMan, m_propBrows);
		}
		delete m_propBrows;
		m_propBrows = nullptr;
		m_LastSelectedItem = nullptr;
	}

	//QWidget* page = static_cast<QWidget*>(ui.abilityWidget->children().at(ui.abilityWidget->currentIndex()));
	//QTreeWidget* tree = static_cast<QTreeWidget*>(page->children().at(0));
	//QTreeWidgetItem* m_LastSelectedItem = tree->selectedItems().at(0);
	m_propBrows = new QtTreePropertyBrowser(ui.propertyWidget);
	m_propBrows->setGeometry(ui.propertyWidget->geometry());


	if(ui.listAbilityComponents->selectedItems().count() == 0)
		return;
	if(ui.listAbilityComponents->selectedItems().at(0) != nullptr)
	{
		m_LastSelectedItem = ui.listAbilityComponents->selectedItems().at(0);
		ui.listAbilityComponents->ViewSelectedData(m_LastSelectedItem, m_propMan, m_propBrows);
	}

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
			if (ui.listAbilityComponents->hasFocus())
			{
				ui.listAbilityComponents->RemoveSelected(ui.listAbilityComponents->currentItem());
			}
			if(ui.OnTabWidget->currentIndex() == 0 && ui.treeOnCreate->hasFocus()) //On Create list
			{
				if(ui.treeOnCreate->currentItem() != 0)
				{
					ui.treeOnCreate->RemoveSelected(ui.treeOnCreate->currentItem());
					//delete ui.treeOnCreate->currentItem();	
					
				}
			}
			else if(ui.OnTabWidget->currentIndex() == 1 && ui.treeOnCollide->hasFocus()) //On Collide list
			{
				if(ui.treeOnCollide->currentItem() != 0)
				{
					ui.treeOnCollide->RemoveSelected(ui.treeOnCollide->currentItem());
					//delete ui.treeOnCollide->currentItem();
					
				}
			}
			else if(ui.OnTabWidget->currentIndex() == 2 && ui.treeOnDestroy->hasFocus()) //On Destroy list
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
		m_scriptGenerator->GenerateScript(path, name, m_entity, m_onCreate, m_onCollide, m_onDestroy);
	}
}

void AbilityEditor::SaveAs()
{
	QFileDialog dial(this);
	dial.setFileMode(QFileDialog::AnyFile);
	dial.setAcceptMode(QFileDialog::AcceptSave);
	m_currentSavePath = dial.getSaveFileName();
	if(m_currentSavePath.compare("") != 0)
		m_exporter->Export(m_currentSavePath.toStdString(), m_entity, m_onCreate, m_onCollide, m_onDestroy);
}

void AbilityEditor::Save()
{
	if(m_currentSavePath.compare("") != 0)
		m_exporter->Export(m_currentSavePath.toStdString(), m_entity, m_onCreate, m_onCollide, m_onDestroy);
}

void AbilityEditor::Load()
{
	QFileDialog dial(this);
	dial.setFileMode(QFileDialog::AnyFile);
	dial.setAcceptMode(QFileDialog::AcceptOpen);
	m_currentSavePath = dial.getOpenFileName();
	if(m_currentSavePath.compare("") != 0)
	{
		//Clear existing values
		ui.listAbilityComponents->Clear();
		ui.treeOnCreate->Clear();
		ui.treeOnCollide->Clear();
		ui.treeOnDestroy->Clear();
		//Import new values
		m_importer->Import(m_currentSavePath.toStdString(), m_entity, m_onCreate, m_onCollide, m_onDestroy);
		//Set new values
		ui.listAbilityComponents->LoadData();
		ui.treeOnCreate->LoadData();
		ui.treeOnCollide->LoadData();
		ui.treeOnDestroy->LoadData();
		//Set name in UI
		ui.abilityNameEdit->setText(m_entity->GetName());
		//Set cooldown in UI
		ui.cooldownSpinbox->setValue(m_entity->GetCooldown());
		//Set charges in UI
		ui.chargesSpinbox->setValue(m_entity->GetCharges());
		//Set charge time in UI
		ui.chargingSpinbox->setValue(m_entity->GetChargeTime());
		//Set channeling time in UI
		ui.channelingSpingbox->setValue(m_entity->GetChannelingTime());
		//Set duration in UI
		ui.durationSpinbox->setValue(m_entity->GetDuration());
	}
}

void AbilityEditor::FileViewDrag( const QModelIndex& p_modelIndex )
{
	QString type = "Entity";
	QPixmap pixeimap(QSize(100,20));
	QPainter painter(&pixeimap);
	painter.setPen(QPen(QColor("blue")));
	QDrag* drag = new QDrag(this);
	QMimeData* data = new QMimeData;
	data->setText(m_fileViewModel->fileInfo(p_modelIndex).fileName());
	data->setObjectName(type);
	drag->setMimeData(data);
	painter.drawText(QRect(10, 0, 170, 20), data->text());
	drag->setPixmap(pixeimap);
	Qt::DropAction dropAction = drag->exec();
}

void AbilityEditor::BrowseName()
{
	QFileDialog dial(this);
	dial.setFileMode(QFileDialog::AnyFile);
	dial.setAcceptMode(QFileDialog::AcceptOpen);
	QString asdf = dial.getOpenFileName();
	if(asdf.compare("") != 0)
	{
		QString name = &(asdf.toStdString().at(asdf.lastIndexOf("/")+1));
		name.chop(name.size()-name.lastIndexOf("."));
		m_propMan->setValue(m_propBrows->currentItem()->property(), name);
	}
}

void AbilityEditor::ChangeAbilityName()
{
	m_entity->SetName(ui.abilityNameEdit->text());
}

void AbilityEditor::SetCooldown( double p_cooldown )
{
	m_entity->SetCooldown((float)p_cooldown);
}

void AbilityEditor::SetCharges( int p_charges )
{
	m_entity->SetCharges(p_charges);
}

void AbilityEditor::SetChargeTime( double p_chargeTime )
{
	m_entity->SetChargeTime((float)p_chargeTime);
}

void AbilityEditor::SetChannelingTime( double p_channelingTime )
{
	m_entity->SetChannelingTime((float)p_channelingTime);
}

void AbilityEditor::SetDuration( double p_duration )
{
	m_entity->SetDuration((float)p_duration);
}