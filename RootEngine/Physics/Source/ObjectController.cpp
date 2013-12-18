#include "ObjectController.h"

class btKinematicClosestNotMeConvexResultCallback : public btCollisionWorld::ClosestConvexResultCallback
{
public:
	btKinematicClosestNotMeConvexResultCallback (btCollisionObject* me, const btVector3& up, btScalar minSlopeDot)
		: btCollisionWorld::ClosestConvexResultCallback(btVector3(0.0, 0.0, 0.0), btVector3(0.0, 0.0, 0.0))
		, m_me(me)
		, m_up(up)
		, m_minSlopeDot(minSlopeDot)
	{
	}

	virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult,bool normalInWorldSpace)
	{
		if (convexResult.m_hitCollisionObject == m_me)
			return btScalar(1.0);

		if (!convexResult.m_hitCollisionObject->hasContactResponse())
			return btScalar(1.0);

		btVector3 hitNormalWorld;
		if (normalInWorldSpace)
		{
			hitNormalWorld = convexResult.m_hitNormalLocal;
		} else
		{
			///need to transform normal into worldspace
			hitNormalWorld = convexResult.m_hitCollisionObject->getWorldTransform().getBasis()*convexResult.m_hitNormalLocal;
		}

		btScalar dotUp = m_up.dot(hitNormalWorld);
		if (dotUp < m_minSlopeDot) {
			return btScalar(1.0);
		}

		return ClosestConvexResultCallback::addSingleResult (convexResult, normalInWorldSpace);
	}
protected:
	btCollisionObject* m_me;
	const btVector3 m_up;
	btScalar m_minSlopeDot;
};
ObjectController::ObjectController()
{

}

ObjectController::~ObjectController()
{

}

void ObjectController::Init(btPairCachingGhostObject* p_ghostObject, btConvexShape* p_convexShape)
{
	m_ghostObject = p_ghostObject;
	m_convexShape = p_convexShape;
}

void ObjectController::Move( glm::vec3 p_targetPos, btDiscreteDynamicsWorld* p_world )
{
	btVector3 startPos = btVector3(m_currentPos.x,m_currentPos.y,m_currentPos.z);
	btVector3 endPos = btVector3(p_targetPos.x,p_targetPos.y,p_targetPos.z);
	btTransform start, end;
	start.setIdentity ();
	end.setIdentity ();
	start.setOrigin (startPos);
	end.setOrigin (endPos);
	btKinematicClosestNotMeConvexResultCallback callback(m_ghostObject, btVector3(0,1,0), btScalar(0.7071));
	callback.m_collisionFilterGroup = m_ghostObject->getBroadphaseHandle()->m_collisionFilterGroup;
	callback.m_collisionFilterMask = m_ghostObject->getBroadphaseHandle()->m_collisionFilterMask;
	m_ghostObject->convexSweepTest (m_convexShape, start, end, callback, p_world->getDispatchInfo().m_allowedCcdPenetration);

}

void ObjectController::SetOrientation( glm::quat p_orientation )
{
	btQuaternion btquat;
	btquat.setW(p_orientation.w);
	btquat.setX(p_orientation.x);
	btquat.setY(p_orientation.y);
	btquat.setZ(p_orientation.z);
	m_ghostObject->getWorldTransform().setRotation(btquat);
}

