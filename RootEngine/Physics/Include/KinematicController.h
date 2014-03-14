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
	void Deactivate();
	void Activate();
	//Assumes the float* is a float[3]
	void Walk(glm::vec3 p_dir, float p_dt);
	void Jump(float p_jumpPower);
	void JumpBoost(float p_boostPower);
	void Knockback(const btVector3& p_velocity, float p_dt);
	void Update(float p_dt);
	void RemovePlayer();
	void Move(glm::vec3 p_target, float p_dt);
	bool IsOnGround();
	void SetUserPointer(void* p_userPointer);
	void SetDebugDrawer(DebugDrawer* p_drawer) const {m_kinController->debugDraw(p_drawer);}
	void SetMass(float p_mass);
	void SetGravity(float p_gravity);
	void SetPosition(const btVector3& p_position);
	void SetJumpForce(float p_jumpForce);
	void SetOrientation(glm::quat p_orientation);
	float GetMass() const {return m_mass;}
	float GetMaxSpeed() const {return m_maxSpeed;}
	float GetStepHeight() const {return m_stepHeight;}
	float GetModelHeight() const {return m_heightOffset;}
	void* GetUserPointer() const {return m_ghostObject->getUserPointer();}
	
	btVector3 GetPosition() const {return m_kinController->getGhostObject()->getWorldTransform().getOrigin();}
	btQuaternion GetOrientation() const {return m_kinController->getGhostObject()->getWorldTransform().getRotation();}
	btTransform GetTransform() const {return m_kinController->getGhostObject()->getWorldTransform();}
	btVector3 GetKnockbackVector() const {return m_kinController->GetKnockbackVector();}
	float GetVerticalVelocity() const {return m_kinController->GetVerticalVelocity();}

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
	bool m_activated;
};

