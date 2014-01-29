
#include <RootEngine/Physics/Include/Ragdoll.h>
#include <glm/gtc/type_ptr.hpp>

const float PI_2 = 1.57079632679489661923f;
const float OFFSET = -0.05f;
namespace Ragdoll
{


	Ragdoll::Ragdoll( btDiscreteDynamicsWorld* p_dynamicWorld ) : m_dynamicWorld(p_dynamicWorld)
	{

	}

	Ragdoll::~Ragdoll()
	{

	}

	void Ragdoll::Activate( glm::mat4 p_bones[20] )
	{
		//Update the posiitions of the bodies and add them to the world
	}

	void Ragdoll::Deactivate()
	{
		//Remove bodies from world, i think this might be the way to doit, mayb not, we w‰ll c‰‰ in ffyradfas
	}

	void Ragdoll::BuildRagdoll( glm::mat4 p_bones[20], aiNode* p_rootNode , std::map<std::string, int>  p_nameToIndex, glm::mat4 p_transform, const btVector3& p_pos )
	{
		//here be dragons nu ‰r det fel i min hj‰rna
		btRigidBody* body;
		btCollisionShape* shape = CreateBone(p_rootNode->mName.data);
		if(shape != nullptr)
		{
			btVector3 inertia = btVector3(0,0,0);
			shape->calculateLocalInertia(10.0f, inertia);
			int index = p_nameToIndex[p_rootNode->mName.data];
			btTransform trans;
			glm::mat4 toTrans = p_transform * p_bones[index];

			const float* data = glm::value_ptr(toTrans);
			trans.setFromOpenGLMatrix(data);

			btDefaultMotionState* motionstate = new btDefaultMotionState(trans);
			body = new btRigidBody(10.0f, motionstate, shape, inertia);
			body->setDamping(0.05f,0.85f);
			body->setDeactivationTime(0.8);
			body->setSleepingThresholds(1.6, 2.5);
			m_bodies[index] = body;

			m_dynamicWorld->addRigidBody(body);

			for(unsigned int i = 0; i < p_rootNode->mNumChildren; i++)
			{		
				btRigidBody* childbody = CreateBody(p_bones, p_rootNode->mChildren[i], p_nameToIndex, toTrans, p_pos, 2);
				if(childbody != nullptr)
				{
					CreateConstraint(body, childbody, p_rootNode->mName.data, p_rootNode->mChildren[i]->mName.data);
				}
			}

		}
		else
		{
			for(unsigned int i = 0; i < p_rootNode->mNumChildren; i++)
				CreateBody(p_bones, p_rootNode->mChildren[i], p_nameToIndex, p_transform, p_pos , 1);
		}
		

	}

	btRigidBody* Ragdoll::CreateBody(glm::mat4 p_bones[20], aiNode* p_rootNode, std::map<std::string, int>  p_nameToIndex, glm::mat4 p_transform, const btVector3& p_pos, int p_massFactor )
	{
		btRigidBody* body;
		btCollisionShape* shape = CreateBone(p_rootNode->mName.data);
		if(shape != nullptr)
		{
			float mass = 10.0f / p_massFactor;
			btVector3 inertia = btVector3(0,0,0);
			shape->calculateLocalInertia(mass, inertia);
			int index = p_nameToIndex[p_rootNode->mName.data];
			btTransform trans;
			glm::mat4 toTrans = p_transform * p_bones[index];

			const float* data = glm::value_ptr(toTrans);
// 			const float* data = glm::value_ptr(p_bones[index]);
 			trans.setFromOpenGLMatrix(data);
			
			trans.setOrigin(trans.getOrigin());
			btDefaultMotionState* motionstate = new btDefaultMotionState(trans);
			body = new btRigidBody(mass, motionstate, shape, inertia);
			body->setDamping(0.05f,0.85f);
			body->setDeactivationTime(0.8);
			body->setSleepingThresholds(1.6, 2.5);
		//	body->setCcdMotionThreshold(0.7f);
			//body->setCcdSweptSphereRadius(0.4f);
			m_bodies[index] = body;
	
			m_dynamicWorld->addRigidBody(body);
			//int index = p_rootNode->mName;

			for(unsigned int i = 0; i < p_rootNode->mNumChildren; i++)
			{
				//float PI_2 = 1.57079632679489661923f;
				btRigidBody* childbody = CreateBody(p_bones, p_rootNode->mChildren[i], p_nameToIndex, toTrans, p_pos, p_massFactor+1);
				if(childbody != nullptr)
				{
					CreateConstraint(body, childbody, p_rootNode->mName.data, p_rootNode->mChildren[i]->mName.data);
					
				}
				
			}
			return body;
		}
		for(unsigned int i = 0; i < p_rootNode->mNumChildren; i++)
			CreateBody(p_bones, p_rootNode->mChildren[i], p_nameToIndex, p_transform, p_pos , p_massFactor);
	
		return nullptr;
	}

