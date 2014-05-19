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

	// [col][row]
	glm::mat3 matrix = m_engineInterface->GetOrientation(p_entity).GetMatrix();

	float x = glm::degrees(atan2f(matrix[1][2], matrix[2][2]));
	float y = glm::degrees(atan2f(-matrix[0][2], sqrtf((matrix[1][2]*matrix[1][2])+(matrix[2][2]*matrix[2][2]))));
	float z = glm::degrees(atan2f(matrix[0][1], matrix[0][0]));

	ui.doubleSpinBox_orientationX->setValue(x);
	ui.doubleSpinBox_orientationY->setValue(y);
	ui.doubleSpinBox_orientationZ->setValue(z);

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

		glm::mat3 matrix = m_engineInterface->GetOrientation(selectedEntity).GetMatrix();

		float x = p_value;
		float y = glm::degrees(atan2f(-matrix[0][2], sqrtf((matrix[1][2]*matrix[1][2])+(matrix[2][2]*matrix[2][2]))));
		float z = glm::degrees(atan2f(matrix[0][1], matrix[0][0]));

		m_engineInterface->SetOrientation(selectedEntity, RootForce::Orientation(x, y, z));
	}
}

void TransformView::OrientationYChanged(double p_value)
{
	int revs = floorf(p_value / 360.0f);
	float degree = p_value - revs * 360;
	
	int flipped = 1;
	if(degree > 270.0f)
	{
		degree -= 360.0f;
	}
	if(degree > 90.0f)
	{
		flipped = -1;
		degree -= 180.0f;
	}
	std::cout << " Degree: " << degree << " Flipped: " << flipped << std::endl; 

	if (m_editorInterface->GetSelection().size() == 1)
	{
		ECS::Entity* selectedEntity = *m_editorInterface->GetSelection().begin();

		glm::mat3 matrix = m_engineInterface->GetOrientation(selectedEntity).GetMatrix();

		float x = glm::degrees(atan2f(matrix[1][2], matrix[2][2]));
		float y = degree;
		float z = glm::degrees(atan2f(matrix[0][1], matrix[0][0]));

		m_engineInterface->SetOrientation(selectedEntity, RootForce::Orientation(x, y, z));
	}
}

void TransformView::OrientationZChanged(double p_value)
{
	if (m_editorInterface->GetSelection().size() == 1)
	{
		ECS::Entity* selectedEntity = *m_editorInterface->GetSelection().begin();

		glm::mat3 matrix = m_engineInterface->GetOrientation(selectedEntity).GetMatrix();

		float x = glm::degrees(atan2f(matrix[1][2], matrix[2][2]));
		float y = glm::degrees(atan2f(-matrix[0][2], sqrtf((matrix[1][2]*matrix[1][2])+(matrix[2][2]*matrix[2][2]))));
		float z = p_value;

		m_engineInterface->SetOrientation(selectedEntity, RootForce::Orientation(x, y, z));
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