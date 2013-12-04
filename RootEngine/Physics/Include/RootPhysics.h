#pragma once
#include <RootEngine/Physics/Include/PhysicsMesh.h>
#include <vector>
#include "PlayerController.h"
#include <RootEngine/Include/SubsystemSharedContext.h>
#include <RootEngine/Render/Include/Renderer.h>
#include <RootEngine/Physics/Include/DebugDrawer.h>
#if defined(_WINDLL)
#define PHYSICS_DLL_EXPORT __declspec(dllexport)
#else
#define PHYSICS_DLL_EXPORT __declspec(dllimport)
#endif


namespace Physics
{

	struct PhysicWorldInfo
	{
		bool m_hasHit;
		int m_index;
		int m_collidedType; //Might not be needed
		float m_position[3];
		int m_type;
	};
	class PhysicsInterface : public RootEngine::SubsystemInterface
	{
	public:
		
		virtual void Init() = 0;
		virtual void CreatePlane(float* p_normal, float* p_position) = 0;
		virtual void Update(float p_dt) = 0;

		virtual int CreateSphere(float p_radius, float p_mass, float* p_position) = 0;
		///Set the direction a controllable object is facing, should be sent in every update and is assumed to be a vec3, the y value is ignored however
		virtual void PlayerKnockback(int p_objectIndex, float* p_pushDirection, float p_pushForce) = 0; ///p_pushDirection is the direction the pushing has, for the love of god normalize it first
		virtual void PlayerMoveXZ(int p_objectIndex, float* p_direction) = 0;
		///Call this when a character jumps
		virtual void PlayerJump(int p_objectIndex, float p_jumpForce) = 0;
		///Object index is the value returned by the add function, velocity is a vec3 of the objects velocity (speed*target) Used for abilities mainly
		virtual void SetDynamicObjectVelocity(int p_objectIndex, float* p_velocity) = 0;
		virtual void SetObjectMass(int p_objectIndex, float p_mass) = 0;
		///Use this to add a static object to the World, i.e trees, rocks and the ground. Both position and rotation are vec3
		virtual void AddStaticObjectToWorld(int p_numTriangles, int* p_indexBuffer, int p_indexStride, int p_numVertices, float* p_vertexBuffer, int p_vertexStride, float* p_position, float* p_rotation) = 0;
		/*	Use this to add a dynamic object to the World, i.e trees, rocks and the ground. Both position and rotation are vec3, mass affect how the object behaves in the world. Note: Mass must be >0 
		virtual The return value is the index to the objects rigidbody and should be used where a index parameter is requested*/
		virtual int AddDynamicObjectToWorld( int p_numTriangles, int* p_indexBuffer, int p_indexStride, int p_numVertices, float* p_vertexBuffer, int p_vertexStride, float* p_position, float* p_rotation, float p_mass) = 0;
		//Use this to add a Controllable object to the world, i.e Players. Return value is the index position of the object. position and rotation is of type float[3]
		virtual int AddPlayerObjectToWorld(int p_numTriangles, int* p_indexBuffer, int p_indexStride, int p_numVertices, float* p_vertexBuffer, int p_vertexStride, float* p_position, float* p_rotation, float p_mass,
			float p_maxSpeed, float p_modelHeight, float p_stepHeight) = 0;
		/// p_playerPos should be of type float[3]
		virtual void GetPlayerPos(int p_objectIndex, float* p_playerPos)= 0;
		/// p_objectPos should be of type float[3]
		virtual void GetObjectPos(int p_objectIndex, float* p_objectPos) = 0;

		virtual void GetObjectOrientation(int p_objectIndex, float* p_objectOrientation) = 0;
		virtual void SetObjectOrientation(int p_objectIndex, float* p_objectOrientation) = 0;
		virtual void SetPlayerOrientation(int p_objectIndex, float* p_playerOrientation) = 0;

		virtual void RemoveObject(int p_objectIndex, int p_type) = 0;
		virtual PhysicsMeshInterface* CreatePhysicsMesh() = 0;
		
	};


	class RootPhysics : public PhysicsInterface
	{
	public:
		void Startup();
		void Shutdown();
		
