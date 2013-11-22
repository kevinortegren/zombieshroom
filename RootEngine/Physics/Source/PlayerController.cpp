#include "PlayerController.h"
#include "Bullet/BulletCollision/CollisionShapes/btShapeHull.h"
PlayerController::PlayerController( void )
{

}

PlayerController::~PlayerController( void )
{
	m_dynamicWorld->removeRigidBody(m_rigidBody);
	m_dynamicWorld->removeCollisionObject(m_ghostObject);
	delete m_rigidBody;
	delete m_ghostObject;
}

void PlayerController::Init( btDiscreteDynamicsWorld* p_world,int p_numTriangles, int* p_indexBuffer, int p_indexStride, int p_numVertices, 
							float* p_vertexBuffer, int p_vertexStride, float* p_position, float* p_rotation, float p_mass, float m_maxSpeed  )
{
	m_dynamicWorld = p_world;

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
	 m_motionState = new btDefaultMotionState(startTransform);

	//create a body
	btRigidBody::btRigidBodyConstructionInfo objectBodyInfo(p_mass, m_motionState,simplifiedObject, fallInertia );
	objectBodyInfo.m_friction = 0.0f;
	objectBodyInfo.m_restitution = 0.0f;
	objectBodyInfo.m_linearDamping = 0.0f;

	m_rigidBody = new btRigidBody(objectBodyInfo);
	//Keep upright
	m_rigidBody->setAngularFactor(0.0f);
	//Don't enter sleep mode if you stand still for a while
	m_rigidBody->setActivationState(DISABLE_DEACTIVATION);

	m_dynamicWorld->addRigidBody(m_rigidBody);

	m_ghostObject = new btPairCachingGhostObject();
	m_ghostObject->setCollisionShape(simplifiedObject);
	m_ghostObject->setUserPointer(this);
	m_ghostObject->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);

	m_dynamicWorld->addCollisionObject(m_ghostObject, btBroadphaseProxy::KinematicFilter,  btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);

	delete indexVertexArray;
	delete objectMeshShape;
	delete objectHull;
}


void PlayerController::Walk(float* p_dir)
{
	float x = p_dir[0] + m_manualVelocity.x();
	float z = p_dir[2] + m_manualVelocity.z();
	float speed = btSqrt(x*x + z*z);

	if(speed > m_maxSpeed)
	{
		x = (x/speed) * m_maxSpeed;
		z = (z/speed) * m_maxSpeed;
	}
	m_manualVelocity.setX(x);
	m_manualVelocity.setZ(z);

}

void PlayerController::Update()
{
	m_ghostObject->setWorldTransform(m_rigidBody->getWorldTransform());

	m_motionState->getWorldTransform(m_motionTransform);

	m_onGround = false;

	ParseGhostContacts();

	UpdatePosition();
	UpdateVelocity();

}

void PlayerController::ParseGhostContacts()
{

}

void PlayerController::UpdatePosition()
{
	
}

void PlayerController::UpdateVelocity()
{
	m_manualVelocity.setY(m_rigidBody->getLinearVelocity().getY());
	m_rigidBody->setLinearVelocity(m_manualVelocity);

	if(m_hittingWall)
	{
		for(unsigned int i = 0, size = m_surfaceHitNormals.size(); i < size; i++)
		{
			//Projects m_manualVelocity onto m_surfaceHitNormals
			btVector3 temp = (m_manualVelocity.dot(m_surfaceHitNormals.at(i))/(m_surfaceHitNormals.at(i).norm() *m_surfaceHitNormals.at(i).norm()) ) * m_surfaceHitNormals.at(i);	
			m_manualVelocity -= temp * 1.05f; //MAGIC NUMBERS ARE C00L
		}

	}
}

void PlayerController::Jump( float p_jumpforce )
{
	if(m_onGround)
	{
		m_rigidBody->applyCentralImpulse(btVector3(0, p_jumpforce, 0));

		const float jumpYOffset = 0.01f;
		float previusY = m_rigidBody->getWorldTransform().getOrigin().getY();

		m_rigidBody->getWorldTransform().getOrigin().setY(previusY + jumpYOffset);
	}
}