	btCollisionShape* Ragdoll::CreateBone( std::string p_name )
	{
		
		if(p_name.compare("Character1_Hips") == 0)
			return new btCapsuleShape(0.1f, 0.1f);
			//return new btCylinderShape(btVector3(0.4f,0.2f,0.4f));
		else if(p_name.compare("Character1_LeftArm") == 0)
			return new btCapsuleShape(0.14f, 0.15f);	
			//return new btCylinderShape(btVector3(0.05f,0.15f,0.05f));
		else if(p_name.compare("Character1_LeftFoot") == 0)
			return new btCapsuleShape(0.1f, 0.08f);
			//return new btCylinderShape(btVector3(0.1f,0.04f,0.1f));
		else if(p_name.compare("Character1_LeftForeArm") == 0)
			return new btCapsuleShape(0.14f, 0.15f);
			//return new btCylinderShape(btVector3(0.05f,0.15f,0.05f));
		else if(p_name.compare("Character1_LeftHand") == 0)
			return new btCapsuleShape(0.1f, 0.04f);
		//	return new btCylinderShape(btVector3(0.1f,0.04f,0.1f));
		else if(p_name.compare("Character1_LeftLeg") == 0)
			return new btCapsuleShape(0.14f, 0.2f);
			//return new btCylinderShape(btVector3(0.1f,0.2f,0.1f));
		else if(p_name.compare("Character1_LeftUpLeg") == 0)
			return new btCapsuleShape(0.14f, 0.2f);
			//return new btCylinderShape(btVector3(0.1f,0.2f,0.1f));
		else if(p_name.compare("Character1_RightArm") == 0)
			return new btCapsuleShape(0.14f, 0.15f);
			//return new btCylinderShape(btVector3(0.05f,0.15f,0.05f));
		else if(p_name.compare("Character1_RightFoot") == 0)
			return new btCapsuleShape(0.1f, 0.04f);
			//return new btCylinderShape(btVector3(0.1f,0.04f,0.1f));
		else if(p_name.compare("Character1_RightForeArm") == 0)
			return new btCapsuleShape(0.14f, 0.15f);
			//return new btCylinderShape(btVector3(0.05f,0.15f,0.05f));
		else if(p_name.compare("Character1_RightHand") == 0)
			return new btCapsuleShape(0.14, 0.04);
			//return new btCylinderShape(btVector3(0.1f,0.04f,0.1f));
		else if(p_name.compare("Character1_RightLeg") == 0)
			return new btCapsuleShape(0.14, 0.25);
			//return new btCylinderShape(btVector3(0.1f,0.25f,0.1f));
		else if(p_name.compare("Character1_RightUpLeg") == 0)
			return new btCapsuleShape(0.14, 0.25);
			//return new btCylinderShape(btVector3(0.1f,0.25f,0.1f));
		else if(p_name.compare("Character1_Spine") == 0)
			return new btCapsuleShape(0.2f,0.3f);
			//return new btCylinderShape(btVector3(0.4f,0.4f,0.4f));

		return nullptr;
	}

	btVector3 Ragdoll::GetPos()
	{
		return m_bodies[13]->getWorldTransform().getOrigin();
	}

	glm::mat4* Ragdoll::GetBones()
	{
		glm::mat4* retVal = new glm::mat4[20];
		for(int i = 0; i < 14; i++)
		{
			btTransform trans = m_bodies[i]->getWorldTransform();
			//trans.setRotation(btQuaternion(trans.getRotation().w(), trans.getRotation().x(), trans.getRotation().y(), trans.getRotation().z()));
			float data[16];
			trans.getOpenGLMatrix(data);
			retVal[i] = glm::make_mat4(data);
		}
		return retVal;
	}

	btQuaternion Ragdoll::GetOrientation()
	{
		return m_bodies[13]->getWorldTransform().getRotation();
	}

	void Ragdoll::SetOrientation( glm::quat p_orientation )
	{
		btQuaternion btquat;
		btquat.setW(p_orientation.w);
		btquat.setX(p_orientation.x);
		btquat.setY(p_orientation.y);
		btquat.setZ(p_orientation.z);
		m_bodies[13]->getWorldTransform().setRotation(btquat);
	}

