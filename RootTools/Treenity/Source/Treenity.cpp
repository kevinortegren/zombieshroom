#include "Treenity.h"
#include <QtWidgets\qmessagebox.h>

Treenity::Treenity(QWidget *parent)
	: QMainWindow(parent), m_running(true)
{
	ui.setupUi(this);

	QActionGroup* group = new QActionGroup(ui.toolBar);

	group->addAction(ui.actionTranslate);
	group->addAction(ui.actionRotate);
	group->addAction(ui.actionResize);

	ui.actionTranslate->setChecked(true);

	connect(ui.action_addEntity, SIGNAL(triggered()), this, SLOT(CreateEntity()));
	connect(ui.action_removeEntity, SIGNAL(triggered()), this, SLOT(DestroyEntity()));
	connect(ui.lineEdit_entityName, SIGNAL(editingFinished()), this, SLOT(RenameEntity()));
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

	// Switch entity-dependent context.
	ui.lineEdit_entityName->setText(m_entityNames.find(m_selectedEntity)->second);
}