#include <RootTools/Treenity/Include/Components/WaterColliderView.h>

WaterColliderView::WaterColliderView( QWidget* p_parent /*= 0*/ )
	: AbstractComponentView(p_parent)
	, m_name("Water Collider")
{
	ui.setupUi(this);
	
	connect(ui.doubleSpinBox_distInterval,	SIGNAL(valueChanged(double)),	this,		SLOT(DisturbIntervalChanged(double)));
	connect(ui.doubleSpinBox_distPower,		SIGNAL(valueChanged(double)),	this,		SLOT(DisturbPowerChanged(double)));
	connect(ui.spinBox_radius,				SIGNAL(valueChanged(int)),		this,		SLOT(RadiusChanged(int)));
}

const QString& WaterColliderView::GetComponentName() const
{
	return m_name;
}

void WaterColliderView::DisplayEntity( ECS::Entity* p_entity )
{
	ui.doubleSpinBox_distInterval->setValue((double)m_engineInterface->GetWaterColliderInterval(p_entity));
	ui.doubleSpinBox_distPower->setValue((double)m_engineInterface->GetWaterColliderPower(p_entity));
	ui.spinBox_radius->setValue(m_engineInterface->GetWaterColliderRadius(p_entity));
}

void WaterColliderView::RadiusChanged( int p_value )
{
	if (m_editorInterface->GetSelection().size() == 1)
	{
		ECS::Entity* selectedEntity = *m_editorInterface->GetSelection().begin();

		m_engineInterface->SetWaterColliderRadius(selectedEntity, p_value);
	}
}

void WaterColliderView::DisturbIntervalChanged( double p_value )
{
	if (m_editorInterface->GetSelection().size() == 1)
	{
		ECS::Entity* selectedEntity = *m_editorInterface->GetSelection().begin();

		m_engineInterface->SetWaterColliderInterval(selectedEntity, (float)p_value);
	}
}

void WaterColliderView::DisturbPowerChanged( double p_value )
{
	if (m_editorInterface->GetSelection().size() == 1)
	{
		ECS::Entity* selectedEntity = *m_editorInterface->GetSelection().begin();

		m_engineInterface->SetWaterColliderPower(selectedEntity, (float)p_value);
	}
}

