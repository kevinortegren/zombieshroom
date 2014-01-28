#pragma once
#include <Bullet/btBulletDynamicsCommon.h>
#include <Bullet/btBulletCollisionCommon.h>
#include <glm/glm.hpp>
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
		void BuildRagdoll(glm::mat4 p_bones[20], aiNode* p_rootNode, std::map<std::string, int>  p_nameToIndex);
	private:
		btDiscreteDynamicsWorld* m_dynamicWorld;
		void CreateBody(glm::mat4 p_bones[20], aiNode* p_rootNode);
		btRigidBody* m_bodies[20];
		btTypedConstraint* m_joints[20];
		bool m_firsttime;
	};
}