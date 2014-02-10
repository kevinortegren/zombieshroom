
#include <RootEngine/Physics/Include/Ragdoll.h>
#include <glm/gtc/type_ptr.hpp>
const float PI_2 = 1.57079632679489661923f;
const float OFFSET = 1.0f;
namespace Ragdoll
{


	Ragdoll::Ragdoll( btDiscreteDynamicsWorld* p_dynamicWorld ) : m_dynamicWorld(p_dynamicWorld), m_constraintCounter(0)
	{
		for(int i = 0; i < BodyPart::TOTAL_BONE_AMUNT; i++)
			m_bodyPosOffset[i] = btVector3(0,0,0);
	}

	Ragdoll::~Ragdoll()
	{
		for (int i = 0; i < m_constraintCounter; i++)
		{
			m_dynamicWorld->removeConstraint(m_joints[i]);
			delete m_joints[i]; m_joints[i] = 0;
		}

	}

	void Ragdoll::Activate( glm::mat4 p_bones[20], glm::mat4 p_transform )
	{
		
		//Update the posiitions of the bodies and add them to the world
		for(int i = 0; i < BodyPart::TOTAL_BONE_AMUNT ; i++) 
		{
			btTransform trans;
			
			m_prevPos[i] = p_transform;
			m_lastBoneMatrix[i] = p_bones[i] * m_boneOffset[i];
			glm::mat4 toTrans;
			if(i < 6)
			{
				if(i < 3) 
					toTrans = glm::rotate(p_transform, -90.0f, glm::vec3(0,0,1)) * m_lastBoneMatrix[i];
				else
					toTrans = glm::rotate(p_transform, 90.0f, glm::vec3(0,0,1)) * m_lastBoneMatrix[i];
			}
			else
			{
				toTrans =  p_transform  /** bonePos*/ * m_lastBoneMatrix[i];
			}	

			const float* data = glm::value_ptr(toTrans);
			trans.setFromOpenGLMatrix(data);
			float x,y,z,w;
			x = trans.getRotation().y();
			y = trans.getRotation().z();
			z = trans.getRotation().w();
			w = trans.getRotation().x();
			trans.setRotation(btQuaternion(x,y,z,w));
			trans.setOrigin(trans.getOrigin() + m_bodyPosOffset[i]);
			m_bodies[i]->setWorldTransform(trans);
			m_bodies[i]->activate(true);
			m_dynamicWorld->addRigidBody(m_bodies[i]);
		}
		for(int i = 0; i < m_constraintCounter; i++)
			m_dynamicWorld->addConstraint(m_joints[i]);
	}

	void Ragdoll::Deactivate()
	{
		//Remove bodies from world, i think this might be the way to doit, mayb not, we w�ll c�� in ffyradfas
		for(int i = 0; i < BodyPart::TOTAL_BONE_AMUNT ; i++) //hardcoded for 14 bones, needs to change
		{
			m_bodies[i]->clearForces();
			m_dynamicWorld->removeRigidBody(m_bodies[i]);
		}
		for(int i = 0; i < m_constraintCounter; i++)
		{
			m_dynamicWorld->removeConstraint(m_joints[i]);
		}
	}

	void Ragdoll::RemoveBodies()
	{
		for(int i = 0; i < BodyPart::TOTAL_BONE_AMUNT ; i++) //hardcoded for 14 bones, needs to change
		{
			m_dynamicWorld->removeRigidBody(m_bodies[i]);
			delete m_bodies[i]; m_bodies[i] = 0;
		}
	}

	void Ragdoll::BuildRagdoll(glm::mat4 p_bones[20], aiNode* p_rootNode, std::map<std::string, int>  p_nameToIndex, glm::mat4 p_transform, glm::mat4 p_boneOffset[20])
	{
		m_bodyPosOffset[BodyPart::SPINE] = btVector3(0, 0.67f, 0);
		//m_bodyPosOffset[BodyPart::LEFTARM] = btVector3(0, 5.67f, 0);
 		//m_bodyPosOffset[BodyPart::RIGHTARM] = btVector3(0.0f, 5.67f, 0);
		m_bodyPosOffset[BodyPart::LEFTUPLEG] = btVector3(0, -0.30f ,0);
		m_bodyPosOffset[BodyPart::RIGHTUPLEG] = btVector3(0, -0.30f ,0);
		m_nameToIndex = p_nameToIndex;
	//	memcpy(m_boneOffset, p_boneOffset, sizeof(glm::mat4) * 20);
		CreateBody(p_bones, p_rootNode, p_transform, 1, btVector3(0,0,0), p_boneOffset);
		m_boneToFollow[13] = 13;
		////here be dragons nu �r det fel i min hj�rna
		
		

	}

