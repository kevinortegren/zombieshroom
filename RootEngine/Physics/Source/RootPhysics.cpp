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
	const int TYPE_PLAYER = 2;
	const int TYPE_ABILITY = 1;
	
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
		for(unsigned int i = 0; i < m_playerObject.size(); i++)
		{
			PlayerController* temp = m_playerObject[i];
			delete temp;
		}
		for(unsigned int i = 0; i < m_userPointer.size(); i++)
		{
			CustomUserPointer* temp = m_userPointer[i];
			delete temp;
		}
		m_dynamicObjects.clear();
		m_playerObject.clear();
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
		if(pointer1 == nullptr)
			return false;
		if(pointer2 == nullptr)
			return false;
		int test = ((CustomUserPointer*)(p_obj1->getCollisionObject()->getUserPointer()))->m_type;
		int test2 = ((CustomUserPointer*)(p_obj2->getCollisionObject()->getUserPointer()))->m_type;
		if (pointer1->m_type == TYPE_PLAYER || pointer2->m_type == TYPE_PLAYER)
		{
			if (pointer2->m_type == TYPE_ABILITY || pointer1->m_type == TYPE_ABILITY)
			{
				//Call the functions here	
			}
		}
		else if (pointer1->m_type == TYPE_ABILITY)
		{
			int asdfahjsdf = 1;
		}
		else if (pointer2->m_type == TYPE_ABILITY)
		{
			int asdf = 0;
		}
		else
		{
			//dynamic object which is not ability
		}
		/*
		Om det är en ability, kalla på den funktionen om den kolliderar med vadsom, skicka in nånslags värde för att säga vad den krockat med
		Om det är en spelare, kalla på funktionen om det är en Ability, och skicka då nån form av värde för att beskriva vad det är för ability

		*/

		


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
		m_debugDrawer->setDebugMode(m_debugDrawer->getDebugMode() | btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawContactPoints);
		m_dynamicWorld->setDebugDrawer(m_debugDrawer);
		m_dynamicWorld->debugDrawWorld();
		

	}


	//Creates an impassable plane
	void RootPhysics::CreatePlane(float* p_normal, float* p_position)
	{
		btCollisionShape* plane = new btStaticPlaneShape(btVector3(p_normal[0],p_normal[1],p_normal[2]), 0);
		btDefaultMotionState* planeMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(p_position[0],p_position[1],p_position[2])));
		btRigidBody::btRigidBodyConstructionInfo planeRigidbodyCI(0, planeMotionState, plane, btVector3(0, 0, 0));
		btRigidBody* planeBody = new btRigidBody(planeRigidbodyCI);
		planeBody->setCollisionFlags(planeBody->getCollisionFlags() | btRigidBody::CF_DISABLE_VISUALIZE_OBJECT);
		m_dynamicWorld->addRigidBody(planeBody);

	}
	//////////////////////////////////////////////////////////////////////////
	//Make a real update
	void RootPhysics::Update(float p_dt)
	{
		
		
		for(unsigned int i = 0; i < m_playerObject.size(); i++)
		{
			m_playerObject.at(i)->Update();
		}
		m_dynamicWorld->stepSimulation(p_dt,10);
		//g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::DEBUG_PRINT, "DebugDrawingWorld");
		m_dynamicWorld->debugDrawWorld();
	}
	//////////////////////////////////////////////////////////////////////////
	//Use this to add a static object to the World, i.e trees, rocks and the ground. Both position and rotation are vec3
	void RootPhysics::AddStaticObjectToWorld( int p_numTriangles, int* p_indexBuffer, int p_indexStride, int p_numVertices, float* p_vertexBuffer, int p_vertexStride, float* p_position, float* p_rotation )
	{
		//TODO if time and need to improve performance: Use compundshapes
		btTriangleIndexVertexArray* indexVertexArray = new btTriangleIndexVertexArray(p_numTriangles, p_indexBuffer, p_indexStride, p_numVertices , (btScalar*) p_vertexBuffer, p_vertexStride);
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

	}
	int* RootPhysics::AddDynamicObjectToWorld( int p_numTriangles, int* p_indexBuffer, int p_indexStride, int p_numVertices, float* p_vertexBuffer, int p_vertexStride, float* p_position, float* p_rotation , float p_mass )
	{
		
		//creates the mesh shape
		btTriangleIndexVertexArray* indexVertexArray = new btTriangleIndexVertexArray(p_numTriangles, p_indexBuffer, p_indexStride, p_numVertices ,  p_vertexBuffer, p_vertexStride);
		
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
		userPointer->m_type = TYPE_ABILITY;
		userPointer->m_id = new int();
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

	int* RootPhysics::AddPlayerObjectToWorld( int p_numTriangles, int* p_indexBuffer, int p_indexStride, int p_numVertices, float* p_vertexBuffer, 
											int p_vertexStride, float* p_position, float* p_rotation, float p_mass, float p_maxSpeed, float p_modelHeight, float p_stepHeight )
	{
		PlayerController* player = new PlayerController();
		player->Init(m_dynamicWorld, p_numTriangles, p_indexBuffer, p_indexStride, p_numVertices, p_vertexBuffer
			, p_vertexStride, p_position, p_rotation, p_mass, p_maxSpeed, p_modelHeight, p_stepHeight );


		
		m_playerObject.push_back(player);
		CustomUserPointer* userPointer = new CustomUserPointer();
		userPointer->m_vectorIndex =  m_playerObject.size()-1;
		m_userPointer.push_back(userPointer);
		userPointer->m_id = new int();
		*(userPointer->m_id) = m_userPointer.size()-1;
		userPointer->m_type = TYPE_PLAYER;
		player->SetUserPointer((void*)userPointer);
		return userPointer->m_id;
	}

	void RootPhysics::PlayerMoveXZ( int p_objectIndex, float* p_direction )
	{
		if(!DoesObjectExist(p_objectIndex))
			return;

		unsigned int index = m_userPointer.at(p_objectIndex)->m_vectorIndex;
		m_playerObject.at(index)->Walk(p_direction);
	}

	void RootPhysics::PlayerJump( int p_objectIndex, float p_jumpForce )
	{
		if(!DoesObjectExist(p_objectIndex))
			return;

		unsigned int index = m_userPointer.at(p_objectIndex)->m_vectorIndex;
		m_playerObject.at(index)->Jump(p_jumpForce);
	}

	void RootPhysics::GetPos( int p_objectIndex, float* p_pos )
	{
		if(!DoesObjectExist(p_objectIndex))
			return;

		unsigned int index = m_userPointer.at(p_objectIndex)->m_vectorIndex;

		btVector3 temp;
		if(m_userPointer.at(p_objectIndex)->m_type == TYPE_PLAYER)
			temp = m_playerObject.at(index)->GetPosition();
		else if(m_userPointer.at(p_objectIndex)->m_type == TYPE_ABILITY)
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
 		m_playerObject.at(index)->Knockback(temp * p_pushForce);
	}

	void RootPhysics::RemoveObject( int p_objectIndex, int p_type )
	{
		if(!DoesObjectExist(p_objectIndex))
			return;

		CustomUserPointer* userPointer = m_userPointer.at(p_objectIndex);
		if(userPointer->m_type == TYPE_PLAYER)
		{
			unsigned int removedIndex = userPointer->m_vectorIndex;
			m_playerObject.at(removedIndex)->RemovePlayer();
			delete m_playerObject.at(removedIndex);
			m_playerObject.erase(m_playerObject.begin() + removedIndex);

			delete userPointer;
			m_userPointer.erase(m_userPointer.begin() + p_objectIndex);
		
			for(unsigned int i = p_objectIndex; i < m_userPointer.size(); i++)
			{
				m_userPointer.at(i)->m_id[0] --;
				if(m_userPointer.at(i)->m_type == TYPE_PLAYER)
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
				if(m_userPointer.at(i)->m_type != TYPE_PLAYER)
				{

					m_userPointer.at(i)->m_vectorIndex--;
				}
			}
		}
	}


	int RootPhysics::CreateSphere( float p_radius, float p_mass, float* p_position)
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
		m_dynamicWorld->addRigidBody(body);
		m_dynamicObjects.push_back(body);
		body = 0;
		sphere = 0;
		motionstate = 0;
		delete sphere;
		delete body;
		delete motionstate;


		return m_dynamicObjects.size()-1;
	}

	void RootPhysics::GetObjectOrientation( int p_objectIndex, float* p_objectOrientation )
	{
		if(!DoesObjectExist(p_objectIndex))
			return;

		unsigned int index = m_userPointer.at(p_objectIndex)->m_vectorIndex;

		if(m_userPointer.at(p_objectIndex)->m_type == TYPE_PLAYER)
		{
			btQuaternion temp = m_playerObject.at(index)->GetOrientation();
			p_objectOrientation[0] = temp.x();
			p_objectOrientation[1] = temp.y();
			p_objectOrientation[2] = temp.z();
			p_objectOrientation[3] = temp.w();
		}
		
		else if(m_userPointer.at(p_objectIndex)->m_type == TYPE_ABILITY)
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
		m_playerObject.at(index)->SetOrientation(p_playerOrientation);
	}

	int* RootPhysics::AddAbilityToWorld(float p_radius, float* p_position, float* p_direction, float p_speed, int p_type /*, void* p_collideFunc */ , float p_mass, float* p_gravity)
	{
		btCollisionShape* sphere = new btSphereShape(p_radius);
		btVector3 pos, velocity, gravity;
		pos.setX( p_position[0]);
		pos.setY( p_position[1]);
		pos.setZ( p_position[2]);
		velocity.setX(p_direction[0] * p_speed);
		velocity.setY(p_direction[1] * p_speed);
		velocity.setZ(p_direction[2] * p_speed);
		gravity.setX(p_gravity[0]);
		gravity.setY(p_gravity[1]);
		gravity.setZ(p_gravity[2]);
		btDefaultMotionState* motionstate = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), pos ));
		btVector3 fallinertia(0,0,0);
		sphere->calculateLocalInertia(p_mass,fallinertia);
		btRigidBody::btRigidBodyConstructionInfo sphereCI(p_mass,motionstate,sphere,fallinertia);
		btRigidBody* body = new btRigidBody(sphereCI);
		body->setActivationState(DISABLE_DEACTIVATION);
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
		body->setLinearVelocity(velocity);
		body->setFlags(BT_DISABLE_WORLD_GRAVITY);
		body->setGravity(gravity);
		body->applyGravity();

		m_dynamicWorld->addRigidBody(body);
		m_dynamicObjects.push_back(body);
		CustomUserPointer* userPointer = new CustomUserPointer();
		userPointer->m_vectorIndex = m_dynamicObjects.size()-1;
		userPointer->m_type = TYPE_ABILITY;
		m_userPointer.push_back(userPointer);
		userPointer->m_id = new int();
		*(userPointer->m_id) = m_userPointer.size()-1;
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
			if(m_userPointer.at(p_objectIndex)->m_type == TYPE_PLAYER)
			{
				if(m_playerObject.size() == 0|| (unsigned int)m_userPointer.at(p_objectIndex)->m_vectorIndex > m_playerObject.size()-1 || m_userPointer.at(p_objectIndex)->m_vectorIndex < 0)
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
		if(m_userPointer.at(p_objectIndex)->m_type == TYPE_PLAYER)
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

	

}
}

RootEngine::Physics::PhysicsInterface* CreatePhysics( RootEngine::SubsystemSharedContext p_context, Render::RendererInterface* p_renderer , RootEngine::ResourceManagerInterface* p_resourceManager)
{
	RootEngine::Physics::g_context = p_context;
	RootEngine::Physics::g_renderer = p_renderer;
	RootEngine::Physics::g_resourceManager = p_resourceManager;
	return RootEngine::Physics::RootPhysics::GetInstance();
}
