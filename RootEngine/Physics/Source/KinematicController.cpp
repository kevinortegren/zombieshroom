#include "KinematicController.h"
#include "Bullet/BulletCollision/CollisionShapes/btShapeHull.h"


KinematicController::KinematicController( void )
{

}

KinematicController::~KinematicController( void )
{
	delete m_kinController;
}
void KinematicController::RemovePlayer()
{
	m_dynamicWorld->removeCollisionObject(m_ghostObject);
	delete m_motionState;
	delete m_ghostObject->getCollisionShape();
	delete m_ghostObject;
	
}

void KinematicController::Init( btDiscreteDynamicsWorld* p_world,int p_numTriangles, int* p_indexBuffer, int p_indexStride, int p_numVertices, 
							float* p_vertexBuffer, int p_vertexStride, glm::vec3 p_position, glm::quat p_rotation, float p_mass, float p_maxSpeed, float p_modelHeight, float p_stepHeight )
{
	m_dynamicWorld = p_world;
	m_heightOffset = p_modelHeight / 2.0f;
	m_stepHeight = p_stepHeight;
	m_maxSpeed = p_maxSpeed;
	m_mass = p_mass;

	//Shape
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
	//End shape

	//Set Inertia
	btVector3 fallInertia =  btVector3(0,0,0);
	simplifiedObject->calculateLocalInertia(p_mass,fallInertia);

	//Set startpos and start rotation and bind them to a motionstate
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(p_position[0],p_position[1],p_position[2]));
	startTransform.setRotation(btQuaternion(p_rotation[0],p_rotation[1], p_rotation[2],p_rotation[3]));
	m_motionState = new btDefaultMotionState(startTransform);

	
	m_ghostObject = new btPairCachingGhostObject();
	m_ghostObject->setCollisionShape(simplifiedObject);
	m_ghostObject->setActivationState(DISABLE_DEACTIVATION);
	m_ghostObject->setCollisionFlags( btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK /* |btCollisionObject::CF_KINEMATIC_OBJECT*/ /*|btCollisionObject::CF_NO_CONTACT_RESPONSE*/ );

	m_kinController = new BulletCharacter(m_ghostObject, simplifiedObject, p_stepHeight);
	
	m_kinController->setGravity(9.82f * 6);
	m_kinController->setJumpSpeed(5);
	m_kinController->setMaxSlope(btRadians(54));
	
	m_hasBeenKnockbacked = false;
	
	m_dynamicWorld->addCollisionObject(m_ghostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
	

}


void KinematicController::Walk(glm::vec3 p_dir, float p_dt)
{
	btVector3 temp = btVector3(p_dir[0], p_dir[1], p_dir[2]);
	temp.normalize();
	m_kinController->setVelocityForTimeInterval(temp*m_maxSpeed, p_dt);
	if(!m_kinController->IsKnockbacked())// && !m_hasStepped)
	{
		m_kinController->playerStep(m_dynamicWorld, p_dt);
		//m_hasStepped = true;
	}
	

}

void KinematicController::Move( glm::vec3 p_target, float p_dt )
{
	btVector3 from,to,traveldist;
	from = m_ghostObject->getWorldTransform().getOrigin();
	to = btVector3(p_target[0], p_target[1], p_target[2]);
	traveldist = to- from;
	//m_kinController->warp(to);
	if(traveldist.length() > 1 )
	{
		m_kinController->warp(to);
		return;
	}
	m_kinController->setVelocityForTimeInterval(traveldist/p_dt, p_dt);
	m_kinController->playerStep(m_dynamicWorld, p_dt);
}

void KinematicController::Update(float p_dt)
{
	
	//m_hasStepped = false;
	m_kinController->updateAction(m_dynamicWorld, p_dt);
	m_kinController->setWalkDirection(btVector3(0,0,0));

	
	
}

void KinematicController::Jump()
{
	if(m_kinController->canJump())
		m_kinController->jump();
}

void KinematicController::Knockback(const btVector3& p_velocity, float p_power )
{

	m_kinController->Knockback(p_velocity, p_power);
	
}

void KinematicController::SetOrientation( glm::quat p_orientation )
{
	btQuaternion btquat;
	btquat.setW(p_orientation.w);
	btquat.setX(p_orientation.x);
	btquat.setY(p_orientation.y);
	btquat.setZ(p_orientation.z);

	m_kinController->getGhostObject()->getWorldTransform().setRotation(btquat);
}

void KinematicController::SetUserPointer( void* p_userPointer )
{
	m_ghostObject->setUserPointer(p_userPointer);
}

void KinematicController::SetMass( float p_mass )
{
	m_mass = p_mass;
}

void KinematicController::SetGravity( float p_gravity )
{
	m_kinController->setGravity(p_gravity);
}

void KinematicController::SetPosition(const btVector3& p_position)
{
	m_kinController->warp(p_position);
}
