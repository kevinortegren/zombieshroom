#include <RootTools/Treenity/Include/Components/PhysicsView.h>

PhysicsView::PhysicsView(QWidget* p_parent)
	: AbstractComponentView(p_parent)
	, m_name("Physics")
{
	ui.setupUi(this);

	connect(ui.doubleSpinBox_mass,				SIGNAL(valueChanged(double)),		this,	SLOT(MassChanged(double)));
	connect(ui.comboBox_type,					SIGNAL(currentIndexChanged(int)),	this,	SLOT(TypeChanged(int)));
	connect(ui.comboBox_shape,					SIGNAL(currentIndexChanged(int)),	this,	SLOT(ShapeChanged(int)));
	connect(ui.checkBox_collideWithStatic,		SIGNAL(clicked(bool)),				this,	SLOT(CollisionWithStaticChanged(bool)));
	connect(ui.checkBox_collideWithWorld,		SIGNAL(clicked(bool)),				this,	SLOT(CollisionWithWorldChanged(bool)));
	connect(ui.doubleSpinBox_coneHeight,		SIGNAL(valueChanged(double)),		this,	SLOT(ConeHeightChanged(double)));
	connect(ui.doubleSpinBox_coneRadius,		SIGNAL(valueChanged(double)),		this,	SLOT(ConeRadiusChanged(double)));
	connect(ui.doubleSpinBox_cylinderHeight,	SIGNAL(valueChanged(double)),		this,	SLOT(CylinderHeightChanged(double)));
	connect(ui.doubleSpinBox_cylinderRadius,	SIGNAL(valueChanged(double)),		this,	SLOT(CylinderRadiusChanged(double)));
	connect(ui.doubleSpinBox_sphereRadius,		SIGNAL(valueChanged(double)),		this,	SLOT(SphereRadiusChanged(double)));
	connect(ui.doubleSpinBox_gravityX,			SIGNAL(valueChanged(double)),		this,	SLOT(GravityXChanged(double)));
	connect(ui.doubleSpinBox_gravityY,			SIGNAL(valueChanged(double)),		this,	SLOT(GravityYChanged(double)));
	connect(ui.doubleSpinBox_gravityZ,			SIGNAL(valueChanged(double)),		this,	SLOT(GravityZChanged(double)));
	connect(ui.lineEdit_physicsMesh,			SIGNAL(editingFinished()),			this,	SLOT(PhysicsMeshChanged()));
	connect(ui.toolButton_physicsMesh,			SIGNAL(clicked()),					this,	SLOT(PhysicsMeshBrowse()));
}

const QString& PhysicsView::GetComponentName() const
{
	return m_name;
}

