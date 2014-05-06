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
	void TransformPositionXChanged(double p_value);
	void TransformPositionYChanged(double p_value);
	void TransformPositionZChanged(double p_value);
	void TransformOrientationXChanged(double p_value);
	void TransformOrientationYChanged(double p_value);
	void TransformOrientationZChanged(double p_value);
	void TransformScaleXChanged(double p_value);
	void TransformScaleYChanged(double p_value);
	void TransformScaleZChanged(double p_value);
};