#include "RootPhysics.h"
#include <stdio.h>
#include <iostream>
#include <Bullet/BulletCollision/CollisionShapes/btShapeHull.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <Bullet/BulletCollision/CollisionDispatch/btInternalEdgeUtility.h>
extern ContactAddedCallback		gContactAddedCallback;

//#include "vld.h"
namespace RootEngine
{


namespace Physics
{
	RootEngine::SubsystemSharedContext g_context;
	RootPhysics* RootPhysics::s_physicsInstance = nullptr;
	Render::RendererInterface* g_renderer;
	RootEngine::ResourceManagerInterface* g_resourceManager;
	
	RootPhysics::RootPhysics()
	{

	}

	RootPhysics::~RootPhysics()
	{

	
	}
	void RootPhysics::Startup()
	{
		Init();
	}
	void RootPhysics::Shutdown()
	{
		//Magic loop of deleting, might not work with ghostobjects(players). Check this before release.
		for(int i = m_dynamicWorld->getNumCollisionObjects()-1; i>=0; i--)
		{
			btCollisionObject* obj = m_dynamicWorld->getCollisionObjectArray()[i];
			btRigidBody* body = btRigidBody::upcast(obj);
			if (body && body->getMotionState())
			{
				delete body->getMotionState();
			}
			if(body && body->getCollisionShape())
			{
				btCollisionShape* temp = body->getCollisionShape();
				delete temp;
			}
			m_dynamicWorld->removeCollisionObject( obj );
			delete obj;
		}
		for(unsigned int i = 0; i < m_playerObjects.size(); i++)
		{
			KinematicController* temp = m_playerObjects[i];
			delete temp;
		}
		for(unsigned int i = 0; i < m_userPointer.size(); i++)
		{
			CustomUserPointer* temp = m_userPointer[i];
			delete temp;
		}
		m_dynamicObjects.clear();
		m_playerObjects.clear();
		m_userPointer.clear();
		delete m_debugDrawer;
		delete m_dynamicWorld;
		delete m_solver;
		delete m_broadphase;
		delete m_dispatcher;
		delete m_collisionConfig;
		g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::DEBUG_PRINT, "Shutdown complete");
		
	}

	///Must be global, used to check custom collision events, NOTE : Don't ever ever use this yourself!
	bool callbackFunc(btManifoldPoint& p_cp,const btCollisionObjectWrapper * p_obj1 , int p_id1, int p_index1, const btCollisionObjectWrapper * p_obj2 , int p_id2, int p_index2 )
	{

		CustomUserPointer* pointer1 = (CustomUserPointer*)(p_obj1->getCollisionObject()->getUserPointer());
		CustomUserPointer* pointer2 = (CustomUserPointer*)(p_obj2->getCollisionObject()->getUserPointer());
		
		if(pointer1 == nullptr|| pointer1->m_id == nullptr)
			return false;
		if(pointer2 == nullptr || pointer2->m_id == nullptr)
			return false;
		if(!pointer1->m_modelHandle.compare("ground0"))
			btAdjustInternalEdgeContacts(p_cp,p_obj1,p_obj2, p_id1,p_index1);
		else if(!pointer2->m_modelHandle.compare("ground0"))
			btAdjustInternalEdgeContacts(p_cp,p_obj2,p_obj1, p_id2,p_index2);

		if(pointer1->m_collidedEntityId != nullptr)
		{
			if(pointer1->m_type == PhysicsType::TYPE_PLAYER || pointer1->m_type == PhysicsType::TYPE_ABILITY)
				pointer1->m_collidedEntityId->insert(pointer2->m_entityId);
		}
			
		if(pointer2->m_collidedEntityId != nullptr)
		{
			if(pointer2->m_type == PhysicsType::TYPE_PLAYER || pointer2->m_type == PhysicsType::TYPE_ABILITY)
				pointer2->m_collidedEntityId->insert(pointer1->m_entityId);
		}


		if(pointer1->m_type == PhysicsType::TYPE_PLAYER || pointer2->m_type == PhysicsType::TYPE_PLAYER)
			if(pointer1->m_type == PhysicsType::TYPE_ABILITY || pointer2->m_type == PhysicsType::TYPE_ABILITY )
				int thisIsOnlyHereSoWeCanBreakHereIfWeWant = 2;

		return true;
	}

	