void PhysicsView::DisplayEntity(ECS::Entity* p_entity)
{
	RootEngine::Physics::PhysicsType::PhysicsType physicsType = m_engineInterface->GetPhysicsType(p_entity);
	if (physicsType == RootEngine::Physics::PhysicsType::TYPE_PLAYER || 
		physicsType == RootEngine::Physics::PhysicsType::TYPE_RAGDOLL ||
		physicsType == RootEngine::Physics::PhysicsType::TYPE_ABILITYSPAWN)
	{
		ui.comboBox_type->setHidden(true);
		ui.checkBox_collideWithWorld->setHidden(true);
		ui.checkBox_collideWithStatic->setHidden(true);
		ui.doubleSpinBox_gravityX->setHidden(true);
		ui.doubleSpinBox_gravityY->setHidden(true);
		ui.doubleSpinBox_gravityZ->setHidden(true);
		ui.doubleSpinBox_mass->setHidden(true);
		ui.comboBox_shape->setHidden(true);
		ui.stackedWidget_shape->setHidden(true);
	}
	else
	{
		ui.comboBox_type->setHidden(false);
		ui.checkBox_collideWithWorld->setHidden(false);
		ui.checkBox_collideWithStatic->setHidden(false);
		ui.doubleSpinBox_gravityX->setHidden(false);
		ui.doubleSpinBox_gravityY->setHidden(false);
		ui.doubleSpinBox_gravityZ->setHidden(false);
		ui.doubleSpinBox_mass->setHidden(false);
		ui.comboBox_shape->setHidden(false);
		ui.stackedWidget_shape->setHidden(false);
	}

	BlockAllSignals(true);

	int type = (m_engineInterface->GetPhysicsType(p_entity) == RootEngine::Physics::PhysicsType::TYPE_DYNAMIC || 
				m_engineInterface->GetPhysicsType(p_entity) == RootEngine::Physics::PhysicsType::TYPE_ABILITY) 
				? TYPE_DYNAMIC : TYPE_STATIC;
	ui.comboBox_type->setCurrentIndex(type);
	ui.stackedWidget_type->setCurrentIndex(type);

	if (type == TYPE_DYNAMIC)
	{
		ui.checkBox_collideWithWorld->setChecked(m_engineInterface->GetCollideWithWorld(p_entity));
		ui.checkBox_collideWithStatic->setChecked(m_engineInterface->GetCollideWithStatic(p_entity));

		glm::vec3 gravity = m_engineInterface->GetGravity(p_entity);
		ui.doubleSpinBox_gravityX->setValue(gravity.x);
		ui.doubleSpinBox_gravityY->setValue(gravity.y);
		ui.doubleSpinBox_gravityZ->setValue(gravity.z);
		ui.doubleSpinBox_mass->setValue(m_engineInterface->GetMass(p_entity));
	}
	
	switch (m_engineInterface->GetPhysicsShape(p_entity))
	{
		case RootEngine::Physics::PhysicsShape::SHAPE_SPHERE:
		{
			ui.comboBox_shape->setCurrentIndex(SHAPE_SPHERE);
			ui.stackedWidget_shape->setCurrentIndex(SHAPE_SPHERE);
			ui.doubleSpinBox_sphereRadius->setValue(m_engineInterface->GetShapeRadius(p_entity));
		} break;

		case RootEngine::Physics::PhysicsShape::SHAPE_CONE:
		{
			ui.comboBox_shape->setCurrentIndex(SHAPE_CONE);
			ui.stackedWidget_shape->setCurrentIndex(SHAPE_CONE);
			ui.doubleSpinBox_coneRadius->setValue(m_engineInterface->GetShapeRadius(p_entity));
			ui.doubleSpinBox_coneHeight->setValue(m_engineInterface->GetShapeHeight(p_entity));
		} break;

		case RootEngine::Physics::PhysicsShape::SHAPE_CYLINDER:
		{
			ui.comboBox_shape->setCurrentIndex(SHAPE_CYLINDER);
			ui.stackedWidget_shape->setCurrentIndex(SHAPE_CYLINDER);
			ui.doubleSpinBox_cylinderRadius->setValue(m_engineInterface->GetShapeRadius(p_entity));
			ui.doubleSpinBox_cylinderHeight->setValue(m_engineInterface->GetShapeHeight(p_entity));
		} break;

		case RootEngine::Physics::PhysicsShape::SHAPE_CUSTOM_MESH:
		{
			ui.comboBox_shape->setCurrentIndex(SHAPE_MESH);
			ui.stackedWidget_shape->setCurrentIndex(SHAPE_MESH);
			ui.lineEdit_physicsMesh->setText(QString::fromStdString(m_engineInterface->GetPhysicsMesh(p_entity)));
		} break;
	}

	BlockAllSignals(false);
}

void PhysicsView::BlockAllSignals(bool p_block)
{
	blockSignals(p_block);
	ui.checkBox_collideWithStatic->blockSignals(p_block);
	ui.checkBox_collideWithWorld->blockSignals(p_block);
	ui.comboBox_shape->blockSignals(p_block);
	ui.comboBox_type->blockSignals(p_block);
	ui.doubleSpinBox_coneHeight->blockSignals(p_block);
	ui.doubleSpinBox_coneRadius->blockSignals(p_block);
	ui.doubleSpinBox_cylinderHeight->blockSignals(p_block);
	ui.doubleSpinBox_cylinderRadius->blockSignals(p_block);
	ui.doubleSpinBox_gravityX->blockSignals(p_block);
	ui.doubleSpinBox_gravityY->blockSignals(p_block);
	ui.doubleSpinBox_gravityZ->blockSignals(p_block);
	ui.doubleSpinBox_mass->blockSignals(p_block);
	ui.doubleSpinBox_sphereRadius->blockSignals(p_block);
	ui.lineEdit_physicsMesh->blockSignals(p_block);
	ui.toolButton_physicsMesh->blockSignals(p_block);
}

