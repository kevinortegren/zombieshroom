#include "BulletCharacter.h"

namespace RootEngine
{

	namespace Physics
	{
		extern RootEngine::SubsystemSharedContext g_context;

	}
}
class btKinematicClosestNotMeConvexResultCallback : public btCollisionWorld::ClosestConvexResultCallback
{
public:
	btKinematicClosestNotMeConvexResultCallback (btCollisionObject* me, const btVector3& up, btScalar minSlopeDot)
		: btCollisionWorld::ClosestConvexResultCallback(btVector3(0.0, 0.0, 0.0), btVector3(0.0, 0.0, 0.0))
		, m_me(me)
		, m_up(up)
		, m_minSlopeDot(minSlopeDot)
	{
	}

	virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult,bool normalInWorldSpace)
	{
		if (convexResult.m_hitCollisionObject == m_me)
			return btScalar(1.0);

		if (!convexResult.m_hitCollisionObject->hasContactResponse())
			return btScalar(1.0);
		
		btVector3 hitNormalWorld;
		if (normalInWorldSpace)
		{
			hitNormalWorld = convexResult.m_hitNormalLocal;
		} else
		{
			///need to transform normal into worldspace
			hitNormalWorld = convexResult.m_hitCollisionObject->getWorldTransform().getBasis()*convexResult.m_hitNormalLocal;
		}

		btScalar dotUp = m_up.dot(hitNormalWorld);
		if (dotUp < m_minSlopeDot) {
			return btScalar(1.0);
		}
		
		return ClosestConvexResultCallback::addSingleResult (convexResult, normalInWorldSpace);
	}
protected:
	btCollisionObject* m_me;
	const btVector3 m_up;
	btScalar m_minSlopeDot;
};

