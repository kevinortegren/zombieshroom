#pragma once
#include <Bullet/btBulletDynamicsCommon.h>
#include <Bullet/btBulletCollisionCommon.h>
#include <Bullet/BulletCollision/CollisionDispatch/btGhostObject.h>
#include <Bullet/BulletDynamics/Character/btKinematicCharacterController.h>
#include <RootEngine/Physics/Include/DebugDrawer.h>
#include <vector>
class KinematicController 
{
public:
	KinematicController(void);
	~KinematicController(void);
	void Init(btDiscreteDynamicsWorld* p_world,int p_numTriangles, int* p_indexBuffer, int p_indexStride, int p_numVertices,
		float* p_vertexBuffer, int p_vertexStride, float* p_position, float* p_rotation, float p_mass, float p_maxSpeed, float p_modelHeight,
		float p_stepHeight );
	//Assumes the float* is a float[3]
	void Walk(float* p_dir, float p_dt);
	void Jump();
	void Knockback(float* p_velocity);
	void Update(float p_dt);
	void SetOrientation(float* p_orientation);
	void RemovePlayer();
	bool IsOnGround() const {return m_onGround;};
	void SetUserPointer(void* p_userPointer);
	void SetDebugDrawer(DebugDrawer* p_drawer) const {m_kinController->debugDraw(p_drawer);}

	btVector3 GetPosition() const {return m_kinController->getGhostObject()->getWorldTransform().getOrigin();}
	btQuaternion GetOrientation() const {return m_kinController->getGhostObject()->getWorldTransform().getRotation();};

private:
	btDiscreteDynamicsWorld* m_dynamicWorld;
	btPairCachingGhostObject* m_ghostObject;
	btMotionState* m_motionState;
	btTransform m_motionTransform;
	btKinematicCharacterController* m_kinController;
	bool m_onGround;
	bool m_hittingWall;
	float m_maxSpeed;
	float m_stepHeight;
	float m_heightOffset;
	std::vector<btVector3> m_surfaceHitNormals;
	btVector3 m_knockbackVelocity;
	btVector3 m_previousPosition;
	bool m_hasBeenKnockbacked;
	void ParseGhostContacts();
	void UpdatePosition();
	void UpdateVelocity();

};