void PhysicsView::TypeChanged( int p_value )
{
	ECS::Entity* selection = *m_editorInterface->GetSelection().begin();

	if (p_value == TYPE_DYNAMIC)
	{
		if (m_engineInterface->GetPhysicsShape(selection) == RootEngine::Physics::PhysicsShape::SHAPE_CUSTOM_MESH)
		{
			m_engineInterface->SetPhysicsShape(selection, RootEngine::Physics::PhysicsShape::SHAPE_CYLINDER);
		}

		ui.comboBox_shape->setItemData(SHAPE_MESH, 0, Qt::UserRole - 1);
	}
	else
	{
		ui.comboBox_shape->setItemData(SHAPE_MESH, 33, Qt::UserRole - 1);
	}

	m_engineInterface->SetPhysicsType(selection, p_value ? false : true);
	DisplayEntity(selection);
}

void PhysicsView::CollisionWithWorldChanged( bool p_value )
{
	ECS::Entity* selection = *m_editorInterface->GetSelection().begin();
	m_engineInterface->SetCollideWithWorld(selection, p_value);
}

void PhysicsView::CollisionWithStaticChanged( bool p_value )
{
	ECS::Entity* selection = *m_editorInterface->GetSelection().begin();
	m_engineInterface->SetCollideWithStatic(selection, p_value);
}

void PhysicsView::GravityXChanged( double p_value )
{
	ECS::Entity* selection = *m_editorInterface->GetSelection().begin();
	glm::vec3 grav = m_engineInterface->GetGravity(selection);
	grav.x = p_value;
	m_engineInterface->SetGravity(selection, grav);
}

void PhysicsView::GravityYChanged( double p_value )
{
	ECS::Entity* selection = *m_editorInterface->GetSelection().begin();
	glm::vec3 grav = m_engineInterface->GetGravity(selection);
	grav.y = p_value;
	m_engineInterface->SetGravity(selection, grav);
}

void PhysicsView::GravityZChanged( double p_value )
{
	ECS::Entity* selection = *m_editorInterface->GetSelection().begin();
	glm::vec3 grav = m_engineInterface->GetGravity(selection);
	grav.z = p_value;
	m_engineInterface->SetGravity(selection, grav);
}

void PhysicsView::MassChanged(double p_value)
{
	if (m_editorInterface->GetSelection().size() == 1)
	{
		ECS::Entity* selectedEntity = *m_editorInterface->GetSelection().begin();

		float mass = m_engineInterface->GetMass(selectedEntity);
		mass = p_value;
		m_engineInterface->SetMass(selectedEntity, mass);
	}
}

void PhysicsView::ShapeChanged( int p_value )
{
	ECS::Entity* selection = *m_editorInterface->GetSelection().begin();
	
	// A dynamic physics entity cannot have a mesh shape.
	if (p_value == SHAPE_MESH && m_engineInterface->GetPhysicsType(selection) == RootEngine::Physics::PhysicsType::TYPE_DYNAMIC)
	{
		ui.comboBox_shape->setCurrentIndex(SHAPE_CYLINDER);
		m_engineInterface->SetPhysicsShape(selection, RootEngine::Physics::PhysicsShape::SHAPE_CYLINDER);
		DisplayEntity(selection);
	}
	else
	{
		m_engineInterface->SetPhysicsShape(selection, (RootEngine::Physics::PhysicsShape::PhysicsShape)p_value);
		DisplayEntity(selection);
	}
}

void PhysicsView::SphereRadiusChanged( double p_value )
{
	ECS::Entity* selection = *m_editorInterface->GetSelection().begin();
	m_engineInterface->SetShapeRadius(selection, p_value);
}

void PhysicsView::ConeRadiusChanged( double p_value )
{
	ECS::Entity* selection = *m_editorInterface->GetSelection().begin();
	m_engineInterface->SetShapeRadius(selection, p_value);
}

void PhysicsView::ConeHeightChanged( double p_value )
{
	ECS::Entity* selection = *m_editorInterface->GetSelection().begin();
	m_engineInterface->SetShapeHeight(selection, p_value);
}

void PhysicsView::CylinderRadiusChanged( double p_value )
{
	ECS::Entity* selection = *m_editorInterface->GetSelection().begin();
	m_engineInterface->SetShapeRadius(selection, p_value);
}

void PhysicsView::CylinderHeightChanged( double p_value )
{
	ECS::Entity* selection = *m_editorInterface->GetSelection().begin();
	m_engineInterface->SetShapeHeight(selection, p_value);
}

void PhysicsView::PhysicsMeshChanged()
{
	ECS::Entity* selection = *m_editorInterface->GetSelection().begin();
	m_engineInterface->SetPhysicsMesh(selection, ui.lineEdit_physicsMesh->text().toStdString());
}

void PhysicsView::PhysicsMeshBrowse()
{
	// TODO
}
