#include "RootPhysics.h"
#include <stdio.h>
#include <iostream>
#include <Bullet/BulletCollision/CollisionShapes/btShapeHull.h>
#include <RootEngine/Include/Logging/Logging.h>
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
		//Magic loop of deleting
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

		if(pointer1->m_type == PhysicsType::TYPE_PLAYER || pointer1->m_type == PhysicsType::TYPE_ABILITY)
			pointer1->m_collidedEntityId.push_back(pointer2->m_entityId);
		if(pointer2->m_type == PhysicsType::TYPE_PLAYER || pointer2->m_type == PhysicsType::TYPE_ABILITY)
			pointer2->m_collidedEntityId.push_back(pointer1->m_entityId);

		if(pointer1->m_type == PhysicsType::TYPE_PLAYER || pointer2->m_type == PhysicsType::TYPE_PLAYER)
			if(pointer1->m_type == PhysicsType::TYPE_ABILITY || pointer2->m_type == PhysicsType::TYPE_ABILITY )
				int lol = 2;

		return false;
	}
	void RootPhysics::Init()
	{
		m_collisionConfig= new btDefaultCollisionConfiguration();
		m_dispatcher = new btCollisionDispatcher(m_collisionConfig);
		m_broadphase = new btDbvtBroadphase();
		m_broadphase->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
		m_solver = new btSequentialImpulseConstraintSolver();
		m_dynamicWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfig);
		m_dynamicWorld->setGravity(btVector3(0.0f, -9.82f, 0.0f));
		gContactAddedCallback = callbackFunc;
		g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::INIT_PRINT, "Physics subsystem initialized!");

		m_debugDrawer = new DebugDrawer();
		m_debugDrawer->setDebugMode(m_debugDrawer->getDebugMode() | btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawContactPoints |btIDebugDraw::DBG_DrawAabb);
		m_dynamicWorld->setDebugDrawer(m_debugDrawer);
		m_dynamicWorld->debugDrawWorld();
		
		m_dynamicWorld->getDispatchInfo().m_allowedCcdPenetration=0.0001f;
	}


	//Creates an impassable plane
	void RootPhysics::CreatePlane(float* p_normal, float* p_position)
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
	//////////////////////////////////////////////////////////////////////////
	//Make a real update
	void RootPhysics::Update(float p_dt)
	{
		
		m_dt = p_dt;
		for(unsigned int i = 0; i < m_playerObjects.size(); i++)
		{
			m_playerObjects.at(i)->Update(m_dt);
		}
		m_dynamicWorld->stepSimulation(m_dt,1);
		//g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::DEBUG_PRINT, "DebugDrawingWorld");
		m_dynamicWorld->debugDrawWorld();
		
	}
	//////////////////////////////////////////////////////////////////////////
	//Use this to add a static object to the World, i.e trees, rocks and the ground. Both position and rotation are vec3
	void RootPhysics::AddStaticObjectToWorld( std::string p_modelHandle, unsigned int p_entityId, float* p_position, float* p_rotation )
	{
		//TODO if time and need to improve performance: Use compundshapes
		PhysicsMeshInterface* tempMesh = g_resourceManager->GetPhysicsMesh(p_modelHandle);
		btTriangleIndexVertexArray* indexVertexArray = new btTriangleIndexVertexArray(tempMesh->GetNrOfFaces(), tempMesh->GetIndices(), 3*sizeof(int), tempMesh->GetNrOfPoints() , (btScalar*) tempMesh->GetMeshPoints(), 3*sizeof(float));
		btScalar mass = 0; //mass is always 0 for static objects
		btCollisionShape* objectMeshShape = new btBvhTriangleMeshShape(indexVertexArray, true); // the bool is a flag that improves memory usage

		//Set startpos and start rotation
		btTransform startTransform;
		startTransform.setIdentity();
		startTransform.setOrigin(btVector3(p_position[0],p_position[1],p_position[2]));
		startTransform.setRotation(btQuaternion(p_rotation[0],p_rotation[1], p_rotation[2],1));
		//Create a motionstate
		btDefaultMotionState* motionstate = new btDefaultMotionState(startTransform);
		//create the body
		btRigidBody* objectBody = new btRigidBody(mass,motionstate,objectMeshShape);
		m_dynamicWorld->addRigidBody(objectBody);
		CustomUserPointer* userPointer = new CustomUserPointer();
		userPointer->m_vectorIndex = -1;
		m_userPointer.push_back(userPointer);
		userPointer->m_type = PhysicsType::TYPE_STATIC;
		userPointer->m_id = new int();
		userPointer->m_modelHandle = p_modelHandle;
		userPointer->m_entityId = p_entityId;
		*(userPointer->m_id) = m_userPointer.size()-1;
		objectBody->setUserPointer((void*)userPointer);


	}
	int* RootPhysics::AddDynamicObjectToWorld(std::string p_modelHandle, unsigned int p_entityId,  float* p_position, float* p_rotation , float p_mass )
	{
		
		//creates the mesh shape
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
			simplifiedObject->addPoint(objectHull->getVertexPointer()[i], false);
			//g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::DEBUG_PRINT,  "vertex : %d x: %f y: %f z: %f", i, objectHull->getVertexPointer()[i].x(),objectHull->getVertexPointer()[i].y(),objectHull->getVertexPointer()[i].z());
		}	
		simplifiedObject->recalcLocalAabb();

		////Set Inertia
		btVector3 fallInertia =  btVector3(0,0,0);
		simplifiedObject->calculateLocalInertia(p_mass,fallInertia);

		//Set startpos and start rotation and bind them to a motionstate
		btTransform startTransform;
		startTransform.setIdentity();
		startTransform.setOrigin(btVector3(p_position[0],p_position[1],p_position[2]));
		startTransform.setRotation(btQuaternion(p_rotation[0],p_rotation[1], p_rotation[2],1));
		btDefaultMotionState* motionstate = new btDefaultMotionState(startTransform);

		//create a body
		btRigidBody::btRigidBodyConstructionInfo objectBodyInfo(p_mass, motionstate,simplifiedObject, fallInertia );
		btRigidBody* objectBody = new btRigidBody(objectBodyInfo);
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

	void RootPhysics::SetDynamicObjectVelocity( int p_objectIndex, float* p_velocity )
	{
		if(!DoesObjectExist(p_objectIndex))
			return;

		m_dynamicObjects.at(p_objectIndex)->setLinearVelocity(btVector3(p_velocity[0], p_velocity[1], p_velocity[2]));
	}

	void RootPhysics::SetObjectMass( int p_objectIndex, float p_mass )
	{
		if(!DoesObjectExist(p_objectIndex))
			return;

		btVector3 fallInertia =  btVector3(0,0,0);
		m_dynamicObjects.at(p_objectIndex)->getCollisionShape()->calculateLocalInertia(p_mass, fallInertia);
		m_dynamicObjects.at(p_objectIndex)->setMassProps(p_mass, fallInertia);
	}

	int* RootPhysics::AddPlayerObjectToWorld( std::string p_modelHandle, unsigned int p_entityId, float* p_position, float* p_rotation, float p_mass, float p_maxSpeed, float p_modelHeight, float p_stepHeight )
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
		player->SetUserPointer((void*)userPointer);
		player->SetDebugDrawer(m_debugDrawer);
		return userPointer->m_id;
	}

	void RootPhysics::PlayerMoveXZ( int p_objectIndex, float* p_direction )
	{
		if(!DoesObjectExist(p_objectIndex))
			return;

		unsigned int index = m_userPointer.at(p_objectIndex)->m_vectorIndex;
		m_playerObjects.at(index)->Walk(p_direction, m_dt); //SADFJAKSJDGKLAS
	}

	void RootPhysics::PlayerJump( int p_objectIndex, float p_jumpForce )
	{
		if(!DoesObjectExist(p_objectIndex))
			return;

		unsigned int index = m_userPointer.at(p_objectIndex)->m_vectorIndex;
		m_playerObjects.at(index)->Jump();
	}

	void RootPhysics::GetPos( int p_objectIndex, float* p_pos )
	{
		if(!DoesObjectExist(p_objectIndex))
			return;

		unsigned int index = m_userPointer.at(p_objectIndex)->m_vectorIndex;

		btVector3 temp;
		if(m_userPointer.at(p_objectIndex)->m_type == PhysicsType::TYPE_PLAYER)
			temp = m_playerObjects.at(index)->GetPosition();
		else if(m_userPointer.at(p_objectIndex)->m_type == PhysicsType::TYPE_ABILITY || m_userPointer.at(p_objectIndex)->m_type == PhysicsType::TYPE_DYNAMIC )
			temp = m_dynamicObjects.at(index)->getWorldTransform().getOrigin();
		p_pos[0] = temp.getX();
		p_pos[1] = temp.getY();
		p_pos[2] = temp.getZ();


	}

	RootPhysics* RootPhysics::GetInstance()
	{
		if(!s_physicsInstance)
		{
			s_physicsInstance = new RootPhysics();
		}
		return s_physicsInstance;
	}

 	void RootPhysics::PlayerKnockback( int p_objectIndex, float* p_pushDirection, float p_pushForce )
	{
		if(!DoesObjectExist(p_objectIndex))
			return;

		unsigned int index = m_userPointer.at(p_objectIndex)->m_vectorIndex;
 		btVector3 temp = btVector3(p_pushDirection[0], p_pushDirection[1], p_pushDirection[2]);
		temp.normalize();
 		m_playerObjects.at(index)->Knockback(temp, p_pushForce);
	}

	void RootPhysics::RemoveObject( int p_objectIndex)
	{
		if(!DoesObjectExist(p_objectIndex))
			return;

		CustomUserPointer* userPointer = m_userPointer.at(p_objectIndex);
		if(userPointer->m_type == PhysicsType::TYPE_PLAYER)
		{
			unsigned int removedIndex = userPointer->m_vectorIndex;
			m_playerObjects.at(removedIndex)->RemovePlayer();
			delete m_playerObjects.at(removedIndex);
			m_playerObjects.erase(m_playerObjects.begin() + removedIndex);

			delete userPointer;
			m_userPointer.erase(m_userPointer.begin() + p_objectIndex);
		
			for(unsigned int i = p_objectIndex; i < m_userPointer.size(); i++)
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
			m_userPointer.erase(m_userPointer.begin() + p_objectIndex);

			for(unsigned int i = p_objectIndex; i < m_userPointer.size(); i++)
			{
			
				m_userPointer.at(i)->m_id[0] --;
				if(m_userPointer.at(i)->m_type != PhysicsType::TYPE_PLAYER)
				{

					m_userPointer.at(i)->m_vectorIndex--;
				}
			}
		}
	}


	btRigidBody* RootPhysics::CreateSphere( float p_radius, float p_mass, float* p_position)
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
	btRigidBody* RootPhysics::CreateCylinder( float p_radius, float p_height, float* p_position, float* p_rotation, float p_mass )
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
		body->setActivationState(DISABLE_DEACTIVATION);
		return body;
	}
	btRigidBody* RootPhysics::CreateCone(float p_radius, float p_height, float* p_position,float* p_rotation, float p_mass)
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
		return body;
	}

	void RootPhysics::GetObjectOrientation( int p_objectIndex, float* p_objectOrientation )
	{
		if(!DoesObjectExist(p_objectIndex))
			return;

		unsigned int index = m_userPointer.at(p_objectIndex)->m_vectorIndex;

		if(m_userPointer.at(p_objectIndex)->m_type == PhysicsType::TYPE_PLAYER)
		{
			btQuaternion temp = m_playerObjects.at(index)->GetOrientation();
			p_objectOrientation[0] = temp.x();
			p_objectOrientation[1] = temp.y();
			p_objectOrientation[2] = temp.z();
			p_objectOrientation[3] = temp.w();
		}
		
		else if(m_userPointer.at(p_objectIndex)->m_type == PhysicsType::TYPE_ABILITY)
		{
			btRigidBody* body = m_dynamicObjects.at(index);
			btTransform transform;
			transform = body->getWorldTransform();
			
			p_objectOrientation[0] = transform.getRotation().x();
			p_objectOrientation[1] = transform.getRotation().y();
			p_objectOrientation[2] = transform.getRotation().z();
			p_objectOrientation[3] = transform.getRotation().w();
		}
		else
		{
			return;
		}
	}

	void RootPhysics::SetObjectOrientation( int p_objectIndex, float* p_objectOrientation )
	{
		if(!DoesObjectExist(p_objectIndex))
			return;

		unsigned int index = m_userPointer.at(p_objectIndex)->m_vectorIndex;
		btRigidBody* body = m_dynamicObjects.at(index);
		float x,y,z, w;
		x = p_objectOrientation[0];
		y = p_objectOrientation[1];
		z = p_objectOrientation[2];
		w = p_objectOrientation[3];
		body->getMotionState()->setWorldTransform(btTransform(btQuaternion(x,y,z, w), body->getWorldTransform().getOrigin()));
		//body->setWorldTransform(btTransform(btQuaternion(x,y,z, w), body->getWorldTransform().getOrigin()));
	}

	void RootPhysics::SetPlayerOrientation( int p_objectIndex, float* p_playerOrientation )
	{
		if(!DoesObjectExist(p_objectIndex))
			return;

		unsigned int index = m_userPointer.at(p_objectIndex)->m_vectorIndex;
		m_playerObjects.at(index)->SetOrientation(p_playerOrientation);
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
		

		if(p_abilityInfo.m_shape == AbilityShape::SHAPE_SPHERE)
			body = CreateSphere(p_abilityInfo.m_radius, p_abilityInfo.m_mass, p_abilityInfo.m_position);
		else if(p_abilityInfo.m_shape == AbilityShape::SHAPE_CONE)
			body = CreateCone(p_abilityInfo.m_radius, p_abilityInfo.m_height, p_abilityInfo.m_position, p_abilityInfo.m_orientation, p_abilityInfo.m_mass);
		else if(p_abilityInfo.m_shape == AbilityShape::SHAPE_CYLINDER)
			body = CreateCylinder(p_abilityInfo.m_radius, p_abilityInfo.m_height, p_abilityInfo.m_position, p_abilityInfo.m_orientation, p_abilityInfo.m_mass);
		else
		{
			g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::WARNING, "Unable to find AbilityShape; %d", p_abilityInfo.m_shape);
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
		body->setActivationState(DISABLE_DEACTIVATION);
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
		userPointer->m_type = PhysicsType::TYPE_ABILITY;
		m_userPointer.push_back(userPointer);
		userPointer->m_id = new int();
		*(userPointer->m_id) = m_userPointer.size()-1;
		userPointer->m_entityId = p_abilityInfo.m_entityId;
		body->setUserPointer((void*)userPointer);



		return m_userPointer.back()->m_id;
	}

	bool RootPhysics::DoesObjectExist( int p_objectIndex )
	{
		if(m_userPointer.size() == 0 || (unsigned int)p_objectIndex >= m_userPointer.size() || p_objectIndex < 0)
		{
			g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::WARNING, "Attemting to access non existing user pointer at index %d", p_objectIndex);
			return false;
		}
		else
		{
			if(m_userPointer.at(p_objectIndex)->m_type == PhysicsType::TYPE_PLAYER)
			{
				if(m_playerObjects.size() == 0|| (unsigned int)m_userPointer.at(p_objectIndex)->m_vectorIndex > m_playerObjects.size()-1 || m_userPointer.at(p_objectIndex)->m_vectorIndex < 0)
				{
					g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::WARNING, "Attemting to access non existing player object at index %d", m_userPointer.at(p_objectIndex)->m_vectorIndex);
					return false;
				}
			}
			else
			{
				if(m_dynamicObjects.size() == 0 || (unsigned int)m_userPointer.at(p_objectIndex)->m_vectorIndex > m_dynamicObjects.size()-1 || m_userPointer.at(p_objectIndex)->m_vectorIndex < 0)
				{
					g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::WARNING, "Attemting to access non existing dynamic object at index %d", m_userPointer.at(p_objectIndex)->m_vectorIndex);
					return false;
				}
			}
		}
		return true;
	}

	void RootPhysics::SetGravity( int p_objectIndex, float* p_gravity )
	{
		if(!DoesObjectExist(p_objectIndex))
			return;
		if(m_userPointer.at(p_objectIndex)->m_type == PhysicsType::TYPE_PLAYER)
		{
			//Implement later, not as cool as abilitys
			int lol = 2;
		}
		else
		{
			int index = m_userPointer.at(p_objectIndex)->m_vectorIndex;
			btVector3 gravity;
			gravity.setX(p_gravity[0]);
			gravity.setY(p_gravity[1]);
			gravity.setZ(p_gravity[2]);
			gravity.normalize();
			m_dynamicObjects.at(index)->setGravity(gravity * 25.0f);
			m_dynamicObjects.at(index)->applyGravity();
		}
	}

	float RootPhysics::GetMass( int p_objectIndex )
	{
		if(!DoesObjectExist(p_objectIndex))
			return -1;
		unsigned int index = m_userPointer.at(p_objectIndex)->m_vectorIndex;

		if(m_userPointer.at(p_objectIndex)->m_type == PhysicsType::TYPE_PLAYER)
		{
			return m_playerObjects.at(index)->GetMass();
		}

		else if(m_userPointer.at(p_objectIndex)->m_type == PhysicsType::TYPE_ABILITY)
		{
			return 1/m_dynamicObjects.at(index)->getInvMass();
		}
	
		return -1 ;
		

	}
	int RootPhysics::GetType(int p_objectIndex)
	{
		if(!DoesObjectExist(p_objectIndex))
			return -1;
		return m_userPointer.at(p_objectIndex)->m_type;

	}

	float RootPhysics::GetStepHeight( int p_objectIndex )
	{
		if(!DoesObjectExist(p_objectIndex))
			return -1;
		if(m_userPointer.at(p_objectIndex)->m_type == PhysicsType::TYPE_PLAYER)
		{
			unsigned index = m_userPointer.at(p_objectIndex)->m_vectorIndex;
			return m_playerObjects.at(index)->GetStepHeight();
		}
		return -1;
	}
	float RootPhysics::GetMaxSpeed( int p_objectIndex )
	{
		if(!DoesObjectExist(p_objectIndex))
			return -1;
		if(m_userPointer.at(p_objectIndex)->m_type == PhysicsType::TYPE_PLAYER)
		{
			unsigned index = m_userPointer.at(p_objectIndex)->m_vectorIndex;
			return m_playerObjects.at(index)->GetMaxSpeed();
		}
		return -1;
	}
	float RootPhysics::GetModelHeight( int p_objectIndex )
	{
		if(!DoesObjectExist(p_objectIndex))
			return -1;
		if(m_userPointer.at(p_objectIndex)->m_type == PhysicsType::TYPE_PLAYER)
		{
			unsigned index = m_userPointer.at(p_objectIndex)->m_vectorIndex;
			return m_playerObjects.at(index)->GetModelHeight();
		}
		return -1;
	}	

	std::string RootPhysics::GetPhysicsModelHandle( int p_objectIndex )
	{
		if(!DoesObjectExist(p_objectIndex))
			return "";
		return m_userPointer.at(p_objectIndex)->m_modelHandle;
		
	}

	std::vector<unsigned int>* RootPhysics::GetCollisionVector( int p_objectIndex )
	{
		return &(m_userPointer.at(p_objectIndex)->m_collidedEntityId);
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
