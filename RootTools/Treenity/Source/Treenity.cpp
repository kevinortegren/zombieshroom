#include "Treenity.h"
#include <QtWidgets\qmessagebox.h>
#include <RootSystems/Include/ComponentTypes.h>
#include <RootSystems/Include/Transform.h>
#include <RootSystems/Include/RenderingSystem.h>

#include <RootTools/Treenity/Include/Log.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <QFileDialog>

#include <RootTools/Treenity/Include/KeyHelper.h>

#include <QPushButton>
extern RootEngine::GameSharedContext g_engineContext;

Treenity::Treenity(QWidget *parent)
	: QMainWindow(parent), m_running(true)
{
	//Set some logging init things
	Log::GetInstance()->setParent(this);
	Log::GetInstance()->setFloating(true);

	// Setup component names in the editor.
	m_componentNames[RootForce::ComponentType::RENDERABLE] = "Renderable";
	m_componentNames[RootForce::ComponentType::TRANSFORM] = "Transform";
	m_componentNames[RootForce::ComponentType::POINTLIGHT] = "Point Light";
	m_componentNames[RootForce::ComponentType::CAMERA] = "Camera";
	m_componentNames[RootForce::ComponentType::HEALTH] = "Health";
	m_componentNames[RootForce::ComponentType::PLAYERCONTROL] = "Player Control";
	m_componentNames[RootForce::ComponentType::PHYSICS] = "Physics";
	m_componentNames[RootForce::ComponentType::NETWORK] = "Network";
	m_componentNames[RootForce::ComponentType::LOOKATBEHAVIOR] = "Look-At Behaviour";
	m_componentNames[RootForce::ComponentType::THIRDPERSONBEHAVIOR] = "Third Person Behaviour";
	m_componentNames[RootForce::ComponentType::SCRIPT] = "Script";
	m_componentNames[RootForce::ComponentType::COLLISION] = "Collision";
	m_componentNames[RootForce::ComponentType::COLLISIONRESPONDER] = "Collision Responder";
	m_componentNames[RootForce::ComponentType::PLAYER] = "Player";
	m_componentNames[RootForce::ComponentType::ANIMATION] = "Animation";
	m_componentNames[RootForce::ComponentType::PARTICLE] = "Particle";
	m_componentNames[RootForce::ComponentType::TDMRULES] = "Team-Deathmatch Rules";
	m_componentNames[RootForce::ComponentType::PLAYERACTION] = "Player Action";
	m_componentNames[RootForce::ComponentType::PLAYERPHYSICS] = "Player Physics";
	m_componentNames[RootForce::ComponentType::ENTITYSTATE] = "Entity State";
	m_componentNames[RootForce::ComponentType::SHADOWCASTER] = "Shadowcaster";
	m_componentNames[RootForce::ComponentType::DIRECTIONALLIGHT] = "Directional Light";
	m_componentNames[RootForce::ComponentType::SERVERINFORMATION] = "Server Information";
	m_componentNames[RootForce::ComponentType::CLIENT] = "Client";
	m_componentNames[RootForce::ComponentType::RAGDOLL] = "Ragdoll";
	m_componentNames[RootForce::ComponentType::WATERCOLLIDER] = "Water Collider";
	m_componentNames[RootForce::ComponentType::ABILITYSPAWN] = "Ability Spawn";
	m_componentNames[RootForce::ComponentType::TRYPICKUPCOMPONENT] = "Try Pickup";
	m_componentNames[RootForce::ComponentType::SOUND] = "Sound";
	m_componentNames[RootForce::ComponentType::TIMER] = "Timer";
	m_componentNames[RootForce::ComponentType::FOLLOW] = "Follow";
	m_componentNames[RootForce::ComponentType::HOMING] = "Homing";
	m_componentNames[RootForce::ComponentType::RAY] = "Ray";
	m_componentNames[RootForce::ComponentType::DAMAGEANDKNOCKBACK] = "Damage and Knockback";
	m_componentNames[RootForce::ComponentType::SCALABLE] = "Scalable";
	m_componentNames[RootForce::ComponentType::STATCHANGE] = "Stat Change";
	m_componentNames[RootForce::ComponentType::KILLANNOUNCEMENT] = "Kill Announcement";
	m_componentNames[RootForce::ComponentType::CONTROLLERACTIONS] = "Controller Action";

	// Setup the main UI.
	ui.setupUi(this);

	// Setup the component view and its items.
	m_compView = new ComponentView();
	ui.verticalLayout->addWidget(m_compView);
	m_componentViews[RootForce::ComponentType::TRANSFORM] = new TransformView();
	m_componentViews[RootForce::ComponentType::RENDERABLE] = new RenderableView();
	m_componentViews[RootForce::ComponentType::PHYSICS] = new PhysicsView();


	for (auto it : m_componentViews)
	{
		it.second->SetEditorInterface(this);
	}
	
	ui.treeView_entityOutliner->SetEditorInterface(this);

	// Match signals with slots.
	connect(ui.actionNew,							SIGNAL(triggered()), this,				SLOT(New()));
	connect(ui.actionOpen_Project,					SIGNAL(triggered()), this,				SLOT(OpenProject()));
	connect(ui.action_saveAs,						SIGNAL(triggered()), this,				SLOT(SaveAs()));
	connect(ui.action_save,							SIGNAL(triggered()), this,				SLOT(Save()));
	connect(ui.actionExit,							SIGNAL(triggered()), this,				SLOT(close()));
	connect(ui.actionLog,							SIGNAL(triggered()), Log::GetInstance(), SLOT(Show()));
	connect(ui.action_addEntity,					SIGNAL(triggered()), this,				SLOT(CreateEntity()));
	connect(ui.action_removeEntity,					SIGNAL(triggered()), this,				SLOT(DestroyEntity()));
	connect(ui.lineEdit_entityName,					SIGNAL(editingFinished()), this,		SLOT(RenameEntity()));
	connect(ui.action_addRenderable,				SIGNAL(triggered()), this,				SLOT(AddRenderable()));
	connect(ui.action_addPhysics,					SIGNAL(triggered()), this,				SLOT(AddPhysics()));
	
	// Setup Qt-to-SDL keymatching.
	InitialiseKeymap();
}

