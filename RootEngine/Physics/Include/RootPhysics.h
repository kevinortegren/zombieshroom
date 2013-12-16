#pragma once

#include <vector>
#include <set>
#include "KinematicController.h"
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
				SHAPE_CYLINDER,
				SHAPE_CUSTOM_MESH
			};
		}
		struct AbilityPhysicsInfo
		{
			int m_entityId; 
			float m_radius;
			float m_height; 
			glm::vec3 m_position; 
			glm::quat m_orientation; 
			glm::vec3 m_direction; 
			float m_speed; 
			PhysicsType::PhysicsType m_type; 
			AbilityShape::AbilityShape m_shape;
			bool m_collidesWorld ;
			float m_mass; 
			glm::vec3 m_gravity;
			std::string m_modelHandle; //No need to specify this unless a SHAPE_CUSTOM_MESH is used
		};

		struct CustomUserPointer
		{
			PhysicsType::PhysicsType m_type;
			int m_vectorIndex;
			unsigned int m_entityId; //My entity id
			std::set<unsigned int>* m_collidedEntityId; //List of all entities collided with since last update
			int* m_id; // The value that is returned as a handle to the game logic, should be updated when a object is removed.
			std::string m_modelHandle;

			CustomUserPointer()
			{
				m_collidedEntityId = nullptr;
			}

			~CustomUserPointer()
			{
				delete m_id;
			}
		};

		class PhysicsInterface : public RootEngine::SubsystemInterface
		{
		public:
			virtual void Init() = 0;
			virtual void CreatePlane(glm::vec3 p_normal, glm::vec3 p_position) = 0;
			virtual void Update(float p_dt) = 0;

			///Set the direction a controllable object is facing, should be sent in every update and is assumed to be a vec3, the y value is ignored however
		//	virtual void PlayerKnockback(int p_objectHandle, glm::vec3 p_pushDirection, float p_pushForce) = 0; ///p_pushDirection is the direction the pushing has, for the love of god normalize it first
		//	virtual void PlayerMoveXZ(int p_objectHandle, glm::vec3 p_direction) = 0;
			///Call this when a character jumps
		//	virtual void PlayerJump(int p_objectHandle, float p_jumpForce) = 0;
			///Object index is the value returned by the add function, velocity is a vec3 of the objects velocity (speed*target) Used for abilities mainly
			virtual void SetDynamicObjectVelocity(int p_objectHandle, glm::vec3 p_velocity) = 0;
			
			virtual void EnableDebugDraw(bool p_enabled) = 0;
			///Use this to add a static object to the World, i.e trees, rocks and the ground. Both position and rotation are vec3
			virtual int* AddStaticObjectToWorld(std::string p_modelHandle, unsigned int p_entityId , glm::vec3 p_position, glm::quat p_rotation) = 0;
			/*	Use this to add a dynamic object to the World, i.e trees, rocks and the ground. Both position and rotation are vec3, mass affect how the object behaves in the world. Note: Mass must be >0 
			virtual The return value is the index to the objects rigidbody and should be used where a index parameter is requested*/
			virtual int* AddDynamicObjectToWorld(std::string p_modelHandle, unsigned int p_entityId,  glm::vec3 p_position, glm::quat p_rotation, float p_mass) = 0;
			//Use this to add a Controllable object to the world, i.e Players. Return value is the index position of the object. position and rotation is of type float[3]
			virtual int* AddPlayerObjectToWorld(std::string p_modelHandle, unsigned int p_entityId, glm::vec3 p_position, glm::quat p_rotation, float p_mass, float p_maxSpeed, float p_modelHeight, float p_stepHeight, std::set<unsigned int>* p_enityCollidedId) = 0;
			virtual int* AddAbilityToWorld(AbilityPhysicsInfo p_abilityInfo) = 0;
			/// p_Pos should be of type float[3]
			virtual glm::vec3 GetPos(int p_objectHandle)= 0;
			virtual glm::vec3 GetVelocity(int p_objectHandle) = 0;
			virtual float GetMass(int p_objectHandle) = 0;
			virtual int GetType(int p_objectHandle) = 0;
			virtual float GetMaxSpeed(int p_objectHandle) = 0;
			virtual float GetStepHeight(int p_objectHandle) = 0;
			virtual float GetModelHeight(int p_objectHandle) = 0;
			virtual std::set<unsigned int>* GetCollisionVector(int p_objectHandle) = 0;
			virtual std::string GetPhysicsModelHandle(int p_objectHandle) = 0;
			virtual glm::quat GetOrientation(int p_objectHandle) = 0;
			virtual void SetOrientation(int p_objectHandle, glm::quat p_objectOrientation) = 0;
			virtual void SetVelocity(int p_objectHandle, glm::vec3 p_velocity) = 0;
			virtual void SetMass(int p_objectHandle, float p_mass) = 0;
			virtual void SetGravity(int p_objectHandle, glm::vec3 p_gravity) = 0;
			virtual void SetPosition(int p_objectHandle , glm::vec3 p_position) = 0;
			//virtual void SetPlayerOrientation(int p_objectHandle, float* p_playerOrientation) = 0;
			

			virtual void RemoveObject(int p_objectHandle) = 0;
			virtual std::shared_ptr<PhysicsMeshInterface> CreatePhysicsMesh() = 0;
		};

		

		class RootPhysics : public PhysicsInterface
		{
		public:
			void Startup();
			void Shutdown();
			static RootPhysics* GetInstance();
			void CreatePlane(glm::vec3 p_normal, glm::vec3 p_position);
			
			void Update(float p_dt);
			///Set the direction a controllable object is facing, should be sent in every update and is assumed to be a vec3, the y value is ignored however
			void PlayerMoveXZ(int p_objectHandle, glm::vec3 p_direction);
		
			void PlayerJump(int p_objectHandle, float p_jumpForce); ///Call this when a character jumps
			void PlayerKnockback(int p_objectHandle, glm::vec3 p_pushDirection, float p_pushForce); ///p_pushDirection is the direction the pushing has, for the love of god normalize it first
			void SetDynamicObjectVelocity(int p_objectHandle, glm::vec3 p_velocity); ///Object index is the value returned by the add function, velocity is a vec3 of the objects velocity (speed*target) Used for abilities mainly

			void EnableDebugDraw(bool p_enabled);
		
			///Use this to add a static object to the World, i.e trees, rocks and the ground. Both position and rotation are vec3
			int* AddStaticObjectToWorld(std::string p_modelHandle,unsigned int p_entityId,  glm::vec3 p_position, glm::quat p_rotation);
			/*	Use this to add a dynamic object to the World, i.e trees, rocks and the ground. Both position and rotation are vec3, mass affect how the object behaves in the world. Note: Mass must be >0 
			The return value is the index to the objects rigidbody and should be used where a index parameter is requested*/
			int* AddDynamicObjectToWorld(std::string p_modelHandle,unsigned int p_entityId,  glm::vec3 p_position, glm::quat p_rotation, float p_mass);
			///Use this to add a Controllable object to the world, i.e Players. Return value is the index position of the object. position and rotation is of type float[3]
			int* AddPlayerObjectToWorld(std::string p_modelHandle,unsigned int p_entityId, glm::vec3 p_position, glm::quat p_rotation, float p_mass,float p_maxSpeed, float p_modelHeight, float p_stepHeight, std::set<unsigned int>* p_enityCollidedId);

			int* AddAbilityToWorld(AbilityPhysicsInfo p_abilityInfo);
			glm::vec3 GetPos(int p_objectHandle);	
			glm::vec3 GetVelocity(int p_objectHandle);
			float GetMass(int p_objectHandle);
			int GetType(int p_objectHandle);
			float GetMaxSpeed(int p_objectHandle);
			float GetStepHeight(int p_objectHandle);
			float GetModelHeight(int p_objectHandle);
			std::set<unsigned int>* GetCollisionVector(int p_objectHandle);
			std::string GetPhysicsModelHandle(int p_objectHandle);
			glm::quat GetOrientation(int p_objectHandle);
			void SetGravity(int p_objectHandle, glm::vec3 p_gravity);
			void SetVelocity(int p_objectHandle, glm::vec3 p_velocity);
			void SetMass(int p_objectHandle, float p_mass);
			void SetOrientation(int p_objectHandle, glm::quat p_objectOrientation);
			void SetPosition(int p_objectHandle , glm::vec3 p_position);
			void RemoveObject(int p_objectHandle);
			std::shared_ptr<PhysicsMeshInterface> CreatePhysicsMesh() { return std::shared_ptr<PhysicsMeshInterface>(new PhysicsMesh); }

		private:
			void Init();
			bool DoesObjectExist(int p_objectHandle);
			btRigidBody* CreateSphere(float p_radius, float p_mass, glm::vec3 p_position);
			btRigidBody* CreateCone(float p_radius, float p_height, glm::vec3 p_position, glm::quat p_rotation, float p_mass);
			btRigidBody* CreateCylinder(float p_radius, float p_height,  glm::vec3 p_position, glm::quat p_rotation, float p_mass);
			btRigidBody* CreateMesh(std::string p_modelHandle, glm::vec3 p_position, glm::quat p_rotation, float p_mass);
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
			std::vector<KinematicController*> m_playerObjects;
			float m_dt;
			bool m_debugDrawEnabled;
			
		};
	}
}
extern "C"
{
	typedef RootEngine::Physics::PhysicsInterface* (*CREATEPHYSICS)(RootEngine::SubsystemSharedContext, Render::RendererInterface *, RootEngine::ResourceManagerInterface*);
	PHYSICS_DLL_EXPORT RootEngine::Physics::PhysicsInterface* CreatePhysics(RootEngine::SubsystemSharedContext p_context, Render::RendererInterface* p_renderer, RootEngine::ResourceManagerInterface* p_resouceManager);
}