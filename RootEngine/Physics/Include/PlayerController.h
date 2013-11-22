#pragma once
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include <vector>
class PlayerController
{
public:
	PlayerController(void);
	~PlayerController(void);
	void Init(btDiscreteDynamicsWorld* p_world,int p_numTriangles, int* p_indexBuffer, int p_indexStride, int p_numVertices,
		float* p_vertexBuffer, int p_vertexStride, float* p_position, float* p_rotation, float p_mass, float m_maxSpeed );
	//Assumes the float* is a float[3]
	void Walk(float* p_dir);
	void Jump(float p_jumpforce);
	void Update();
	btVector3 GetPosition() const;
	btVector3 GetVelocity() const;
	bool IsOnGround() const;
private:
	btDiscreteDynamicsWorld* m_dynamicWorld;
	btRigidBody* m_rigidBody;
	btPairCachingGhostObject* m_ghostObject;
	btMotionState* m_motionState;
	btTransform m_motionTransform;
	bool m_onGround;
	bool m_hittingWall;
	float m_maxSpeed;
	std::vector<btVector3> m_surfaceHitNormals;
	btVector3 m_manualVelocity;
	btVector3 m_previousPosition;
	void ParseGhostContacts();
	void UpdatePosition();
	void UpdateVelocity();

};