	btRigidBody* Ragdoll::CreateBody(glm::mat4 p_bones[20], aiNode* p_rootNode, glm::mat4 p_transform, int p_massFactor, const btVector3& p_parentBodyOffset, glm::mat4 p_boneOffset[20] )
	{
		
		btCollisionShape* shape = CreateBone(p_rootNode->mName.data);
		if(shape != nullptr)
		{
			float mass;
			if(std::string(p_rootNode->mName.data).compare("Character1_Hips") == 0)
				m_hipsNode = p_rootNode;
			btRigidBody* body;
			if(std::string(p_rootNode->mName.data).compare("Character1_Hips") == 0 || std::string(p_rootNode->mName.data).compare("Character1_Spine") == 0)
				mass = 5.0f;
			else
				mass = 5.0f /*/ p_massFactor*/;
			btVector3 inertia = btVector3(0,0,0);
			shape->calculateLocalInertia(mass, inertia);
			
			int index = m_nameToIndex[p_rootNode->mName.data];
			glm::mat4 toGlm = glm::mat4();
			//body
		//	aiMatrix4x4 test = p_rootNode->mTransformation; 
			//memcpy(&toGlm[0][0], &test[0][0], sizeof(aiMatrix4x4));
			//glm::mat4 bonePos =  glm::transpose(toGlm);
			btTransform trans;
			glm::vec3 temp = glm::vec3(((p_boneOffset[index][3].x,p_boneOffset[index][3].y,p_boneOffset[index][3].z)));

			
			glm::mat4 test = glm::mat4(1.0f);
			test *= glm::translate(test, temp );
			m_boneOffset[index] = p_boneOffset[index];
			m_bodyPosOffset[index] += p_parentBodyOffset;
			//m_boneTransform[index] = bonePos;
			m_prevPos[index] = p_transform;
			//test
			m_lastBoneMatrix[index] = p_bones[index] /** test*/ * m_boneOffset[index];
			glm::mat4 toTrans ;
			if(index < 6)
			{
				if(index < 3) 
					toTrans = glm::rotate(p_transform, -90.0f, glm::vec3(0,0,1)) * /*test*/ m_lastBoneMatrix[index];
				else
					toTrans = glm::rotate(p_transform, 90.0f, glm::vec3(0,0,1)) * /* test*/ m_lastBoneMatrix[index];
			}
			else
			{
				toTrans =  p_transform  /** bonePos*/ * m_lastBoneMatrix[index];
			}
			
			//working - not for arms however
	//		if( index != 5 || index != 2)
	//			m_lastBoneMatrix[index] = p_bones[index] * m_boneOffset[index];
	//		else
//				m_lastBoneMatrix[index] = p_bones[index];
			
		//	glm::mat4 toTrans =  p_transform  /** bonePos*/ * m_lastBoneMatrix[index];	
			const float* data = glm::value_ptr(toTrans);
 			trans.setFromOpenGLMatrix(data);
			//if(index > 5)
			//{
				float x,y,z,w;
				x = trans.getRotation().y();
				y = trans.getRotation().z();
				z = trans.getRotation().w();
				w = trans.getRotation().x();
				trans.setRotation(btQuaternion(x,y,z,w));
			//}
			trans.setOrigin(trans.getOrigin() + m_bodyPosOffset[index]);
			btDefaultMotionState* motionstate = new btDefaultMotionState(trans);
			body = new btRigidBody(mass, motionstate, shape, inertia);
			body->setDamping(0.05f,0.85f);
			m_dynamicWorld->addRigidBody(body);
			body->setCcdMotionThreshold(0.7f);
			body->setCcdSweptSphereRadius(0.4f);
			body->setRestitution(1.3f);
			//body->setFriction(1.5f);
			m_bodies[index] = body;
			
			
			for(unsigned int i = 0; i < p_rootNode->mNumChildren; i++)
			{
				btRigidBody* childbody = CreateBody(p_bones, p_rootNode->mChildren[i],  p_transform,  p_massFactor+1 , m_bodyPosOffset[index], p_boneOffset);
				if(childbody != nullptr)
				{
					btTypedConstraint* temp = CreateConstraint(body, childbody, p_rootNode->mName.data, p_rootNode->mChildren[i]->mName.data);
					if(temp != nullptr)
					{
						m_joints[m_constraintCounter] = temp;
						m_constraintCounter++;
					}
					SetBoneRelation(index, m_nameToIndex[p_rootNode->mChildren[i]->mName.data]);
					
				}
				
			}
			return body;
		}
		for(unsigned int i = 0; i < p_rootNode->mNumChildren; i++)
			CreateBody(p_bones, p_rootNode->mChildren[i], p_transform,  p_massFactor, p_parentBodyOffset, p_boneOffset);
	
		return nullptr;
	}

