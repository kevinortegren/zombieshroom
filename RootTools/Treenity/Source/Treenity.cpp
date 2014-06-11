#include "Treenity.h"
#include <QtWidgets\qmessagebox.h>
#include <RootSystems/Include/ComponentTypes.h>
#include <RootSystems/Include/Transform.h>
#include <RootSystems/Include/RenderingSystem.h>

#include <RootTools/Treenity/Include/Utils.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/InputManager/Include/InputManager.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <QFileDialog>

#include <RootTools/Treenity/Include/KeyHelper.h>

#include <QPushButton>
#include <QScrollArea>
#include <QShortcut>
#include <QInputDialog>
#include <RootTools/Treenity/Include/Components/ParticleView.h>

extern RootEngine::GameSharedContext g_engineContext;

Treenity::Treenity(QWidget *parent)
	: QMainWindow(parent), m_running(true)
{
	//Set some utility init things
	Utils::GetInstance()->setParent(this);
	Utils::GetInstance()->setFloating(true);

	// Setup component names in the editor.
	m_componentNames[RootForce::ComponentType::RENDERABLE]			= "Renderable";
	m_componentNames[RootForce::ComponentType::TRANSFORM]			= "Transform";
	m_componentNames[RootForce::ComponentType::POINTLIGHT]			= "Point Light";
	m_componentNames[RootForce::ComponentType::CAMERA]				= "Camera";
	m_componentNames[RootForce::ComponentType::HEALTH]				= "Health";
	m_componentNames[RootForce::ComponentType::PLAYERCONTROL]		= "Player Control";
	m_componentNames[RootForce::ComponentType::PHYSICS]				= "Physics";
	m_componentNames[RootForce::ComponentType::NETWORK]				= "Network";
	m_componentNames[RootForce::ComponentType::LOOKATBEHAVIOR]		= "Look-At Behaviour";
	m_componentNames[RootForce::ComponentType::THIRDPERSONBEHAVIOR] = "Third Person Behaviour";
	m_componentNames[RootForce::ComponentType::SCRIPT]				= "Script";
	m_componentNames[RootForce::ComponentType::COLLISION]			= "Physics";	// Required since the component list searches on name.
	m_componentNames[RootForce::ComponentType::COLLISIONRESPONDER]	= "Collision Responder";
	m_componentNames[RootForce::ComponentType::PLAYER]				= "Player";
	m_componentNames[RootForce::ComponentType::ANIMATION]			= "Animation";
	m_componentNames[RootForce::ComponentType::PARTICLE]			= "Particle";
	m_componentNames[RootForce::ComponentType::TDMRULES]			= "Team-Deathmatch Rules";
	m_componentNames[RootForce::ComponentType::PLAYERACTION]		= "Player Action";
	m_componentNames[RootForce::ComponentType::PLAYERPHYSICS]		= "Player Physics";
	m_componentNames[RootForce::ComponentType::ENTITYSTATE]			= "Entity State";
	m_componentNames[RootForce::ComponentType::SHADOWCASTER]		= "Shadowcaster";
	m_componentNames[RootForce::ComponentType::DIRECTIONALLIGHT]	= "Directional Light";
	m_componentNames[RootForce::ComponentType::SERVERINFORMATION]	= "Server Information";
	m_componentNames[RootForce::ComponentType::CLIENT]				= "Client";
	m_componentNames[RootForce::ComponentType::RAGDOLL]				= "Ragdoll";
	m_componentNames[RootForce::ComponentType::WATERCOLLIDER]		= "Water Collider";
	m_componentNames[RootForce::ComponentType::ABILITYSPAWN]		= "Ability Spawn";
	m_componentNames[RootForce::ComponentType::TRYPICKUPCOMPONENT]	= "Try Pickup";
	m_componentNames[RootForce::ComponentType::SOUND]				= "Sound";
	m_componentNames[RootForce::ComponentType::TIMER]				= "Timer";
	m_componentNames[RootForce::ComponentType::FOLLOW]				= "Follow";
	m_componentNames[RootForce::ComponentType::HOMING]				= "Homing";
	m_componentNames[RootForce::ComponentType::RAY]					= "Ray";
	m_componentNames[RootForce::ComponentType::DAMAGEANDKNOCKBACK]	= "Damage and Knockback";
	m_componentNames[RootForce::ComponentType::SCALABLE]			= "Scalable";
	m_componentNames[RootForce::ComponentType::STATCHANGE]			= "Stat Change";
	m_componentNames[RootForce::ComponentType::KILLANNOUNCEMENT]	= "Kill Announcement";
	m_componentNames[RootForce::ComponentType::CONTROLLERACTIONS]	= "Controller Action";

	// Setup the main UI.
	ui.setupUi(this);

	setCorner( Qt::TopRightCorner, Qt::RightDockWidgetArea );
	setCorner( Qt::BottomRightCorner, Qt::RightDockWidgetArea );

	m_compView = new ComponentView();
	m_scrollArea = new VerticalScrollArea();
	m_scrollArea->setWidget(m_compView);
	m_scrollArea->setFrameStyle(QFrame::Plain);
	// Setup the component view and its items.
	
	ui.verticalLayout->addWidget(m_scrollArea);
	m_componentViews[RootForce::ComponentType::TRANSFORM]			= new TransformView();
	m_componentViews[RootForce::ComponentType::RENDERABLE]			= new RenderableView();	
	m_componentViews[RootForce::ComponentType::COLLISION]			= new PhysicsView();
	m_componentViews[RootForce::ComponentType::WATERCOLLIDER]		= new WaterColliderView();
	m_componentViews[RootForce::ComponentType::SCRIPT]				= new ScriptView();
	m_componentViews[RootForce::ComponentType::COLLISIONRESPONDER]	= new CollisionResponderView();
	m_componentViews[RootForce::ComponentType::PARTICLE]			= new ParticleView();

	// Block component views from updates while in game mode.
	m_componentViews[RootForce::ComponentType::RENDERABLE]->SetReceiveUpdates(false);

	for (auto it : m_componentViews)
	{
		it.second->SetEditorInterface(this);
	}
	
	ui.treeView_entityOutliner->SetEditorInterface(this);

	//Set up water dialog
	m_waterToolDockable = new WaterTool(this);

	//Set up terrin dialogs
	m_terrainDialog = new TerrainDialog(this);
	m_terrainDialog->SetEditorInterface(this);

	m_terrainTextureDialog = new TerrainTextureDialog(this);
	m_terrainTextureDialog->SetEditorInterface(this);

	// Match signals with slots.
	connect(ui.actionNew,							SIGNAL(triggered()),		this,					SLOT(New()));
	connect(ui.actionOpen_Project,					SIGNAL(triggered()),		this,					SLOT(OpenProject()));
	connect(ui.action_saveAs,						SIGNAL(triggered()),		this,					SLOT(SaveAs()));
	connect(ui.action_save,							SIGNAL(triggered()),		this,					SLOT(Save()));
	connect(ui.actionExit,							SIGNAL(triggered()),		this,					SLOT(close()));
	connect(ui.actionLog,							SIGNAL(triggered()),		Utils::GetInstance(),	SLOT(Show()));
	connect(ui.action_addEntity,					SIGNAL(triggered()),		this,					SLOT(CreateEntity()));
	connect(ui.action_removeEntity,					SIGNAL(triggered()),		this,					SLOT(DestroyEntity()));
	connect(ui.lineEdit_entityName,					SIGNAL(editingFinished()),	this,					SLOT(RenameEntity()));
	connect(ui.action_addRenderable,				SIGNAL(triggered()),		this,					SLOT(AddRenderable()));
	connect(ui.action_addPhysics,					SIGNAL(triggered()),		this,					SLOT(AddPhysics()));
	connect(ui.action_addWaterCollider,				SIGNAL(triggered()),		this,					SLOT(AddWaterCollider()));
	connect(ui.action_addScript,					SIGNAL(triggered()),		this,					SLOT(AddScriptComponent()));
	connect(ui.action_collisionResponder,			SIGNAL(triggered()),		this,					SLOT(AddCollisionResponder()));
	connect(ui.action_addParticle,					SIGNAL(triggered()),		this,					SLOT(AddParticleComponent()));
	connect(ui.pushButton_playMode,					SIGNAL(clicked()),			this,					SLOT(Play()));
	connect(ui.pushButton_translateMode,			SIGNAL(clicked()),			this,					SLOT(SetTranslateTool()));
	connect(ui.pushButton_rotateMode,				SIGNAL(clicked()),			this,					SLOT(SetRotateTool()));
	connect(ui.pushButton_scaleMode,				SIGNAL(clicked()),			this,					SLOT(SetResizeTool()));
	connect(ui.pushButton_terrainGeometryMode,		SIGNAL(clicked()),			this,					SLOT(SetTerrainGeometryTool()));
	connect(ui.pushButton_terrainPaintMode,			SIGNAL(clicked()),			this,					SLOT(SetTerrainTextureTool()));
	connect(ui.comboBox_mode,						SIGNAL(currentIndexChanged(int)), this,				SLOT(ChangeToolMode(int)));
	connect(ui.actionWaterSetting,					SIGNAL(triggered()),		m_waterToolDockable,	SLOT(Show()));
	connect(ui.actionAdd_terrain,					SIGNAL(triggered()),		this,					SLOT(AddTerrain()));
	connect(ui.actionTerrain_model,					SIGNAL(triggered()),		this,					SLOT(ExportTerrainModel()));


	connect(m_componentViews[RootForce::ComponentType::RENDERABLE],			SIGNAL(deleted(ECS::Entity*)), this, SLOT(RemoveRenderable(ECS::Entity*)));
	connect(m_componentViews[RootForce::ComponentType::COLLISION],			SIGNAL(deleted(ECS::Entity*)), this, SLOT(RemovePhysics(ECS::Entity*))); 
	connect(m_componentViews[RootForce::ComponentType::WATERCOLLIDER],		SIGNAL(deleted(ECS::Entity*)), this, SLOT(RemoveWaterCollider(ECS::Entity*))); 
	connect(m_componentViews[RootForce::ComponentType::SCRIPT],				SIGNAL(deleted(ECS::Entity*)), this, SLOT(RemoveScriptComponent(ECS::Entity*))); 
	connect(m_componentViews[RootForce::ComponentType::COLLISIONRESPONDER], SIGNAL(deleted(ECS::Entity*)), this, SLOT(RemoveCollisionResponder(ECS::Entity*))); 

	// Setup Qt-to-SDL keymatching.
	InitialiseKeymap();

	ui.widget_canvas3D->SetEditorInterface(this);
	// Set tool mode.
	m_toolMode = ToolMode::LOCAL;

	//Automatically cleaned up at program exi
	new QShortcut(QKeySequence(Qt::Key_F), this, SLOT(FocusEntity()));

	m_terrainTextureBrush = new TerrainTextureBrush();
	m_terrainTextureBrush->SetSize(10);
	m_terrainGeometryBrush = new TerrainGeometryBrush();

}