	void Ragdoll::CreateConstraint( btRigidBody* p_bodyA, btRigidBody* p_bodyB, std::string p_nameA, std::string p_nameB )
	{
		btTransform localA, localB;
		localA.setIdentity(); localB.setIdentity();
		localA.getBasis().setEulerZYX(0,0,0);
		localB.getBasis().setEulerZYX(0,0,0); 
		//Hips-spine
		if(p_nameA.compare("Character1_Hips") == 0 && p_nameB.compare("Character1_Spine") == 0 )
		{
			localA.getBasis().setEulerZYX(0,PI_2,0);
			localB.getBasis().setEulerZYX(0,PI_2,0); 
			localA.setOrigin(btVector3(0, 0.25f + OFFSET, 0)); 
			localB.setOrigin(btVector3(0, -0.25f - OFFSET, 0));
			btHingeConstraint* constraint = new btHingeConstraint(*p_bodyA, *p_bodyB, localA, localB/*, btVector3(0,1,0), btVector3(0,-1,0)*/);
			constraint->setLimit(0,0);
		
			//constraint->setLimit(PI_2, PI_2, PI_2);
			m_dynamicWorld->addConstraint(constraint, true);
			constraint->setDbgDrawSize(0.5f);
		}
		//Hips - Left upper leg
		else if(p_nameA.compare("Character1_Hips") == 0 && p_nameB.compare("Character1_LeftUpLeg") == 0 )
		{
			localA.setOrigin(btVector3(-0.1, -0.24f- OFFSET, 0)); 
			localB.setOrigin(btVector3(0, 0.24f + OFFSET, 0));
			btPoint2PointConstraint* constraint = new btPoint2PointConstraint(*p_bodyA, *p_bodyB, localA.getOrigin(), localB.getOrigin());
			//constraint->setLimit(PI_2, PI_2, PI_2);
			m_dynamicWorld->addConstraint(constraint);
			constraint->setDbgDrawSize(0.5f);
		}
		////Hips - Right upper leg
		else if(p_nameA.compare("Character1_Hips") == 0 && p_nameB.compare("Character1_RightUpLeg") == 0 )
		{
			localA.setOrigin(btVector3(0.30, -0.3f - OFFSET, 0)); 
			localB.setOrigin(btVector3(0, 0.3f + OFFSET, 0));
			btPoint2PointConstraint* constraint = new btPoint2PointConstraint(*p_bodyA, *p_bodyB, localA.getOrigin(), localB.getOrigin());
			//constraint->setLimit(PI_2, PI_2, PI_2);
			m_dynamicWorld->addConstraint(constraint);
			constraint->setDbgDrawSize(0.5f);
		}
		//Left upper leg - Left lower leg
		else if(p_nameA.compare("Character1_LeftUpLeg") == 0 && p_nameB.compare("Character1_LeftLeg") == 0 )
		{
			localA.setOrigin(btVector3(0, -0.5f- OFFSET, 0)); 
			localB.setOrigin(btVector3(0, 0.2f + OFFSET, 0));
			btPoint2PointConstraint* constraint = new btPoint2PointConstraint(*p_bodyA, *p_bodyB, localA.getOrigin(), localB.getOrigin());
			//constraint->setLimit(PI_2, PI_2, PI_2);
			m_dynamicWorld->addConstraint(constraint);
			constraint->setDbgDrawSize(0.5f);
		}
		//Right upper leg - Right lower leg
		else if(p_nameA.compare("Character1_RightUpLeg") == 0 && p_nameB.compare("Character1_RightLeg") == 0 )
		{
			localA.setOrigin(btVector3(0, -0.25f - OFFSET, 0)); 
			localB.setOrigin(btVector3(0, 0.2f + OFFSET, 0));
			btPoint2PointConstraint* constraint = new btPoint2PointConstraint(*p_bodyA, *p_bodyB, localA.getOrigin(), localB.getOrigin());
			//constraint->setLimit(PI_2, PI_2, PI_2);
			m_dynamicWorld->addConstraint(constraint);
			constraint->setDbgDrawSize(0.5f);
		}
		//Left lower leg - Left foot
		else if(p_nameA.compare("Character1_LeftLeg") == 0 && p_nameB.compare("Character1_LeftFoot") == 0 )
		{
			localA.setOrigin(btVector3(0, -0.21f - OFFSET, 0)); 
			localB.setOrigin(btVector3(0, 0.09f+ OFFSET, 0));
			btPoint2PointConstraint* constraint = new btPoint2PointConstraint(*p_bodyA, *p_bodyB, localA.getOrigin(), localB.getOrigin());
			//constraint->setLimit(PI_2, PI_2, PI_2);
			m_dynamicWorld->addConstraint(constraint);
			constraint->setDbgDrawSize(0.5f);
		}
		//Right lower leg - Right foot
		else if(p_nameA.compare("Character1_RightLeg") == 0 && p_nameB.compare("Character1_RightFoot") == 0 )
		{
			localA.setOrigin(btVector3(0, -0.2f - OFFSET, 0)); 
			localB.setOrigin(btVector3(0, 0.04f + OFFSET, 0));
			btPoint2PointConstraint* constraint = new btPoint2PointConstraint(*p_bodyA, *p_bodyB, localA.getOrigin(), localB.getOrigin());
			//constraint->setLimit(PI_2, PI_2, PI_2);
			m_dynamicWorld->addConstraint(constraint);
			constraint->setDbgDrawSize(0.5f);
		}
		////Spine - Left upper arm
		else if(p_nameA.compare("Character1_Spine") == 0 && p_nameB.compare("Character1_LeftArm") == 0 )
		{
			localA.getBasis().setEulerZYX(0,0,1);
			localB.getBasis().setEulerZYX(0,0,1); 
			localA.setOrigin(btVector3(-0.2f, 0.10f +  OFFSET, 0)); 
			localB.setOrigin(btVector3(0.05f, 0.05f + OFFSET, 0));
			btPoint2PointConstraint* constraint = new btPoint2PointConstraint(*p_bodyA, *p_bodyB, localA.getOrigin(), localB.getOrigin());
			//constraint->setLimit(0, 0, 0);
			m_dynamicWorld->addConstraint(constraint);
			constraint->setDbgDrawSize(0.5f);
		}
		////Spine - Right upper arm
		else if(p_nameA.compare("Character1_Spine") == 0 && p_nameB.compare("Character1_RightArm") == 0 )
		{
			localA.getBasis().setEulerZYX(0,0,1);
			localB.getBasis().setEulerZYX(0,0,1); 
			localA.setOrigin(btVector3(0.2f, 0.10f +  OFFSET, 0)); 
			localB.setOrigin(btVector3(-0.05f, 0.05f +  OFFSET, 0));
			btPoint2PointConstraint* constraint = new btPoint2PointConstraint(*p_bodyA, *p_bodyB, localA.getOrigin(), localB.getOrigin());
			//constraint->setLimit(0, 0, 0);
			m_dynamicWorld->addConstraint(constraint);
			constraint->setDbgDrawSize(0.5f);
		}
		////Left upper arm - Left lower arm
		else if(p_nameA.compare("Character1_LeftArm") == 0 && p_nameB.compare("Character1_LeftForeArm") == 0 )
		{
			localA.getBasis().setEulerZYX(0,0,0);
			localB.getBasis().setEulerZYX(0,0,0); 
			localA.setOrigin(btVector3(0, -0.20f - OFFSET, 0)); 
			localB.setOrigin(btVector3(0, 0.20f + OFFSET, 0));
			btPoint2PointConstraint* constraint = new btPoint2PointConstraint(*p_bodyA, *p_bodyB, localA.getOrigin(), localB.getOrigin());
			
			m_dynamicWorld->addConstraint(constraint);
			constraint->setDbgDrawSize(0.5f);
		}
		//Right upper arm - Right lower arm
		else if(p_nameA.compare("Character1_RightArm") == 0 && p_nameB.compare("Character1_RightForeArm") == 0 )
		{
			localA.getBasis().setEulerZYX(0,0,0);
			localB.getBasis().setEulerZYX(0,0,0); 
			localA.setOrigin(btVector3(0, -0.20f - OFFSET, 0)); 
			localB.setOrigin(btVector3(0, 0.20f + OFFSET, 0));
			btPoint2PointConstraint* constraint = new btPoint2PointConstraint(*p_bodyA, *p_bodyB, localA.getOrigin(), localB.getOrigin());
			
			m_dynamicWorld->addConstraint(constraint);
			constraint->setDbgDrawSize(0.5f);
		}
		//Left lower arm - Left hand
		else if(p_nameA.compare("Character1_LeftForeArm") == 0 && p_nameB.compare("Character1_LeftHand") == 0 )
		{
			localA.setOrigin(btVector3(0, -0.2f - OFFSET, 0)); 
			localB.setOrigin(btVector3(0, 0.04f + OFFSET, 0));
			btPoint2PointConstraint* constraint = new btPoint2PointConstraint(*p_bodyA, *p_bodyB, localA.getOrigin(), localB.getOrigin());

			m_dynamicWorld->addConstraint(constraint);
			constraint->setDbgDrawSize(0.5f);
		}
		//Right lower arm - Right hand
		else if(p_nameA.compare("Character1_RightForeArm") == 0 && p_nameB.compare("Character1_RightHand") == 0 )
		{
			localA.setOrigin(btVector3(0, -0.2f - OFFSET, 0)); 
			localB.setOrigin(btVector3(0, 0.04f + OFFSET, 0));
			btPoint2PointConstraint* constraint = new btPoint2PointConstraint(*p_bodyA, *p_bodyB, localA.getOrigin(), localB.getOrigin());

			m_dynamicWorld->addConstraint(constraint);
			constraint->setDbgDrawSize(0.5f);
		}
	}

}