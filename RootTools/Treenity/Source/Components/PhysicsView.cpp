#include <RootTools/Treenity/Include/Components/PhysicsView.h>

PhysicsView::PhysicsView(QWidget* p_parent)
	: AbstractComponentView(p_parent)
	, m_name("Physics")
{
	ui.setupUi(this);

	connect(ui.doubleSpinBox_velocityX,	SIGNAL(valueChanged(double)), this,		SLOT(PhysicsVelocityXChanged(double)));
	connect(ui.doubleSpinBox_velocityY,	SIGNAL(valueChanged(double)), this,		SLOT(PhysicsVelocityYChanged(double)));
	connect(ui.doubleSpinBox_velocityZ,	SIGNAL(valueChanged(double)), this,		SLOT(PhysicsVelocityZChanged(double)));
	connect(ui.doubleSpinBox_mass,		SIGNAL(valueChanged(double)), this,		SLOT(PhysicsMassChanged(double)));
}

const QString& PhysicsView::GetComponentName() const
{
	return m_name;
}

void PhysicsView::DisplayEntity(ECS::Entity* p_entity)
{
	ui.doubleSpinBox_velocityX->setValue(m_engineInterface->GetVelocity(p_entity).x);
	ui.doubleSpinBox_velocityY->setValue(m_engineInterface->GetVelocity(p_entity).y);
	ui.doubleSpinBox_velocityZ->setValue(m_engineInterface->GetVelocity(p_entity).z);

	ui.doubleSpinBox_mass->setValue(m_engineInterface->GetMass(p_entity));

}

void PhysicsView::VelocityXChanged(double p_value)
{
	if (m_editorInterface->GetSelection().size() == 1)
	{
		ECS::Entity* selectedEntity = *m_editorInterface->GetSelection().begin();

		glm::vec3 velocity = m_engineInterface->GetVelocity(selectedEntity);
		velocity.x = p_value;
		m_engineInterface->SetVelocity(selectedEntity, velocity);
	}
}

void PhysicsView::VelocityYChanged(double p_value)
{
	if (m_editorInterface->GetSelection().size() == 1)
	{
		ECS::Entity* selectedEntity = *m_editorInterface->GetSelection().begin();

		glm::vec3 velocity = m_engineInterface->GetVelocity(selectedEntity);
		velocity.y = p_value;
		m_engineInterface->SetVelocity(selectedEntity, velocity);
	}
}

void PhysicsView::VelocityZChanged(double p_value)
{
	if (m_editorInterface->GetSelection().size() == 1)
	{
		ECS::Entity* selectedEntity = *m_editorInterface->GetSelection().begin();

		glm::vec3 velocity = m_engineInterface->GetVelocity(selectedEntity);
		velocity.z = p_value;
		m_engineInterface->SetVelocity(selectedEntity, velocity);
	}
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
