#pragma once
#include <Bullet/btBulletDynamicsCommon.h>
#include <Bullet/btBulletCollisionCommon.h>
#include <Bullet/BulletCollision/CollisionDispatch/btGhostObject.h>
#include <vector>
class PlayerController
{
public:
	PlayerController(void);
	~PlayerController(void);
	void Init(btDiscreteDynamicsWorld* p_world,int p_numTriangles, int* p_indexBuffer, int p_indexStride, int p_numVertices,
		float* p_vertexBuffer, int p_vertexStride, float* p_position, float* p_rotation, float p_mass, float p_maxSpeed, float p_modelHeight,
		float p_stepHeight );
	//Assumes the float* is a float[3]
	void Walk(float* p_dir);
	void Jump(float p_jumpforce);
	void Knockback(float* p_velocity);
	void Update();
	void SetOrientation(float* p_orientation);
	void RemovePlayer();
	void SetPrevVelocity();
	btVector3 GetPosition() const {return m_motionTransform.getOrigin();}
	btVector3 GetVelocity() const {return m_rigidBody->getLinearVelocity();}
	btQuaternion GetOrientation() const {return m_rigidBody->getWorldTransform().getRotation();}
	float GetMass() const {return 1/m_rigidBody->getInvMass();}
	float GetMaxSpeed() const {return m_maxSpeed;}
	float GetStepHeight() const {return m_stepHeight;}
	float GetModetHeight() const {return m_heightOffset*2;}
	bool IsOnGround() const {return m_onGround;}
	void SetUserPointer(void* p_userPointer);
private:
	btDiscreteDynamicsWorld* m_dynamicWorld;
	btRigidBody* m_rigidBody;
	btPairCachingGhostObject* m_ghostObject;
	btMotionState* m_motionState;
	btTransform m_motionTransform;
	bool m_onGround;
	bool m_hittingWall;
	float m_maxSpeed;
	float m_stepHeight;
	float m_heightOffset;
	std::vector<btVector3> m_surfaceHitNormals;
	btVector3 m_manualVelocity;
	btVector3 m_previousPosition;
	void ParseGhostContacts();
	void UpdatePosition();
	void UpdateVelocity();

};