BulletCharacter::BulletCharacter( btPairCachingGhostObject* p_ghostObject, btConvexShape* p_convexShape, btScalar p_stepHeight ) : btKinematicCharacterController(p_ghostObject,p_convexShape,p_stepHeight)
{
	m_addedMargin = 0.02f;
	m_hasBeenKnockbacked = false;
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
	if(m_wasOnGround && m_hasBeenKnockbacked)
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

void BulletCharacter::Knockback( const btVector3& p_direction, float p_power )
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



void BulletCharacter::stepUp ( btCollisionWorld* world)
{
	// phase 1: up
	btTransform start, end;
	m_targetPosition = m_currentPosition + getUpAxisDirections()[m_upAxis] * (m_stepHeight + (m_verticalOffset > 0.f?m_verticalOffset:0.f));

	start.setIdentity ();
	end.setIdentity ();
	//RootEngine::Physics::g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::DEBUG_PRINT, "Current pos: %f %f %f", m_currentPosition.x(), m_currentPosition.y(), m_currentPosition.z());
	//RootEngine::Physics::g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::DEBUG_PRINT, "target pos: %f %f %f", m_targetPosition.x(), m_targetPosition.y(), m_targetPosition.z());
	/* FIXME: Handle penetration properly */
	start.setOrigin (m_currentPosition + getUpAxisDirections()[m_upAxis] * (m_convexShape->getMargin() + m_addedMargin));
	end.setOrigin (m_targetPosition);
	btKinematicClosestNotMeConvexResultCallback callback (m_ghostObject, -getUpAxisDirections()[m_upAxis], btScalar(0.7071));
	callback.m_collisionFilterGroup = getGhostObject()->getBroadphaseHandle()->m_collisionFilterGroup;
	callback.m_collisionFilterMask = getGhostObject()->getBroadphaseHandle()->m_collisionFilterMask;
	
	if (m_useGhostObjectSweepTest)
	{
		m_ghostObject->convexSweepTest (m_convexShape, start, end, callback, world->getDispatchInfo().m_allowedCcdPenetration);
	}
	else
	{
		world->convexSweepTest (m_convexShape, start, end, callback);
	}

	if (callback.hasHit())
	{
		//RootEngine::Physics::g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::DEBUG_PRINT, "Bad triangle...");
		//////////////////////////////////////////////////////////////////////////
		//float asdf = callback.m_hitNormalWorld.dot(getUpAxisDirections()[m_upAxis]);
		//RootEngine::Physics::g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::DEBUG_PRINT, "Normal dot Up: %f", asdf);
		//////////////////////////////////////////////////////////////////////////
		// Only modify the position if the hit was a slope and not a wall or ceiling.

		if(callback.m_hitNormalWorld.dot(getUpAxisDirections()[m_upAxis]) > 0.0)
		{
			// we moved up only a fraction of the step height
			m_currentStepOffset = m_stepHeight * callback.m_closestHitFraction;
			if (/*m_interpolateUp == */true)
				m_currentPosition.setInterpolate3 (m_currentPosition, m_targetPosition, callback.m_closestHitFraction);
			else
				m_currentPosition = m_targetPosition;
		}
		m_verticalVelocity = 0.0;
		m_verticalOffset = 0.0;
	} 
	else 
	{
		m_currentStepOffset = m_stepHeight;
		m_currentPosition = m_targetPosition;
	}
}
void BulletCharacter::stepForwardAndStrafe ( btCollisionWorld* collisionWorld, const btVector3& walkMove)
{
	// printf("m_normalizedDirection=%f,%f,%f\n",
	// 	m_normalizedDirection[0],m_normalizedDirection[1],m_normalizedDirection[2]);
	// phase 2: forward and strafe
	btTransform start, end;
	m_targetPosition = m_currentPosition + walkMove;
	if(m_targetPosition == m_currentPosition)
		return;
	start.setIdentity ();
	end.setIdentity ();

	btScalar fraction = 1.0;
	btScalar distance2 = (m_currentPosition-m_targetPosition).length2();
	//	printf("distance2=%f\n",distance2);

	if (m_touchingContact)
	{
		if (m_normalizedDirection.dot(m_touchingNormal) > btScalar(0.0))
		{
			//interferes with step movement
			//updateTargetPositionBasedOnCollision (m_touchingNormal);
		}
	}

	int maxIter = 10;

	while (fraction > btScalar(0.01) && maxIter-- > 0)
	{
		start.setOrigin (m_currentPosition);
		end.setOrigin (m_targetPosition);
		btVector3 sweepDirNegative(m_currentPosition - m_targetPosition);

		btKinematicClosestNotMeConvexResultCallback callback (m_ghostObject, sweepDirNegative, btScalar(0.0));
		callback.m_collisionFilterGroup = getGhostObject()->getBroadphaseHandle()->m_collisionFilterGroup;
		callback.m_collisionFilterMask = getGhostObject()->getBroadphaseHandle()->m_collisionFilterMask;


		btScalar margin = m_convexShape->getMargin();
		m_convexShape->setMargin(margin + m_addedMargin);

		if (m_useGhostObjectSweepTest)
		{
			m_ghostObject->convexSweepTest (m_convexShape, start, end, callback, collisionWorld->getDispatchInfo().m_allowedCcdPenetration);
		} else
		{
			collisionWorld->convexSweepTest (m_convexShape, start, end, callback, collisionWorld->getDispatchInfo().m_allowedCcdPenetration);
		}

		m_convexShape->setMargin(margin);


		fraction -= callback.m_closestHitFraction;

		if (callback.hasHit())
		{	
			// we moved only a fraction
			btScalar hitDistance;
			hitDistance = (callback.m_hitPointWorld - m_currentPosition).length();

				//		m_currentPosition.setInterpolate3 (m_currentPosition, m_targetPosition, callback.m_closestHitFraction);

			updateTargetPositionBasedOnCollision (callback.m_hitNormalWorld);
			btVector3 currentDir = m_targetPosition - m_currentPosition;
			distance2 = currentDir.length2();
			if (distance2 > SIMD_EPSILON)
			{
				currentDir.normalize();
				/* See Quake2: "If velocity is against original velocity, stop ead to avoid tiny oscilations in sloping corners." */
				if (currentDir.dot(m_normalizedDirection) <= btScalar(0.0))
				{
					break;
				}
			} else
			{
				//				printf("currentDir: don't normalize a zero vector\n");
				break;
			}

		} else {
			// we moved whole way
			m_currentPosition = m_targetPosition;
		}

		//	if (callback.m_closestHitFraction == 0.f)
		//		break;

	}
}
void BulletCharacter::stepDown ( btCollisionWorld* collisionWorld, btScalar dt)
{
	btTransform start, end, end_double;
	bool runonce = false;

	// phase 3: down
	/*btScalar additionalDownStep = (m_wasOnGround && !onGround()) ? m_stepHeight : 0.0;
	btVector3 step_drop = getUpAxisDirections()[m_upAxis] * (m_currentStepOffset + additionalDownStep);
	btScalar downVelocity = (additionalDownStep == 0.0 && m_verticalVelocity<0.0?-m_verticalVelocity:0.0) * dt;
	btVector3 gravity_drop = getUpAxisDirections()[m_upAxis] * downVelocity; 
	m_targetPosition -= (step_drop + gravity_drop);*/

	btVector3 orig_position = m_targetPosition;
	
	btScalar downVelocity = (m_verticalVelocity<0.f?-m_verticalVelocity:0.f) * dt;

	if(downVelocity > 0.0 && downVelocity > m_fallSpeed
		&& (m_wasOnGround || !m_wasJumping))
		downVelocity = m_fallSpeed;

	btVector3 step_drop = getUpAxisDirections()[m_upAxis] * (m_currentStepOffset + downVelocity);
	m_targetPosition -= step_drop;

	btKinematicClosestNotMeConvexResultCallback callback (m_ghostObject, getUpAxisDirections()[m_upAxis], m_maxSlopeCosine);
        callback.m_collisionFilterGroup = getGhostObject()->getBroadphaseHandle()->m_collisionFilterGroup;
        callback.m_collisionFilterMask = getGhostObject()->getBroadphaseHandle()->m_collisionFilterMask;

        btKinematicClosestNotMeConvexResultCallback callback2 (m_ghostObject, getUpAxisDirections()[m_upAxis], m_maxSlopeCosine);
        callback2.m_collisionFilterGroup = getGhostObject()->getBroadphaseHandle()->m_collisionFilterGroup;
        callback2.m_collisionFilterMask = getGhostObject()->getBroadphaseHandle()->m_collisionFilterMask;

	while (1)
	{
		start.setIdentity ();
		end.setIdentity ();

		end_double.setIdentity ();

		start.setOrigin (m_currentPosition);
		end.setOrigin (m_targetPosition);

		//set double test for 2x the step drop, to check for a large drop vs small drop
		end_double.setOrigin (m_targetPosition - step_drop);

		if (m_useGhostObjectSweepTest)
		{
			m_ghostObject->convexSweepTest (m_convexShape, start, end, callback, collisionWorld->getDispatchInfo().m_allowedCcdPenetration);

			if (!callback.hasHit())
			{
				//test a double fall height, to see if the character should interpolate it's fall (full) or not (partial)
				m_ghostObject->convexSweepTest (m_convexShape, start, end_double, callback2, collisionWorld->getDispatchInfo().m_allowedCcdPenetration);
			}
		} else
		{
			collisionWorld->convexSweepTest (m_convexShape, start, end, callback, collisionWorld->getDispatchInfo().m_allowedCcdPenetration);

			if (!callback.hasHit())
					{
							//test a double fall height, to see if the character should interpolate it's fall (large) or not (small)
							collisionWorld->convexSweepTest (m_convexShape, start, end_double, callback2, collisionWorld->getDispatchInfo().m_allowedCcdPenetration);
					}
		}
	
		btScalar downVelocity2 = (m_verticalVelocity<0.f?-m_verticalVelocity:0.f) * dt;
		bool has_hit = false;
		if (false/*bounce_fix == true*/)
			has_hit = callback.hasHit() || callback2.hasHit();
		else
			has_hit = callback2.hasHit();

		if(downVelocity2 > 0.0 && downVelocity2 < m_stepHeight && has_hit == true && runonce == false
					&& (m_wasOnGround || !m_wasJumping))
		{
			//redo the velocity calculation when falling a small amount, for fast stairs motion
			//for larger falls, use the smoother/slower interpolated movement by not touching the target position

			m_targetPosition = orig_position;
					downVelocity = m_stepHeight;

				btVector3 step_drop = getUpAxisDirections()[m_upAxis] * (m_currentStepOffset + downVelocity);
			m_targetPosition -= step_drop;
			runonce = true;
			continue; //re-run previous tests
		}
		break;
	}

	if (callback.hasHit() || runonce == true)
	{
		// we dropped a fraction of the height -> hit floor

		//////////////////////////////////////////////////////////////////////////
		float asdf = callback.m_hitNormalWorld.dot(getUpAxisDirections()[m_upAxis]);
		//RootEngine::Physics::g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::DEBUG_PRINT, "Normal dot Up: %f", asdf);
		//////////////////////////////////////////////////////////////////////////

		btScalar fraction = (m_currentPosition.getY() - callback.m_hitPointWorld.getY()) / 2;

		//printf("hitpoint: %g - pos %g\n", callback.m_hitPointWorld.getY(), m_currentPosition.getY());

		if (false/*bounce_fix == true*/)
		{
			if (/*full_drop == */true)
                                m_currentPosition.setInterpolate3 (m_currentPosition, m_targetPosition, callback.m_closestHitFraction);
                        else
                                //due to errors in the closestHitFraction variable when used with large polygons, calculate the hit fraction manually
                                m_currentPosition.setInterpolate3 (m_currentPosition, m_targetPosition, fraction);
		}
		else
			m_currentPosition.setInterpolate3 (m_currentPosition, m_targetPosition, callback.m_closestHitFraction);

		//full_drop = false;

		m_verticalVelocity = 0.0;
		m_verticalOffset = 0.0;
		m_wasJumping = false;
	}
	else 
	{
		// we dropped the full height
		
		//full_drop = true;

		if (false/*bounce_fix == true*/)
		{
			downVelocity = (m_verticalVelocity<0.f?-m_verticalVelocity:0.f) * dt;
			if (downVelocity > m_fallSpeed && (m_wasOnGround || !m_wasJumping))
			{
				m_targetPosition += step_drop; //undo previous target change
				downVelocity = m_fallSpeed;
				step_drop = getUpAxisDirections()[m_upAxis] * (m_currentStepOffset + downVelocity);
				m_targetPosition -= step_drop;
			}
		}
		//printf("full drop - %g, %g\n", m_currentPosition.getY(), m_targetPosition.getY());

		m_currentPosition = m_targetPosition;
	}

}

float BulletCharacter::test( const btVector3& p_start,const btVector3& p_end, btCollisionWorld* world )
{
	btTransform start, end;
	start.setIdentity ();
	end.setIdentity ();
	start.setOrigin (p_start);
	end.setOrigin (p_end);
	btKinematicClosestNotMeConvexResultCallback callback(m_ghostObject, -getUpAxisDirections()[m_upAxis], btScalar(0.7071));
	callback.m_collisionFilterGroup = getGhostObject()->getBroadphaseHandle()->m_collisionFilterGroup;
	callback.m_collisionFilterMask = getGhostObject()->getBroadphaseHandle()->m_collisionFilterMask;
	m_ghostObject->convexSweepTest (m_convexShape, start, end, callback, world->getDispatchInfo().m_allowedCcdPenetration);
	return callback.m_closestHitFraction;
}