Treenity::~Treenity()
{

}

void Treenity::SetEngineInterface(EngineInterface* p_engineInterface)
{
	m_engineInterface = p_engineInterface;

	ui.treeView_entityOutliner->SetEngineInterface(m_engineInterface);
	for (auto it : m_componentViews)
	{
		it.second->SetEngineInterface(p_engineInterface);
	}
}

void Treenity::SetProjectManager(ProjectManager* p_projectManager)
{
	m_projectManager = p_projectManager;
}

void Treenity::CreateNewScene()
{
	New();
}

void Treenity::UpdateWindowTitle()
{
	setWindowTitle("Treenity - " + m_currentProjectFile);
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
	ui.treeView_entityOutliner->EntityCreated(p_entity, m_projectManager->GetEntityName(p_entity));

	Log::Write("Entity added: " + QString::number(p_entity->GetId()));
}

void Treenity::EntityRemoved(ECS::Entity* p_entity)
{
	ui.treeView_entityOutliner->EntityRemoved(p_entity);
}

void Treenity::ComponentCreated(ECS::Entity* p_entity, int p_componentType)
{
	if (m_selectedEntities.find(p_entity) != m_selectedEntities.end() && m_selectedEntities.size() == 1)
	{
		m_compView->AddItem(new ComponentViewItem(m_componentViews[p_componentType]));

		//QWidget* widget = new QWidget();
		//SetupUIForComponent(widget, p_componentType);
		//m_compView->AddItem(new ComponentViewItem(m_componentNames[p_componentType], widget));
	}
}

void Treenity::ComponentRemoved(ECS::Entity* p_entity, int p_componentType)
{
	if (m_selectedEntities.find(p_entity) != m_selectedEntities.end() && m_selectedEntities.size() == 1)
	{
		m_compView->RemoveItem(m_componentNames[p_componentType]);
	}
}

void Treenity::TagAdded(ECS::Entity* p_entity, const std::string& p_tag)
{
	ui.treeView_entityOutliner->TagAdded(p_entity, p_tag);
}

void Treenity::TagRemoved(ECS::Entity* p_entity, const std::string& p_tag)
{
	ui.treeView_entityOutliner->TagRemoved(p_entity, p_tag);
}

void Treenity::EntityAddedToGroup(ECS::Entity* p_entity, const std::string& p_group)
{
	ui.treeView_entityOutliner->EntityAddedToGroup(p_entity, p_group);
}

void Treenity::EntityRemovedFromGroup(ECS::Entity* p_entity, const std::string& p_group)
{
	ui.treeView_entityOutliner->EntityRemovedFromGroup(p_entity, p_group);
}

void Treenity::CreateOpenGLContext()
{
	ui.widget_canvas3D->CreateOpenGLContext();
	g_engineContext.m_logger->LogText(LogTag::TOOLS, LogLevel::START_PRINT, "Creating OpenGL context in treenity.");
}

void Treenity::New()
{
	m_engineInterface->NewScene();
}

void Treenity::OpenProject()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Open Project", "", "World File (*.world)");

	m_currentProjectFile = fileName;

	UpdateWindowTitle();

	m_engineInterface->ClearScene();
	
	m_projectManager->Import(fileName);


	m_engineInterface->AddDefaultEntities();
	m_engineInterface->InitializeScene();
}

