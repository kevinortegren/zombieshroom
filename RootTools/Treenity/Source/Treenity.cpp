#include "Treenity.h"
#include <QtWidgets\qmessagebox.h>
#include <RootTools/Treenity/Include/Log.h>

Treenity::Treenity(QWidget *parent)
	: QMainWindow(parent), m_running(true), m_selectedEntity(nullptr)
{
	ui.setupUi(this);

	//Set some logging init things
	Log::GetInstance()->setParent(this);
	Log::GetInstance()->setFloating(true);

	QActionGroup* group = new QActionGroup(ui.toolBar);

	group->addAction(ui.actionTranslate);
	group->addAction(ui.actionRotate);
	group->addAction(ui.actionResize);

	ui.actionTranslate->setChecked(true);

	connect(ui.actionLog,			SIGNAL(triggered()), Log::GetInstance(),	SLOT(Show()));
	connect(ui.action_addEntity, SIGNAL(triggered()), this, SLOT(CreateEntity()));
	connect(ui.action_removeEntity, SIGNAL(triggered()), this, SLOT(DestroyEntity()));
	connect(ui.lineEdit_entityName, SIGNAL(editingFinished()), this, SLOT(RenameEntity()));
	connect(ui.treeView_entityOutliner, SIGNAL(itemSelectionChanged()), this, SLOT(OutlinerSelectEntity()));
}

Treenity::~Treenity()
{

}

void Treenity::SetEngineInterface(EngineInterface* p_engineInterface)
{
	m_engineInterface = p_engineInterface;
}

bool Treenity::IsRunning()
{
	return m_running;
}

void Treenity::closeEvent(QCloseEvent *event)
{
	m_running = false;
}


void Treenity::EntityCreated(ECS::Entity* p_entity)
{
	m_entityNames[p_entity] = "Unnamed";

	ui.treeView_entityOutliner->EntityCreated(p_entity, m_entityNames.find(p_entity)->second);
}

void Treenity::EntityDestroyed(ECS::Entity* p_entity)
{
	ui.treeView_entityOutliner->EntityDestroyed(p_entity);
}


void Treenity::CreateEntity()
{
	ECS::Entity* e = m_engineInterface->CreateEntity();
	Log::Write("Entity created! ID: " + QString::number(e->GetId()));
}

void Treenity::DestroyEntity()
{
	if (m_selectedEntity != nullptr)
	{
		m_engineInterface->DeleteEntity(m_selectedEntity);
	}
}

void Treenity::RenameEntity()
{
	if (m_selectedEntity != nullptr)
	{
		m_entityNames[m_selectedEntity] = ui.lineEdit_entityName->text();

		ui.treeView_entityOutliner->EntityRenamed(m_selectedEntity, ui.lineEdit_entityName->text());
	}
}

void Treenity::OutlinerSelectEntity()
{
	m_selectedEntity = ui.treeView_entityOutliner->GetSelectedEntity();
	QString name = m_entityNames.find(m_selectedEntity) != m_entityNames.end() ? m_entityNames.find(m_selectedEntity)->second : "";

	// Switch entity-dependent context.
	ui.lineEdit_entityName->setText(name);
}