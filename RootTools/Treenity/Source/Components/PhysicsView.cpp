#include <RootTools/Treenity/Include/Components/PhysicsView.h>

PhysicsView::PhysicsView(QWidget* p_parent)
	: AbstractComponentView(p_parent)
	, m_name("Physics")
{
	ui.setupUi(this);

	connect(ui.doubleSpinBox_mass,		SIGNAL(valueChanged(double)), this,		SLOT(MassChanged(double)));
	connect(ui.comboBox_type,			SIGNAL(currentIndexChanged(int)), this, SLOT(TypeChanged(int)));
}

const QString& PhysicsView::GetComponentName() const
{
	return m_name;
}

void PhysicsView::DisplayEntity(ECS::Entity* p_entity)
{
	int type = m_engineInterface->GetPhysicsType(p_entity) == RootEngine::Physics::PhysicsType::TYPE_DYNAMIC ? 0 : 1;
	ui.comboBox_type->setCurrentIndex(type);
	ui.stackedWidget_type->setCurrentIndex(type);

	if (type == RootEngine::Physics::PhysicsType::TYPE_DYNAMIC)
	{
		ui.checkBox_collideWithWorld->setChecked(m_engineInterface->GetCollideWithWorld(p_entity));
		ui.checkBox_collideWithStatic->setChecked(m_engineInterface->GetCollideWithStatic(p_entity));
		ui.doubleSpinBox_gravityX->setValue(m_engineInterface->GetGravity(p_entity).x);
		ui.doubleSpinBox_gravityY->setValue(m_engineInterface->GetGravity(p_entity).y);
		ui.doubleSpinBox_gravityZ->setValue(m_engineInterface->GetGravity(p_entity).z);
		ui.doubleSpinBox_mass->setValue(m_engineInterface->GetMass(p_entity));
	}
	
	switch (m_engineInterface->GetPhysicsShape(p_entity))
	{
		case RootEngine::Physics::PhysicsShape::SHAPE_SPHERE:
		{
			ui.comboBox_shape->setCurrentIndex(0);
			ui.stackedWidget_shape->setCurrentIndex(0);
			ui.doubleSpinBox_sphereRadius->setValue(m_engineInterface->GetShapeRadius(p_entity));
		} break;

		case RootEngine::Physics::PhysicsShape::SHAPE_CONE:
		{
			ui.comboBox_shape->setCurrentIndex(1);
			ui.stackedWidget_shape->setCurrentIndex(1);
			ui.doubleSpinBox_coneRadius->setValue(m_engineInterface->GetShapeRadius(p_entity));
			ui.doubleSpinBox_coneHeight->setValue(m_engineInterface->GetShapeHeight(p_entity));
		} break;

		case RootEngine::Physics::PhysicsShape::SHAPE_CYLINDER:
		{
			ui.comboBox_shape->setCurrentIndex(2);
			ui.stackedWidget_shape->setCurrentIndex(2);
			ui.doubleSpinBox_cylinderRadius->setValue(m_engineInterface->GetShapeRadius(p_entity));
			ui.doubleSpinBox_cylinderHeight->setValue(m_engineInterface->GetShapeHeight(p_entity));
		} break;

		case RootEngine::Physics::PhysicsShape::SHAPE_CUSTOM_MESH:
		{
			ui.comboBox_shape->setCurrentIndex(3);
			ui.stackedWidget_shape->setCurrentIndex(3);
			ui.lineEdit_physicsMesh->setText(QString::fromStdString(m_engineInterface->GetPhysicsMesh(p_entity)));
		} break;
	}

}

void PhysicsView::TypeChanged( int p_value )
{
	ECS::Entity* selection = *m_editorInterface->GetSelection().begin();
	m_engineInterface->SetPhysicsType(selection, p_value ? false : true);
	DisplayEntity(selection);
}

void PhysicsView::CollisionWithWorldChanged( bool p_value )
{

}

void PhysicsView::CollisionWithStaticChanged( bool p_value )
{

}

void PhysicsView::GravityXChanged( double p_value )
{

}

void PhysicsView::GravityYChanged( double p_value )
{

}

void PhysicsView::GravityZChanged( double p_value )
{

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

}

void PhysicsView::SphereRadiusChanged( double p_value )
{

}

void PhysicsView::ConeRadiusChanged( double p_value )
{

}

void PhysicsView::ConeHeightChanged( double p_value )
{

}

void PhysicsView::CylinderRadiusChanged( double p_value )
{

}

void PhysicsView::CylinderHeightChanged( double p_value )
{

}

void PhysicsView::PhysicsMeshChanged( const QString& p_value )
{

}

void PhysicsView::PhysicsMeshBrowse()
{

}
