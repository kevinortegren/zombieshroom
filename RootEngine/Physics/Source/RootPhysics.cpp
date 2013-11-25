#include "RootPhysics.h"
#include <stdio.h>
#include <iostream>
#include "Bullet/BulletCollision/CollisionShapes/btShapeHull.h"


using namespace Physics;

RootPhysics::RootPhysics()
{

}

RootPhysics::~RootPhysics()
{

	
}
void RootPhysics::Startup()
{

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
	delete m_dynamicWorld;
	delete m_solver;
	delete m_collisionConfig;
	delete m_broadphase;
	delete m_dispatcher;
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
	
}


//Creates a impassable plane
void RootPhysics::CreatePlane(float* p_normal, float* p_position)
{
	btCollisionShape* plane = new btStaticPlaneShape(btVector3(p_normal[0],p_normal[1],p_normal[2]), 0);
	btDefaultMotionState* planeMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(p_position[0],p_position[1],p_position[2])));
	btRigidBody::btRigidBodyConstructionInfo planeRigidbodyCI(0, planeMotionState, plane, btVector3(0, 0, 0));
	btRigidBody* planeBody = new btRigidBody(planeRigidbodyCI);
	m_dynamicWorld->addRigidBody(planeBody);
}

//Make a real update
void RootPhysics::Update()
{
//	for (int i=0 ; i<300 ; i++) 
	{
		for(unsigned int i = 0; i < m_playerObject.size(); i++)
		{
			m_playerObject.at(i)->Update();
		}
		m_dynamicWorld->stepSimulation(1/60.f,10);
		//btTransform trans;
		//trans.setOrigin(m_playerObject.at(0)->GetPosition());


	//	std::cout << "bunny height: " << trans.getOrigin().getY() << " bunny x: " << trans.getOrigin().getX() << " bunny z: " << trans.getOrigin().getZ() << std::endl;
		
	}
}
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
	btTriangleIndexVertexArray* indexVertexArray = new btTriangleIndexVertexArray(p_numTriangles, p_indexBuffer, p_indexStride, p_numVertices , (btScalar*) p_vertexBuffer, p_vertexStride);
	btConvexShape* objectMeshShape = new btConvexTriangleMeshShape(indexVertexArray);
	//Cull unneccesary vertices to improve performance 
	btShapeHull* objectHull = new btShapeHull(objectMeshShape);
	btScalar margin = objectMeshShape->getMargin();
	objectHull->buildHull(margin);
	btConvexHullShape* simplifiedObject = new btConvexHullShape();
	for(int i = 0; i < objectHull->numVertices(); i++)
	{
		simplifiedObject->addPoint(objectHull->getVertexPointer()[i], false);
	}
	simplifiedObject->recalcLocalAabb();
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
	btRigidBody* objectBody = new btRigidBody(objectBodyInfo);

	//add the body to the world
	m_dynamicWorld->addRigidBody(objectBody);

	//add to the dynamic object vector
	m_dynamicObjects.push_back(objectBody);

	delete indexVertexArray;
	delete objectMeshShape;
	delete objectHull;

	//return index of body to caller as a reference 
	return m_dynamicObjects.size()-1;
}

void RootPhysics::SetDynamicObjectVelocity( int p_objectIndex, float* p_velocity )
{
	m_dynamicObjects.at(p_objectIndex)->setLinearVelocity(btVector3(p_velocity[0], p_velocity[1], p_velocity[2]));
}

void RootPhysics::SetObjectMass( int p_objectIndex, float p_mass )
{
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
	m_playerObject.at(p_objectIndex)->Walk(p_direction);
}

void RootPhysics::PlayerJump( int p_objectIndex, float p_jumpForce )
{
	m_playerObject.at(p_objectIndex)->Jump(p_jumpForce);
}

void RootPhysics::GetPlayerPos( int p_objectIndex, float* p_playerPos )
{
	btVector3 temp = m_playerObject.at(p_objectIndex)->GetPosition();
	p_playerPos[0] = temp.getX();
	p_playerPos[1] = temp.getY();
	p_playerPos[2] = temp.getZ();


}
void RootPhysics::GetObjectPos(int p_objectIndex, float* p_objectPos)
{
	btVector3 temp = m_dynamicObjects.at(p_objectIndex)->getWorldTransform().getOrigin();

	p_objectPos[0] = temp.getX();
	p_objectPos[1] = temp.getY();
	p_objectPos[2] = temp.getZ();
}