Treenity::~Treenity()
{
	delete m_terrainTextureBrush;
	delete m_terrainGeometryBrush;
}

void Treenity::SetEngineInterface(EngineInterface* p_engineInterface)
{
	m_engineInterface = p_engineInterface;

	ui.treeView_entityOutliner->SetEngineInterface(m_engineInterface);
	for (auto it : m_componentViews)
	{
		it.second->SetEngineInterface(p_engineInterface);
	}
	ui.widget_canvas3D->SetEngineInterface(p_engineInterface);
	m_waterToolDockable->SetEngineInterface(p_engineInterface);
	m_terrainDialog->SetEngineInterface(p_engineInterface);
	m_terrainTextureDialog->SetEngineInterface(p_engineInterface);
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

	Utils::Write("Entity added: " + QString::number(p_entity->GetId()));

	Select(p_entity);
}

void Treenity::EntityRemoved(ECS::Entity* p_entity)
{
	ui.treeView_entityOutliner->EntityRemoved(p_entity);
}

void Treenity::ComponentCreated(ECS::Entity* p_entity, int p_componentType)
{
	if (m_selectedEntities.find(p_entity) != m_selectedEntities.end() && m_selectedEntities.size() == 1)
	{
		// Temporary solution to deal with physics and collider
		if(p_componentType == RootForce::ComponentType::PHYSICS)
			return;

		if (m_componentViews.find(p_componentType) != m_componentViews.end())
		{
			m_compView->AddItem(m_componentViews[p_componentType]);
			m_componentViews[p_componentType]->DisplayEntity(p_entity);
		}
	}
}

