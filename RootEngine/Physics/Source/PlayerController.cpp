#include "PlayerController.h"
#include "Bullet/BulletCollision/CollisionShapes/btShapeHull.h"


struct IgnoreBodyAndGhostCast : public btCollisionWorld::ClosestRayResultCallback
{
	btRigidBody* Body;
	btPairCachingGhostObject* Ghost;

	IgnoreBodyAndGhostCast(btRigidBody* p_body, btPairCachingGhostObject* p_ghostObject)
		:btCollisionWorld::ClosestRayResultCallback(btVector3(0.0,0.0,0.0), btVector3(0.0,0.0,0.0))
	{
		Body = p_body;
		Ghost = p_ghostObject;	
	}
	//Ignore colliding with self
	btScalar addSingleResult(btCollisionWorld::LocalRayResult& p_rayResult, bool p_normalInWorldSpace)
	{
		//Don't collide with ourself
		if(p_rayResult.m_collisionObject == Body || p_rayResult.m_collisionObject == Ghost)
			return 1.0f;
		//Do this to avoid getting stuck in non collide object, such as aoe abilitys
		if((p_rayResult.m_collisionObject->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE) == btCollisionObject::CF_NO_CONTACT_RESPONSE)
			return 1.0f;
		
		return btCollisionWorld::ClosestRayResultCallback::addSingleResult(p_rayResult, p_normalInWorldSpace);
	}

};

PlayerController::PlayerController( void )
{
	
}

PlayerController::~PlayerController( void )
{

}
void PlayerController::RemovePlayer()
{
	//Note fix this
	//delete m_rigidBody;
	//delete m_ghostObject;
	m_dynamicWorld->removeRigidBody(m_rigidBody);
	m_dynamicWorld->removeCollisionObject(m_ghostObject);
}

void PlayerController::Init( btDiscreteDynamicsWorld* p_world,int p_numTriangles, int* p_indexBuffer, int p_indexStride, int p_numVertices, 
							float* p_vertexBuffer, int p_vertexStride, float* p_position, float* p_rotation, float p_mass, float p_maxSpeed, float p_modelHeight, float p_stepHeight )
{
	m_dynamicWorld = p_world;
	m_heightOffset = p_modelHeight / 2.0f;
	m_stepHeight = p_stepHeight;
	m_maxSpeed = p_maxSpeed;
	m_previousPosition = btVector3(p_position[0],p_position[1],p_position[2]);
	m_manualVelocity = btVector3(0.0f,0.0f,0.0f);
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
	//btCollisionShape* simplifiedObject = new btCapsuleShape(0.5, 2);
	
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
	m_rigidBody->setCollisionFlags(m_rigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK |btCollisionObject::CF_CHARACTER_OBJECT /*| btCollisionObject::CF_NO_CONTACT_RESPONSE */);
	m_dynamicWorld->addRigidBody(m_rigidBody);

	m_ghostObject = new btPairCachingGhostObject();
	m_ghostObject->setCollisionShape(simplifiedObject);
	//m_ghostObject->setUserPointer(this);
	m_ghostObject->setActivationState(DISABLE_DEACTIVATION);
	m_ghostObject->setCollisionFlags(m_ghostObject->getCollisionFlags() |btCollisionObject::CF_NO_CONTACT_RESPONSE);
	m_dynamicWorld->addCollisionObject(m_ghostObject, btBroadphaseProxy::KinematicFilter, /* btBroadphaseProxy::StaticFilter |*/ btBroadphaseProxy::DefaultFilter);
	//indexVertexArray = 0;
	//objectMeshShape = 0;
	//objectHull = 0;
	//delete indexVertexArray;

	//delete objectMeshShape;
	//delete objectHull;
	
	
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
	btManifoldArray manifoldArray;
	btBroadphasePairArray& pairArray = m_ghostObject->getOverlappingPairCache()->getOverlappingPairArray();
	int numPairs = pairArray.size();
	m_hittingWall = false,
	m_surfaceHitNormals.clear();

	for(int i = 0; i < numPairs ; i++ )
	{
		manifoldArray.clear();
		const btBroadphasePair& pair = pairArray[i];
		btBroadphasePair* collisionPair = m_dynamicWorld->getPairCache()->findPair(pair.m_pProxy0, pair.m_pProxy1);

		if(collisionPair == NULL)
			continue;

		if(collisionPair->m_algorithm != NULL)
			collisionPair->m_algorithm->getAllContactManifolds(manifoldArray);

		for (int j = 0; j < manifoldArray.size(); j++)
		{
			btPersistentManifold* pManifold = manifoldArray[j];

			//Skip the rigid body that the ghost monitors
			if(pManifold->getBody0() == m_rigidBody)
				continue;
			if(pManifold->getBody0() == m_ghostObject)
				if((pManifold->getBody1()->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE) == btCollisionObject::CF_NO_CONTACT_RESPONSE)
					continue;
			if(pManifold->getBody1() == m_ghostObject)
				if((pManifold->getBody0()->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE) == btCollisionObject::CF_NO_CONTACT_RESPONSE)
					continue;
			
			for(int k = 0; k < pManifold->getNumContacts(); k++)
			{
				const btManifoldPoint& point = pManifold->getContactPoint(k);
				

				if(point.getDistance() < 0.0f)
				{
					const btVector3& ptB = point.getPositionWorldOnB();
					
					//Check if on ground
					if(ptB.getY() < m_motionTransform.getOrigin().getY() - m_heightOffset /*Offset to get under ground*/ - 0.01f)
					{
						m_onGround = true;
					}
					else
					{
						m_hittingWall = true;
						m_surfaceHitNormals.push_back(point.m_normalWorldOnB); //Was const before point stuff
					}
				}
			}
		}
	}
}

