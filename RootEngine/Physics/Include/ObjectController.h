#pragma once
#include <Bullet/btBulletCollisionCommon.h>
#include <Bullet/btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <Bullet/BulletCollision/CollisionDispatch/btGhostObject.h>
class ObjectController
{
public:
	ObjectController();
	~ObjectController();
	void Init(btPairCachingGhostObject* p_ghostObject, btConvexShape* p_convexShape);
	void Move( glm::vec3 p_targetPos, btDiscreteDynamicsWorld* p_world );
	glm::vec3 GetPos() const{return m_currentPos;}
	//float GetMass() const {return }
private:
	glm::vec3 m_currentPos;
	btPairCachingGhostObject* m_ghostObject;
	btConvexShape*	m_convexShape;
	
};