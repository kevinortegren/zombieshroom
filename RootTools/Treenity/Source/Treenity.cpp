#include "Treenity.h"
#include <QtWidgets\qmessagebox.h>
#include <RootSystems/Include/ComponentTypes.h>
#include <RootSystems/Include/Transform.h>
#include <RootSystems/Include/RenderingSystem.h>

#include <RootTools/Treenity/Include/Log.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <QFileDialog>

extern RootEngine::GameSharedContext g_engineContext;

Treenity::Treenity(QWidget *parent)
	: QMainWindow(parent), m_running(true), m_selectedEntity(nullptr)
{
	//Set some logging init things
	Log::GetInstance()->setParent(this);
	Log::GetInstance()->setFloating(true);

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

	connect(ui.action_saveAs,						SIGNAL(triggered()), this,				SLOT(SaveAs()));
	connect(ui.actionLog,							SIGNAL(triggered()), Log::GetInstance(), SLOT(Show()));
	connect(ui.action_addEntity,					SIGNAL(triggered()), this,				SLOT(CreateEntity()));
	connect(ui.action_removeEntity,					SIGNAL(triggered()), this,				SLOT(DestroyEntity()));
	connect(ui.lineEdit_entityName,					SIGNAL(editingFinished()), this,		SLOT(RenameEntity()));
	connect(ui.treeView_entityOutliner,				SIGNAL(itemSelectionChanged()), this,	SLOT(OutlinerSelectEntity()));
	connect(ui.action_renderable,					SIGNAL(triggered()), this,				SLOT(AddRenderable()));
	connect(transformUI.doubleSpinBox_translationX, SIGNAL(valueChanged(double)), this,		SLOT(TransformPositionXChanged(double)));
	connect(transformUI.doubleSpinBox_translationY, SIGNAL(valueChanged(double)), this,		SLOT(TransformPositionYChanged(double)));
	connect(transformUI.doubleSpinBox_translationZ, SIGNAL(valueChanged(double)), this,		SLOT(TransformPositionZChanged(double)));
	connect(transformUI.doubleSpinBox_orientationX, SIGNAL(valueChanged(double)), this,		SLOT(TransformOrientationXChanged(double)));
	connect(transformUI.doubleSpinBox_orientationY, SIGNAL(valueChanged(double)), this,		SLOT(TransformOrientationYChanged(double)));
	connect(transformUI.doubleSpinBox_orientationZ, SIGNAL(valueChanged(double)), this,		SLOT(TransformOrientationZChanged(double)));
	connect(transformUI.doubleSpinBox_scaleX,		SIGNAL(valueChanged(double)), this,		SLOT(TransformScaleXChanged(double)));
	connect(transformUI.doubleSpinBox_scaleY,		SIGNAL(valueChanged(double)), this,		SLOT(TransformScaleYChanged(double)));
	connect(transformUI.doubleSpinBox_scaleZ,		SIGNAL(valueChanged(double)), this,		SLOT(TransformScaleZChanged(double)));
}

Treenity::~Treenity()
{

}

void Treenity::SetEngineInterface(EngineInterface* p_engineInterface)
{
	m_engineInterface = p_engineInterface;
}

