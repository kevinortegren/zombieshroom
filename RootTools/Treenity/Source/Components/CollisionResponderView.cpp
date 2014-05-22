#include <RootTools/Treenity/Include/Components/CollisionResponderView.h>

CollisionResponderView::CollisionResponderView( QWidget* p_parent /*= 0*/ )
	: AbstractComponentView(p_parent)
	, m_name("Collision Responder")
{

}

const QString& CollisionResponderView::GetComponentName() const
{
	return m_name;
}

void CollisionResponderView::DisplayEntity( ECS::Entity* p_entity )
{
}

