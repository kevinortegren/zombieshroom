#pragma once

#include <RootTools/Treenity/Include/Components/AbstractComponentView.h>
#include <RootTools/Treenity/GeneratedFiles/ui_TransformComponent.h>


class TransformView : public AbstractComponentView
{
	Q_OBJECT
public:
	TransformView(QWidget* p_parent = 0);

	const QString& GetComponentName() const;
	void DisplayEntity(ECS::Entity* p_entity);
private:
	QString m_name;
	Ui::Transform ui;
private slots:
	void PositionXChanged(double p_value);
	void PositionYChanged(double p_value);
	void PositionZChanged(double p_value);
	void OrientationXChanged(double p_value);
	void OrientationYChanged(double p_value);
	void OrientationZChanged(double p_value);
	void ScaleXChanged(double p_value);
	void ScaleYChanged(double p_value);
	void ScaleZChanged(double p_value);
};