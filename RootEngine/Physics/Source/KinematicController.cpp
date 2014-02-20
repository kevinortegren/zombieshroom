#include "KinematicController.h"
#include "Bullet/BulletCollision/CollisionShapes/btShapeHull.h"

namespace RootEngine
{

	namespace Physics
	{
		extern RootEngine::SubsystemSharedContext g_context;

	}
}
KinematicController::KinematicController( void )
{

}

KinematicController::~KinematicController( void )
{
	delete m_kinController;
}
void KinematicController::RemovePlayer()
{
	if(m_activated)
		m_dynamicWorld->removeCollisionObject(m_ghostObject);
	delete m_motionState;
	delete m_ghostObject->getCollisionShape();
	delete m_ghostObject;
}

void KinematicController::Init( btDiscreteDynamicsWorld* p_world,int p_numTriangles, int* p_indexBuffer, int p_indexStride, int p_numVertices, 
							float* p_vertexBuffer, int p_vertexStride, glm::vec3 p_position, glm::quat p_rotation, float p_mass, float p_maxSpeed, float p_modelHeight, float p_stepHeight )
{
	m_dynamicWorld = p_world;
	m_heightOffset = p_modelHeight / 2.0f; //cleanup
	m_stepHeight = p_stepHeight;
	m_maxSpeed = p_maxSpeed; //cleanup
	m_mass = p_mass;
	m_activated = true;
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
		simplifiedObject->addPoint(objectHull->getVertexPointer()[i]);
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

	btCapsuleShape* capsuleShape = new btCapsuleShape(0.8f, 2.5f);
	//btSphereShape* capsuleShape = new btSphereShape(2);

	capsuleShape->calculateLocalInertia(p_mass, fallInertia);
	
	
	m_ghostObject = new btPairCachingGhostObject();
	//capsuleShape->setMargin(0.04f);
	m_ghostObject->setCollisionShape(capsuleShape);
	
	m_ghostObject->setContactProcessingThreshold(0.f);
	
	m_ghostObject->setActivationState(DISABLE_DEACTIVATION);
	m_ghostObject->setCollisionFlags(btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK  |btCollisionObject::CF_CHARACTER_OBJECT/* |btCollisionObject::CF_NO_CONTACT_RESPONSE*/ );

	m_kinController = new BulletCharacter(m_ghostObject, capsuleShape, p_stepHeight);
	
	m_kinController->setGravity(9.82f * 3.0f);
	m_kinController->setJumpSpeed(20.0f);
	m_kinController->setFallSpeed(200.0f);
	//m_kinController->setMaxJumpHeight(0.001f); //Does not seem to do anything

	m_kinController->setMaxSlope(btRadians(45.0f));
	
	m_hasBeenKnockbacked = false;
	m_dynamicWorld->addCollisionObject(m_ghostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::AllFilter );
	//m_dynamicWorld->addAction(m_kinController);

}


void KinematicController::Walk(glm::vec3 p_dir, float p_dt)
{
	if(!m_activated)
		return;
	btVector3 temp = btVector3(p_dir[0], p_dir[1], p_dir[2]);
	if (!temp.fuzzyZero())
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
	if(!m_activated)
		return;
	if(!m_hasBeenKnockbacked) //Don't allow movement when a player is knockbacked
	{
		btVector3 from,to,traveldist;
		from = m_ghostObject->getWorldTransform().getOrigin();
		to = btVector3(p_target[0], p_target[1], p_target[2]);
	
		traveldist = to - from;
		//RootEngine::Physics::g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::DEBUG_PRINT, "Distance: %f. Delta movement: %f, %f, %f", traveldist.length(), traveldist.x(), traveldist.y(), traveldist.z());
		/*if(traveldist.length() > 100 )
		{
			m_kinController->warp(to);
			return;
		}*/
		//float temp = m_kinController->test(from, to, m_dynamicWorld);
		//m_kinController->warp(to + traveldist * temp);
		m_kinController->setVelocityForTimeInterval(traveldist, p_dt);
		//m_kinController->playerStep(m_dynamicWorld, p_dt);
	}
}

void KinematicController::Update(float p_dt)
{
	if(!m_activated)
		return;
	//m_hasStepped = false;
	m_kinController->updateAction(m_dynamicWorld, p_dt);
	m_kinController->setWalkDirection(btVector3(0,0,0));
	
	
	
}

void KinematicController::Jump()
{
	if(m_kinController->canJump())
	{
		m_kinController->StopKnockback();
		m_kinController->jump();
	}
}

void KinematicController::JumpBoost( float p_boostPower )
{
	m_kinController->JumpBoost(p_boostPower);
}

void KinematicController::Knockback(const btVector3& p_velocity, float p_power )
{
	if(!m_activated)
		return;
	m_kinController->Knockback(p_velocity, p_power);
	
}

void KinematicController::SetOrientation( glm::quat p_orientation )
{
	if(!m_activated)
		return;
	btQuaternion btquat;
	btquat.setW(p_orientation.w);
	btquat.setX(p_orientation.x);
	btquat.setY(p_orientation.y);
	btquat.setZ(p_orientation.z);

	m_kinController->getGhostObject()->getWorldTransform().setRotation(btquat);
}

void KinematicController::SetUserPointer( void* p_userPointer )
{
	if(!m_activated)
		return;
	m_ghostObject->setUserPointer(p_userPointer);
}

void KinematicController::SetMass( float p_mass )
{
	if(!m_activated)
		return;
	m_mass = p_mass;
}

void KinematicController::SetGravity( float p_gravity )
{
	if(!m_activated)
		return;
	m_kinController->setGravity(p_gravity);
}

void KinematicController::SetPosition(const btVector3& p_position)
{
	if(!m_activated)
		return;
	m_kinController->warp(p_position);
}

void KinematicController::SetJumpForce( float p_jumpForce )
{
	if(!m_activated)
		return;
	m_kinController->setJumpSpeed(p_jumpForce);
}

bool KinematicController::IsOnGround()
{
	if(!m_activated)
		return false;
	return m_kinController->onGround();
}

void KinematicController::Deactivate()
{
	m_activated = false;
	m_dynamicWorld->removeCollisionObject(m_ghostObject);
}

void KinematicController::Activate()
{
	m_activated = true;
	m_kinController->StopKnockback();
	m_dynamicWorld->addCollisionObject(m_ghostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::AllFilter);
	
}