	void RootPhysics::Init()
	{
		m_collisionConfig= new btDefaultCollisionConfiguration();
		m_dispatcher = new btCollisionDispatcher(m_collisionConfig);
		m_broadphase = new btDbvtBroadphase();
		//m_broadphase = new btAxisSweep3(btVector3(-1000,-1000,-1000), btVector3(1000,1000,1000));
		m_broadphase->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
		m_solver = new btSequentialImpulseConstraintSolver();
		m_dynamicWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfig);
		m_dynamicWorld->setGravity(btVector3(0.0f, -9.82f, 0.0f));
		gContactAddedCallback = callbackFunc;
		g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::INIT_PRINT, "Physics subsystem initialized!");
		
		m_debugDrawer = new DebugDrawer();
		m_debugDrawer->setDebugMode(m_debugDrawer->getDebugMode() | btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawContactPoints | btIDebugDraw::DBG_DrawAabb);
		m_dynamicWorld->setDebugDrawer(m_debugDrawer);
		m_dynamicWorld->debugDrawWorld();
		m_debugDrawEnabled = false;
		m_dynamicWorld->getDispatchInfo().m_allowedCcdPenetration=0.0001f;

		//btSetDebugDrawer(m_debugDrawer);

	}


	//////////////////////////////////////////////////////////////////////////
	
	void RootPhysics::Update(float p_dt)
	{
		
		m_dt = p_dt;
		m_dynamicWorld->stepSimulation(m_dt,1);
		for(unsigned int i = 0; i < m_playerObjects.size(); i++)
		{
			m_playerObjects.at(i)->Update(m_dt);
		}
		
		if(m_debugDrawEnabled)
			m_dynamicWorld->debugDrawWorld();
		
	}
	void RootPhysics::RemoveObject( int p_objectHandle)
	{
		if(!DoesObjectExist(p_objectHandle))
			return;

		CustomUserPointer* userPointer = m_userPointer.at(p_objectHandle);
		if(userPointer->m_type == PhysicsType::TYPE_PLAYER)
		{
			unsigned int removedIndex = userPointer->m_vectorIndex;
			m_playerObjects.at(removedIndex)->RemovePlayer();
			delete m_playerObjects.at(removedIndex);
			m_playerObjects.erase(m_playerObjects.begin() + removedIndex);

			delete userPointer;
			m_userPointer.erase(m_userPointer.begin() + p_objectHandle);
		
			for(unsigned int i = p_objectHandle; i < m_userPointer.size(); i++)
			{
				m_userPointer.at(i)->m_id[0] --;
				if(m_userPointer.at(i)->m_type == PhysicsType::TYPE_PLAYER)
				{
					
						m_userPointer.at(i)->m_vectorIndex--;
				}
			}

		}
		else
		{
			unsigned int removedIndex = userPointer->m_vectorIndex;
			m_dynamicWorld->removeRigidBody(m_dynamicObjects.at(removedIndex));
			delete m_dynamicObjects.at(removedIndex);
			m_dynamicObjects.erase(m_dynamicObjects.begin() + removedIndex);

			delete userPointer;
			m_userPointer.erase(m_userPointer.begin() + p_objectHandle);

			for(unsigned int i = p_objectHandle; i < m_userPointer.size(); i++)
			{
			
				m_userPointer.at(i)->m_id[0] --;
				if(m_userPointer.at(i)->m_type != PhysicsType::TYPE_PLAYER)
				{

					m_userPointer.at(i)->m_vectorIndex--;
				}
			}
		}
	}


	int* RootPhysics::CreateHandle( unsigned int p_entityId, PhysicsType::PhysicsType p_physicsType, bool p_externalControlled )
	{
		CustomUserPointer* userPointer = new CustomUserPointer();
		userPointer->m_vectorIndex = -1;
		m_userPointer.push_back(userPointer);
		userPointer->m_type = p_physicsType;
		userPointer->m_id = new int();
		userPointer->m_externalControlled = p_externalControlled;
		userPointer->m_entityId = p_entityId;
		*(userPointer->m_id) = m_userPointer.size()-1;

		return userPointer->m_id;
	}

	


	void RootPhysics::BindSphereShape( int p_objectHandle, glm::vec3 p_position, glm::quat p_rotation, float p_radius, float p_mass, bool p_collideWithWorld )
	{
		
		btVector3 pos;
		pos.setX( p_position[0]);
		pos.setY( p_position[1]);
		pos.setZ( p_position[2]);
		btQuaternion quat = btQuaternion(p_rotation[0], p_rotation[1], p_rotation[2],p_rotation[3]);
		btDefaultMotionState* motionstate = new btDefaultMotionState(btTransform(quat, pos ));
		ShapeStruct temp;
		temp.m_radius = p_radius;
		temp.m_type = PhysicsShape::SHAPE_SPHERE;
		btCollisionShape* shape = CreateShape(temp);
		if(m_userPointer.at(p_objectHandle)->m_type != PhysicsType::TYPE_STATIC)
		{
			shape->calculateLocalInertia(p_mass, btVector3(0,0,0));
		}
		if(!m_userPointer.at(p_objectHandle)->m_externalControlled) //if physics driven, i.e a rigidbody 
		{
			
			btRigidBody* body = new btRigidBody(p_mass, motionstate , shape);
			if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_STATIC)
				body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
			else if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_ABILITY)
				body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
			m_dynamicWorld->addRigidBody(body);
			m_dynamicObjects.push_back(body);
			m_userPointer.at(p_objectHandle)->m_vectorIndex = m_dynamicObjects.size()-1;
			body->setUserPointer((void*)m_userPointer.at(p_objectHandle));
			return;
		}
		else //Create a controller
		{
			btPairCachingGhostObject* ghostObject = new btPairCachingGhostObject();
			ghostObject->setCollisionShape(shape);
			if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_ABILITY)
				ghostObject->setCollisionFlags(btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK || btCollisionObject::CF_KINEMATIC_OBJECT);
			else
			{
				ghostObject->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
			}
			if(!p_collideWithWorld)
				ghostObject->setCollisionFlags(ghostObject->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
			ObjectController* controller = new ObjectController();
			controller->Init(ghostObject, (btConvexShape*)shape);
			m_dynamicWorld->addCollisionObject(ghostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
			m_externallyControlled.push_back(controller);
			m_userPointer.at(p_objectHandle)->m_vectorIndex = m_externallyControlled.size()-1;
			ghostObject->setUserPointer((void*)m_userPointer.at(p_objectHandle));
			return;
		}
	}

	void RootPhysics::BindCylinderShape( int p_objectHandle, glm::vec3 p_position, glm::quat p_rotation, float p_height, float p_radius, float p_mass, bool p_collideWithWorld )
	{
		btVector3 pos;
		pos.setX( p_position[0]);
		pos.setY( p_position[1]);
		pos.setZ( p_position[2]);
		btQuaternion quat = btQuaternion(p_rotation[0], p_rotation[1], p_rotation[2],p_rotation[3]);
		btDefaultMotionState* motionstate = new btDefaultMotionState(btTransform(quat, pos ));
		ShapeStruct temp;
		temp.m_radius = p_radius;
		temp.m_height = p_height;
		temp.m_type = PhysicsShape::SHAPE_CYLINDER;
		btCollisionShape* shape = CreateShape(temp);
		if(m_userPointer.at(p_objectHandle)->m_type != PhysicsType::TYPE_STATIC)
		{
			shape->calculateLocalInertia(p_mass, btVector3(0,0,0));
		}
		if(!m_userPointer.at(p_objectHandle)->m_externalControlled) //if physics driven, i.e a rigidbody 
		{

			btRigidBody* body = new btRigidBody(p_mass, motionstate , shape);
			if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_STATIC)
				body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
			else if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_ABILITY)
				body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
			m_dynamicWorld->addRigidBody(body);
			m_dynamicObjects.push_back(body);
			m_userPointer.at(p_objectHandle)->m_vectorIndex = m_dynamicObjects.size()-1;
			body->setUserPointer((void*)m_userPointer.at(p_objectHandle));
			return;
		}
		else //Create a controller
		{
			btPairCachingGhostObject* ghostObject = new btPairCachingGhostObject();
			ghostObject->setCollisionShape(shape);
			if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_ABILITY)
				ghostObject->setCollisionFlags(btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK || btCollisionObject::CF_KINEMATIC_OBJECT);
			else
			{
				ghostObject->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
			}
			if(!p_collideWithWorld)
				ghostObject->setCollisionFlags(ghostObject->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
			ObjectController* controller = new ObjectController();
			controller->Init(ghostObject, (btConvexShape*)shape);
			m_dynamicWorld->addCollisionObject(ghostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
			m_externallyControlled.push_back(controller);
			m_userPointer.at(p_objectHandle)->m_vectorIndex = m_externallyControlled.size()-1;
			ghostObject->setUserPointer((void*)m_userPointer.at(p_objectHandle));
			return;
		}
	}

	void RootPhysics::BindConeShape( int p_objectHandle, glm::vec3 p_position, glm::quat p_rotation, float p_height, float p_radius, float p_mass, bool p_collideWithWorld )
	{
		btVector3 pos;
		pos.setX( p_position[0]);
		pos.setY( p_position[1]);
		pos.setZ( p_position[2]);
		btQuaternion quat = btQuaternion(p_rotation[0], p_rotation[1], p_rotation[2],p_rotation[3]);
		btDefaultMotionState* motionstate = new btDefaultMotionState(btTransform(quat, pos ));
		ShapeStruct temp;
		temp.m_radius = p_radius;
		temp.m_height = p_height;
		temp.m_type = PhysicsShape::SHAPE_CONE;
		btCollisionShape* shape = CreateShape(temp);
		if(m_userPointer.at(p_objectHandle)->m_type != PhysicsType::TYPE_STATIC)
		{
			shape->calculateLocalInertia(p_mass, btVector3(0,0,0));
		}
		if(!m_userPointer.at(p_objectHandle)->m_externalControlled) //if physics driven, i.e a rigidbody 
		{

			btRigidBody* body = new btRigidBody(p_mass, motionstate , shape);
			if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_STATIC)
				body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
			else if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_ABILITY)
				body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
			m_dynamicWorld->addRigidBody(body);
			m_dynamicObjects.push_back(body);
			m_userPointer.at(p_objectHandle)->m_vectorIndex = m_dynamicObjects.size()-1;
			body->setUserPointer((void*)m_userPointer.at(p_objectHandle));
			return;
		}
		else //Create a controller
		{
			btPairCachingGhostObject* ghostObject = new btPairCachingGhostObject();
			ghostObject->setCollisionShape(shape);
			if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_ABILITY)
				ghostObject->setCollisionFlags(btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK || btCollisionObject::CF_KINEMATIC_OBJECT);
			else
			{
				ghostObject->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
			}
			if(!p_collideWithWorld)
				ghostObject->setCollisionFlags(ghostObject->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
			ObjectController* controller = new ObjectController();
			controller->Init(ghostObject, (btConvexShape*)shape);
			m_dynamicWorld->addCollisionObject(ghostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
			m_externallyControlled.push_back(controller);
			m_userPointer.at(p_objectHandle)->m_vectorIndex = m_externallyControlled.size()-1;
			ghostObject->setUserPointer((void*)m_userPointer.at(p_objectHandle));
			return;
		}
	}

	void RootPhysics::BindMeshShape( int p_objectHandle, std::string p_modelHandle, glm::vec3 p_position, glm::quat p_rotation, glm::vec3 p_scale, float p_mass, bool p_collideWithWorld )
	{
		btVector3 pos;
		pos.setX( p_position[0]);
		pos.setY( p_position[1]);
		pos.setZ( p_position[2]);
		btQuaternion quat = btQuaternion(p_rotation[0], p_rotation[1], p_rotation[2],p_rotation[3]);
		btTransform trans = btTransform(quat, pos );
		btDefaultMotionState* motionstate = new btDefaultMotionState(trans);
		ShapeStruct temp;
		temp.m_modelHandle = p_modelHandle;
		temp.m_scale = p_scale;
		temp.m_type = PhysicsShape::SHAPE_CUSTOM_MESH;
		btCollisionShape* shape = CreateShape(temp);
		if(m_userPointer.at(p_objectHandle)->m_type != PhysicsType::TYPE_STATIC)
		{
			shape->calculateLocalInertia(p_mass, btVector3(0,0,0));
		}
		if(!m_userPointer.at(p_objectHandle)->m_externalControlled) //if physics driven, i.e a rigidbody 
		{

			btRigidBody* body = new btRigidBody(p_mass, motionstate , shape);
			if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_STATIC)
				body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
			else if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_ABILITY)
				body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
			m_dynamicWorld->addRigidBody(body);
			m_dynamicObjects.push_back(body);
			m_userPointer.at(p_objectHandle)->m_vectorIndex = m_dynamicObjects.size()-1;
			body->setUserPointer((void*)m_userPointer.at(p_objectHandle));
			return;
		}
		else //Create a controller
		{
			btPairCachingGhostObject* ghostObject = new btPairCachingGhostObject();
			ghostObject->setCollisionShape(shape);
			if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_ABILITY)
				ghostObject->setCollisionFlags(btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK || btCollisionObject::CF_KINEMATIC_OBJECT);
			else
			{
				ghostObject->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
			}
			if(!p_collideWithWorld)
				ghostObject->setCollisionFlags(ghostObject->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
			ObjectController* controller = new ObjectController();
			controller->Init(ghostObject, (btConvexShape*)shape);
			ghostObject->setWorldTransform(trans);
			m_dynamicWorld->addCollisionObject(ghostObject, btBroadphaseProxy::DefaultFilter, btBroadphaseProxy::AllFilter);
			m_externallyControlled.push_back(controller);
			m_userPointer.at(p_objectHandle)->m_vectorIndex = m_externallyControlled.size()-1;
			ghostObject->setUserPointer((void*)m_userPointer.at(p_objectHandle));
			return;
		}
	}

	void RootPhysics::BindHullShape( int p_objectHandle, std::string p_modelHandle, glm::vec3 p_position, glm::quat p_rotation, float p_mass, bool p_collideWithWorld )
	{
		btRigidBody* body = CreateMesh(p_modelHandle, p_position, p_rotation, p_mass);
		if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_STATIC)
			body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
		m_dynamicWorld->addRigidBody(body);
		m_dynamicObjects.push_back(body);
		m_userPointer.at(p_objectHandle)->m_vectorIndex = m_dynamicObjects.size()-1;
		body->setUserPointer((void*)m_userPointer.at(p_objectHandle));
		m_userPointer.at(p_objectHandle)->m_modelHandle = p_modelHandle;
	}
	//Creates an impassable plane
	void RootPhysics::CreatePlane(glm::vec3 p_normal, glm::vec3 p_position)
	{
		btCollisionShape* plane = new btStaticPlaneShape(btVector3(p_normal[0],p_normal[1],p_normal[2]), 0);
		btDefaultMotionState* planeMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(p_position[0],p_position[1],p_position[2])));
		btRigidBody::btRigidBodyConstructionInfo planeRigidbodyCI(0, planeMotionState, plane, btVector3(0, 0, 0));
		btRigidBody* planeBody = new btRigidBody(planeRigidbodyCI);
		planeBody->setCollisionFlags(planeBody->getCollisionFlags() /*| btRigidBody::CF_DISABLE_VISUALIZE_OBJECT*/);
		m_dynamicWorld->addRigidBody(planeBody);
		CustomUserPointer* userPointer = new CustomUserPointer();
		userPointer->m_vectorIndex = -1;
		m_userPointer.push_back(userPointer);
		userPointer->m_type = PhysicsType::TYPE_STATIC;
		userPointer->m_id = new int();
		userPointer->m_entityId = -1;
		*(userPointer->m_id) = m_userPointer.size()-1;
		planeBody->setUserPointer((void*)userPointer);

	}
	btRigidBody* RootPhysics::CreateSphere( float p_radius, float p_mass, glm::vec3 p_position)
	{
		btCollisionShape* sphere = new btSphereShape(p_radius);
		btVector3 pos;
		pos.setX( p_position[0]);
		pos.setY( p_position[1]);
		pos.setZ( p_position[2]);
		btDefaultMotionState* motionstate = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), pos ));
		btVector3 fallinertia(0,0,0);
		sphere->calculateLocalInertia(p_mass,fallinertia);
		btRigidBody::btRigidBodyConstructionInfo sphereCI(p_mass,motionstate,sphere,fallinertia);
		btRigidBody* body = new btRigidBody(sphereCI);
		body->setActivationState(DISABLE_DEACTIVATION);
		return body;
	}
	btRigidBody* RootPhysics::CreateCylinder( float p_radius, float p_height, glm::vec3 p_position, glm::quat p_rotation, float p_mass )
	{
		btCollisionShape* cylinder = new btCylinderShape(btVector3(p_radius, p_height, p_radius));
		
		btVector3 pos;
		pos.setX( p_position[0]);
		pos.setY( p_position[1]);
		pos.setZ( p_position[2]);
		btQuaternion quat = btQuaternion(p_rotation[0], p_rotation[1], p_rotation[2],p_rotation[3]);
		btDefaultMotionState* motionstate = new btDefaultMotionState(btTransform(quat, pos ));
		btVector3 fallinertia(0,0,0);
		cylinder->calculateLocalInertia(p_mass,fallinertia);
		btRigidBody::btRigidBodyConstructionInfo cylinderCI(p_mass,motionstate,cylinder,fallinertia);
		btRigidBody* body = new btRigidBody(cylinderCI);
		//body->setContactProcessingThreshold(0);
		body->setActivationState(DISABLE_DEACTIVATION);
		return body;
	}
	btRigidBody* RootPhysics::CreateCone(float p_radius, float p_height, glm::vec3 p_position,glm::quat p_rotation, float p_mass)
	{
		btCollisionShape* cone = new btConeShape(p_radius, p_height);
		btVector3 pos;
		pos.setX( p_position[0]);
		pos.setY( p_position[1]);
		pos.setZ( p_position[2]);
		btQuaternion quat = btQuaternion(p_rotation[0], p_rotation[1], p_rotation[2],p_rotation[3]);
		btDefaultMotionState* motionstate = new btDefaultMotionState(btTransform(quat, pos ));
		btVector3 fallinertia(0,0,0);
		cone->calculateLocalInertia(p_mass,fallinertia);
		btRigidBody::btRigidBodyConstructionInfo coneCI(p_mass,motionstate,cone,fallinertia);
		btRigidBody* body = new btRigidBody(coneCI);
		body->setActivationState(DISABLE_DEACTIVATION);
		body->setContactProcessingThreshold(0);
		return body;
	}

	btRigidBody* RootPhysics::CreateMesh( std::string p_modelHandle, glm::vec3 p_position, glm::quat p_rotation, float p_mass )
	{
		PhysicsMeshInterface* tempMesh = g_resourceManager->GetPhysicsMesh(p_modelHandle);
		btTriangleIndexVertexArray* indexVertexArray = new btTriangleIndexVertexArray(tempMesh->GetNrOfFaces(), tempMesh->GetIndices(), 3*sizeof(int), tempMesh->GetNrOfPoints() , (btScalar*) tempMesh->GetMeshPoints(), 3*sizeof(float));

		btConvexShape* objectMeshShape = new btConvexTriangleMeshShape(indexVertexArray);
		//Cull unneccesary vertices to improve performance 
		btShapeHull* objectHull = new btShapeHull(objectMeshShape);

		btScalar margin = objectMeshShape->getMargin();
		objectHull->buildHull(margin);
		btConvexHullShape* simplifiedObject = new btConvexHullShape();
		for(int i = 0; i < objectHull->numVertices(); i++)
		{
			simplifiedObject->addPoint(objectHull->getVertexPointer()[i]);
		}	
		simplifiedObject->recalcLocalAabb();
		////Set Inertia
		btVector3 fallInertia =  btVector3(0,0,0);
		simplifiedObject->calculateLocalInertia(p_mass,fallInertia);

		//Set startpos and start rotation and bind them to a motionstate
		btTransform startTransform;
		startTransform.setIdentity();
		startTransform.setOrigin(btVector3(p_position[0],p_position[1],p_position[2]));
		startTransform.setRotation(btQuaternion(p_rotation[0],p_rotation[1], p_rotation[2],p_rotation[3]));
		btDefaultMotionState* motionstate = new btDefaultMotionState(startTransform);

		//create a body
		btRigidBody::btRigidBodyConstructionInfo objectBodyInfo(p_mass, motionstate,simplifiedObject, fallInertia );
		btRigidBody* objectBody = new btRigidBody(objectBodyInfo);
		return objectBody;
	}

	btCollisionShape* RootPhysics::CreateShape( ShapeStruct p_shapeStruct )
	{
		if(p_shapeStruct.m_type == PhysicsShape::SHAPE_SPHERE)
		{
			btCollisionShape* sphere = new btSphereShape(p_shapeStruct.m_radius);
			return sphere;
		}
		else if(p_shapeStruct.m_type == PhysicsShape::SHAPE_CONE)
		{
			btCollisionShape* cone = new btConeShape(p_shapeStruct.m_radius, p_shapeStruct.m_height);
			return cone;
		}
		else if(p_shapeStruct.m_type == PhysicsShape::SHAPE_CYLINDER)
		{
			btCollisionShape* cylinder = new btCylinderShape(btVector3(p_shapeStruct.m_radius, p_shapeStruct.m_height, p_shapeStruct.m_radius));
			return cylinder;
		}
		else if(p_shapeStruct.m_type == PhysicsShape::SHAPE_HULL)
		{
			PhysicsMeshInterface* tempMesh = g_resourceManager->GetPhysicsMesh(p_shapeStruct.m_modelHandle);
			btTriangleIndexVertexArray* indexVertexArray = new btTriangleIndexVertexArray(tempMesh->GetNrOfFaces(), tempMesh->GetIndices(), 3*sizeof(int), tempMesh->GetNrOfPoints() , (btScalar*) tempMesh->GetMeshPoints(), 3*sizeof(float));

			btConvexShape* objectMeshShape = new btConvexTriangleMeshShape(indexVertexArray);
			//Cull unneccesary vertices to improve performance 
			btShapeHull* objectHull = new btShapeHull(objectMeshShape);

			btScalar margin = objectMeshShape->getMargin();
			objectHull->buildHull(margin);
			btConvexHullShape* simplifiedObject = new btConvexHullShape();
			for(int i = 0; i < objectHull->numVertices(); i++)
			{
				simplifiedObject->addPoint(objectHull->getVertexPointer()[i]);
			}	
			simplifiedObject->recalcLocalAabb();
			simplifiedObject->setLocalScaling(btVector3(p_shapeStruct.m_scale.x, p_shapeStruct.m_scale.y, p_shapeStruct.m_scale.z));
			
			return (btCollisionShape*)simplifiedObject;
		}
		else if(p_shapeStruct.m_type == PhysicsShape::SHAPE_CUSTOM_MESH)
		{
			PhysicsMeshInterface* tempMesh = g_resourceManager->GetPhysicsMesh(p_shapeStruct.m_modelHandle);
			btTriangleIndexVertexArray* indexVertexArray = new btTriangleIndexVertexArray(tempMesh->GetNrOfFaces(), tempMesh->GetIndices(), 3* sizeof(int), tempMesh->GetNrOfPoints() , (btScalar*) tempMesh->GetMeshPoints(), 3*sizeof(btScalar));
			btScalar mass = 0; //mass is always 0 for static objects

			btBvhTriangleMeshShape* objectMeshShape = new btBvhTriangleMeshShape(indexVertexArray, true);
			objectMeshShape->setLocalScaling(btVector3(p_shapeStruct.m_scale.x, p_shapeStruct.m_scale.y, p_shapeStruct.m_scale.z));
			btTriangleInfoMap* trinfoMap = new btTriangleInfoMap();
			trinfoMap->m_edgeDistanceThreshold = 0.01f;
			trinfoMap->m_equalVertexThreshold = 0.01f;
			btGenerateInternalEdgeInfo(objectMeshShape, trinfoMap);
			return (btCollisionShape*)objectMeshShape;
		}
		else
		{
			g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::WARNING, "Shape %d not specified", p_shapeStruct.m_type);
			return nullptr;
		}
	
	}

	//////////////////////////////////////////////////////////////////////////
	//Use this to add a static object to the World, i.e trees, rocks and the ground. Both position and rotation are vec3
	int* RootPhysics::AddStaticObjectToWorld(unsigned int p_entityId)
	{

		CustomUserPointer* userPointer = new CustomUserPointer();
		userPointer->m_vectorIndex = -1;
		m_userPointer.push_back(userPointer);
		userPointer->m_type = PhysicsType::TYPE_STATIC;
		userPointer->m_id = new int();
		
		userPointer->m_entityId = p_entityId;
		*(userPointer->m_id) = m_userPointer.size()-1;

		return userPointer->m_id;
	}

	int* RootPhysics::AddDynamicObjectToWorld(std::string p_modelHandle, unsigned int p_entityId,  glm::vec3 p_position, glm::quat p_rotation , float p_mass )
	{	
		btRigidBody* objectBody = CreateMesh(p_modelHandle, p_position, p_rotation, p_mass);
		objectBody->setActivationState(DISABLE_DEACTIVATION);
		objectBody->setCollisionFlags(objectBody->getCollisionFlags()| btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK );
		//add the body to the world,  TODO : We should also set a user defined gravity for the object
		m_dynamicWorld->addRigidBody(objectBody);

		//add to the dynamic object vector
		m_dynamicObjects.push_back(objectBody);

		CustomUserPointer* userPointer = new CustomUserPointer();
		userPointer->m_vectorIndex = m_dynamicObjects.size()-1;
		m_userPointer.push_back(userPointer);
		userPointer->m_type = PhysicsType::TYPE_DYNAMIC;
		userPointer->m_id = new int();
		userPointer->m_modelHandle = p_modelHandle;
		userPointer->m_entityId = p_entityId;
	
		*(userPointer->m_id) = m_userPointer.size()-1;
		objectBody->setUserPointer((void*)userPointer);
		return userPointer->m_id;
	}

	int* RootPhysics::AddPlayerObjectToWorld( std::string p_modelHandle, unsigned int p_entityId, glm::vec3 p_position, glm::quat p_rotation, float p_mass, float p_maxSpeed, float p_modelHeight, float p_stepHeight, std::set<unsigned int>* p_enityCollidedId )
	{
		PhysicsMeshInterface* tempMesh = g_resourceManager->GetPhysicsMesh(p_modelHandle);
		KinematicController* player = new KinematicController();
		
		player->Init(m_dynamicWorld, tempMesh->GetNrOfFaces(), tempMesh->GetIndices(), 3*sizeof(int), 
			tempMesh->GetNrOfPoints() , (btScalar*) tempMesh->GetMeshPoints(), 3*sizeof(float), p_position, p_rotation, p_mass, p_maxSpeed, p_modelHeight, p_stepHeight );

		m_playerObjects.push_back(player);
		CustomUserPointer* userPointer = new CustomUserPointer();
		userPointer->m_vectorIndex =  m_playerObjects.size()-1;
		m_userPointer.push_back(userPointer);
		userPointer->m_id = new int();
		*(userPointer->m_id) = m_userPointer.size()-1;
		userPointer->m_type = PhysicsType::TYPE_PLAYER;
		userPointer->m_modelHandle = p_modelHandle;
		userPointer->m_entityId = p_entityId;
		userPointer->m_collidedEntityId = p_enityCollidedId;
		player->SetUserPointer((void*)userPointer);
		player->SetDebugDrawer(m_debugDrawer);
		return userPointer->m_id;
	}
	int* RootPhysics::AddAbilityToWorld(AbilityPhysicsInfo p_abilityInfo)
	{
		btRigidBody* body = nullptr;
		btVector3 velocity, gravity;
		
		velocity.setX(p_abilityInfo.m_direction[0] * p_abilityInfo.m_speed);
		velocity.setY(p_abilityInfo.m_direction[1] * p_abilityInfo.m_speed);
		velocity.setZ(p_abilityInfo.m_direction[2] * p_abilityInfo.m_speed);
		gravity.setX(p_abilityInfo.m_gravity[0]);
		gravity.setY(p_abilityInfo.m_gravity[1]);
		gravity.setZ(p_abilityInfo.m_gravity[2]);

		//Don't allow a orientation of {0,0,0,0} since that breaks bullet
		if(p_abilityInfo.m_shape == PhysicsShape::SHAPE_CYLINDER ||p_abilityInfo.m_shape == PhysicsShape::SHAPE_CONE)
		{
			if(p_abilityInfo.m_orientation[0] == 0 && p_abilityInfo.m_orientation[1] == 0 && p_abilityInfo.m_orientation[2] == 0 && p_abilityInfo.m_orientation[3] == 0)
			{
				g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::NON_FATAL_ERROR, "Orientation for cone/cylinder shapes can't be 0,0,0,0");
				return nullptr;
			}
		}
		else if(p_abilityInfo.m_shape == PhysicsShape::SHAPE_CUSTOM_MESH)
		{
			if(p_abilityInfo.m_modelHandle.empty())
			{
				g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::NON_FATAL_ERROR, "ModelHandle must be specified when using SHAPE_CUSTOM_MESH");
				return nullptr;
			}
		}
		if(p_abilityInfo.m_shape == PhysicsShape::SHAPE_SPHERE)
			body = CreateSphere(p_abilityInfo.m_radius, p_abilityInfo.m_mass, p_abilityInfo.m_position);
		else if(p_abilityInfo.m_shape == PhysicsShape::SHAPE_CONE)
			body = CreateCone(p_abilityInfo.m_radius, p_abilityInfo.m_height, p_abilityInfo.m_position, p_abilityInfo.m_orientation, p_abilityInfo.m_mass);
		else if(p_abilityInfo.m_shape == PhysicsShape::SHAPE_CYLINDER)
			body = CreateCylinder(p_abilityInfo.m_radius, p_abilityInfo.m_height, p_abilityInfo.m_position, p_abilityInfo.m_orientation, p_abilityInfo.m_mass);
		else if(p_abilityInfo.m_shape == PhysicsShape::SHAPE_CUSTOM_MESH)
			body = CreateMesh(p_abilityInfo.m_modelHandle, p_abilityInfo.m_position, p_abilityInfo.m_orientation, p_abilityInfo.m_mass);
		else
		{
			g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::WARNING, "Unable to find PhysicsShape; %d", p_abilityInfo.m_shape);
			return nullptr;
		}
		if(body == nullptr)
		{
			g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::NON_FATAL_ERROR, "Unable to create body for ability shape: %d", p_abilityInfo.m_shape);
			return nullptr;
		}

		

		

		body->setLinearVelocity(velocity);
		body->setFlags(BT_DISABLE_WORLD_GRAVITY);
		body->setGravity(gravity);
		body->applyGravity();
	//	body->setActivationState(DISABLE_DEACTIVATION);
		if(p_abilityInfo.m_collidesWorld)
		{
			body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
		}
		else
		{
			body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
		}
		m_dynamicWorld->addRigidBody(body);
		m_dynamicObjects.push_back(body);
		CustomUserPointer* userPointer = new CustomUserPointer();
		userPointer->m_vectorIndex = m_dynamicObjects.size()-1;
		userPointer->m_type = p_abilityInfo.m_type;
		m_userPointer.push_back(userPointer);
		userPointer->m_id = new int();
		*(userPointer->m_id) = m_userPointer.size()-1;
		userPointer->m_entityId = p_abilityInfo.m_entityId;
		body->setUserPointer((void*)userPointer);



		return m_userPointer.back()->m_id;
	}

	//TODO : remake so it works for all types of controller, second TODO: implement different types of controllers
	void RootPhysics::PlayerMoveXZ( int p_objectHandle, glm::vec3 p_direction )
	{
		if(!DoesObjectExist(p_objectHandle))
			return;

		unsigned int index = m_userPointer.at(p_objectHandle)->m_vectorIndex;
		m_playerObjects.at(index)->Walk(p_direction, m_dt); //SADFJAKSJDGKLAS
	}

	void RootPhysics::PlayerJump( int p_objectHandle, float p_jumpForce )
	{
		if(!DoesObjectExist(p_objectHandle))
			return;

		unsigned int index = m_userPointer.at(p_objectHandle)->m_vectorIndex;
		m_playerObjects.at(index)->Jump();
	}
	
	void RootPhysics::PlayerKnockback( int p_objectHandle, glm::vec3 p_pushDirection, float p_pushForce )
	{
		if(!DoesObjectExist(p_objectHandle))
			return;

		unsigned int index = m_userPointer.at(p_objectHandle)->m_vectorIndex;
		btVector3 temp = btVector3(p_pushDirection[0], p_pushDirection[1], p_pushDirection[2]);
		temp.normalize();
		m_playerObjects.at(index)->Knockback(temp, p_pushForce);
	}

	bool RootPhysics::DoesObjectExist( int p_objectHandle )
	{
		if(m_userPointer.size() == 0 || (unsigned int)p_objectHandle >= m_userPointer.size() || p_objectHandle < 0)
		{
			g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::WARNING, "Attemting to access non existing user pointer at index %d", p_objectHandle);
			return false;
		}
		else
		{
			if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_PLAYER)
			{
				if(m_playerObjects.size() == 0|| (unsigned int)m_userPointer.at(p_objectHandle)->m_vectorIndex > m_playerObjects.size()-1 || m_userPointer.at(p_objectHandle)->m_vectorIndex < 0)
				{
					g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::WARNING, "Attemting to access non existing player object at index %d", m_userPointer.at(p_objectHandle)->m_vectorIndex);
					return false;
				}
			}
			else
			{
				if(m_dynamicObjects.size() == 0 || (unsigned int)m_userPointer.at(p_objectHandle)->m_vectorIndex > m_dynamicObjects.size()-1 || m_userPointer.at(p_objectHandle)->m_vectorIndex < 0)
				{
					g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::WARNING, "Attemting to access non existing dynamic object at index %d", m_userPointer.at(p_objectHandle)->m_vectorIndex);
					return false;
				}
			}
		}
		return true;
	}

	void RootPhysics::EnableDebugDraw( bool p_enabled )
	{
		if(p_enabled == true)
			g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::DEBUG_PRINT, "Turned physics debugdraw on ");
		else
			g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::DEBUG_PRINT, "Turned physics debugdraw off");
		
		m_debugDrawEnabled = p_enabled;
	}

	glm::vec3 RootPhysics::GetPos(int p_objectHandle)
	{
		if(!DoesObjectExist(p_objectHandle))
			return glm::vec3(0,0,0);

		unsigned int index = m_userPointer.at(p_objectHandle)->m_vectorIndex;
		
		btVector3 temp;
		if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_PLAYER)
		{
			temp = m_playerObjects.at(index)->GetPosition();
		}
		else if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_ABILITY || m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_DYNAMIC )
			temp = m_dynamicObjects.at(index)->getWorldTransform().getOrigin();
		glm::vec3 retVal;
		retVal[0] = temp.getX();
		retVal[1] = temp.getY();
		retVal[2] = temp.getZ();
		return retVal;

	}

	float RootPhysics::GetMass( int p_objectHandle )
	{
		if(!DoesObjectExist(p_objectHandle))
			return -1;
		unsigned int index = m_userPointer.at(p_objectHandle)->m_vectorIndex;

		if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_PLAYER)
		{
			return m_playerObjects.at(index)->GetMass();
		}

		else if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_ABILITY)
		{
			return 1/m_dynamicObjects.at(index)->getInvMass();
		}
	
		return -1 ;
		

	}
	int RootPhysics::GetType(int p_objectHandle)
	{
		if(!DoesObjectExist(p_objectHandle))
			return -1;
		return m_userPointer.at(p_objectHandle)->m_type;

	}

	float RootPhysics::GetStepHeight( int p_objectHandle )
	{
		if(!DoesObjectExist(p_objectHandle))
			return -1;
		if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_PLAYER)
		{
			unsigned index = m_userPointer.at(p_objectHandle)->m_vectorIndex;
			return m_playerObjects.at(index)->GetStepHeight();
		}
		return -1;
	}
	glm::quat RootPhysics::GetOrientation( int p_objectHandle)
	{
		if(!DoesObjectExist(p_objectHandle))
			return glm::quat(0,0,0,0);

		unsigned int index = m_userPointer.at(p_objectHandle)->m_vectorIndex;
		glm::quat retVal;
		if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_PLAYER)
		{
			btQuaternion temp = m_playerObjects.at(index)->GetOrientation();
			retVal[0] = temp.x();
			retVal[1] = temp.y();
			retVal[2] = temp.z();
			retVal[3] = temp.w();
			
		}
		
		else if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_ABILITY)
		{
			btRigidBody* body = m_dynamicObjects.at(index);
			btTransform transform;
			transform = body->getWorldTransform();
			
			retVal[0] = transform.getRotation().x();
			retVal[1] = transform.getRotation().y();
			retVal[2] = transform.getRotation().z();
			retVal[3] = transform.getRotation().w();
		}
		else
		{
			return glm::quat(0,0,0,0);
		}
		return retVal;
	}

	float RootPhysics::GetMaxSpeed( int p_objectHandle )
	{
		if(!DoesObjectExist(p_objectHandle))
			return -1;
		if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_PLAYER)
		{
			unsigned index = m_userPointer.at(p_objectHandle)->m_vectorIndex;
			return m_playerObjects.at(index)->GetMaxSpeed();
		}
		return -1;
	}
	float RootPhysics::GetModelHeight( int p_objectHandle )
	{
		if(!DoesObjectExist(p_objectHandle))
			return -1;
		if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_PLAYER)
		{
			unsigned index = m_userPointer.at(p_objectHandle)->m_vectorIndex;
			return m_playerObjects.at(index)->GetModelHeight();
		}
		return -1;
	}	

	std::string RootPhysics::GetPhysicsModelHandle( int p_objectHandle )
	{
		if(!DoesObjectExist(p_objectHandle))
			return "";
		return m_userPointer.at(p_objectHandle)->m_modelHandle;
		
	}

	std::set<unsigned int>* RootPhysics::GetCollisionVector( int p_objectHandle )
	{
		return (m_userPointer.at(p_objectHandle)->m_collidedEntityId);
	}


	glm::vec3 RootPhysics::GetVelocity( int p_objectHandle )
	{
		if(!DoesObjectExist(p_objectHandle))
			return glm::vec3(0,0,0);
		unsigned int index = m_userPointer.at(p_objectHandle)->m_vectorIndex;
		if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_PLAYER)
		{
			//No, player doesn't have a constant velocity, and im gonna go ahead and guess that ability controllers won't either
			return glm::vec3(0,0,0);
		}
		else
		{

			btVector3 temp = m_dynamicObjects.at(index)->getLinearVelocity();

			return glm::vec3(temp[0], temp[1], temp[2]);
		}
		
	}
	RootPhysics* RootPhysics::GetInstance()
	{
		if(!s_physicsInstance)
		{
			s_physicsInstance = new RootPhysics();
		}
		return s_physicsInstance;
	}

	void RootPhysics::SetVelocity( int p_objectHandle, glm::vec3 p_velocity )
	{
		if(!DoesObjectExist(p_objectHandle))
			return;
		btVector3 temp (p_velocity[0], p_velocity[1], p_velocity[2]);
		unsigned int index = m_userPointer.at(p_objectHandle)->m_vectorIndex;
		if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_PLAYER)
		{
			m_playerObjects.at(index)->Walk(p_velocity, m_dt);
		}
		else if(m_userPointer.at(p_objectHandle)->m_type = PhysicsType::TYPE_ABILITY)
		{
			//Change if abilities becomes controllerz
			m_dynamicObjects.at(index)->setLinearVelocity(temp);
		}
		else
		{
			m_dynamicObjects.at(index)->setLinearVelocity(temp);
		}
	}

	void RootPhysics::SetOrientation( int p_objectHandle, glm::quat p_objectOrientation )
	{
		if(!DoesObjectExist(p_objectHandle))
			return;

		unsigned int index = m_userPointer.at(p_objectHandle)->m_vectorIndex;
		
		if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_PLAYER)
		{
			m_playerObjects.at(index)->SetOrientation(p_objectOrientation);
		}
		else
		{
			float x,y,z, w;
			btRigidBody* body = m_dynamicObjects.at(index);
		
			x = p_objectOrientation[0];
			y = p_objectOrientation[1];
			z = p_objectOrientation[2];
			w = p_objectOrientation[3];
			body->getMotionState()->setWorldTransform(btTransform(btQuaternion(x,y,z, w), body->getWorldTransform().getOrigin()));
			
		}
	}


	void RootPhysics::SetGravity( int p_objectHandle, glm::vec3 p_gravity )
	{
		if(!DoesObjectExist(p_objectHandle))
			return;
		int index = m_userPointer.at(p_objectHandle)->m_vectorIndex;
		if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_PLAYER)
		{
			m_playerObjects.at(index)->SetGravity(p_gravity[1]);		
		}
		else
		{
			
			btVector3 gravity;
			gravity.setX(p_gravity[0]);
			gravity.setY(p_gravity[1]);
			gravity.setZ(p_gravity[2]);
			m_dynamicObjects.at(index)->setGravity(gravity);
			m_dynamicObjects.at(index)->applyGravity();
		}
	}

	//TODO : Remake to work for all objecttypes
	void RootPhysics::SetMass( int p_objectHandle, float p_mass )
	{
		if(!DoesObjectExist(p_objectHandle))
			return;
		unsigned int index = m_userPointer.at(p_objectHandle)->m_vectorIndex;
		if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_PLAYER)
			m_playerObjects.at(index)->SetMass(p_mass);
		else
		{
			btVector3 fallInertia =  btVector3(0,0,0);
			m_dynamicObjects.at(index)->getCollisionShape()->calculateLocalInertia(p_mass, fallInertia);
			m_dynamicObjects.at(index)->setMassProps(p_mass, fallInertia);
		}
		
		
	}

	//TODO :Remake to work for all objecttypes
	void RootPhysics::SetDynamicObjectVelocity( int p_objectHandle, glm::vec3 p_velocity )
	{
		if(!DoesObjectExist(p_objectHandle))
			return;
		unsigned int index = m_userPointer.at(p_objectHandle)->m_vectorIndex;
		if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_PLAYER)
		{
			return;
		}
		else
			m_dynamicObjects.at(index)->setLinearVelocity(btVector3(p_velocity[0], p_velocity[1], p_velocity[2]));
	}

	void RootPhysics::SetPosition( int p_objectHandle , glm::vec3 p_position )
	{
		if(!DoesObjectExist(p_objectHandle))
			return;

		unsigned int index = m_userPointer.at(p_objectHandle)->m_vectorIndex;
		btVector3 temp (p_position[0], p_position[1], p_position[2]);
		if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_PLAYER)
		{
			m_playerObjects.at(index)->Move(p_position, m_dt);
		}
		else
		{
			m_dynamicObjects.at(index)->getWorldTransform().setOrigin(temp);
		}
	}

}
}

RootEngine::Physics::PhysicsInterface* CreatePhysics( RootEngine::SubsystemSharedContext p_context, Render::RendererInterface* p_renderer , RootEngine::ResourceManagerInterface* p_resourceManager)
{
	RootEngine::Physics::g_context = p_context;
	RootEngine::Physics::g_renderer = p_renderer;
	RootEngine::Physics::g_resourceManager = p_resourceManager;
	return RootEngine::Physics::RootPhysics::GetInstance();
}
