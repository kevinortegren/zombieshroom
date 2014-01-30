
#include <RootEngine/Physics/Include/Ragdoll.h>
#include <glm/gtc/type_ptr.hpp>

const float PI_2 = 1.57079632679489661923f;
const float OFFSET = -0.8f;
namespace Ragdoll
{


	Ragdoll::Ragdoll( btDiscreteDynamicsWorld* p_dynamicWorld ) : m_dynamicWorld(p_dynamicWorld), m_constraintCounter(0)
	{
		
	}

	Ragdoll::~Ragdoll()
	{
		for (int i = 0; i < m_constraintCounter; ++i)
		{
			m_dynamicWorld->removeConstraint(m_joints[i]);
			delete m_joints[i]; m_joints[i] = 0;
		}
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
		CreateBody(p_bones, p_rootNode, p_nameToIndex, p_transform, p_pos, 1);
		m_boneToFollow[13] = 13;
		////here be dragons nu ‰r det fel i min hj‰rna
		
		

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
			glm::mat4 toGlm = glm::mat4();
			aiMatrix4x4 test = p_rootNode->mTransformation; //this might be the thing that will solve everything, everyyythiiiinnnggg
			memcpy(&toGlm[0][0], &test[0][0], sizeof(aiMatrix4x4));
			glm::mat4 bonePos =  glm::transpose(toGlm);
			btTransform trans;
			bonePos = bonePos /** p_bones[index]*/;
			glm::mat4 toTrans = /*bonePos **/ p_transform;
			
			const float* data = glm::value_ptr(toTrans);
// 			const float* data = glm::value_ptr(p_bones[index]);
 			trans.setFromOpenGLMatrix(data);
			//m_lastUpdatePos[index] = p_bones[index];
			trans.setOrigin(trans.getOrigin());
			btDefaultMotionState* motionstate = new btDefaultMotionState(trans);
			body = new btRigidBody(mass, motionstate, shape, inertia);
			body->setDamping(0.05f,0.85f);
			//body->setDeactivationTime(0.8f);
			//body->setSleepingThresholds(1.6f, 2.5f);
			
		//	body->setCcdMotionThreshold(0.6f);
		//	body->setCcdSweptSphereRadius(0.3f);

#pragma warning	Okej, ide ‰r att vi skapar ett ghostobject som har samma pos som bodyn fast utan offseten frÂn benen, sen tar vi inversen pÂ den o gÂngrar med body transen sÂ kanske nÂt kul h‰nder, annars vetefan.
			m_bodies[index] = body;
			m_lastUpdatePos[index] = bonePos * p_bones[index];
			m_dynamicWorld->addRigidBody(body);
			//int index = p_rootNode->mName;

			for(unsigned int i = 0; i < p_rootNode->mNumChildren; i++)
			{
				//float PI_2 = 1.57079632679489661923f;
				btRigidBody* childbody = CreateBody(p_bones, p_rootNode->mChildren[i], p_nameToIndex, p_transform, p_pos, p_massFactor+1);
				if(childbody != nullptr)
				{
					btTypedConstraint* temp = CreateConstraint(body, childbody, p_rootNode->mName.data, p_rootNode->mChildren[i]->mName.data);
					if(temp != nullptr)
					{
						m_joints[m_constraintCounter] = temp;
						m_constraintCounter++;
					}
					SetBoneRelation(index, p_nameToIndex[p_rootNode->mChildren[i]->mName.data]);
					
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
			return new btCapsuleShape(0.2f, 0.2f);
			//return new btCylinderShape(btVector3(0.4f,0.2f,0.4f));
		else if(p_name.compare("Character1_LeftArm") == 0)
			return new btCapsuleShape(0.28f, 0.30f);	
			//return new btCylinderShape(btVector3(0.05f,0.15f,0.05f));
		else if(p_name.compare("Character1_LeftFoot") == 0)
			return new btCapsuleShape(0.2f, 0.16f);
			//return new btCylinderShape(btVector3(0.1f,0.04f,0.1f));
		else if(p_name.compare("Character1_LeftForeArm") == 0)
			return new btCapsuleShape(0.28f, 0.30f);
			//return new btCylinderShape(btVector3(0.05f,0.15f,0.05f));
		else if(p_name.compare("Character1_LeftHand") == 0)
			return new btCapsuleShape(0.2f, 0.08f);
		//	return new btCylinderShape(btVector3(0.1f,0.04f,0.1f));
		else if(p_name.compare("Character1_LeftLeg") == 0)
			return new btCapsuleShape(0.28f, 0.4f);
			//return new btCylinderShape(btVector3(0.1f,0.2f,0.1f));
		else if(p_name.compare("Character1_LeftUpLeg") == 0)
			return new btCapsuleShape(0.28f, 0.4f);
			//return new btCylinderShape(btVector3(0.1f,0.2f,0.1f));
		else if(p_name.compare("Character1_RightArm") == 0)
			return new btCapsuleShape(0.28f, 0.3f);
			//return new btCylinderShape(btVector3(0.05f,0.15f,0.05f));
		else if(p_name.compare("Character1_RightFoot") == 0)
			return new btCapsuleShape(0.2f, 0.08f);
			//return new btCylinderShape(btVector3(0.1f,0.04f,0.1f));
		else if(p_name.compare("Character1_RightForeArm") == 0)
			return new btCapsuleShape(0.28f, 0.3f);
			//return new btCylinderShape(btVector3(0.05f,0.15f,0.05f));
		else if(p_name.compare("Character1_RightHand") == 0)
			return new btCapsuleShape(0.28f, 0.08f);
			//return new btCylinderShape(btVector3(0.1f,0.04f,0.1f));
		else if(p_name.compare("Character1_RightLeg") == 0)
			return new btCapsuleShape(0.28f, 0.5f);
			//return new btCylinderShape(btVector3(0.1f,0.25f,0.1f));
		else if(p_name.compare("Character1_RightUpLeg") == 0)
			return new btCapsuleShape(0.28f, 0.5f);
			//return new btCylinderShape(btVector3(0.1f,0.25f,0.1f));
		else if(p_name.compare("Character1_Spine") == 0)
			return new btCapsuleShape(0.4f,0.6f);
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
			float data[16];
			btTransform trans = m_bodies[i]->getWorldTransform();
			trans.getOpenGLMatrix(data);
			
			//trans.getOrigin().normalize();
			//trans.setRotation(btQuaternion(trans.getRotation().w(), trans.getRotation().x(), trans.getRotation().y(), trans.getRotation().z()));
		
		//	m_lastUpdatePos[i] = m_bodies[i]->getWorldTransform();
			int index = m_boneToFollow[i];
		//	trans.getOrigin() = trans.getOrigin() - m_bodies[index]->getWorldTransform().getOrigin();
		//	trans.getRotation() = trans.getRotation() - m_bodies[index]->getWorldTransform().getRotation();

		//	trans.setOrigin((trans.getOrigin() - m_bodies[index]->getWorldTransform().getOrigin()));
			float test[16];
			trans.inverse().getOpenGLMatrix(test);
		
			
			retVal[i] = glm::make_mat4(data) * m_lastUpdatePos[i] * glm::make_mat4(test); /* * m_lastUpdatePos[i]*//* * m_boneTransform[i]*/;
			
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

	btTypedConstraint* Ragdoll::CreateConstraint( btRigidBody* p_bodyA, btRigidBody* p_bodyB, std::string p_nameA, std::string p_nameB )
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
			return constraint;
		}
		//Hips - Left upper leg
		else if(p_nameA.compare("Character1_Hips") == 0 && p_nameB.compare("Character1_LeftUpLeg") == 0 )
		{
			localA.setOrigin(btVector3(-0.1f, -0.24f- OFFSET, 0)); 
			localB.setOrigin(btVector3(0, 0.24f + OFFSET, 0));
			btPoint2PointConstraint* constraint = new btPoint2PointConstraint(*p_bodyA, *p_bodyB, localA.getOrigin(), localB.getOrigin());
			//constraint->setLimit(PI_2, PI_2, PI_2);
			m_dynamicWorld->addConstraint(constraint);
			constraint->setDbgDrawSize(0.5f);
			return constraint;
		}
		////Hips - Right upper leg
		else if(p_nameA.compare("Character1_Hips") == 0 && p_nameB.compare("Character1_RightUpLeg") == 0 )
		{
			localA.setOrigin(btVector3(0.30f, -0.3f - OFFSET, 0)); 
			localB.setOrigin(btVector3(0, 0.3f + OFFSET, 0));
			btPoint2PointConstraint* constraint = new btPoint2PointConstraint(*p_bodyA, *p_bodyB, localA.getOrigin(), localB.getOrigin());
			//constraint->setLimit(PI_2, PI_2, PI_2);
			m_dynamicWorld->addConstraint(constraint);
			constraint->setDbgDrawSize(0.5f);
			return constraint;
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
			return constraint;
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
			return constraint;
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
			return constraint;
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
			return constraint;
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
			return constraint;
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
			return constraint;
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
			return constraint;
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
			return constraint;
		}
		//Left lower arm - Left hand
		else if(p_nameA.compare("Character1_LeftForeArm") == 0 && p_nameB.compare("Character1_LeftHand") == 0 )
		{
			localA.setOrigin(btVector3(0, -0.2f - OFFSET, 0)); 
			localB.setOrigin(btVector3(0, 0.04f + OFFSET, 0));
			btPoint2PointConstraint* constraint = new btPoint2PointConstraint(*p_bodyA, *p_bodyB, localA.getOrigin(), localB.getOrigin());

			m_dynamicWorld->addConstraint(constraint);
			constraint->setDbgDrawSize(0.5f);
			return constraint;
		}
		//Right lower arm - Right hand
		else if(p_nameA.compare("Character1_RightForeArm") == 0 && p_nameB.compare("Character1_RightHand") == 0 )
		{
			localA.setOrigin(btVector3(0, -0.2f - OFFSET, 0)); 
			localB.setOrigin(btVector3(0, 0.04f + OFFSET, 0));
			btPoint2PointConstraint* constraint = new btPoint2PointConstraint(*p_bodyA, *p_bodyB, localA.getOrigin(), localB.getOrigin());

			m_dynamicWorld->addConstraint(constraint);
			constraint->setDbgDrawSize(0.5f);
			return constraint;
		}
		return nullptr;
	}

	void Ragdoll::SetBoneRelation( int p_parentIndex, int p_childIndex )
	{
		m_boneToFollow[p_childIndex] = p_parentIndex;
		/*btTransform childTrans = m_bodies[p_childIndex]->getWorldTransform();
		btTransform parentTrans = m_bodies[p_parentIndex]->getWorldTransform();
		float data[16];
		childTrans.getOpenGLMatrix(data);
		glm::mat4 childMat = glm::make_mat4(data);
		parentTrans.getOpenGLMatrix(data);
		glm::mat4 parentMat = glm::make_mat4(data);
		parentMat = parentMat._inverse();

		m_boneTransform[p_childIndex] = m_lastUpdatePos[p_childIndex] * parentMat;*/
	}

	void Ragdoll::SetVelocity( const btVector3& p_velocity )
	{
		m_bodies[13]->setLinearVelocity(p_velocity);
	}

}