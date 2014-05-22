#pragma once

#include <RootTools/Treenity/Include/Components/AbstractComponentView.h>

class CollisionResponderView : public AbstractComponentView
{
	Q_OBJECT
public:
	CollisionResponderView(QWidget* p_parent = 0);

	const QString& GetComponentName() const;
	void DisplayEntity(ECS::Entity* p_entity);
private:
	QString m_name;
};