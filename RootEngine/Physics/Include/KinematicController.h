#pragma once
#include <Bullet/btBulletDynamicsCommon.h>
#include <Bullet/btBulletCollisionCommon.h>
#include <Bullet/BulletCollision/CollisionDispatch/btGhostObject.h>
#include <RootEngine/Physics/Include/BulletCharacter.h>
#include <RootEngine/Physics/Include/DebugDrawer.h>
#include <vector>
class KinematicController 
{
public:
	KinematicController(void);
	~KinematicController(void);
	void Init(btDiscreteDynamicsWorld* p_world,int p_numTriangles, int* p_indexBuffer, int p_indexStride, int p_numVertices,
		float* p_vertexBuffer, int p_vertexStride, glm::vec3 p_position, glm::quat p_rotation, float p_mass, float p_maxSpeed, float p_modelHeight,
		float p_stepHeight );
	//Assumes the float* is a float[3]
	void Walk(glm::vec3 p_dir, float p_dt);
	void Jump();
	void Knockback(const btVector3& p_velocity, float p_dt);
	void Update(float p_dt);
	void SetOrientation(glm::quat p_orientation);
	void RemovePlayer();
	void SetUserPointer(void* p_userPointer);
	void SetDebugDrawer(DebugDrawer* p_drawer) const {m_kinController->debugDraw(p_drawer);}
	void SetMass(float p_mass);
	void SetGravity(float p_gravity);
	float GetMass() const {return m_mass;}
	float GetMaxSpeed() const {return m_maxSpeed;}
	float GetStepHeight() const {return m_stepHeight;}
	float GetModelHeight() const {return m_heightOffset;}
	btVector3 GetPosition() const {return m_kinController->getGhostObject()->getWorldTransform().getOrigin();}
	btQuaternion GetOrientation() const {return m_kinController->getGhostObject()->getWorldTransform().getRotation();}

private:
	btDiscreteDynamicsWorld* m_dynamicWorld;
	btPairCachingGhostObject* m_ghostObject;
	btMotionState* m_motionState;
	BulletCharacter* m_kinController;
	bool m_hasStepped;
	float m_maxSpeed;
	float m_stepHeight;
	float m_heightOffset;
	float m_mass;
	bool m_hasBeenKnockbacked;

};