		static RootPhysics* GetInstance();
		void CreatePlane(float* p_normal, float* p_position);
		int CreateSphere(float p_radius, float p_mass, float* p_position);
		void Update(float p_dt);
		///Set the direction a controllable object is facing, should be sent in every update and is assumed to be a vec3, the y value is ignored however
		void PlayerMoveXZ(int p_objectIndex, float* p_direction);
		
		void PlayerJump(int p_objectIndex, float p_jumpForce); ///Call this when a character jumps
		void PlayerKnockback(int p_objectIndex, float* p_pushDirection, float p_pushForce); ///p_pushDirection is the direction the pushing has, for the love of god normalize it first
		void SetDynamicObjectVelocity(int p_objectIndex, float* p_velocity); ///Object index is the value returned by the add function, velocity is a vec3 of the objects velocity (speed*target) Used for abilities mainly

		void SetObjectMass(int p_objectIndex, float p_mass);
		
		///Use this to add a static object to the World, i.e trees, rocks and the ground. Both position and rotation are vec3
		void AddStaticObjectToWorld(int p_numTriangles, int* p_indexBuffer, int p_indexStride, int p_numVertices, float* p_vertexBuffer, int p_vertexStride, float* p_position, float* p_rotation);
		/*	Use this to add a dynamic object to the World, i.e trees, rocks and the ground. Both position and rotation are vec3, mass affect how the object behaves in the world. Note: Mass must be >0 
		The return value is the index to the objects rigidbody and should be used where a index parameter is requested*/
		int AddDynamicObjectToWorld( int p_numTriangles, int* p_indexBuffer, int p_indexStride, int p_numVertices, float* p_vertexBuffer, int p_vertexStride, float* p_position, float* p_rotation, float p_mass);
		///Use this to add a Controllable object to the world, i.e Players. Return value is the index position of the object. position and rotation is of type float[3]
		int AddPlayerObjectToWorld(int p_numTriangles, int* p_indexBuffer, int p_indexStride, int p_numVertices, float* p_vertexBuffer, int p_vertexStride,
									float* p_position, float* p_rotation, float p_mass,float p_maxSpeed, float p_modelHeight, float p_stepHeight);
		void GetPlayerPos(int p_objectIndex, float* p_playerPos);	/// p_playerPos should be of type float[3]	
		void GetObjectPos(int p_objectIndex, float* p_objectPos);/// p_objectPos should be of type float[3]

		void GetObjectOrientation(int p_objectIndex, float* p_objectOrientation);
		void SetObjectOrientation(int p_objectIndex, float* p_objectOrientation);
		void SetPlayerOrientation(int p_objectIndex, float* p_playerOrientation);
		void RemoveObject(int p_objectIndex, int p_type);
		PhysicsMeshInterface* CreatePhysicsMesh() { return new PhysicsMesh; }

	private:
		/*const int TERRAIN = 0;
		const int PLAYER = 1;
		const int ABILITY = 2;*/
		struct CustomUserPointer
		{
			void* m_object; //btRigidbody for abilitys and playercontroller for players
			PhysicWorldInfo m_worldInfo;
			int m_vectorIndex;
		};
		void Init();
		RootPhysics();
		~RootPhysics();
		DebugDrawer* m_debugDrawer;
		btDiscreteDynamicsWorld* m_dynamicWorld;
		btDefaultCollisionConfiguration* m_collisionConfig;
		btCollisionDispatcher* m_dispatcher;
		btSequentialImpulseConstraintSolver* m_solver;
		btBroadphaseInterface* m_broadphase;
		static RootPhysics* s_physicsInstance;
		std::vector<CustomUserPointer*> m_userPointer;
		std::vector<btRigidBody*> m_dynamicObjects;
		std::vector<PlayerController*> m_playerObject;
	};
}
extern "C"
{
	typedef Physics::PhysicsInterface* (*CREATEPHYSICS)(RootEngine::SubsystemSharedContext, Render::RendererInterface *, RootEngine::ResourceManagerInterface*);
	PHYSICS_DLL_EXPORT Physics::PhysicsInterface* CreatePhysics(RootEngine::SubsystemSharedContext p_context, Render::RendererInterface* p_renderer, RootEngine::ResourceManagerInterface* p_resouceManager);
}