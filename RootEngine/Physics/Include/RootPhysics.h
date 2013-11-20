#pragma once
#include "Bullet/btBulletDynamicsCommon.h"
#include "Bullet/btBulletCollisionCommon.h"
#include <vector>

namespace PhysicsSubSystem
{
	class RootPhysics
	{
	public:
		RootPhysics();
		~RootPhysics();
		void Init();
		void CreatePlane(float* p_normal, float* p_position);
		void supertestfunc(); //Super mega awsosme temp
		void Update();
		//Use this to add a static object to the World, i.e trees, rocks and the ground. Both position and rotation are vec3
		void AddStaticObjectToWorld(int p_numTriangles, int* p_indexBuffer, int p_indexStride, int p_numVertices, float* p_vertexBuffer, int p_vertexStride, float* p_position, float* p_rotation);
		/*	Use this to add a dynamic object to the World, i.e trees, rocks and the ground. Both position and rotation are vec3, mass affect how the object behaves in the world. Note: Mass must be >0 
		The return value is the index to the objects rigidbody and should be used where a index parameter is requested*/
		int AddDynamicObjectToWorld( int p_numTriangles, int* p_indexBuffer, int p_indexStride, int p_numVertices, float* p_vertexBuffer, int p_vertexStride, float* p_position, float* p_rotation, float p_mass);
	private:
		btDiscreteDynamicsWorld* m_dynamicWorld;
		btDefaultCollisionConfiguration* m_collisionConfig;
		btCollisionDispatcher* m_dispatcher;
		btSequentialImpulseConstraintSolver* m_solver;
		btBroadphaseInterface* m_broadphase;
		btRigidBody* fallingballbody;
		std::vector<btRigidBody*> m_dynamicObjects;
	};
}