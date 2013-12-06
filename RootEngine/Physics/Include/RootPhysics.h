#pragma once

#include <vector>
#include "PlayerController.h"
#include <RootEngine/Include/SubsystemSharedContext.h>
#include <RootEngine/Render/Include/Renderer.h>
#include <RootEngine/Physics/Include/DebugDrawer.h>
#include <RootEngine/Physics/Include/PhysicsMesh.h>

#if defined(_WINDLL)
#define PHYSICS_DLL_EXPORT __declspec(dllexport)
#else
#define PHYSICS_DLL_EXPORT __declspec(dllimport)
#endif

namespace RootEngine
{
	namespace Physics
	{	
		namespace PhysicsType
		{
			enum PhysicsType
			{
				TYPE_STATIC,
				TYPE_ABILITY,
				TYPE_DYNAMIC,
				TYPE_PLAYER
			};
		}
		namespace AbilityShape
		{
			enum AbilityShape
			{
				SHAPE_SPHERE,
				SHAPE_CONE,
				SHAPE_CYLINDER
			};
		}
		struct AbilityPhysicsInfo
		{
			int m_entityId; 
			float m_radius;
			float m_height; 
			float* m_position; 
			float* m_orientation; 
			float* m_direction; 
			float m_speed; 
			PhysicsType::PhysicsType m_type; 
			AbilityShape::AbilityShape m_shape;
			bool m_collidesWorld ;
			float m_mass; 
			float* m_gravity;
		};



		struct CustomUserPointer
		{
			int m_type;
			int m_vectorIndex;
			unsigned int m_entityId; //My entity id
			std::vector<unsigned int> m_collidedEntityId; //List of all entities collided with since last update
			int* m_id; // The value that is returned as a handle to the game logic, should be updated when a object is removed.
			bool m_collided;
			std::string m_modelHandle;
			bool m_isGhostObject;
			~CustomUserPointer()
			{
				delete m_id;
			}
		};

		class PhysicsInterface : public RootEngine::SubsystemInterface
		{
		public:
			virtual void Init() = 0;
			virtual void CreatePlane(float* p_normal, float* p_position) = 0;
			virtual void Update(float p_dt) = 0;

			///Set the direction a controllable object is facing, should be sent in every update and is assumed to be a vec3, the y value is ignored however
			virtual void PlayerKnockback(int p_objectIndex, float* p_pushDirection, float p_pushForce) = 0; ///p_pushDirection is the direction the pushing has, for the love of god normalize it first
			virtual void PlayerMoveXZ(int p_objectIndex, float* p_direction) = 0;
			///Call this when a character jumps
			virtual void PlayerJump(int p_objectIndex, float p_jumpForce) = 0;
			///Object index is the value returned by the add function, velocity is a vec3 of the objects velocity (speed*target) Used for abilities mainly
			virtual void SetDynamicObjectVelocity(int p_objectIndex, float* p_velocity) = 0;
			virtual void SetObjectMass(int p_objectIndex, float p_mass) = 0;
			///Use this to add a static object to the World, i.e trees, rocks and the ground. Both position and rotation are vec3
			virtual void AddStaticObjectToWorld(std::string p_modelHandle, unsigned int p_entityId , float* p_position, float* p_rotation) = 0;
			/*	Use this to add a dynamic object to the World, i.e trees, rocks and the ground. Both position and rotation are vec3, mass affect how the object behaves in the world. Note: Mass must be >0 
			virtual The return value is the index to the objects rigidbody and should be used where a index parameter is requested*/
			virtual int* AddDynamicObjectToWorld(std::string p_modelHandle, unsigned int p_entityId,  float* p_position, float* p_rotation, float p_mass) = 0;
			//Use this to add a Controllable object to the world, i.e Players. Return value is the index position of the object. position and rotation is of type float[3]
			virtual int* AddPlayerObjectToWorld(std::string p_modelHandle, unsigned int p_entityId, float* p_position, float* p_rotation, float p_mass, float p_maxSpeed, float p_modelHeight, float p_stepHeight) = 0;
			virtual int* AddAbilityToWorld(AbilityPhysicsInfo p_abilityInfo) = 0;
			/// p_Pos should be of type float[3]
			virtual void GetPos(int p_objectIndex, float* p_pos)= 0;
			virtual float GetMass(int p_objectIndex) = 0;
			virtual int GetType(int p_objectIndex) = 0;
			virtual float GetMaxSpeed(int p_objectIndex) = 0;
			virtual float GetStepHeight(int p_objectIndex) = 0;
			virtual float GetModelHeight(int p_objectIndex) = 0;
			virtual std::vector<unsigned int>* GetCollisionVector(int p_objectIndex) = 0;
			virtual std::string GetPhysicsModelHandle(int p_objectIndex) = 0;
			virtual void GetObjectOrientation(int p_objectIndex, float* p_objectOrientation) = 0;
			virtual void SetObjectOrientation(int p_objectIndex, float* p_objectOrientation) = 0;
			virtual void SetPlayerOrientation(int p_objectIndex, float* p_playerOrientation) = 0;
			
