#pragma once

#include <RootTools/Treenity/Include/Components/AbstractComponentView.h>
#include <RootTools/Treenity/GeneratedFiles/ui_PhysicsComponent.h>


class PhysicsView : public AbstractComponentView
{
	Q_OBJECT
public:
	PhysicsView(QWidget* p_parent = 0);

	const QString& GetComponentName() const;
	void DisplayEntity(ECS::Entity* p_entity);
private:
	QString m_name;
	Ui::Physics ui;
private slots:
	/*
	void VelocityXChanged(double p_value);
	void VelocityYChanged(double p_value);
	void VelocityZChanged(double p_value);
	*/
	void MassChanged(double p_value);
};