	btCollisionShape* Ragdoll::CreateBone( std::string p_name )
	{
		//Nu ska vi g�ra allt till boxar f�r det �r coolare
		if(p_name.compare("Character1_Hips") == 0)
		{
			m_modelSpaceOffset[BodyPart::HIPS] = glm::vec3(0,0.1f, 0);
			return new btBoxShape(btVector3(0.4f, 0.2f, 0.4f));
			//return new btCapsuleShape(0.4f, 0.01f);
			//return new btCylinderShape(btVector3(0.4f,0.2f,0.4f));
		}
		else if(p_name.compare("Character1_LeftArm") == 0)
		{
			m_modelSpaceOffset[BodyPart::LEFTARM] = glm::vec3(0,0.15f, 0);
			return new btBoxShape(btVector3(0.3f, 0.2f, 0.2f));
			//return new btCapsuleShape(0.28f, 0.30f);	
			//return new btCylinderShape(btVector3(0.05f,0.15f,0.05f));
		}
		else if(p_name.compare("Character1_LeftFoot") == 0)
		{
			m_modelSpaceOffset[BodyPart::LEFTFOOT] = glm::vec3(0,0.05f, 0);
			return new btBoxShape(btVector3(0.25f, 0.1f, 0.25f));
			//return new btCapsuleShape(0.2f, 0.16f);
			//return new btCylinderShape(btVector3(0.1f,0.04f,0.1f));
		}
		else if(p_name.compare("Character1_LeftForeArm") == 0)
		{
			m_modelSpaceOffset[BodyPart::LEFTFOREARM] = glm::vec3(0,0.15f, 0);
			return new btBoxShape(btVector3(0.3f, 0.2f, 0.2f));
			//return new btCapsuleShape(0.28f, 0.30f);
			//return new btCylinderShape(btVector3(0.05f,0.15f,0.05f));
		}
		else if(p_name.compare("Character1_LeftHand") == 0)
		{
			m_modelSpaceOffset[BodyPart::LEFTHAND] = glm::vec3(0,0.1f, 0);
			return new btBoxShape(btVector3(0.25f, 0.1f, 0.1f));
			//return new btCapsuleShape(0.2f, 0.08f);
		//	return new btCylinderShape(btVector3(0.1f,0.04f,0.1f));
		}
		else if(p_name.compare("Character1_LeftLeg") == 0)
		{
			m_modelSpaceOffset[BodyPart::LEFTLEG] = glm::vec3(0,0.15f, 0);
			return new btBoxShape(btVector3(0.2f, 0.3f, 0.2f));
			//return new btCapsuleShape(0.20f, 0.36f);
			//return new btCylinderShape(btVector3(0.1f,0.2f,0.1f));
		}
		else if(p_name.compare("Character1_LeftUpLeg") == 0)
		{
			m_modelSpaceOffset[BodyPart::LEFTUPLEG] = glm::vec3(0,0.15f, 0);
			return new btBoxShape(btVector3(0.2f, 0.3f, 0.2f));
			//return new btCapsuleShape(0.2f, 0.36f);
			//return new btCylinderShape(btVector3(0.1f,0.2f,0.1f));
		}
		else if(p_name.compare("Character1_RightArm") == 0)
		{
			m_modelSpaceOffset[BodyPart::RIGHTARM] = glm::vec3(0,0.15f, 0);
			return new btBoxShape(btVector3(0.3f, 0.2f, 0.2f));
			//return new btCapsuleShape(0.28f, 0.3f);
			//return new btCylinderShape(btVector3(0.05f,0.15f,0.05f));
		}
		else if(p_name.compare("Character1_RightFoot") == 0)
		{
			m_modelSpaceOffset[BodyPart::RIGHTFOOT] = glm::vec3(0,0.05f, 0);
			return new btBoxShape(btVector3(0.25f, 0.1f, 0.25f));
		//	return new btCapsuleShape(0.2f, 0.08f);
			//return new btCylinderShape(btVector3(0.1f,0.04f,0.1f));
		}
		else if(p_name.compare("Character1_RightForeArm") == 0)
		{
			m_modelSpaceOffset[BodyPart::RIGHTFOREARM] = glm::vec3(0,0.15f, 0);
			return new btBoxShape(btVector3(0.3f, 0.2f, 0.2f));
			//return new btCapsuleShape(0.28f, 0.3f);
			//return new btCylinderShape(btVector3(0.05f,0.15f,0.05f));
		}
		else if(p_name.compare("Character1_RightHand") == 0)
		{
			m_modelSpaceOffset[BodyPart::RIGHTHAND] = glm::vec3(0,0.1f, 0);
			return new btBoxShape(btVector3(0.25f, 0.1f, 0.1f));
			//return new btCapsuleShape(0.28f, 0.08f);
			//return new btCylinderShape(btVector3(0.1f,0.04f,0.1f));
		}
		else if(p_name.compare("Character1_RightLeg") == 0)
		{
			m_modelSpaceOffset[BodyPart::RIGHTLEG] = glm::vec3(0,0.15f, 0);
			return new btBoxShape(btVector3(0.2f, 0.3f, 0.2f));
			//return new btCapsuleShape(0.20f, 0.26f);
			//return new btCylinderShape(btVector3(0.1f,0.25f,0.1f));
		}
		else if(p_name.compare("Character1_RightUpLeg") == 0)
		{
			m_modelSpaceOffset[BodyPart::RIGHTUPLEG] = glm::vec3(0,0.15f, 0);
			return new btBoxShape(btVector3(0.2f, 0.3f, 0.2f));
			//return new btCapsuleShape(0.20f, 0.36f);
			//return new btCylinderShape(btVector3(0.1f,0.25f,0.1f));
		}
		else if(p_name.compare("Character1_Spine") == 0)
		{
			m_modelSpaceOffset[BodyPart::SPINE] = glm::vec3(0,0.6f, 0);
			return new btBoxShape(btVector3(0.4f, 0.6f, 0.4f));
			//return new btCapsuleShape(0.4f,0.6f);
			//return new btCylinderShape(btVector3(0.4f,0.4f,0.4f));
		}

		return nullptr;
	}

