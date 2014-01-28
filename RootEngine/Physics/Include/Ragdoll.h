#pragma once
#include <Bullet/btBulletDynamicsCommon.h>
#include <Bullet/btBulletCollisionCommon.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <assimp/scene.h>  
#include <map>
namespace Ragdoll
{
	class Ragdoll
	{
	public:
		Ragdoll(btDiscreteDynamicsWorld* p_dynamicWorld);
		~Ragdoll();
		void Activate(glm::mat4 p_bones[20]);
		void Deactivate();
		void BuildRagdoll(glm::mat4 p_bones[20], aiNode* p_rootNode, std::map<std::string, int>  p_nameToIndex, glm::mat4 p_transform, const btVector3& p_pos );
		btVector3 GetPos();
		glm::mat4* GetBones();
		btQuaternion GetOrientation();
		void SetOrientation(glm::quat p_orientation);
	private:
		btDiscreteDynamicsWorld* m_dynamicWorld;
		btRigidBody* CreateBody(glm::mat4 p_bones[20], aiNode* p_rootNode, std::map<std::string, int>  p_nameToIndex, glm::mat4 p_transform, const btVector3& p_pos );
		btCylinderShape* CreateBone(std::string p_name);
		btRigidBody* m_bodies[20];
		btTypedConstraint* m_joints[20];
		bool m_firsttime;
	};
}