void Treenity::Save()
{
	m_projectManager->Export(m_currentProjectFile);
}

void Treenity::SaveAs()
{
	QString fileName = QFileDialog::getSaveFileName(this, "Save Project", "", "World File (*.world)");

	m_currentProjectFile = fileName;

	UpdateWindowTitle();

	m_projectManager->Export(fileName);
}

void Treenity::Select(ECS::Entity* p_entity)
{
	m_selectedEntities.clear();

	if(p_entity != nullptr)
		m_selectedEntities.insert(p_entity);

	UpdateOnSelection();
}

void Treenity::Select(const std::set<ECS::Entity*>& p_entities)
{
	m_selectedEntities = p_entities;

	UpdateOnSelection();
}

void Treenity::AddToSelection(ECS::Entity* p_entity)
{
	m_selectedEntities.insert(p_entity);

	UpdateOnSelection();
}

void Treenity::ClearSelection()
{
	m_selectedEntities.clear();

	UpdateOnSelection();
}

const std::set<ECS::Entity*>& Treenity::GetSelection() const
{
	return m_selectedEntities;
}

void Treenity::RenameEntity(ECS::Entity* p_entity, const std::string& p_name)
{
	ui.treeView_entityOutliner->EntityRenamed(p_entity, QString::fromStdString(p_name));
	m_projectManager->SetEntityName(p_entity, QString::fromStdString(p_name));
}

void Treenity::CreateEntity()
{
	ECS::Entity* e = m_engineInterface->CreateEntity();
}

void Treenity::DestroyEntity()
{
	for (auto entity : m_selectedEntities)
	{
		m_engineInterface->DeleteEntity(entity);
	}

	ClearSelection();
}

void Treenity::RenameEntity()
{
	for (auto entity : m_selectedEntities)
	{
		m_projectManager->SetEntityName(entity, ui.lineEdit_entityName->text());
		
		ui.treeView_entityOutliner->EntityRenamed(entity, ui.lineEdit_entityName->text());
	}
}

void Treenity::AddRenderable()
{
	if (m_selectedEntities.size() == 1)
	{
		m_engineInterface->AddRenderable(*m_selectedEntities.begin());
	}
}

void Treenity::AddPhysics()
{
	if(m_selectedEntities.size() == 1)
	{
		m_engineInterface->AddPhysics(*m_selectedEntities.begin());
	}
}

void Treenity::UpdateOnSelection()
{
	ui.treeView_entityOutliner->SetCurrentItems(m_selectedEntities);

	if (m_selectedEntities.size() == 0)
	{
		// Disable name entry.
		ui.lineEdit_entityName->setText("");
		ui.lineEdit_entityName->setEnabled(false);

		// Clear component list.
		m_compView->RemoveItems();

		ui.treeView_entityOutliner->clearSelection();

	}
	else if (m_selectedEntities.size() == 1)
	{
		ECS::Entity* selectedEntity = *m_selectedEntities.begin();

		//ui.treeView_entityOutliner->setS

		// Enable and print name.
		QString name = m_projectManager->GetEntityName(selectedEntity);
		ui.lineEdit_entityName->setEnabled(true);
		ui.lineEdit_entityName->setText(name);

		// Show all components in the component list.
		m_compView->RemoveItems();

		uint64_t flag = selectedEntity->GetFlag();
		for (int i = 0; i < 64; ++i)
		{
			uint64_t mask = 1ULL << i;
			if ((flag & mask) != 0)
			{
				if (m_componentViews.find(i) != m_componentViews.end())
				{
					m_compView->AddItem(new ComponentViewItem(m_componentViews[i]));
					m_componentViews[i]->DisplayEntity(selectedEntity);
				}
			}
		}
	}
	else if (m_selectedEntities.size() > 1)
	{
		// Enable name, allow all names to be changed.
		ui.lineEdit_entityName->setText("");
		ui.lineEdit_entityName->setEnabled(true);

		// Clear component list (potential change in future, show transforms).
		m_compView->RemoveItems();
	}
}

void Treenity::keyPressEvent( QKeyEvent* event )
{
	if(event->key() == 70)
	{
		if(m_selectedEntities.size() > 0)
		m_engineInterface->TargetEntity(*m_selectedEntities.begin());
	}
	//g_engineContext.m_logger->LogText(LogTag::INPUT, LogLevel::PINK_PRINT, "Key pressed: %d", event->key() );
}

void Treenity::keyReleaseEvent( QKeyEvent* event )
{
	//g_engineContext.m_logger->LogText(LogTag::INPUT, LogLevel::HELP_PRINT, "Key released: %d", event->key() );
}

void Treenity::Init()
{
	m_assetManagerWidget = new AssetManagerWidget();
	ui.verticalLayout_assetManager->addWidget(m_assetManagerWidget);
}