	btVector3 Ragdoll::GetPos()
	{
		return m_bodies[BodyPart::HIPS]->getWorldTransform().getOrigin();
	}

	glm::mat4* Ragdoll::GetBones()
	{
		glm::mat4* retVal = new glm::mat4[20];
		retVal = FixPosition(retVal, m_hipsNode); 
		return retVal;
	//	
	//	for(int i = 13; i >=0; i--)
	//	{
	//		float data[16];
	//		btTransform trans = m_bodies[i]->getWorldTransform();
	//		
	//		float x,y,z,w;
	//		x = trans.getRotation().w();
	//		y = trans.getRotation().x();
	//		z = trans.getRotation().y();
	//		w = trans.getRotation().z();
	//		trans.setRotation(btQuaternion(x,y,z,w));
	//		trans.getOpenGLMatrix(data);
	//		
	//		int index  = m_boneToFollow[i];
	//		
	//		float test[16];
	//		btTransform worldTrans = m_bodies[BodyPart::HIPS]->getWorldTransform();
	//		x = worldTrans.getRotation().w();
	//		y = worldTrans.getRotation().x();
	//		z = worldTrans.getRotation().y();
	//		w = worldTrans.getRotation().z();
	//		worldTrans.setRotation(btQuaternion(x,y,z,w));
	//		worldTrans.getOpenGLMatrix(test);
	//		
	//		m_prevPos[i] =  glm::make_mat4(test) /* *  glm::inverse(m_lastBoneMatrix[i]) */;
	//		//World to local space
 //			retVal[i] = glm::inverse(m_prevPos[i])  /** glm::inverse(retVal[i])*/ *   glm::make_mat4(data) /* * m_lastBoneMatrix[i]*/; 

	//	
	//		retVal[i] *= m_boneOffset[i];
	//	}
	}

	btQuaternion Ragdoll::GetOrientation()
	{
		return m_bodies[BodyPart::HIPS]->getWorldTransform().getRotation();
	}

	void Ragdoll::SetOrientation( glm::quat p_orientation )
	{
		btQuaternion btquat;
		
		btquat.setX(p_orientation.y);
		btquat.setY(p_orientation.z);
		btquat.setZ(p_orientation.w);
		btquat.setW(p_orientation.x);
		m_bodies[BodyPart::HIPS]->getWorldTransform().setRotation(btquat);
	}

