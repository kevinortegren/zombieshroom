#pragma once
#include <Bullet/btBulletDynamicsCommon.h>
#include <Bullet/btBulletCollisionCommon.h>
#include <Bullet/BulletDynamics/Character/btKinematicCharacterController.h>
#include <Bullet/BulletCollision/CollisionDispatch/btGhostObject.h>
#include <RootEngine/Include/SubsystemSharedContext.h>
class BulletCharacter : public btKinematicCharacterController 
{
public:
	BulletCharacter(btPairCachingGhostObject* p_ghostObject, btConvexShape* p_convexShape, btScalar p_stepHeight);
	~BulletCharacter(void);
	
	void Init();
	void playerStep(btCollisionWorld* collisionWorld, btScalar dt);
	void Knockback(const btVector3& p_direction, float p_power);
	void setWalkDirection(const btVector3& walkDirection);
	bool IsKnockbacked();
	void StopKnockback();
	void stepForwardAndStrafe ( btCollisionWorld* collisionWorld, const btVector3& walkMove);
	void stepDown ( btCollisionWorld* collisionWorld, btScalar dt);
	void stepUp ( btCollisionWorld* world);
	float test(const btVector3& start,const btVector3& end, btCollisionWorld* world );
	btVector3 GetKnockbackVector() const {return m_knockbackVelocity;}
private:
	bool m_hasBeenKnockbacked;
	btVector3 m_knockbackVelocity;
};