void Treenity::SetProjectManager(ProjectManager* p_projectManager)
{
	m_projectManager = p_projectManager;
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

void Treenity::SaveAs()
{
	QString fileName = QFileDialog::getSaveFileName(this, "Save Project", "", "World File (*.world)");
	m_projectManager->Export(fileName);
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
		m_projectManager->SetEntityName(m_selectedEntity, ui.lineEdit_entityName->text());

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

void Treenity::TransformPositionXChanged(double p_value)
{
	if (m_selectedEntity != nullptr)
	{
		glm::vec3 position = m_engineInterface->GetPosition(m_selectedEntity);
		position.x = p_value;
		m_engineInterface->SetPosition(m_selectedEntity, position);
	}
}

void Treenity::TransformPositionYChanged(double p_value)
{
	if (m_selectedEntity != nullptr)
	{
		glm::vec3 position = m_engineInterface->GetPosition(m_selectedEntity);
		position.y = p_value;
		m_engineInterface->SetPosition(m_selectedEntity, position);
	}
}

void Treenity::TransformPositionZChanged(double p_value)
{
	if (m_selectedEntity != nullptr)
	{
		glm::vec3 position = m_engineInterface->GetPosition(m_selectedEntity);
		position.z = p_value;
		m_engineInterface->SetPosition(m_selectedEntity, position);
	}
}

void Treenity::TransformOrientationXChanged(double p_value)
{
	if (m_selectedEntity != nullptr)
	{
		glm::vec3 euler = glm::eulerAngles(m_engineInterface->GetOrientation(m_selectedEntity).GetQuaternion());
		euler.x = p_value;
		m_engineInterface->SetOrientation(m_selectedEntity, RootForce::Orientation(euler.x, euler.y, euler.z));
	}
}

void Treenity::TransformOrientationYChanged(double p_value)
{
	if (m_selectedEntity != nullptr)
	{
		glm::vec3 euler = glm::eulerAngles(m_engineInterface->GetOrientation(m_selectedEntity).GetQuaternion());
		euler.y = p_value;
		m_engineInterface->SetOrientation(m_selectedEntity, RootForce::Orientation(euler.x, euler.y, euler.z));
	}
}

void Treenity::TransformOrientationZChanged(double p_value)
{
	if (m_selectedEntity != nullptr)
	{
		glm::vec3 euler = glm::eulerAngles(m_engineInterface->GetOrientation(m_selectedEntity).GetQuaternion());
		euler.z = p_value;
		m_engineInterface->SetOrientation(m_selectedEntity, RootForce::Orientation(euler.x, euler.y, euler.z));
	}
}

void Treenity::TransformScaleXChanged(double p_value)
{
	if (m_selectedEntity != nullptr)
	{
		glm::vec3 scale = m_engineInterface->GetPosition(m_selectedEntity);
		scale.x = p_value;
		m_engineInterface->SetPosition(m_selectedEntity, scale);
	}
}

void Treenity::TransformScaleYChanged(double p_value)
{
	if (m_selectedEntity != nullptr)
	{
		glm::vec3 scale = m_engineInterface->GetPosition(m_selectedEntity);
		scale.y = p_value;
		m_engineInterface->SetPosition(m_selectedEntity, scale);
	}
}

void Treenity::TransformScaleZChanged(double p_value)
{
	if (m_selectedEntity != nullptr)
	{
		glm::vec3 scale = m_engineInterface->GetPosition(m_selectedEntity);
		scale.z = p_value;
		m_engineInterface->SetPosition(m_selectedEntity, scale);
	}
}


void Treenity::SelectEntity(ECS::Entity* p_entity)
{
	m_selectedEntity = p_entity;

	// Update the general properties.
	QString name = m_projectManager->GetEntityName(m_selectedEntity);
	ui.lineEdit_entityName->setText(name);

	// Update the component toolbox.
	while (ui.toolBox_components->count() > 0)
	{
		ui.toolBox_components->removeItem(0);
	}

	if (p_entity != nullptr)
	{
		uint64_t flag = p_entity->GetFlag();
		for (int i = 0; i < 64; ++i)
		{
			uint64_t mask = 1ULL << i;
			if ((flag & mask) != 0)
			{
				QWidget* widget = new QWidget(ui.toolBox_components);
				SetupUIForComponent(widget, i);
				ui.toolBox_components->addItem(widget, m_componentNames[i]);

				switch (i)
				{
					case RootForce::ComponentType::TRANSFORM:
					{
						transformUI.doubleSpinBox_translationX->setValue(m_engineInterface->GetPosition(p_entity).x);
						transformUI.doubleSpinBox_translationY->setValue(m_engineInterface->GetPosition(p_entity).y);
						transformUI.doubleSpinBox_translationZ->setValue(m_engineInterface->GetPosition(p_entity).z);

						glm::vec3 euler = glm::eulerAngles(m_engineInterface->GetOrientation(p_entity).GetQuaternion());

						transformUI.doubleSpinBox_orientationX->setValue(euler.x);
						transformUI.doubleSpinBox_orientationY->setValue(euler.y);
						transformUI.doubleSpinBox_orientationZ->setValue(euler.z);

						transformUI.doubleSpinBox_scaleX->setValue(m_engineInterface->GetScale(p_entity).x);
						transformUI.doubleSpinBox_scaleY->setValue(m_engineInterface->GetScale(p_entity).y);
						transformUI.doubleSpinBox_scaleZ->setValue(m_engineInterface->GetScale(p_entity).z);

						connect(transformUI.doubleSpinBox_translationX, SIGNAL(valueChanged(double)), this,		SLOT(TransformPositionXChanged(double)));
						connect(transformUI.doubleSpinBox_translationY, SIGNAL(valueChanged(double)), this,		SLOT(TransformPositionYChanged(double)));
						connect(transformUI.doubleSpinBox_translationZ, SIGNAL(valueChanged(double)), this,		SLOT(TransformPositionZChanged(double)));
						connect(transformUI.doubleSpinBox_orientationX, SIGNAL(valueChanged(double)), this,		SLOT(TransformOrientationXChanged(double)));
						connect(transformUI.doubleSpinBox_orientationY, SIGNAL(valueChanged(double)), this,		SLOT(TransformOrientationYChanged(double)));
						connect(transformUI.doubleSpinBox_orientationZ, SIGNAL(valueChanged(double)), this,		SLOT(TransformOrientationZChanged(double)));
						connect(transformUI.doubleSpinBox_scaleX,		SIGNAL(valueChanged(double)), this,		SLOT(TransformScaleXChanged(double)));
						connect(transformUI.doubleSpinBox_scaleY,		SIGNAL(valueChanged(double)), this,		SLOT(TransformScaleYChanged(double)));
						connect(transformUI.doubleSpinBox_scaleZ,		SIGNAL(valueChanged(double)), this,		SLOT(TransformScaleZChanged(double)));
					} break;

					case RootForce::ComponentType::RENDERABLE:
					{

					} break;
				}
			}
		}
	}
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

QWidget* Treenity::GetComponentToolboxItemByType(int p_componentType)
{
	for (int i = 0; i < ui.toolBox_components->count(); ++i)
	{
		if (ui.toolBox_components->widget(i)->windowTitle() == m_componentNames[p_componentType])
		{
			return ui.toolBox_components->widget(i);
		}
	}

	return nullptr;
}