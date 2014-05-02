#include "Treenity.h"
#include <QtWidgets\qmessagebox.h>
#include <RootSystems/Include/ComponentTypes.h>

Treenity::Treenity(QWidget *parent)
	: QMainWindow(parent), m_running(true), m_selectedEntity(nullptr)
{
	m_componentNames[RootForce::ComponentType::TRANSFORM] = "Transform";
	m_componentNames[RootForce::ComponentType::RENDERABLE] = "Renderable";

	ui.setupUi(this);
	
	QWidget* transformWidget = new QWidget(ui.toolBox_components);
	SetupUIForComponent(transformWidget, RootForce::ComponentType::TRANSFORM);
	ui.toolBox_components->removeItem(0);
	ui.toolBox_components->addItem(transformWidget, m_componentNames[RootForce::ComponentType::TRANSFORM]);

	QActionGroup* group = new QActionGroup(ui.toolBar);

	group->addAction(ui.actionTranslate);
	group->addAction(ui.actionRotate);
	group->addAction(ui.actionResize);

	ui.actionTranslate->setChecked(true);

	connect(ui.action_addEntity, SIGNAL(triggered()), this, SLOT(CreateEntity()));
	connect(ui.action_removeEntity, SIGNAL(triggered()), this, SLOT(DestroyEntity()));
	connect(ui.lineEdit_entityName, SIGNAL(editingFinished()), this, SLOT(RenameEntity()));
	connect(ui.treeView_entityOutliner, SIGNAL(itemSelectionChanged()), this, SLOT(OutlinerSelectEntity()));
	connect(ui.action_renderable, SIGNAL(triggered()), this, SLOT(AddRenderable()));
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

void Treenity::EntityRemoved(ECS::Entity* p_entity)
{
	ui.treeView_entityOutliner->EntityRemoved(p_entity);
}

void Treenity::ComponentCreated(ECS::Entity* p_entity, int p_componentType)
{
	if (m_selectedEntity == p_entity)
	{
		QWidget* widget = new QWidget(ui.toolBox_components);
		SetupUIForComponent(widget, p_componentType);
		ui.toolBox_components->addItem(widget, m_componentNames[p_componentType]);
	}
}

void Treenity::ComponentRemoved(ECS::Entity* p_entity, int p_componentType)
{
	if (m_selectedEntity == p_entity)
	{
		for (int i = 0; i < ui.toolBox_components->count(); ++i)
		{
			if (ui.toolBox_components->widget(i)->windowTitle() == m_componentNames[p_componentType])
			{
				ui.toolBox_components->removeItem(i);
				break;
			}
		}
	}
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
	SelectEntity(ui.treeView_entityOutliner->GetSelectedEntity());
}

void Treenity::AddRenderable()
{
	if (m_selectedEntity != nullptr)
	{
		m_engineInterface->AddRenderable(m_selectedEntity);
	}
}

void Treenity::SelectEntity(ECS::Entity* p_entity)
{
	m_selectedEntity = p_entity;

	// Update the general properties.
	QString name = m_entityNames.find(m_selectedEntity) != m_entityNames.end() ? m_entityNames.find(m_selectedEntity)->second : "";
	ui.lineEdit_entityName->setText(name);

	// Update the component toolbox.
	
}

void Treenity::SetupUIForComponent(QWidget* p_widget, int p_componentType)
{
	switch (p_componentType)
	{
		case RootForce::ComponentType::TRANSFORM:
		{
			transformUI.setupUi(p_widget);
		} break;

		case RootForce::ComponentType::RENDERABLE:
		{
			renderableUI.setupUi(p_widget);
		} break;
	}
}