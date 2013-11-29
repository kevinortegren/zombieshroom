#include "RootPhysics.h"
#include <stdio.h>
#include <iostream>
#include <Bullet/BulletCollision/CollisionShapes/btShapeHull.h>
#include <RootEngine/Include/Logging/Logging.h>


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
				delete body->getCollisionShape();
			}
			m_dynamicWorld->removeCollisionObject( obj );
			delete obj;
		}
		for(unsigned int i = 0; i < m_playerObject.size(); i++)
		{
			delete m_playerObject[i];
		}
		for(unsigned int i = 0; i < m_dynamicObjects.size(); i++)
		{
			delete m_dynamicObjects[i];
		}
		m_dynamicObjects.clear();
		m_playerObject.clear();
		delete m_dynamicWorld;
		delete m_solver;
		delete m_collisionConfig;
		delete m_broadphase;
		delete m_dispatcher;
	}
	/*
	bool hti
	void* object;
	id
	bool isAbility
	int id, id=0=Terrain, id1= ability id2 = player
	*/
	///Must be global, used to check custom collision events, NOTE : Don't ever ever use this yourself!
	bool CallbackFunc(btManifoldPoint& p_cp,const btCollisionObjectWrapper * p_obj1 , int p_id1, int p_index1, const btCollisionObjectWrapper * p_obj2 , int p_id2, int p_index2 )
	{
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
		gContactAddedCallback = &CallbackFunc;
		g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::DEBUG_PRINT, "Physics subsystem loaded");
		m_debugDrawer = new DebugDrawer();
		m_debugDrawer->setDebugMode(m_debugDrawer->getDebugMode() | btIDebugDraw::DBG_DrawAabb | btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawContactPoints);
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
		//g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::DEBUG_PRINT, "Clearing debug vectors");
		
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
	//Done
	int RootPhysics::AddDynamicObjectToWorld( int p_numTriangles, int* p_indexBuffer, int p_indexStride, int p_numVertices, float* p_vertexBuffer, int p_vertexStride, float* p_position, float* p_rotation , float p_mass )
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
		
		btCompoundShape* compShape = new btCompoundShape(true);
		btTransform localTransform;
		localTransform.setIdentity();
		localTransform.setRotation(btQuaternion(p_rotation[0],p_rotation[1], p_rotation[2],1));
		localTransform.setOrigin(btVector3(p_position[0],p_position[1],p_position[2]));

		compShape->addChildShape(localTransform, simplifiedObject);

		btVector3 fallInertia =  btVector3(0,0,0);
		compShape->calculateLocalInertia(p_mass,fallInertia);

		btTransform massTrans;
		massTrans.setIdentity();
		massTrans.setRotation(btQuaternion(p_rotation[0],p_rotation[1], p_rotation[2],1));
		massTrans.setOrigin(btVector3(p_position[0],p_position[1]-1,p_position[2]));

		localTransform.inverse();
		massTrans.inverse();

		btMotionState* state = new btDefaultMotionState( massTrans, localTransform);

		//create a body
		btRigidBody::btRigidBodyConstructionInfo objectBodyInfo(p_mass, state,simplifiedObject, fallInertia );
		//btRigidBody::btRigidBodyConstructionInfo objectBodyInfo(p_mass, motionstate,objectMeshShape, fallInertia );
		btRigidBody* objectBody = new btRigidBody(objectBodyInfo);
		objectBody->setActivationState(DISABLE_DEACTIVATION);

		//add the body to the world,  TODO : We should also set a user defined gravity for the object
		m_dynamicWorld->addRigidBody(objectBody);

		//add to the dynamic object vector
		m_dynamicObjects.push_back(objectBody);