void Treenity::ComponentRemoved(ECS::Entity* p_entity, int p_componentType)
{
	if (m_selectedEntities.find(p_entity) != m_selectedEntities.end() && m_selectedEntities.size() == 1)
	{
		// Temporary solution to deal with physics and collider
		if(p_componentType == RootForce::ComponentType::PHYSICS)
			return;

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

void Treenity::InitializeTools(ECS::World* p_world)
{
	m_toolManager.Initialize(p_world);
	m_toolManager.SetEditorInterface(this);
	m_toolManager.SetTool(ToolBox::TRANSLATION_TOOL);
}

//TODO: Rename.
void Treenity::CreateOpenGLContext()
{
	ui.widget_canvas3D->CreateOpenGLContext();
	g_engineContext.m_logger->LogText(LogTag::TOOLS, LogLevel::START_PRINT, "Creating OpenGL context in treenity.");

	//Set application full screen 
	showFullScreen();
	showMaximized();
}

Ui::TreenityClass& Treenity::GetUi()
{
	return ui;
}

/*
QPoint Treenity::GetCanvasCenter() const
{
	return ui.widget_canvas3D->geometry().center();
}
*/

void Treenity::New()
{
	m_engineInterface->NewScene();
}

void Treenity::OpenProject()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Open Project", "", "World File (*.world)");

	if (fileName != "")
	{
		m_currentProjectFile = fileName;

		UpdateWindowTitle();
		m_engineInterface->ClearScene();
		m_projectManager->Import(fileName);
		m_engineInterface->AddDefaultEntities();
		m_engineInterface->InitializeScene();
	}
}

void Treenity::Save()
{
	m_projectManager->Export(m_currentProjectFile);
}

void Treenity::SaveAs()
{
	QString fileName = QFileDialog::getSaveFileName(this, "Save Project", "", "World File (*.world)");

	if (fileName != "")
	{
		m_currentProjectFile = fileName;

		UpdateWindowTitle();

		m_projectManager->Export(fileName);
	}
}

void Treenity::Play()
{
	Utils::Write("Starting game session");
	ui.treedeetoolbar->setEnabled(false);
	ui.widget_rightSideLeftAligned->setEnabled(false);

	m_engineInterface->UpdateTerrainPhysicsMesh();

	ClearSelection();
	m_engineInterface->EnterPlayMode();
}

void Treenity::ExitPlayMode()
{
	ui.treedeetoolbar->setEnabled(true);
	ui.widget_rightSideLeftAligned->setEnabled(true);
}

void Treenity::Select(ECS::Entity* p_entity)
{
	if(!m_engineInterface->GetWorld()->GetGroupManager()->IsEntityInGroup(p_entity, "NonSelectable"))
	{
		m_previouslySelectedEntities = m_selectedEntities;
		m_selectedEntities.clear();

		if(p_entity != nullptr)
			m_selectedEntities.insert(p_entity);

		UpdateOnSelection();
	}
}

void Treenity::Select(const std::set<ECS::Entity*>& p_entities)
{
	m_previouslySelectedEntities = m_selectedEntities;
	m_selectedEntities = p_entities;

	for(auto itr = m_selectedEntities.begin(); itr != m_selectedEntities.end();)
	{
		if(m_engineInterface->GetWorld()->GetGroupManager()->IsEntityInGroup((*itr), "NonSelectable"))
		{
			itr = m_selectedEntities.erase(itr);
		}
		else
		{
			itr++;
		}
	}

	UpdateOnSelection();
}

void Treenity::AddToSelection(ECS::Entity* p_entity)
{
	if(!m_engineInterface->GetWorld()->GetGroupManager()->IsEntityInGroup(p_entity, "NonSelectable"))
	{
		m_previouslySelectedEntities = m_selectedEntities;
		m_selectedEntities.insert(p_entity);

		UpdateOnSelection();
	}
}

void Treenity::ClearSelection()
{
	m_previouslySelectedEntities = m_selectedEntities;
	m_selectedEntities.clear();

	UpdateOnSelection();
}

const std::set<ECS::Entity*>& Treenity::GetSelection() const
{
	return m_selectedEntities;
}

void Treenity::RenameEntity(ECS::Entity* p_entity, const QString& p_name)
{
	ui.treeView_entityOutliner->EntityRenamed(p_entity, p_name);
	m_projectManager->SetEntityName(p_entity, p_name);
}

ToolMode::ToolMode Treenity::GetToolMode()
{
	return m_toolMode;
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
		RenameEntity(entity, ui.lineEdit_entityName->text());
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
		m_engineInterface->AddPhysics(*m_selectedEntities.begin(), true);
	}
}