void PlayerController::UpdatePosition()
{
	IgnoreBodyAndGhostCast rayCallBack_bottom( m_rigidBody, m_ghostObject);
	//Check if we hit the ground
	//If this is wrong we will need  to find a offset value for the hull shape
	m_dynamicWorld->rayTest(m_rigidBody->getWorldTransform().getOrigin(), 
		m_rigidBody->getWorldTransform().getOrigin() - btVector3(0.0f , m_heightOffset + m_stepHeight ,0.0f), rayCallBack_bottom);
	//If we hit something below us
	if(rayCallBack_bottom.hasHit())
	{
		float previousY = m_rigidBody->getWorldTransform().getOrigin().getY();
		m_rigidBody->getWorldTransform().getOrigin().setY(previousY + (m_heightOffset + m_stepHeight ) * (1.0f - rayCallBack_bottom.m_closestHitFraction));

		btVector3 vel(m_rigidBody->getLinearVelocity());
		vel.setY(0.0f);

		m_rigidBody->setLinearVelocity(vel);
		m_onGround = true;
// 		if(rayCallBack_bottom.m_hitNormalWorld.angle(btVector3(0,1,0)) > 0.1)
// 		{
// 			m_rigidBody->setLinearVelocity(btVector3(0,0,0));
// 		}
	}

	//check if we hit something above us
	IgnoreBodyAndGhostCast rayCallBack_top(m_rigidBody, m_ghostObject);
	m_dynamicWorld->rayTest(m_rigidBody->getWorldTransform().getOrigin(), 
		m_rigidBody->getWorldTransform().getOrigin() + btVector3(0.0f , m_heightOffset + 0.5f, 0.0f), rayCallBack_top); //vad är 2:an?? - Jo min herre min herre, den är en temporär lösning då 0 punkten är i fötterna och bör tas bort då denna punkten hamnar i mitten av karaktären
	if(rayCallBack_top.hasHit())
	{
		m_rigidBody->getWorldTransform().setOrigin(m_previousPosition);
		btVector3 vel(m_rigidBody->getLinearVelocity());
		vel.setY(0.0f);
		m_rigidBody->setLinearVelocity(vel);
	}
	m_previousPosition = m_rigidBody->getWorldTransform().getOrigin();

}

void PlayerController::UpdateVelocity()
{
	m_manualVelocity.setY(m_rigidBody->getLinearVelocity().getY());
	m_rigidBody->setLinearVelocity(m_manualVelocity);

	//Deaccelerate
	if(m_onGround)
	{
		m_manualVelocity -= m_manualVelocity * 0.5f; // The constant will need to be fine tuned later and should be a define or constant
					
	}
	if(m_hittingWall)
	{
		
		for(unsigned int i = 0, size = m_surfaceHitNormals.size(); i < size; i++)
		{
			//Projects m_manualVelocity onto m_surfaceHitNormals
			btVector3 temp = (m_manualVelocity.dot(m_surfaceHitNormals.at(i))/(m_surfaceHitNormals.at(i).norm() * m_surfaceHitNormals.at(i).norm()) )*(m_surfaceHitNormals.at(i));
			temp.normalize();
			m_manualVelocity -= temp * 1.05f; //MAGIC NUMBERS ARE C00l
			m_manualVelocity.setY(0);
			m_rigidBody->setLinearVelocity(m_manualVelocity);
		}

		//(manul x S/|S|^2) * S
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

void PlayerController::Knockback(float* p_velocity )
{
	m_rigidBody->applyCentralImpulse(btVector3(p_velocity[0], p_velocity[1], p_velocity[2]));
	m_manualVelocity += btVector3(p_velocity[0], p_velocity[1], p_velocity[2]);
}

void PlayerController::SetOrientation( float* p_orientation )
{

	float x,y,z, w;
	x = p_orientation[0];
	y = p_orientation[1];
	z = p_orientation[2];
	w = p_orientation[3];
	
	btTransform transform;
	transform.setIdentity();
	transform.setRotation(btQuaternion(x,y,z, w));
	transform.setOrigin(m_rigidBody->getWorldTransform().getOrigin());
	m_rigidBody->setWorldTransform(transform);

}

void PlayerController::SetUserPointer( void* p_userPointer )
{
	m_rigidBody->setUserPointer(p_userPointer);
}

void PlayerController::SetPrevVelocity()
{
	m_rigidBody->setLinearVelocity(m_manualVelocity);
}








