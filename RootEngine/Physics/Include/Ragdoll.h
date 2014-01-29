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
		btRigidBody* CreateBody(glm::mat4 p_bones[20], aiNode* p_rootNode, std::map<std::string, int>  p_nameToIndex, glm::mat4 p_transform, const btVector3& p_pos, int p_massFactor );
		btCollisionShape* CreateBone(std::string p_name);
		void SetBoneRelation(int p_childIndex, int p_parentIndex);
		void CreateConstraint(btRigidBody* p_bodyA, btRigidBody* p_bodyB, std::string p_nameA, std::string p_nameB);
		btRigidBody* m_bodies[20];
		btTypedConstraint* m_joints[20];
		glm::mat4 m_boneTransform[20];
		int m_boneToFollow[20];
		bool m_firsttime;
	};
}