void Treenity::AddWaterCollider()
{
	if(m_selectedEntities.size() == 1)
	{
		m_engineInterface->AddWaterCollider(*m_selectedEntities.begin());
	}
}

void Treenity::AddScriptComponent()
{
	if(m_selectedEntities.size() == 1)
	{
		m_engineInterface->AddScript(*m_selectedEntities.begin());
	}
}

void Treenity::AddCollisionResponder()
{
	if(m_selectedEntities.size() == 1)
	{
		m_engineInterface->AddCollisionResponder(*m_selectedEntities.begin());
	}
}

void Treenity::AddParticleComponent()
{
	if(m_selectedEntities.size() == 1)
	{
		m_engineInterface->AddParticle(*m_selectedEntities.begin());
	}
}

void Treenity::UpdateOnSelection()
{
	ui.treeView_entityOutliner->SetCurrentItems(m_selectedEntities);

	if (m_selectedEntities.size() == 0)
	{
		m_toolManager.GetSelectedTool()->SetSelectedEntity(nullptr);

		// Disable name entry.
		ui.lineEdit_entityName->setText("");
		ui.lineEdit_entityName->setEnabled(false);

		// Clear component list.
		m_compView->RemoveItems();
	}
	else if (m_selectedEntities.size() == 1)
	{
		ECS::Entity* selectedEntity = *m_selectedEntities.begin();

		m_toolManager.GetSelectedTool()->SetSelectedEntity(selectedEntity);	

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
					m_compView->AddItem(m_componentViews[i]);
					m_componentViews[i]->DisplayEntity(selectedEntity);
				}
			}
		}
	}
	else if (m_selectedEntities.size() > 1)
	{
		m_toolManager.GetSelectedTool()->SetSelectedEntity(nullptr);

		// Enable name, allow all names to be changed.
		ui.lineEdit_entityName->setText("");
		ui.lineEdit_entityName->setEnabled(true);

		// Clear component list (potential change in future, show transforms).
		m_compView->RemoveItems();
	}

	

	// For all selected entities that has a collision component, visualize their shape.
	for (auto entity : m_selectedEntities)
	{
		//Don't handle terrain in physics debug drawing
		if(m_engineInterface->GetEntityByTag("Terrain") == entity)
			continue;

		if ((entity->GetFlag() & (1ULL << RootForce::ComponentType::COLLISION)) != 0)
		{
			m_engineInterface->SetCollisionVisualized(entity, true);
		}
	}

	// Remove collision visualization for deselected entities.
	for (auto entity : m_previouslySelectedEntities)
	{
		//Don't handle terrain in physics debug drawing
		if(m_engineInterface->GetEntityByTag("Terrain") == entity)
			continue;

		if ((entity->GetFlag() & (1ULL << RootForce::ComponentType::COLLISION)) != 0)
		{
			if (m_selectedEntities.find(entity) == m_selectedEntities.end())
			{
				m_engineInterface->SetCollisionVisualized(entity, false);
			}
		}
	}
}

