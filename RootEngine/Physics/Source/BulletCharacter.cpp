#include "BulletCharacter.h"




BulletCharacter::BulletCharacter( btPairCachingGhostObject* p_ghostObject, btConvexShape* p_convexShape, btScalar p_stepHeight ) : btKinematicCharacterController(p_ghostObject,p_convexShape,p_stepHeight)
{
	
}

BulletCharacter::~BulletCharacter( void )
{

}


void BulletCharacter::Init()
{
	
}

void BulletCharacter::playerStep( btCollisionWorld* collisionWorld, btScalar dt )
{
	//	printf("playerStep(): ");
	//	printf("  dt = %f", dt);

	// quick check...
	if (!m_useWalkDirection && m_velocityTimeInterval <= 0.0)
	{
		//		printf("\n");
		return;		// no motion
	}
	
	// vertical velocity or vertical offset != 0?
	m_wasOnGround = onGround();
	if(m_wasOnGround)
	{
		m_knockbackVelocity.setZero();
		m_hasBeenKnockbacked = false;
	}

	// Update fall velocity.
	m_verticalVelocity -= m_gravity * dt;

	// cap the vertical velocity at the jump speed
	if(m_verticalVelocity > 0.0 && m_verticalVelocity > m_jumpSpeed)
	{
		m_verticalVelocity = m_jumpSpeed;
	}

	// cap falling speed
	if(m_verticalVelocity < 0.0 && btFabs(m_verticalVelocity) > btFabs(m_fallSpeed))
	{
		m_verticalVelocity = -btFabs(m_fallSpeed);
	}

	// as we move for some distance, the vertical offset will change
	m_verticalOffset = m_verticalVelocity * dt;

	// get the current position from our ghost object
	btTransform xform;
	xform = m_ghostObject->getWorldTransform();

	//	printf("walkDirection(%f,%f,%f)\n",walkDirection[0],walkDirection[1],walkDirection[2]);
	//	printf("walkSpeed=%f\n",walkSpeed);

	// Check whether we currently walked up a step by performing a sweep
	// test from a position that is translated along the up axis for step
	// height. 
	//
	// This check might have updated the information of the ground contact
	// information. Also it potentially leaves our position in mid-air
	// (therefore we need to call stepDown() later on)
	stepUp (collisionWorld);
	if (m_hasBeenKnockbacked)
	{
		m_knockbackVelocity += m_walkDirection*dt;
		stepForwardAndStrafe (collisionWorld, m_knockbackVelocity*dt);
	}
	// perform a translation if we are currently walking
	else if (m_useWalkDirection) 
	{
		// Make sure we do not run into any objects. This is ensured by
		// performing sweep tests and only move tangential to any collision
		// surfaces.
		//
		// After calling this function, m_current
		stepForwardAndStrafe (collisionWorld, m_walkDirection);
	} 
	else 
	{
		//printf("  time: %f", m_velocityTimeInterval);
		// still have some time left for moving!
		btScalar dtMoving =
			(dt < m_velocityTimeInterval) ? dt : m_velocityTimeInterval;
		m_velocityTimeInterval -= dt;

		// how far will we move while we are moving?
		btVector3 move = m_walkDirection * dtMoving;
		
		//printf("  dtMoving: %f", dtMoving);

		// okay, step
		stepForwardAndStrafe(collisionWorld, move);
	}

	// undo the change along the up axis when calling stepUp()
	stepDown (collisionWorld, dt);

	// printf("\n");

	// update the position
	xform.setOrigin (m_currentPosition);// + btVector3(m_knockbackVelocity.x(), 0 , m_knockbackVelocity.z())*dt);
	m_ghostObject->setWorldTransform (xform);
}

void BulletCharacter::Knockback( float* p_direction, float p_power )
{
	btVector3 temp = btVector3(p_direction[0], p_direction[1], p_direction[2]);
	m_knockbackVelocity = temp.normalize()*p_power;
	m_verticalVelocity = m_knockbackVelocity.y(); //testing only
	//m_verticalVelocity += m_knockbackVelocity.y();
	m_hasBeenKnockbacked = true;
}

void BulletCharacter::setWalkDirection( const btVector3& walkDirection )
{
	m_useWalkDirection = true;
	m_walkDirection = walkDirection;
	if (m_walkDirection.isZero())
	{
		m_normalizedDirection.setZero();
	}
	else
	{
		m_normalizedDirection = m_walkDirection.normalize();
	}
}

bool BulletCharacter::IsKnockbacked()
{
	return m_hasBeenKnockbacked;
}

