#pragma once

#include <vector>
#include <map>
#include "KinematicController.h"
#include "ObjectController.h"
#include "ShapelessObject.h"
#include <RootEngine/Include/SubsystemSharedContext.h>
#include <RootEngine/Render/Include/Renderer.h>
#include <RootEngine/Physics/Include/DebugDrawer.h>
#include <RootEngine/Physics/Include/PhysicsMesh.h>
#include <RootSystems/Include/CollisionInfo.h>
#include <RootEngine/Physics/Include/Ragdoll.h>

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
				TYPE_PLAYER,
				TYPE_RAGDOLL,
				TYPE_ABILITYSPAWN,
			};
		}
		namespace PhysicsShape	
		{
			enum PhysicsShape
			{
				SHAPE_SPHERE,
				SHAPE_CONE,
				SHAPE_CYLINDER,
				SHAPE_CUSTOM_MESH,
				SHAPE_HULL,
				SHAPE_NONE
			};
		}
		struct AbilityPhysicsInfo
		{
			void* m_entity; 
			float m_radius;
			float m_height; 
			glm::vec3 m_position; 
			glm::quat m_orientation; 
			glm::vec3 m_direction; 
			float m_speed; 
			PhysicsType::PhysicsType m_type; 
			PhysicsShape::PhysicsShape m_shape;
			bool m_collidesWorld;
			float m_mass; 
			glm::vec3 m_gravity;
			std::string m_modelHandle; //No need to specify this unless a SHAPE_CUSTOM_MESH is used
		};
		

		struct CustomUserPointer
		{
			PhysicsType::PhysicsType m_type;
			PhysicsShape::PhysicsShape m_shape;
			int m_vectorIndex; //Index in one of the different lists
			void* m_entity; //My entity
			std::map<void*, RootForce::CollisionInfo>* m_collisions; //List of all entities collided with since last update and data of the collisions
			int* m_id; // The value that is returned as a handle to the game logic, should be updated when a object is removed.
			std::string m_modelHandle;
			bool m_externalControlled;
			int m_ragdollIndex;
			CustomUserPointer()
			{
				m_collisions = nullptr;
				m_ragdollIndex = -1;
			}

			~CustomUserPointer()
			{
				delete m_id;
				m_id= nullptr;
			}
		};

		class PhysicsInterface : public RootEngine::SubsystemInterface
		{
		public:
			
			virtual void Init() = 0;
			virtual void CreatePlane(glm::vec3 p_normal, glm::vec3 p_position) = 0;
			virtual void Update(float p_dt) = 0;
			virtual void EnableDebugDraw(bool p_enabled) = 0;
			virtual void RemoveObject(int p_objectHandle) = 0;
			virtual void RemoveAll() = 0;
			
			virtual std::shared_ptr<PhysicsMeshInterface> CreatePhysicsMesh() = 0;

			//Player only related funcs
			virtual void PlayerJump(int p_objectHandle, float p_jumpForce) = 0;
			virtual void PlayerJumpBoost(int p_objectHandle, float p_boostPower) = 0;
			virtual bool IsOnGround(int p_objectHandle) = 0;
			virtual void KnockbackObject(int p_objectHandle, glm::vec3 p_pushDirection, float p_pushForce) = 0;
			virtual void StopPlayerKnockback(int p_objectHandle) = 0;
			virtual int* AddPlayerObjectToWorld(std::string p_modelHandle, void* p_entity, glm::vec3 p_position, glm::quat p_rotation, float p_mass, float p_maxSpeed, float p_modelHeight, float p_stepHeight, std::map<void*, RootForce::CollisionInfo>* p_collisions) = 0;
			virtual glm::vec3 GetPlayerKnockbackVector(int p_objectHandle) = 0;
			virtual float GetPlayerVerticalVelocity(int p_objectHandle) = 0;

			//Instant methods
			virtual float RayTest(glm::vec3 p_startPos, glm::vec3 p_endPos) = 0;
			virtual void* CastRay(int p_objectHandle, glm::vec3 p_startPos, glm::vec3 p_direction, float p_length, glm::vec3* p_hitPos, bool p_isAbility) = 0;
			virtual void RadiusCheck(int p_objectHandle, glm::vec3 p_pos, float p_radius) = 0;
			virtual void* GetPlayerAtAim(int p_objectHandle, glm::vec3 p_startPos, glm::vec3 p_direction, float p_length) = 0;

			
			

			///Creates a handle
			virtual int* CreateHandle(void* p_entity, PhysicsType::PhysicsType p_physicsType, bool p_externalControlled) = 0;
			
			//Ragdoll affecting functions
			virtual void BuildRagdoll(int p_objectHandle, glm::mat4 p_bones[20], aiNode* p_rootNode, std::map<std::string, int>  p_nameToIndex, glm::mat4 p_boneOffset[20], glm::vec3 p_right ) = 0;
			virtual bool IsRagdoll(int p_objecthandle) = 0;
			virtual void DeactivateRagdoll(int p_objectHandle) = 0;

			//Binds a shape to a handle
			virtual void BindSphereShape(int p_objectHandle,  glm::vec3 p_position, glm::quat p_rotation, float p_radius, float p_mass, bool p_collideWithWorld) = 0;
			virtual void BindCylinderShape(int p_objectHandle,  glm::vec3 p_position, glm::quat p_rotation, float p_height, float p_radius, float p_mass, bool p_collideWithWorld) = 0;
			virtual void BindConeShape(int p_objectHandle,  glm::vec3 p_position, glm::quat p_rotation, float p_height, float p_radius, float p_mass, bool p_collideWithWorld) = 0;
			virtual void BindMeshShape( int p_objectHandle, std::string p_modelHandle, glm::vec3 p_position, glm::quat p_rotation, glm::vec3 p_scale, float p_mass, bool p_collideWithWorld ) = 0;
			virtual void BindHullShape(int p_objectHandle, std::string p_modelHandle,  glm::vec3 p_position, glm::quat p_rotation, float p_mass, bool p_collideWithWorld) = 0;
			virtual void BindNoShape(int p_objectHandle, glm::vec3 p_position, glm::quat p_rotation) = 0;
			
			//Getters
			virtual glm::vec3 GetPos(int p_objectHandle)= 0;
			virtual glm::vec3 GetVelocity(int p_objectHandle) = 0;
			virtual float GetMass(int p_objectHandle) = 0;
			virtual int GetType(int p_objectHandle) = 0;
			virtual float GetMaxSpeed(int p_objectHandle) = 0;
			virtual float GetStepHeight(int p_objectHandle) = 0;
			virtual float GetModelHeight(int p_objectHandle) = 0;
			virtual std::map<void*, RootForce::CollisionInfo>* GetCollisionVector(int p_objectHandle) = 0;
			virtual glm::mat4* GetBones(int p_objectHandle) = 0;
			virtual std::string GetPhysicsModelHandle(int p_objectHandle) = 0;
			virtual glm::quat GetOrientation(int p_objectHandle) = 0;
			
			//Setters
			virtual void SetOrientation(int p_objectHandle, glm::quat p_objectOrientation) = 0;
			virtual void SetVelocity(int p_objectHandle, glm::vec3 p_velocity) = 0;
			virtual void SetMass(int p_objectHandle, float p_mass) = 0;
			virtual void SetGravity(int p_objectHandle, glm::vec3 p_gravity) = 0;
			virtual void SetPosition(int p_objectHandle, glm::vec3 p_position) = 0;
			virtual void Move(int p_objectHandle , glm::vec3 p_position) = 0;
			virtual void SetCollisionContainer(int p_objectHandle, std::map<void*, RootForce::CollisionInfo>* p_collisions) = 0;

			//virtual void SetPlayerOrientation(int p_objectHandle, float* p_playerOrientation) = 0;
			

			
		};

		

		class RootPhysics : public PhysicsInterface
		{
		public:
			void Startup();
			void Shutdown();
			static RootPhysics* GetInstance();
			void CreatePlane(glm::vec3 p_normal, glm::vec3 p_position);
			
			void Update(float p_dt);
			
			
		
			void PlayerJump(int p_objectHandle, float p_jumpForce); 
			void PlayerJumpBoost(int p_objectHandle, float p_boostPower);
			bool IsOnGround(int p_objectHandle);
			void KnockbackObject(int p_objectHandle, glm::vec3 p_pushDirection, float p_pushForce); 
			void StopPlayerKnockback(int p_objectHandle);
			void EnableDebugDraw(bool p_enabled);

			float RayTest(glm::vec3 p_startPos, glm::vec3 p_endPos);
			void* CastRay(int p_objectHandle, glm::vec3 p_startPos, glm::vec3 p_direction, float p_length, glm::vec3* p_hitPos, bool p_isAbilitys);
			void RadiusCheck(int p_objectHandle, glm::vec3 p_pos, float p_radius);
			void* GetPlayerAtAim(int p_objectHandle, glm::vec3 p_startPos, glm::vec3 p_direction, float p_length);

			int* CreateHandle(void* p_entity, PhysicsType::PhysicsType p_physicsType, bool p_externalControlled);

			void BuildRagdoll(int p_objectHandle, glm::mat4 p_bones[20], aiNode* p_rootNode, std::map<std::string, int> p_nameToIndex , glm::mat4 p_boneOffset[20], glm::vec3 p_right );
			bool IsRagdoll(int p_objecthandle);
			void DeactivateRagdoll(int p_objectHandle);

			//Binds
			void BindSphereShape(int p_objectHandle, glm::vec3 p_position, glm::quat p_rotation, float p_radius, float p_mass, bool p_collideWithWorld);
			void BindCylinderShape(int p_objectHandle,  glm::vec3 p_position, glm::quat p_rotation, float p_height, float p_radius, float p_mass, bool p_collideWithWorld);
			void BindConeShape(int p_objectHandle,  glm::vec3 p_position, glm::quat p_rotation, float p_height, float p_radius, float p_mass, bool p_collideWithWorld);
			void BindMeshShape(int p_objectHandle, std::string p_modelHandle, glm::vec3 p_position, glm::quat p_rotation, glm::vec3 p_scale, float p_mass, bool p_collideWithWorld );
			void BindHullShape(int p_objectHandle, std::string p_modelHandle, glm::vec3 p_position, glm::quat p_rotation, float p_mass, bool p_collideWithWorld);
			void BindNoShape(int p_objectHandle, glm::vec3 p_position, glm::quat p_rotation);

			
			int* AddPlayerObjectToWorld(std::string p_modelHandle, void* p_entity, glm::vec3 p_position, glm::quat p_rotation, float p_mass, float p_maxSpeed, float p_modelHeight, float p_stepHeight, std::map<void*, RootForce::CollisionInfo>* p_collisions);


			//Getters
			glm::vec3 GetPos(int p_objectHandle);	
			glm::vec3 GetVelocity(int p_objectHandle);
			glm::mat4* GetBones(int p_objectHandle);
			float GetMass(int p_objectHandle);
			int GetType(int p_objectHandle);
			float GetMaxSpeed(int p_objectHandle);
			float GetStepHeight(int p_objectHandle);
			float GetModelHeight(int p_objectHandle);
			std::map<void*, RootForce::CollisionInfo>* GetCollisionVector(int p_objectHandle);
			std::string GetPhysicsModelHandle(int p_objectHandle);
			glm::quat GetOrientation(int p_objectHandle);
			glm::vec3 GetPlayerKnockbackVector(int p_objectHandle);
			float GetPlayerVerticalVelocity(int p_objectHandle);
			//Setters
			void SetGravity(int p_objectHandle, glm::vec3 p_gravity);
			void SetVelocity(int p_objectHandle, glm::vec3 p_velocity);
			void SetMass(int p_objectHandle, float p_mass);
			void SetOrientation(int p_objectHandle, glm::quat p_objectOrientation);
			void SetPosition(int p_objectHandle, glm::vec3 p_position);
			void Move(int p_objectHandle , glm::vec3 p_position);
			void SetCollisionContainer(int p_objectHandle , std::map<void*, RootForce::CollisionInfo>* p_collisions);
			void RemoveObject(int p_objectHandle);
			void RemoveAll();
			std::shared_ptr<PhysicsMeshInterface> CreatePhysicsMesh() { return std::shared_ptr<PhysicsMeshInterface>(new PhysicsMesh); }

		private:
			struct ShapeStruct
			{
				float m_radius;
				float m_height;
				std::string m_modelHandle;
				PhysicsShape::PhysicsShape m_type;
				glm::vec3 m_scale;
			};
			void Init();
			bool DoesObjectExist(int p_objectHandle);
			bool DoesUserPointerExist( int p_objectHandle );
			void AddRigidBody(int p_objectHandle, btRigidBody* p_body, bool p_collideWithWorld);
			void AddController(int p_objectHandle, btCollisionShape* p_collisionShape, bool p_collideWithWorld, const btTransform& p_transform);
			btRigidBody* CreateMesh(std::string p_modelHandle, glm::vec3 p_position, glm::quat p_rotation, float p_mass);
			btCollisionShape* CreateShape(ShapeStruct p_shapeStruct);
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
			std::vector<ObjectController*> m_externallyControlled;
			std::vector<ShapelessObject*> m_shapelessObjects;
			std::vector<KinematicController*> m_playerObjects;
			std::vector<Ragdoll::Ragdoll*> m_ragdolls;
			float m_dt;
			bool m_debugDrawEnabled;
			float m_removeMeLater;
			
		};
	}
}
extern "C"
{
	typedef RootEngine::Physics::PhysicsInterface* (*CREATEPHYSICS)(RootEngine::SubsystemSharedContext, Render::RendererInterface *, RootEngine::ResourceManagerInterface*);
	PHYSICS_DLL_EXPORT RootEngine::Physics::PhysicsInterface* CreatePhysics(RootEngine::SubsystemSharedContext p_context, Render::RendererInterface* p_renderer, RootEngine::ResourceManagerInterface* p_resouceManager);
}