void Treenity::DisplayEntity(ECS::Entity* p_selectedEntity)
{
	// Update information about the selected entity.
	static_cast<TransformView*>(m_componentViews[RootForce::ComponentType::TRANSFORM])->DisplayEntity(p_selectedEntity);
}

void Treenity::keyPressEvent( QKeyEvent* event )
{
	if(m_engineInterface->GetMode() == EditorMode::EDITOR)
	{
		if(event->key() == Qt::Key_Delete)
		{
			if(m_selectedEntities.size() > 0)
			{
				for(auto e : m_selectedEntities)
				{
					m_engineInterface->DeleteEntity(e);
				}
			}
		}
		else if(event->key() == Qt::Key_A)
		{
			ClearSelection();
		}
		else if( event->key() == Qt::Key_S)
		{
			int index = (m_toolMode == ToolMode::GLOBAL) ? 0 : 1;
			ui.comboBox_mode->setCurrentIndex(index);
		}
		else if(event->key() == Qt::Key_G)
		{
			std::stringstream ss;
			ss << "r l ssao 0";
			g_engineContext.m_renderer->ParseCommands(&ss);
		}
		else if(event->key() == Qt::Key_H)
		{
			std::stringstream ss;
			ss << "r l ssao 1";
			g_engineContext.m_renderer->ParseCommands(&ss);
		}
	}
	else if(m_engineInterface->GetMode() == EditorMode::GAME)
	{
		if (event->key() == Qt::Key_Escape)
		{
			if (m_engineInterface->GetMode() == EditorMode::GAME)
			{
				Utils::Write("Stopping game session. Restoring world.");
				m_engineInterface->ExitPlayMode();
			}
		}
		else if(event->key() == Qt::Key_Delete)
		{
			if(m_selectedEntities.size() > 0)
			{
				for(auto e : m_selectedEntities)
				{
					m_engineInterface->DeleteEntity(e);
				}
			}
		}
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

void Treenity::Update(float p_dt)
{
	if(m_engineInterface->GetMode() == EditorMode::GAME)
	{
		// Poll for the component views (since we cannot get events when component data is changed).
		if (m_selectedEntities.size() == 1)
		{
			ECS::Entity* selectedEntity = *m_selectedEntities.begin();

			uint64_t flag = selectedEntity->GetFlag();
			for (int i = 0; i < 64; ++i)
			{
				uint64_t mask = 1ULL << i;
				if ((flag & mask) != 0)
				{
					if (m_componentViews.find(i) != m_componentViews.end())
					{
						if(m_componentViews[i]->IsReceivingUpdates())
						{
							m_componentViews[i]->DisplayEntity(selectedEntity);
						}		
					}
				}
			}
		}
	}
}

void Treenity::SetTranslateTool()
{
	if(m_engineInterface->GetMode() != EditorMode::GAME)
	{
		m_toolManager.SetTool(ToolBox::TRANSLATION_TOOL);
		m_terrainDialog->hide();
		m_terrainTextureDialog->hide();
	}
	
}

void Treenity::SetRotateTool()
{
	if(m_engineInterface->GetMode() != EditorMode::GAME)
	{
		m_toolManager.SetTool(ToolBox::ROTATION_TOOL);
		m_terrainDialog->hide();
		m_terrainTextureDialog->hide();
	}
}

void Treenity::SetResizeTool()
{
	if(m_engineInterface->GetMode() != EditorMode::GAME)
	{
		//Set resize tool
		m_terrainDialog->hide();
		m_terrainTextureDialog->hide();
	}
}

void Treenity::SetTerrainGeometryTool()
{
	if(m_engineInterface->GetMode() != EditorMode::GAME)
	{
		m_toolManager.SetTool(ToolBox::TERRAIN_TOOL);
		m_terrainTextureDialog->hide();
		m_terrainDialog->Show();
	}
}

void Treenity::SetTerrainTextureTool()
{
	if(m_engineInterface->GetMode() != EditorMode::GAME)
	{
		m_toolManager.SetTool(ToolBox::TERRAIN_TEXTURE_TOOL);
		m_terrainDialog->hide();
		m_terrainTextureDialog->Show();
	}
}

void Treenity::ChangeToolMode(int index)
{
	if(m_engineInterface->GetMode() != EditorMode::GAME)
	{
		m_toolMode = (ToolMode::ToolMode)index;
	}
}

void Treenity::RemoveRenderable(ECS::Entity* p_entity)
{
	m_engineInterface->RemoveRenderable(p_entity);
}

void Treenity::RemovePhysics(ECS::Entity* p_entity)
{
	m_engineInterface->RemovePhysics(p_entity);
}

void Treenity::RemoveWaterCollider(ECS::Entity* p_entity)
{
	m_engineInterface->RemoveWaterCollider(p_entity);
}

void Treenity::RemoveScriptComponent(ECS::Entity* p_entity)
{
	m_engineInterface->RemoveScript(p_entity);
}

void Treenity::RemoveCollisionResponder(ECS::Entity* p_entity)
{
	m_engineInterface->RemoveCollisionResponder(p_entity);
}

void Treenity::AddTerrain()
{
	m_engineInterface->CreateTerrainEntity(128, 128);
}

void Treenity::FocusEntity()
{
	if(m_engineInterface->GetMode() != EditorMode::GAME)
	{
		if(m_selectedEntities.size() > 0)
			m_engineInterface->TargetEntity(*m_selectedEntities.begin());
	}
}

TerrainGeometryBrush* Treenity::GetTerrainGeometryBrush()
{
	return m_terrainGeometryBrush;
}

TerrainTextureBrush* Treenity::GetTerrainTextureBrush()
{
	return m_terrainTextureBrush;
}

void Treenity::ExportTerrainModel()
{
	bool ok;
	QString name = QInputDialog::getText(this, tr("Enter Terrain Model Name"), tr("Name:"), QLineEdit::Normal, "TerrainModel", &ok);

	if(ok && !name.isEmpty())
		m_engineInterface->ExportTerrainModel(name.toStdString());
}

void Treenity::ExportTerrainBlendMap()
{

}

