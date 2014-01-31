#pragma once
#include <Bullet/btBulletDynamicsCommon.h>
#include <Bullet/btBulletCollisionCommon.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <assimp/scene.h>  
#include <map>
#include <Bullet/BulletCollision/CollisionDispatch/btGhostObject.h>
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
		void SetVelocity(const btVector3& p_velocity);
	private:
		btDiscreteDynamicsWorld* m_dynamicWorld;
		btRigidBody* CreateBody(glm::mat4 p_bones[20], aiNode* p_rootNode, std::map<std::string, int>  p_nameToIndex, glm::mat4 p_transform, const btVector3& p_pos, int p_massFactor );
		btCollisionShape* CreateBone(std::string p_name);
		void SetBoneRelation(int p_parentIndex, int p_childIndex, glm::mat4 p_pose);
		btTypedConstraint* CreateConstraint(btRigidBody* p_bodyA, btRigidBody* p_bodyB, std::string p_nameA, std::string p_nameB);
		btRigidBody* m_bodies[20];
		btTypedConstraint* m_joints[20];
		glm::mat4 m_boneTransform[20];
		glm::mat4 m_lastUpdatePos[20];
		glm::mat4 m_lastBoneMatrix[20];
		int m_boneToFollow[20];
		
		int m_constraintCounter;
		bool m_firsttime;
	};
}