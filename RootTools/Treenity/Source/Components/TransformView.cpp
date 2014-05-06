#include <RootTools/Treenity/Include/Components/TransformView.h>

TransformView::TransformView(QWidget* p_parent)
	: AbstractComponentView(p_parent)
	, m_name("Transform")
{
	ui.setupUi(this);

	connect(ui.doubleSpinBox_translationX,	SIGNAL(valueChanged(double)), this,		SLOT(TransformPositionXChanged(double)));
	connect(ui.doubleSpinBox_translationY,	SIGNAL(valueChanged(double)), this,		SLOT(TransformPositionYChanged(double)));
	connect(ui.doubleSpinBox_translationZ,	SIGNAL(valueChanged(double)), this,		SLOT(TransformPositionZChanged(double)));
	connect(ui.doubleSpinBox_orientationX,	SIGNAL(valueChanged(double)), this,		SLOT(TransformOrientationXChanged(double)));
	connect(ui.doubleSpinBox_orientationY,	SIGNAL(valueChanged(double)), this,		SLOT(TransformOrientationYChanged(double)));
	connect(ui.doubleSpinBox_orientationZ,	SIGNAL(valueChanged(double)), this,		SLOT(TransformOrientationZChanged(double)));
	connect(ui.doubleSpinBox_scaleX,		SIGNAL(valueChanged(double)), this,		SLOT(TransformScaleXChanged(double)));
	connect(ui.doubleSpinBox_scaleY,		SIGNAL(valueChanged(double)), this,		SLOT(TransformScaleYChanged(double)));
	connect(ui.doubleSpinBox_scaleZ,		SIGNAL(valueChanged(double)), this,		SLOT(TransformScaleZChanged(double)));
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

	ui.doubleSpinBox_orientationX->setValue(euler.x);
	ui.doubleSpinBox_orientationY->setValue(euler.y);
	ui.doubleSpinBox_orientationZ->setValue(euler.z);

	ui.doubleSpinBox_scaleX->setValue(m_engineInterface->GetScale(p_entity).x);
	ui.doubleSpinBox_scaleY->setValue(m_engineInterface->GetScale(p_entity).y);
	ui.doubleSpinBox_scaleZ->setValue(m_engineInterface->GetScale(p_entity).z);
}

void TransformView::TransformPositionXChanged(double p_value)
{
	if (m_editorInterface->GetSelection().size() == 1)
	{
		ECS::Entity* selectedEntity = *m_editorInterface->GetSelection().begin();

		glm::vec3 position = m_engineInterface->GetPosition(selectedEntity);
		position.x = p_value;
		m_engineInterface->SetPosition(selectedEntity, position);
	}
}

void TransformView::TransformPositionYChanged(double p_value)
{
	if (m_editorInterface->GetSelection().size() == 1)
	{
		ECS::Entity* selectedEntity = *m_editorInterface->GetSelection().begin();

		glm::vec3 position = m_engineInterface->GetPosition(selectedEntity);
		position.y = p_value;
		m_engineInterface->SetPosition(selectedEntity, position);
	}
}

void TransformView::TransformPositionZChanged(double p_value)
{
	if (m_editorInterface->GetSelection().size() == 1)
	{
		ECS::Entity* selectedEntity = *m_editorInterface->GetSelection().begin();

		glm::vec3 position = m_engineInterface->GetPosition(selectedEntity);
		position.z = p_value;
		m_engineInterface->SetPosition(selectedEntity, position);
	}
}

void TransformView::TransformOrientationXChanged(double p_value)
{
	if (m_editorInterface->GetSelection().size() == 1)
	{
		ECS::Entity* selectedEntity = *m_editorInterface->GetSelection().begin();

		glm::vec3 euler = glm::eulerAngles(m_engineInterface->GetOrientation(selectedEntity).GetQuaternion());
		euler.x = p_value;
		m_engineInterface->SetOrientation(selectedEntity, RootForce::Orientation(euler.x, euler.y, euler.z));
	}
}

void TransformView::TransformOrientationYChanged(double p_value)
{
	if (m_editorInterface->GetSelection().size() == 1)
	{
		ECS::Entity* selectedEntity = *m_editorInterface->GetSelection().begin();

		glm::vec3 euler = glm::eulerAngles(m_engineInterface->GetOrientation(selectedEntity).GetQuaternion());
		euler.y = p_value;
		m_engineInterface->SetOrientation(selectedEntity, RootForce::Orientation(euler.x, euler.y, euler.z));
	}
}

void TransformView::TransformOrientationZChanged(double p_value)
{
	if (m_editorInterface->GetSelection().size() == 1)
	{
		ECS::Entity* selectedEntity = *m_editorInterface->GetSelection().begin();

		glm::vec3 euler = glm::eulerAngles(m_engineInterface->GetOrientation(selectedEntity).GetQuaternion());
		euler.z = p_value;
		m_engineInterface->SetOrientation(selectedEntity, RootForce::Orientation(euler.x, euler.y, euler.z));
	}
}

void TransformView::TransformScaleXChanged(double p_value)
{
	if (m_editorInterface->GetSelection().size() == 1)
	{
		ECS::Entity* selectedEntity = *m_editorInterface->GetSelection().begin();

		glm::vec3 scale = m_engineInterface->GetPosition(selectedEntity);
		scale.x = p_value;
		m_engineInterface->SetPosition(selectedEntity, scale);
	}
}

void TransformView::TransformScaleYChanged(double p_value)
{
	if (m_editorInterface->GetSelection().size() == 1)
	{
		ECS::Entity* selectedEntity = *m_editorInterface->GetSelection().begin();

		glm::vec3 scale = m_engineInterface->GetPosition(selectedEntity);
		scale.y = p_value;
		m_engineInterface->SetPosition(selectedEntity, scale);
	}
}

void TransformView::TransformScaleZChanged(double p_value)
{
	if (m_editorInterface->GetSelection().size() == 1)
	{
		ECS::Entity* selectedEntity = *m_editorInterface->GetSelection().begin();

		glm::vec3 scale = m_engineInterface->GetPosition(selectedEntity);
		scale.z = p_value;
		m_engineInterface->SetPosition(selectedEntity, scale);
	}
}