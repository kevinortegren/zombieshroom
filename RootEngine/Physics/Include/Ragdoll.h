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
	namespace BodyPart
	{
		enum BodyPart
		{
			RIGHTHAND,
			RIGHTFOREARM,
			RIGHTARM,
			LEFTHAND,
			LEFTFOREARM,
			LEFTARM,
			SPINE,
			RIGHTFOOT,
			RIGHTLEG,
			RIGHTUPLEG,
			LEFTFOOT,
			LEFTLEG,
			LEFTUPLEG,
			HIPS,
			TOTAL_BONE_AMUNT
		};
	}
	class Ragdoll
	{
	public:
		Ragdoll(btDiscreteDynamicsWorld* p_dynamicWorld);
		~Ragdoll();
		void Deactivate();
		void BuildRagdoll(glm::mat4 p_bones[20], aiNode* p_rootNode, std::map<std::string, int>  p_nameToIndex, glm::mat4 p_transform, glm::mat4 p_boneOffset[20] , glm::vec3 p_right);

		btVector3 GetPos();
		glm::mat4* GetBones();
		btQuaternion GetOrientation();
		void SetOrientation(glm::quat p_orientation);
		void SetVelocity(const btVector3& p_velocity);
		void SetGravity(const btVector3& p_gravity);
	private:
		btDiscreteDynamicsWorld* m_dynamicWorld;
		btRigidBody* CreateBody(glm::mat4 p_bones[20], aiNode* p_rootNode,  glm::mat4 p_transform , int p_massFactor, const btVector3& p_parentBodyOffset, glm::mat4 p_boneOffset[20]);
		btCollisionShape* CreateBone(std::string p_name);
		void FixPosition(glm::mat4 p_bones[20], aiNode* p_rootNode );
		void SetBoneRelation(int p_parentIndex, int p_childIndex);
		void CalculateConstraintTransform(btRigidBody* p_bodyA, btRigidBody* p_bodyB,  float p_offsetXbodyA , float p_offsetYbodyA , float p_offsetZbodyA , float p_offsetXbodyB  , float p_offsetYbodyB, float p_offsetZbodyB,
										float p_quatX, float p_quatY, float p_quatZ, float p_quatW, btTransform* p_transBodyA, btTransform* p_transBodyB );
		btTypedConstraint* CreateConstraint(btRigidBody* p_bodyA, btRigidBody* p_bodyB, std::string p_nameA, std::string p_nameB);
		btRigidBody* m_bodies[20];
		btTypedConstraint* m_joints[20];

		glm::mat4 m_prevPos[20];
		glm::mat4 m_lastBoneMatrix[20];
		glm::mat4 m_boneOffset[20];
		int m_boneToFollow[20];
		glm::vec3 m_boneShapeSize[20];
		btVector3 m_bodyPosOffset[20];
		std::map<std::string, int> m_nameToIndex;
		aiNode* m_hipsNode;
		glm::vec3 m_right;
		int m_constraintCounter;
		bool m_firsttime;
	};
}