			//Best conflict ever
// <<<<<<< HEAD
// 		
// =======
// >>>>>>> 4a674b62a86cff39358cc2383b8a4ccee0d43c66
			virtual void RemoveObject(int p_objectIndex) = 0;
			virtual std::shared_ptr<PhysicsMeshInterface> CreatePhysicsMesh() = 0;
		};

		

		class RootPhysics : public PhysicsInterface
		{
		public:
			void Startup();
			void Shutdown();
			static RootPhysics* GetInstance();
			void CreatePlane(float* p_normal, float* p_position);
			
			void Update(float p_dt);
			///Set the direction a controllable object is facing, should be sent in every update and is assumed to be a vec3, the y value is ignored however
			void PlayerMoveXZ(int p_objectIndex, float* p_direction);
		
			void PlayerJump(int p_objectIndex, float p_jumpForce); ///Call this when a character jumps
			void PlayerKnockback(int p_objectIndex, float* p_pushDirection, float p_pushForce); ///p_pushDirection is the direction the pushing has, for the love of god normalize it first
			void SetDynamicObjectVelocity(int p_objectIndex, float* p_velocity); ///Object index is the value returned by the add function, velocity is a vec3 of the objects velocity (speed*target) Used for abilities mainly

			void SetObjectMass(int p_objectIndex, float p_mass);
		
			///Use this to add a static object to the World, i.e trees, rocks and the ground. Both position and rotation are vec3
			void AddStaticObjectToWorld(std::string p_modelHandle,unsigned int p_entityId,  float* p_position, float* p_rotation);
			/*	Use this to add a dynamic object to the World, i.e trees, rocks and the ground. Both position and rotation are vec3, mass affect how the object behaves in the world. Note: Mass must be >0 
			The return value is the index to the objects rigidbody and should be used where a index parameter is requested*/
			int* AddDynamicObjectToWorld(std::string p_modelHandle,unsigned int p_entityId,  float* p_position, float* p_rotation, float p_mass);
			///Use this to add a Controllable object to the world, i.e Players. Return value is the index position of the object. position and rotation is of type float[3]
			int* AddPlayerObjectToWorld(std::string p_modelHandle,unsigned int p_entityId, float* p_position, float* p_rotation, float p_mass,float p_maxSpeed, float p_modelHeight, float p_stepHeight);

			int* AddAbilityToWorld(AbilityPhysicsInfo p_abilityInfo);
			void GetPos(int p_objectIndex, float* p_pos);	/// p_pos should be of type float[3]
			float GetMass(int p_objectIndex);
			int GetType(int p_objectIndex);
			float GetMaxSpeed(int p_objectIndex);
			float GetStepHeight(int p_objectIndex);
			float GetModelHeight(int p_objectIndex);
			std::vector<unsigned int>* GetCollisionVector(int p_objectIndex);
			std::string GetPhysicsModelHandle(int p_objectIndex);
			void SetGravity(int p_objectIndex, float* p_gravity);
			void GetObjectOrientation(int p_objectIndex, float* p_objectOrientation);
			void SetObjectOrientation(int p_objectIndex, float* p_objectOrientation);
			void SetPlayerOrientation(int p_objectIndex, float* p_playerOrientation);
			void RemoveObject(int p_objectIndex);
			std::shared_ptr<PhysicsMeshInterface> CreatePhysicsMesh() { return std::shared_ptr<PhysicsMeshInterface>(new PhysicsMesh); }

		private:
			void Init();
			bool DoesObjectExist(int p_objectIndex);
			btRigidBody* CreateSphere(float p_radius, float p_mass, float* p_position);
			btRigidBody* CreateCone(float p_radius, float p_height, float* p_position, float* p_rotation, float p_mass);
			btRigidBody* CreateCylinder(float p_radius, float p_height,  float* p_position, float* p_rotation, float p_mass);
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
			std::vector<PlayerController*> m_playerObjects;
			
		};
	}
}
extern "C"
{
	typedef RootEngine::Physics::PhysicsInterface* (*CREATEPHYSICS)(RootEngine::SubsystemSharedContext, Render::RendererInterface *, RootEngine::ResourceManagerInterface*);
	PHYSICS_DLL_EXPORT RootEngine::Physics::PhysicsInterface* CreatePhysics(RootEngine::SubsystemSharedContext p_context, Render::RendererInterface* p_renderer, RootEngine::ResourceManagerInterface* p_resouceManager);
}