	btTypedConstraint* Ragdoll::CreateConstraint( btRigidBody* p_bodyA, btRigidBody* p_bodyB, std::string p_nameA, std::string p_nameB )
	{
		btTransform localA, localB;
		//Hips-spine
		if(p_nameA.compare("Character1_Hips") == 0 && p_nameB.compare("Character1_Spine") == 0 )
		{
			CalculateConstraintTransform(p_bodyA, p_bodyB, 
				0 , 0.20f * OFFSET, 0, 
				0, -0.20f * OFFSET, 0, 
				0 , 0, 0, 1, &localA, &localB);
			btConeTwistConstraint* constraint = new btConeTwistConstraint(*p_bodyA, *p_bodyB, localA, localB/*, btVector3(0,1,0), btVector3(0,-1,0)*/);
			constraint->setLimit(0,0,PI_2,0,1,1);
			//constraint->setDamping(0.85f);
			

			
			m_dynamicWorld->addConstraint(constraint);
			constraint->setDbgDrawSize(0.5f);
			return constraint;
		}
// 		//Hips - Left upper leg
 			else if(p_nameA.compare("Character1_Hips") == 0 && p_nameB.compare("Character1_LeftUpLeg") == 0 )
 			{
 				CalculateConstraintTransform(p_bodyA, p_bodyB,
 						0.28f , -0.0f * OFFSET, 0,
 					0, 0.25f * OFFSET, 0,
 					0 , 0, 0.92f, 0.38f, &localA, &localB);

 				btHingeConstraint* constraint = new btHingeConstraint(*p_bodyA, *p_bodyB, localA, localB);
				constraint->setLimit(- PI_2 , PI_2 / 2.0f);
 				m_dynamicWorld->addConstraint(constraint);
 				constraint->setDbgDrawSize(0.5f);
 				return constraint;
 			}
 			////Hips - Right upper leg
 			else if(p_nameA.compare("Character1_Hips") == 0 && p_nameB.compare("Character1_RightUpLeg") == 0 )
 			{
 				CalculateConstraintTransform(p_bodyA, p_bodyB, 
 					-0.28f , -0.0f * OFFSET, 0,
 					0, 0.25f * OFFSET, 0,
 					0 , 0, 0.92f, 0.38f, &localA, &localB);
 
 				btHingeConstraint* constraint = new btHingeConstraint(*p_bodyA, *p_bodyB, localA, localB);
 				constraint->setLimit(- PI_2 , PI_2 / 2.0f);
 				//btHingeConstraint* constraint = new btHingeConstraint(*p_bodyA, *p_bodyB, localA, localB);
 				
 				m_dynamicWorld->addConstraint(constraint);
 				constraint->setDbgDrawSize(0.5f);
 				return constraint;
 			}
 			//Left upper leg - Left lower leg
 		 	else if(p_nameA.compare("Character1_LeftUpLeg") == 0 && p_nameB.compare("Character1_LeftLeg") == 0 )
 		 	{
 				CalculateConstraintTransform(p_bodyA, p_bodyB, 
 					0.0f , -0.20f * OFFSET, 0,
 					0, 0.20f * OFFSET, 0,
 					0 , 0.7f, 0, 0.7f, &localA, &localB);
 	
 		 		btHingeConstraint* constraint = new btHingeConstraint(*p_bodyA, *p_bodyB, localA, localB);
 		 		constraint->setLimit(- PI_2 , PI_2 / 2.0f);
 		 		m_dynamicWorld->addConstraint(constraint);
 		 		constraint->setDbgDrawSize(0.5f);
 		 		return constraint;
 		 	}
 		 	//Right upper leg - Right lower leg
 		 	else if(p_nameA.compare("Character1_RightUpLeg") == 0 && p_nameB.compare("Character1_RightLeg") == 0 )
 		 	{
 				CalculateConstraintTransform(p_bodyA, p_bodyB, 
 					0.0f , -0.20f * OFFSET, 0,
 					0, 0.20f * OFFSET, 0,
 					0 , 0.7f, 0, 0.7f, &localA, &localB);
 		 	
 		 		btHingeConstraint* constraint = new btHingeConstraint(*p_bodyA, *p_bodyB, localA, localB);
 		 		constraint->setLimit(- PI_2 , PI_2 / 2.0f);
 		 		m_dynamicWorld->addConstraint(constraint);
 		 		constraint->setDbgDrawSize(0.5f);
 		 		return constraint;
 		 	}
 		 	//Left lower leg - Left foot
 		 	else if(p_nameA.compare("Character1_LeftLeg") == 0 && p_nameB.compare("Character1_LeftFoot") == 0 )
 		 	{
 				CalculateConstraintTransform(p_bodyA, p_bodyB, 
 					0.0f , -0.20f * OFFSET, 0,
 					0, 0.15f * OFFSET, 0,
 					0 , 0.7f, 0, 0.7f, &localA, &localB);
 		 
 		 		btHingeConstraint* constraint = new btHingeConstraint(*p_bodyA, *p_bodyB, localA, localB);
 		 		constraint->setLimit(- PI_2 , PI_2 / 2.0f);
 		 		m_dynamicWorld->addConstraint(constraint);
 		 		constraint->setDbgDrawSize(0.5f);
 		 		return constraint;
 		 	}
 		 	//Right lower leg - Right foot
 		 	else if(p_nameA.compare("Character1_RightLeg") == 0 && p_nameB.compare("Character1_RightFoot") == 0 )
 		 	{
 				CalculateConstraintTransform(p_bodyA, p_bodyB, 
 					0.0f , -0.20f * OFFSET, 0,
 					0, 0.15f * OFFSET, 0,
 					0 , 0.7f, 0, 0.7f, &localA, &localB);
 		 		
 		 		btHingeConstraint* constraint = new btHingeConstraint(*p_bodyA, *p_bodyB, localA, localB);
 		 		constraint->setLimit(-PI_2, PI_2 / 2.0f);
 		 		m_dynamicWorld->addConstraint(constraint);
 		 		constraint->setDbgDrawSize(0.5f);
 		 		return constraint;
 		 	}
 		 	//Spine - Left upper arm
 		 	else if(p_nameA.compare("Character1_Spine") == 0 && p_nameB.compare("Character1_LeftArm") == 0 )
 		 	{
 				CalculateConstraintTransform(p_bodyA, p_bodyB, 
 					-0.00f , 0.0f * OFFSET, 0,
 					0.2f, -0.0f * OFFSET, 0,
 					0 , 0, 1, 0, &localA, &localB);
 		 	
 		 		btConeTwistConstraint* constraint = new btConeTwistConstraint(*p_bodyA, *p_bodyB, localA, localB);
 		 		constraint->setLimit(-PI_2, PI_2 / 2.0f);
 		 		m_dynamicWorld->addConstraint(constraint);
 		 		constraint->setDbgDrawSize(0.5f);
 		 		return constraint;
 		 	}
 		 	////Spine - Right upper arm
			else if(p_nameA.compare("Character1_Spine") == 0 && p_nameB.compare("Character1_RightArm") == 0 )
			{
				CalculateConstraintTransform(p_bodyA, p_bodyB, 
					0.00f , 0.0f * OFFSET, 0,
					-0.2f, -0.00f * OFFSET, 0,
					0 , 0, 1, 0, &localA, &localB);

				btConeTwistConstraint* constraint = new btConeTwistConstraint(*p_bodyA, *p_bodyB, localA, localB);
				constraint->setLimit(-PI_2, PI_2 / 2.0f);
				m_dynamicWorld->addConstraint(constraint);
				constraint->setDbgDrawSize(0.5f);
				return constraint;
			}
 		 	////Left upper arm - Left lower arm
 		 	else if(p_nameA.compare("Character1_LeftArm") == 0 && p_nameB.compare("Character1_LeftForeArm") == 0 )
 		 	{
 				CalculateConstraintTransform(p_bodyA, p_bodyB, 
 					-0.2f , -0.0f * OFFSET, 0,
 					0.15f, 0.0f * OFFSET, 0,
 					0 , 0.7f, 0, 0.7f, &localA, &localB);
 		 	
 		 		btHingeConstraint* constraint = new btHingeConstraint(*p_bodyA, *p_bodyB, localA, localB);
 		 		constraint->setLimit(- PI_2 , PI_2 / 2.0f);
 		 		m_dynamicWorld->addConstraint(constraint);
 		 		constraint->setDbgDrawSize(0.5f);
 		 		return constraint;
 		 	}
 		 	//Right upper arm - Right lower arm
 		 	else if(p_nameA.compare("Character1_RightArm") == 0 && p_nameB.compare("Character1_RightForeArm") == 0 )
 		 	{
 				CalculateConstraintTransform(p_bodyA, p_bodyB, 
 					0 , -0.20f * OFFSET, 0,
 					0, 0.15f * OFFSET, 0,
 					0 , 0.7f, 0, 0.7f, &localA, &localB);
 		 		
 		 		btHingeConstraint* constraint = new btHingeConstraint(*p_bodyA, *p_bodyB, localA, localB);
 		 		constraint->setLimit(- PI_2 , PI_2 / 2.0f);
 		 		m_dynamicWorld->addConstraint(constraint);
 		 		constraint->setDbgDrawSize(0.5f);
 		 		return constraint;
 		 	}
 		 	//Left lower arm - Left hand
 		 	else if(p_nameA.compare("Character1_LeftForeArm") == 0 && p_nameB.compare("Character1_LeftHand") == 0 )
 		 	{
 				CalculateConstraintTransform(p_bodyA, p_bodyB, 
 					0.1f , 0.00f * OFFSET, 0,
 					-0.2f, -0.0f * OFFSET, 0,
 					0 , 0.7f, 0, 0.7f, &localA, &localB);
 		 		
 		 		btHingeConstraint* constraint = new btHingeConstraint(*p_bodyA, *p_bodyB, localA, localB);
 					constraint->setLimit(- PI_2 , PI_2 / 2.0f);
 		 		m_dynamicWorld->addConstraint(constraint);
 		 		constraint->setDbgDrawSize(0.5f);
 		 		return constraint;
 		 	}
 		 	//Right lower arm - Right hand
 		 	else if(p_nameA.compare("Character1_RightForeArm") == 0 && p_nameB.compare("Character1_RightHand") == 0 )
 		 	{
 				CalculateConstraintTransform(p_bodyA, p_bodyB, 
 					0.0f , -0.1f * OFFSET, 0,
 					0.0, 0.2f * OFFSET, 0,
 					0 , 0.7f, 0, 0.7f, &localA, &localB);
 		 		
 		 		btHingeConstraint* constraint = new btHingeConstraint(*p_bodyA, *p_bodyB, localA, localB);
 				constraint->setLimit(- PI_2 , PI_2 / 2.0f);
 		 		m_dynamicWorld->addConstraint(constraint);
 		 		constraint->setDbgDrawSize(0.5f);
 		 		return constraint;
 		 	}
		return nullptr;
	}

