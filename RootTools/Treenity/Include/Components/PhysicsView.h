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
	static const int TYPE_DYNAMIC = 0;
	static const int TYPE_STATIC = 1;

	static const int SHAPE_SPHERE = 0;
	static const int SHAPE_CONE = 1;
	static const int SHAPE_CYLINDER = 2;
	static const int SHAPE_MESH = 3;

	QString m_name;
	Ui::Physics ui;

	void BlockAllSignals(bool p_block);
private slots:
	void TypeChanged(int p_value);
	void CollisionWithWorldChanged(bool p_value);
	void CollisionWithStaticChanged(bool p_value);
	void GravityXChanged(double p_value);
	void GravityYChanged(double p_value);
	void GravityZChanged(double p_value);
	void MassChanged(double p_value);
	void ShapeChanged(int p_value);
	void SphereRadiusChanged(double p_value);
	void ConeRadiusChanged(double p_value);
	void ConeHeightChanged(double p_value);
	void CylinderRadiusChanged(double p_value);
	void CylinderHeightChanged(double p_value);
	void PhysicsMeshChanged();
	void PhysicsMeshBrowse();
};