/*
		//Set Inertia
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
		//btRigidBody::btRigidBodyConstructionInfo objectBodyInfo(p_mass, motionstate,objectMeshShape, fallInertia );
		btRigidBody* objectBody = new btRigidBody(objectBodyInfo);
		objectBody->setActivationState(DISABLE_DEACTIVATION);
		
		//add the body to the world,  TODO : We should also set a user defined gravity for the object
		m_dynamicWorld->addRigidBody(objectBody);

		//add to the dynamic object vector
		m_dynamicObjects.push_back(objectBody);

		//The user pointer should be something we can use to detect which ability a player collides with
		/*PhysicWorldInfo tempInfo;
		tempInfo.m_collidedType = -1;
		tempInfo.m_hasHit = false;
		tempInfo.m_index = m_userPointer.size();
		tempInfo.m_type = ABILITY;
		tempInfo.m_position[0] = p_position[0];
		tempInfo.m_position[1] = p_position[1];
		tempInfo.m_position[2] = p_position[2];
		CustomUserPointer* temp = new CustomUserPointer();
		temp->m_object = objectBody;
		temp->m_worldInfo = tempInfo;
		temp->m_vectorIndex = m_dynamicObjects.size()-1;
		objectBody->setUserPointer(temp);
		m_userPointer.push_back(temp);*/
		delete indexVertexArray;
		delete objectMeshShape;
		delete objectHull;

		return m_dynamicObjects.size()-1;
		//return index of body to caller as a reference 
		//return m_dynamicObjects.size()-1;
	}

	void RootPhysics::SetDynamicObjectVelocity( int p_objectIndex, float* p_velocity )
	{
		if(m_dynamicObjects.size() == 0 || (unsigned int)p_objectIndex > m_dynamicObjects.size()-1)
		{
			g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::WARNING, "Attemting to access non existing object at index %d", p_objectIndex);
			return;
		}
		m_dynamicObjects.at(p_objectIndex)->setLinearVelocity(btVector3(p_velocity[0], p_velocity[1], p_velocity[2]));
	}

	void RootPhysics::SetObjectMass( int p_objectIndex, float p_mass )
	{
		if(m_dynamicObjects.size() == 0 || (unsigned int)p_objectIndex > m_dynamicObjects.size()-1)
		{
			g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::WARNING, "Attemting to access non existing object at index %d", p_objectIndex);
			return;
		}
		btVector3 fallInertia =  btVector3(0,0,0);
		m_dynamicObjects.at(p_objectIndex)->getCollisionShape()->calculateLocalInertia(p_mass, fallInertia);
		m_dynamicObjects.at(p_objectIndex)->setMassProps(p_mass, fallInertia);
	}

	int RootPhysics::AddPlayerObjectToWorld( int p_numTriangles, int* p_indexBuffer, int p_indexStride, int p_numVertices, float* p_vertexBuffer, 
											int p_vertexStride, float* p_position, float* p_rotation, float p_mass, float p_maxSpeed, float p_modelHeight, float p_stepHeight )
	{
		PlayerController* player = new PlayerController();
		player->Init(m_dynamicWorld, p_numTriangles, p_indexBuffer, p_indexStride, p_numVertices, p_vertexBuffer
			, p_vertexStride, p_position, p_rotation, p_mass, p_maxSpeed, p_modelHeight, p_stepHeight );

		m_playerObject.push_back(player);
		return m_playerObject.size() -1;
	}

	void RootPhysics::PlayerMoveXZ( int p_objectIndex, float* p_direction )
	{
		if(m_playerObject.size() == 0)
		{
			g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::WARNING, "Attemting to access non existing playerobject at index %d", p_objectIndex);
			return;
		}
		m_playerObject.at(p_objectIndex)->Walk(p_direction);
	}

	void RootPhysics::PlayerJump( int p_objectIndex, float p_jumpForce )
	{
		if(m_playerObject.size() == 0)
		{
			g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::WARNING, "Attemting to access non existing playerobject at index %d", p_objectIndex);
			return;
		}
		m_playerObject.at(p_objectIndex)->Jump(p_jumpForce);
	}

	void RootPhysics::GetPlayerPos( int p_objectIndex, float* p_playerPos )
	{
		if(m_playerObject.size() == 0)
		{
			g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::WARNING, "Attemting to access non existing playerobject at index %d", p_objectIndex);
			return;
		}
		btVector3 temp = m_playerObject.at(p_objectIndex)->GetPosition();
		p_playerPos[0] = temp.getX();
		p_playerPos[1] = temp.getY();
		p_playerPos[2] = temp.getZ();


	}
	void RootPhysics::GetObjectPos(int p_objectIndex, float* p_objectPos)
	{
		if(m_dynamicObjects.size() == 0 || (unsigned int)p_objectIndex > m_dynamicObjects.size()-1)
		{
			g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::WARNING, "Attemting to access non existing object at index %d", p_objectIndex);
			return;
		}
		btVector3 temp = m_dynamicObjects.at(p_objectIndex)->getWorldTransform().getOrigin();

		p_objectPos[0] = temp.getX();
		p_objectPos[1] = temp.getY();
		p_objectPos[2] = temp.getZ();
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
		if(m_playerObject.size() == 0)
		{
			g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::WARNING, "Attemting to access non existing playerobject at index %d", p_objectIndex);
			return;
		}
 		btVector3 temp = btVector3(p_pushDirection[0], p_pushDirection[1], p_pushDirection[2]);
		temp.normalize();
		//This might be so incredibly broken that i don't even how the compiler lets us do it
 		m_playerObject.at(p_objectIndex)->Knockback(temp * p_pushForce);
	}

	void RootPhysics::RemoveObject( int p_objectIndex, int p_type )
	{
		//if(p_type == PLAYER)
		//{
		//	//m_playerObject.at(p_objectIndex)->
		//}
		//if(p_type == ABILITY)
		//{
		//	CustomUserPointer* temp = m_userPointer.at(p_objectIndex);
		//	delete [] temp->m_worldInfo.m_position;
		//	btRigidBody* body = (btRigidBody*)temp->m_object;
		//	delete body->getMotionState();
		//	delete body->getCollisionShape();
		//	m_dynamicWorld->removeCollisionObject(body);
		//	delete body;	
		//	delete m_dynamicObjects[temp->m_vectorIndex];
		//	
		//}
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
		return m_dynamicObjects.size()-1;
	}

	void RootPhysics::GetObjectOrientation( int p_objectIndex, float* p_objectOrientation )
	{
		if(m_dynamicObjects.size() == 0 || (unsigned int)p_objectIndex > m_dynamicObjects.size()-1)
		{
			g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::WARNING, "Attemting to access non existing object at index %d", p_objectIndex);
			return;
		}
		btRigidBody* body = m_dynamicObjects.at(p_objectIndex);
		btTransform transform;
		transform = body->getWorldTransform();
		
		p_objectOrientation[0] = transform.getRotation().w();
		p_objectOrientation[1] = transform.getRotation().x();
		p_objectOrientation[2] = transform.getRotation().y();
		p_objectOrientation[3] = transform.getRotation().z();
			/*p_objectOrientation[0] = body->getOrientation().x();
			p_objectOrientation[1] = body->getOrientation().y();
			p_objectOrientation[2] = body->getOrientation().z();
			p_objectOrientation[3] = body->getOrientation().w();*/
	}

	void RootPhysics::SetObjectOrientation( int p_objectIndex, float* p_objectOrientation )
	{
		if(m_dynamicObjects.size() == 0 || (unsigned int)p_objectIndex > m_dynamicObjects.size()-1)
		{
			g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::WARNING, "Attemting to access non existing object at index %d", p_objectIndex);
			return;
		}
		btRigidBody* body = m_dynamicObjects.at(p_objectIndex);
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
		if(m_playerObject.size() == 0)
		{
			g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::WARNING, "Attemting to access non existing playerobject at index %d", p_objectIndex);
			return;
		}
		m_playerObject.at(p_objectIndex)->SetOrientation(p_playerOrientation);
		//g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::DEBUG_PRINT, "ROTATTIONN : %f %f %f %f",p_playerOrientation[0] ,p_playerOrientation[1], p_playerOrientation[2], p_playerOrientation[3]);
	}

	

}

Physics::PhysicsInterface* CreatePhysics( RootEngine::SubsystemSharedContext p_context, Render::RendererInterface* p_renderer , RootEngine::ResourceManagerInterface* p_resourceManager)
{
	Physics::g_context = p_context;
	Physics::g_renderer = p_renderer;
	Physics::g_resourceManager = p_resourceManager;
	return Physics::RootPhysics::GetInstance();
}