	void Ragdoll::SetBoneRelation( int p_parentIndex, int p_childIndex )
	{
		m_boneToFollow[p_childIndex] = p_parentIndex;
	//	//btTransform childTrans = m_bodies[p_childIndex]->getWorldTransform();
	//	btTransform parentTrans = m_bodies[p_childIndex]->getWorldTransform();
	//	float data[16];
	//	//childTrans.getOpenGLMatrix(data);
	////	glm::mat4 childMat = glm::make_mat4(data);
	//	glm::mat4 childMat = m_lastUpdatePos[p_childIndex];
	//	childMat = childMat * p_pose;
	//	parentTrans.getOpenGLMatrix(data);
	//	float data2[16];
	//	parentTrans.getOpenGLMatrix(data2);
	//	glm::mat4 parentMat = glm::make_mat4(data2);
	//	parentMat =  glm::inverse(parentMat);
		
		//m_boneTransform[p_childIndex] = childMat * parentMat;
	}

	void Ragdoll::SetVelocity( const btVector3& p_velocity )
	{
		//for(int i = 0; i < BodyPart::TOTAL_BONE_AMUNT; i++)
			m_bodies[BodyPart::HIPS]->setLinearVelocity(p_velocity);
	}

	//The magic function of constraint calculations,
	void Ragdoll::CalculateConstraintTransform(btRigidBody* p_bodyA, btRigidBody* p_bodyB,  float p_offsetXbodyA , float p_offsetYbodyA , float p_offsetZbodyA , 
		float p_offsetXbodyB  , float p_offsetYbodyB, float p_offsetZbodyB,
		float p_quatX, float p_quatY, float p_quatZ, float p_quatW, btTransform* p_transBodyA, btTransform* p_transBodyB  )
	{
		glm::mat4 bodyAGlm, bodyBGlm;
		btTransform bodyTransformA = p_bodyA->getWorldTransform();
		btTransform bodyTransformB = p_bodyB->getWorldTransform();
		float toGlmA[16], toGlmB[16];
		bodyTransformA.getOpenGLMatrix(toGlmA);
		bodyAGlm = glm::make_mat4(toGlmA);
		bodyTransformB.getOpenGLMatrix(toGlmB);
		bodyBGlm = glm::make_mat4(toGlmB);
		//Offset vector
		glm::vec4 offsetA = glm::vec4(p_offsetXbodyA, p_offsetYbodyA, p_offsetZbodyA, 1);
		glm::vec4 offsetB = glm::vec4(p_offsetXbodyB, p_offsetYbodyB, p_offsetZbodyB, 1);
		//To world space
		glm::vec3 offsetAWorld = glm::vec3(bodyAGlm * offsetA);
		glm::vec3 offsetBWorld =  glm::vec3(bodyBGlm * offsetB) ;
	
		//And back to bullet vectors again
		btVector3 pointA = btVector3(offsetAWorld.x, offsetAWorld.y, offsetAWorld.z);
		btVector3 pointB = btVector3(offsetBWorld.x, offsetBWorld.y, offsetBWorld.z);

		btVector3 constraintOrigin = btVector3(pointA.x() + ((pointB.x() - pointA.x()) / 2.0f),
			pointA.y() + ((pointB.y() - pointA.y()) / 2.0f),
			pointA.z() + ((pointB.z() - pointA.z()) / 2.0f));

		btTransform constraintWorld ;
		constraintWorld.setIdentity();
		constraintWorld.setOrigin(constraintOrigin);
		constraintWorld.setRotation(btQuaternion(p_quatX, p_quatY, p_quatZ, p_quatW));
		p_transBodyA->setIdentity();
		*p_transBodyA = bodyTransformA.inverse() * constraintWorld;

		p_transBodyB->setIdentity();
		*p_transBodyB = bodyTransformB.inverse() * constraintWorld;
	}

