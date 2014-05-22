#include <RootTools/Treenity/Include/Components/TransformView.h>
#include <glm/gtx/euler_angles.hpp>

TransformView::TransformView(QWidget* p_parent)
	: AbstractComponentView(p_parent)
	, m_name("Transform")
{
	ui.setupUi(this);

	connect(ui.doubleSpinBox_translationX,	SIGNAL(valueChanged(double)), this,		SLOT(PositionXChanged(double)));
	connect(ui.doubleSpinBox_translationY,	SIGNAL(valueChanged(double)), this,		SLOT(PositionYChanged(double)));
	connect(ui.doubleSpinBox_translationZ,	SIGNAL(valueChanged(double)), this,		SLOT(PositionZChanged(double)));
	connect(ui.doubleSpinBox_orientationX,	SIGNAL(valueChanged(double)), this,		SLOT(OrientationXChanged(double)));
	connect(ui.doubleSpinBox_orientationY,	SIGNAL(valueChanged(double)), this,		SLOT(OrientationYChanged(double)));
	connect(ui.doubleSpinBox_orientationZ,	SIGNAL(valueChanged(double)), this,		SLOT(OrientationZChanged(double)));
	connect(ui.doubleSpinBox_scaleX,		SIGNAL(valueChanged(double)), this,		SLOT(ScaleXChanged(double)));
	connect(ui.doubleSpinBox_scaleY,		SIGNAL(valueChanged(double)), this,		SLOT(ScaleYChanged(double)));
	connect(ui.doubleSpinBox_scaleZ,		SIGNAL(valueChanged(double)), this,		SLOT(ScaleZChanged(double)));
}

const QString& TransformView::GetComponentName() const
{
	return m_name;
}

void TransformView::DisplayEntity(ECS::Entity* p_entity)
{
	ui.doubleSpinBox_translationX->setValue(m_engineInterface->GetPosition(p_entity).x);
	ui.doubleSpinBox_translationY->setValue(m_engineInterface->GetPosition(p_entity).y);
	ui.doubleSpinBox_translationZ->setValue(m_engineInterface->GetPosition(p_entity).z);

	glm::vec3 euler = glm::eulerAngles(m_engineInterface->GetOrientation(p_entity).GetQuaternion());

	m_currentOrientationEuler.x = euler.x;
	m_currentOrientationEuler.y = euler.y;
	m_currentOrientationEuler.z = euler.z;

	ui.doubleSpinBox_orientationX->setValue(euler.x);
	ui.doubleSpinBox_orientationY->setValue(euler.y);
	ui.doubleSpinBox_orientationZ->setValue(euler.z);

	ui.doubleSpinBox_scaleX->setValue(m_engineInterface->GetScale(p_entity).x);
	ui.doubleSpinBox_scaleY->setValue(m_engineInterface->GetScale(p_entity).y);
	ui.doubleSpinBox_scaleZ->setValue(m_engineInterface->GetScale(p_entity).z);
}

void TransformView::PositionXChanged(double p_value)
{
	if (m_editorInterface->GetSelection().size() == 1)
	{
		ECS::Entity* selectedEntity = *m_editorInterface->GetSelection().begin();

		glm::vec3 position = m_engineInterface->GetPosition(selectedEntity);
		position.x = p_value;
		m_engineInterface->SetPosition(selectedEntity, position);
	}
}

void TransformView::PositionYChanged(double p_value)
{
	if (m_editorInterface->GetSelection().size() == 1)
	{
		ECS::Entity* selectedEntity = *m_editorInterface->GetSelection().begin();

		glm::vec3 position = m_engineInterface->GetPosition(selectedEntity);
		position.y = p_value;
		m_engineInterface->SetPosition(selectedEntity, position);
	}
}

void TransformView::PositionZChanged(double p_value)
{
	if (m_editorInterface->GetSelection().size() == 1)
	{
		ECS::Entity* selectedEntity = *m_editorInterface->GetSelection().begin();

		glm::vec3 position = m_engineInterface->GetPosition(selectedEntity);
		position.z = p_value;
		m_engineInterface->SetPosition(selectedEntity, position);
	}
}

void TransformView::OrientationXChanged(double p_value)
{
	if (m_editorInterface->GetSelection().size() == 1)
	{
		ECS::Entity* selectedEntity = *m_editorInterface->GetSelection().begin();

		m_currentOrientationEuler.x = p_value;

		m_engineInterface->SetOrientation(selectedEntity, RootForce::Orientation(p_value, m_currentOrientationEuler.y, m_currentOrientationEuler.z));
	}
}

void TransformView::OrientationYChanged(double p_value)
{
	int revs = floorf(p_value / 360.0f);
	float degree = (p_value - (revs * 360.0f)) - 90.0f;
	
	if (m_editorInterface->GetSelection().size() == 1)
	{
		ECS::Entity* selectedEntity = *m_editorInterface->GetSelection().begin();

		m_currentOrientationEuler.y = p_value;

		m_engineInterface->SetOrientation(selectedEntity, RootForce::Orientation(m_currentOrientationEuler.x, p_value, m_currentOrientationEuler.z));

	}
}

void TransformView::OrientationZChanged(double p_value)
{
	if (m_editorInterface->GetSelection().size() == 1)
	{
		ECS::Entity* selectedEntity = *m_editorInterface->GetSelection().begin();

		m_currentOrientationEuler.z = p_value;

		m_engineInterface->SetOrientation(selectedEntity, RootForce::Orientation(m_currentOrientationEuler.x, m_currentOrientationEuler.y, p_value));
	}
}

void TransformView::ScaleXChanged(double p_value)
{
	if (m_editorInterface->GetSelection().size() == 1)
	{
		ECS::Entity* selectedEntity = *m_editorInterface->GetSelection().begin();

		glm::vec3 scale = m_engineInterface->GetScale(selectedEntity);
		scale.x = p_value;
		m_engineInterface->SetScale(selectedEntity, scale);
	}
}

void TransformView::ScaleYChanged(double p_value)
{
	if (m_editorInterface->GetSelection().size() == 1)
	{
		ECS::Entity* selectedEntity = *m_editorInterface->GetSelection().begin();

		glm::vec3 scale = m_engineInterface->GetScale(selectedEntity);
		scale.y = p_value;
		m_engineInterface->SetScale(selectedEntity, scale);
	}
}

void TransformView::ScaleZChanged(double p_value)
{
	if (m_editorInterface->GetSelection().size() == 1)
	{
		ECS::Entity* selectedEntity = *m_editorInterface->GetSelection().begin();

		glm::vec3 scale = m_engineInterface->GetScale(selectedEntity);
		scale.z = p_value;
		m_engineInterface->SetScale(selectedEntity, scale);
	}
}