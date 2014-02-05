#pragma once
#include <Bullet/btBulletCollisionCommon.h>
#include <Bullet/btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
class ShapelessObject
{
public:
	ShapelessObject();
	~ShapelessObject();


	glm::vec3 GetPos() { return m_currentPos; }
	glm::quat GetOrientation() { return m_orientation; }
	void SetPos(glm::vec3 p_pos) { m_currentPos = p_pos; }
	void SetOrientation(glm::quat p_orientation) { m_orientation = p_orientation; }

private:
	glm::vec3 m_currentPos;
	glm::quat m_orientation;
};