	glm::mat4* Ragdoll::FixPosition( glm::mat4 p_bones[20], aiNode* p_rootNode )
	{
		int myIndex = m_nameToIndex[p_rootNode->mName.data];
		float data[16];
		btTransform trans = m_bodies[myIndex]->getWorldTransform();
		float x,y,z,w;
		//btVector3 offseteru = btVector3(m_modelSpaceOffset[myIndex].x, m_modelSpaceOffset[myIndex].y, m_modelSpaceOffset[myIndex].z);
		//trans.setOrigin(trans.getOrigin() - offseteru );
		trans.getOpenGLMatrix(data);
		int index  = m_boneToFollow[myIndex];
		float test[16];
		btTransform worldTrans = m_bodies[BodyPart::HIPS]->getWorldTransform();
		x = worldTrans.getRotation().w();
		y = worldTrans.getRotation().x();
		z = worldTrans.getRotation().y();
		w = worldTrans.getRotation().z();
		worldTrans.setRotation(btQuaternion(x,y,z,w));
		worldTrans.getOpenGLMatrix(test);


		//rotate the modelspace offset according to the shape rotation
	//
		
		btVector3 rot = trans.getRotation().getAxis();
		float angle = trans.getRotation().getAngle();
		glm::vec3 rotateByGlm = glm::vec3(rot.x(), rot.y(), rot.z());
		glm::mat4 rotateMat = glm::mat4(1.0f);
		rotateMat = glm::rotate(rotateMat, angle, rotateByGlm);
		glm::vec3 offsetValue = glm::vec3(glm::vec4(m_modelSpaceOffset[myIndex], 1) * rotateMat);
		/*btTransform testoi = worldTrans.inverse() * trans;
		testoi.getOpenGLMatrix(test);
		p_bones[myIndex] = glm::make_mat4(test);*/
		m_prevPos[myIndex] =  glm::make_mat4(test) /* *  glm::inverse(m_lastBoneMatrix[myIndex]) */;
		//World to local space
		p_bones[myIndex] = /*glm::inverse(m_boneTransform[myIndex]) **/ (glm::inverse(m_prevPos[myIndex])  /** glm::inverse(retVal[myIndex])*/ *   glm::make_mat4(data)) /* * m_lastBoneMatrix[myIndex]*/; 
		
		//p_bones[myIndex] = glm::rotate(p_bones[myIndex], 180.0f, glm::vec3(1,0,0));
		
		p_bones[myIndex] *= m_boneOffset[myIndex];
		//p_bones[myIndex][3] -= (glm::vec4(offsetValue,0) * 2); // this does'nt work SADFACE try different orderrs or something
		//p_bones[myIndex][3].y -= (m_modelSpaceOffset[myIndex] * 2); // Verkar bero p� vilket h�ll man �r vinklad om man ska ta - p� x,y eller z. M�ste komma p� n�n generell fix.
		
//#pragma warning	NOTE TO SELF: Borde g� att l�sa att modellens start punkt �r i kubernas mitt genom att offsetta p_bones y v�rde med halva boxens h�jd, kanske eventuellt, men nu, mot arm helveterna och vidare!
		for(unsigned int i = 0; i < p_rootNode->mNumChildren; i++)
		{
			FixPosition(p_bones, p_rootNode->mChildren[i]);
		}
				return p_bones;
	}

	/*glm::mat4* Ragdoll::FixPosition( glm::mat4 p_bones[20], aiNode* p_rootNode )
	{
		int myIndex = m_nameToIndex[p_rootNode->mName.data];
		for(unsigned int i = 0; i < p_rootNode->mNumChildren; i++)
		{
			int childIndex = m_nameToIndex[p_rootNode->mChildren[i]->mName.data];
			(p_bones[childIndex]) = (p_bones[childIndex]) * glm::inverse((p_bones[myIndex]));
			FixPosition(p_bones, p_rootNode->mChildren[i]);
			
			
		}
		return p_bones;
	}*/

}