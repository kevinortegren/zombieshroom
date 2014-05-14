#pragma once

#include <RootTools/Treenity/Include/Components/AbstractComponentView.h>
#include <RootTools/Treenity/GeneratedFiles/ui_WaterColliderComponent.h>


class WaterColliderView : public AbstractComponentView
{
	Q_OBJECT
public:
	WaterColliderView(QWidget* p_parent = 0);

	const QString& GetComponentName() const;
	void DisplayEntity(ECS::Entity* p_entity);
private:
	QString m_name;
	Ui::WaterCollider ui;
private slots:
	void RadiusChanged(int p_value);
	void DisturbIntervalChanged(double p_value);
	void DisturbPowerChanged(double p_value);
};