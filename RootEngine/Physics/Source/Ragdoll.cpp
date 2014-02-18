
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
		Deactivate();
	}



	void Ragdoll::Deactivate()
	{
		//Remove bodies from world, 
		for(int i = 0; i < BodyPart::TOTAL_BONE_AMUNT ; i++)
		{
			m_bodies[i]->clearForces();
			m_dynamicWorld->removeRigidBody(m_bodies[i]);
		}
		for(int i = 0; i < m_constraintCounter; i++)
		{
			m_dynamicWorld->removeConstraint(m_joints[i]);
		}
	}


	void Ragdoll::BuildRagdoll(glm::mat4 p_bones[20], aiNode* p_rootNode, std::map<std::string, int>  p_nameToIndex, glm::mat4 p_transform, glm::mat4 p_boneOffset[20], glm::vec3 p_right)
	{
		
		m_bodyPosOffset[BodyPart::SPINE] = btVector3(0, 0.57f, 0);

		m_bodyPosOffset[BodyPart::LEFTUPLEG] = btVector3(-0.0, -0.30f ,0);
		m_bodyPosOffset[BodyPart::RIGHTUPLEG] = btVector3(0.0, -0.30f ,0);
		//m_bodyPosOffset[BodyPart::HIPS]= btVector3(0.0f , -0.4f, 0.0f);
		m_nameToIndex = p_nameToIndex;
		m_right = p_right;
		CreateBody(p_bones, p_rootNode, p_transform, 1, btVector3(0,0,0), p_boneOffset);
		m_boneToFollow[13] = 13;
		////here be dragons nu är det fel i min hjärna
		
		

	}

	btRigidBody* Ragdoll::CreateBody(glm::mat4 p_bones[20], aiNode* p_rootNode, glm::mat4 p_transform, int p_massFactor, const btVector3& p_parentBodyOffset, glm::mat4 p_boneOffset[20] )
	{
		
		//Create shapes for the bones, these values are hardcoded atm, for a more dynamical approach they should preferably be written in some sort of file.
		btCollisionShape* shape = CreateBone(p_rootNode->mName.data);
		if(shape != nullptr)
		{
			float mass;
			//set the hips as startpoint for traveling the tree for when getbones is called
			if(std::string(p_rootNode->mName.data).compare("Character1_Hips") == 0)
				m_hipsNode = p_rootNode;
			btRigidBody* body;
			//Set mass, Hips and spine should be the same to make it more stable, the rest weighs less and less the deeper down they are in the animation tree.
			if(std::string(p_rootNode->mName.data).compare("Character1_Hips") == 0 || std::string(p_rootNode->mName.data).compare("Character1_Spine") == 0)
				mass = 5.0f;
			else
				mass = 5.0f / p_massFactor;

			btVector3 inertia = btVector3(0,0,0);
			shape->calculateLocalInertia(mass, inertia);
			//Get bone index from name
			int index = m_nameToIndex[p_rootNode->mName.data];
			//Build transform in glm
			m_boneOffset[index] = p_boneOffset[index];
			m_bodyPosOffset[index] += p_parentBodyOffset;
			m_prevPos[index] = p_transform;
			m_lastBoneMatrix[index] =  p_bones[index] /** m_boneOffset[index]*/;
			glm::mat4 toTrans = glm::mat4(1.0f);
			
			////Rotate the arms, for some reason this is required
// 			if(index < 6)
// 			{
// 				if(index < 3) 
// 					toTrans *= glm::rotate(p_transform, 90.0f, m_right) *  m_lastBoneMatrix[index];
// 				else
// 					toTrans *= glm::rotate(p_transform, -90.0f, m_right) *   m_lastBoneMatrix[index];
// 
// 			}
// 			else
 			{
				toTrans *=  p_transform  /** bonePos*/ * m_lastBoneMatrix[index];
				
				
			}
 			
			//if(index > 5)
				toTrans *=  m_boneOffset[index];
			
				if(index < 6)
				{
					toTrans *= glm::rotate(glm::mat4(1.0f), 180.0f, m_right);
				}
 		//	if( index == BodyPart::RIGHTARM || index == BodyPart::LEFTARM)
 		//	{
 		//		toTrans[3].y = (m_bodies[BodyPart::SPINE]->getWorldTransform().getOrigin().y() - 0.3f);
 		//		if(index == BodyPart::RIGHTARM)
 		//		{
 		//			toTrans[3].x = (m_bodies[BodyPart::SPINE]->getWorldTransform().getOrigin().x() + m_right.x* 0.8f);
 		//			toTrans[3].z  = (m_bodies[BodyPart::SPINE]->getWorldTransform().getOrigin().z() + m_right.z* 0.8f);
 		//			/*m_bodyPosOffset[index].setX((m_bodyPosOffset[index].getX() + (m_bodies[BodyPart::SPINE]->getWorldTransform().getOrigin().x() - toTrans[3].x) * 0.8f) / ** m_right.x* /);
 		//			m_bodyPosOffset[index].setZ((m_bodyPosOffset[index].getZ() + (m_bodies[BodyPart::SPINE]->getWorldTransform().getOrigin().z() - toTrans[3].z) * 0.8f) / ** m_right.z* /);*/
 		//		}
 		//		else
 		//		{
 		//			toTrans[3].x = (m_bodies[BodyPart::SPINE]->getWorldTransform().getOrigin().x() - m_right.x* 0.8f);
 		//			toTrans[3].z = (m_bodies[BodyPart::SPINE]->getWorldTransform().getOrigin().z() - m_right.z* 0.8f);
 		//			/*m_bodyPosOffset[index].setX((m_bodyPosOffset[index].getX() + (m_bodies[BodyPart::SPINE]->getWorldTransform().getOrigin().x() - toTrans[3].x) * 0.8f) / ** m_right.x* /);
 		//			m_bodyPosOffset[index].setZ((m_bodyPosOffset[index].getZ() + (m_bodies[BodyPart::SPINE]->getWorldTransform().getOrigin().z() - toTrans[3].z) * 0.8f) / ** m_right.z* /);*/
 		//		}
 		//	}
 		//	if(index < BodyPart::LEFTARM && index != BodyPart::RIGHTARM)
 		//	{
 		//		btVector3 btAxis = (m_bodies[index + 1]->getWorldTransform().getRotation().getAxis());
 		//		glm::vec3 axis = glm::vec3(btAxis.y(), btAxis.x(), btAxis.z());
 		//	//	glm::vec3 rot = axis * 0.4f;
 		//		glm::vec3 rot = m_boneShapeSize[index] *  axis ;
 		//		//glm::vec3 rot = glm::rotate(glm::vec3(0.0f, 0.3f, 0.0f), m_bodies[index + 1]->getWorldTransform().getRotation().getAngle(), axis);
 		//		toTrans[3].y = m_bodies[index + 1]->getWorldTransform().getOrigin().y() - rot.y;
 		//		if(index < 2)
 		//		{
 		//			toTrans[3].x = (m_bodies[index + 1]->getWorldTransform().getOrigin().x() - rot.x);
 		//			toTrans[3].z = (m_bodies[index + 1]->getWorldTransform().getOrigin().z() - rot.z);
 		//			
 		//		}
 		//		else
 		//		{
 		//			toTrans[3].x = (m_bodies[index + 1]->getWorldTransform().getOrigin().x() - rot.x);
 		//			toTrans[3].z = (m_bodies[index + 1]->getWorldTransform().getOrigin().z() - rot.z);
 		//			//toTrans[3].y = m_bodies[index + 1]->getWorldTransform().getOrigin().y() - (m_boneShapeSize[index].y + m_boneShapeSize[index + 1].y);
 		//		}
			//	
			//		
			//}
			
			/*if(index < 6)
			{

				glm::vec3 front = glm::cross(m_right, glm::vec3(0,1,0));
				//	toTrans *= glm::rotate(glm::mat4(1.0f), 180.0f, m_right);

				if(index < 3)
					toTrans *= glm::rotate(glm::mat4(1.0f), 90.0f, m_right);
				else
					toTrans *= glm::rotate(glm::mat4(1.0f), 90.0f,m_right);
			}*/
			//From glm to bullet transform
			btTransform trans;
			const float* data = glm::value_ptr(toTrans);
 			trans.setFromOpenGLMatrix(data);
			
			//Glm and bullet represents quaternions differently, move from glm to bullet quaternion
			float x,y,z,w;
			x = trans.getRotation().y();
			y = trans.getRotation().z();
			z = trans.getRotation().w();
			w = trans.getRotation().x();
			trans.setRotation(btQuaternion(x,y,z,w));
			//Make a slight hardcoded (bad bad hardcode) offset to avoid shapes spawning inside each other which would cause huge jitter
			
			trans.setOrigin(trans.getOrigin() + m_bodyPosOffset[index]);

			
			//Set up the bullet rigidbody and add it to the world
			btDefaultMotionState* motionstate = new btDefaultMotionState(trans);
			body = new btRigidBody(mass, motionstate, shape, inertia);
			body->setDamping(0.05f,0.85f);
			m_dynamicWorld->addRigidBody(body);
			body->setCcdMotionThreshold(0.6f);
			body->setCcdSweptSphereRadius(0.4f);
			body->setRestitution(1.0f);
			body->setActivationState(DISABLE_DEACTIVATION);
			//This is supposed to stop collision  with abilities and other players, doesn't work atm
			body->setCollisionFlags(btBroadphaseProxy::DebrisFilter);
			m_bodies[index] = body;
			
			//create children and constraints
			for(unsigned int i = 0; i < p_rootNode->mNumChildren; i++)
			{
				btRigidBody* childbody = CreateBody(p_bones, p_rootNode->mChildren[i], p_transform,  p_massFactor+1 , m_bodyPosOffset[index], p_boneOffset);
				if(childbody != nullptr)
				{
					//"link" a node to its parent
					SetBoneRelation(index, m_nameToIndex[p_rootNode->mChildren[i]->mName.data]);
					
					//Create constraint
					btTypedConstraint* temp = CreateConstraint(body, childbody, p_rootNode->mName.data, p_rootNode->mChildren[i]->mName.data);
					if(temp != nullptr)
					{
						
						m_joints[m_constraintCounter] = temp;
						m_constraintCounter++;
					}
				}
				
			}
			return body;
		}
		//If the node don't have a specified boneshape
		for(unsigned int i = 0; i < p_rootNode->mNumChildren; i++)
			CreateBody(p_bones, p_rootNode->mChildren[i], p_transform,  p_massFactor, p_parentBodyOffset, p_boneOffset);
	
		return nullptr;
	}

	btCollisionShape* Ragdoll::CreateBone( std::string p_name )
	{
		
		if(p_name.compare("Character1_Hips") == 0)
		{
			m_boneShapeSize[BodyPart::HIPS] = glm::vec3(0,0.1f, 0);
			return new btBoxShape(btVector3(0.4f, 0.2f, 0.4f));
		}
		
		else if(p_name.compare("Character1_LeftFoot") == 0)
		{
			m_boneShapeSize[BodyPart::LEFTFOOT] = glm::vec3(0,0.05f, 0);
			return new btBoxShape(btVector3(0.25f, 0.1f, 0.25f));
		}
		
		else if(p_name.compare("Character1_LeftLeg") == 0)
		{
			m_boneShapeSize[BodyPart::LEFTLEG] = glm::vec3(0,0.15f, 0);
			return new btBoxShape(btVector3(0.2f, 0.3f, 0.2f));
		}
		else if(p_name.compare("Character1_LeftUpLeg") == 0)
		{
			m_boneShapeSize[BodyPart::LEFTUPLEG] = glm::vec3(0,0.15f, 0);
			return new btBoxShape(btVector3(0.2f, 0.3f, 0.2f));
		}
		
		else if(p_name.compare("Character1_RightFoot") == 0)
		{
			m_boneShapeSize[BodyPart::RIGHTFOOT] = glm::vec3(0,0.05f, 0);
			return new btBoxShape(btVector3(0.25f, 0.1f, 0.25f));
		}
		
		else if(p_name.compare("Character1_RightLeg") == 0)
		{
			m_boneShapeSize[BodyPart::RIGHTLEG] = glm::vec3(0,0.15f, 0);
			return new btBoxShape(btVector3(0.2f, 0.3f, 0.2f));
		}
		else if(p_name.compare("Character1_RightUpLeg") == 0)
		{
			m_boneShapeSize[BodyPart::RIGHTUPLEG] = glm::vec3(0,0.15f, 0);
			return new btBoxShape(btVector3(0.2f, 0.3f, 0.2f));
		}
		else if(p_name.compare("Character1_Spine") == 0)
		{
			m_boneShapeSize[BodyPart::SPINE] = glm::vec3(0,-0.40f, 0);
			return new btBoxShape(btVector3(0.4f, 0.6f, 0.4f));
		}
		else if(p_name.compare("Character1_LeftArm") == 0)
		{
			m_boneShapeSize[BodyPart::LEFTARM] = glm::vec3(-0.120f,0.0f, 0);
			return new btBoxShape(btVector3(0.24f, 0.2f, 0.2f));
		}
		else if(p_name.compare("Character1_LeftForeArm") == 0)
		{
			m_boneShapeSize[BodyPart::LEFTFOREARM] = glm::vec3(-0.12f,0.0f, 0);
			return new btBoxShape(btVector3(0.24f, 0.2f, 0.2f));
		}
		else if(p_name.compare("Character1_LeftHand") == 0)
		{
			m_boneShapeSize[BodyPart::LEFTHAND] = glm::vec3(-0.125f,0.0f, 0);
			return new btBoxShape(btVector3(0.25f, 0.1f, 0.1f));
		}
		else if(p_name.compare("Character1_RightArm") == 0)
		{
			m_boneShapeSize[BodyPart::RIGHTARM] = glm::vec3(0.12f,0.0f, 0);
			return new btBoxShape(btVector3(0.24f, 0.2f, 0.2f));
		}
		else if(p_name.compare("Character1_RightForeArm") == 0)
		{
			m_boneShapeSize[BodyPart::RIGHTFOREARM] = glm::vec3(0.12f,0.0f, 0);
			return new btBoxShape(btVector3(0.24f, 0.2f, 0.2f));
		}
		else if(p_name.compare("Character1_RightHand") == 0)
		{
			m_boneShapeSize[BodyPart::RIGHTHAND] = glm::vec3(0.125f,0.0f, 0);
			return new btBoxShape(btVector3(0.25f, 0.1f, 0.1f));
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
		//Recursive function the travels the animation tree with the hips as startpoint
		FixPosition(retVal, m_hipsNode); 
		return retVal;
	}

	btQuaternion Ragdoll::GetOrientation()
	{
		//return btQuaternion(0,1,0,1);
		return m_bodies[BodyPart::HIPS]->getWorldTransform().getRotation();
	}

	void Ragdoll::SetOrientation( glm::quat p_orientation )
	{
		/*	btQuaternion btquat;

		btquat.setX(p_orientation.y);
		btquat.setY(p_orientation.z);
		btquat.setZ(p_orientation.w);
		btquat.setW(p_orientation.x);
		m_bodies[BodyPart::HIPS]->getWorldTransform().setRotation(btquat);*/
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
			//btHingeConstraint* constraint = new btHingeConstraint(*p_bodyA, *p_bodyB, localA, localB);
		//	constraint->setLimit(0 ,0 );


			m_dynamicWorld->addConstraint(constraint,true);
			constraint->setDbgDrawSize(0.5f);
			return constraint;
		}
		// //Hips - Left upper leg
		else if(p_nameA.compare("Character1_Hips") == 0 && p_nameB.compare("Character1_LeftUpLeg") == 0 )
		{
			
			glm::vec3 rotFix = -m_right * (0.28f);
			CalculateConstraintTransform(p_bodyA, p_bodyB,
				/*0.28f , -0.0f * OFFSET, 0,*/
				rotFix.x, rotFix.y, rotFix.z,
				0, 0.25f * OFFSET, 0,
				0 , 0, 0.92f, 0.38f, &localA, &localB);

			btHingeConstraint* constraint = new btHingeConstraint(*p_bodyA, *p_bodyB, localA, localB);
			constraint->setLimit(- PI_2 , PI_2 );
			m_dynamicWorld->addConstraint(constraint);
			constraint->setDbgDrawSize(0.5f);
			return constraint;
		}
 			////Hips - Right upper leg
 			else if(p_nameA.compare("Character1_Hips") == 0 && p_nameB.compare("Character1_RightUpLeg") == 0 )
 			{
				glm::vec3 rotFix = m_right * (0.28f);
				CalculateConstraintTransform(p_bodyA, p_bodyB,
					/*0.28f , -0.0f * OFFSET, 0,*/
					rotFix.x, rotFix.y, rotFix.z,
					0, 0.25f * OFFSET, 0,
					0 , 0, 0.92f, 0.38f, &localA, &localB);
 
 				btHingeConstraint* constraint = new btHingeConstraint(*p_bodyA, *p_bodyB, localA, localB);
 				constraint->setLimit(- PI_2 , PI_2 );			
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
 		 		constraint->setLimit(- PI_2 , PI_2 );
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
 		 		constraint->setLimit(- PI_2 , PI_2 );
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
 		 		constraint->setLimit(- PI_2 , PI_2 );
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
 		 		constraint->setLimit(-PI_2, PI_2 );
 		 		m_dynamicWorld->addConstraint(constraint);
 		 		constraint->setDbgDrawSize(0.5f);
 		 		return constraint;
 		 	}
 		 	//Spine - Left upper arm
 			else if(p_nameA.compare("Character1_Spine") == 0 && p_nameB.compare("Character1_LeftArm") == 0 )
			{
				CalculateConstraintTransform(p_bodyA, p_bodyB, 
					0, 0.0f , 0,
					-0, 0.0f * OFFSET, 0,
					0 , 0, 1, 0, &localA, &localB); 	
			 	btConeTwistConstraint* constraint = new btConeTwistConstraint(*p_bodyA, *p_bodyB, localA, localB);
			 	constraint->setLimit(-PI_2, 0, PI_2);
			 	m_dynamicWorld->addConstraint(constraint);
			 	constraint->setDbgDrawSize(0.5f);
			 	return constraint;
			}
			////Spine - Right upper arm
			else if(p_nameA.compare("Character1_Spine") == 0 && p_nameB.compare("Character1_RightArm") == 0 )
			{
				CalculateConstraintTransform(p_bodyA, p_bodyB, 
					0, 0.0f , 0,
					-0, 0.0f * OFFSET, 0,
					0 , 0, 1, 0, &localA, &localB); 	
				btConeTwistConstraint* constraint = new btConeTwistConstraint(*p_bodyA, *p_bodyB, localA, localB);
				constraint->setLimit(-PI_2, 0, PI_2);
				m_dynamicWorld->addConstraint(constraint);
				constraint->setDbgDrawSize(0.5f);
				return constraint;
			}
 		// 	////Left upper arm - Left lower arm
 		 	else if(p_nameA.compare("Character1_LeftArm") == 0 && p_nameB.compare("Character1_LeftForeArm") == 0 )
 		 	{
 				CalculateConstraintTransform(p_bodyA, p_bodyB, 
 					-0.0f , -0.0f * OFFSET, 0,
 					0.0f, 0.0f * OFFSET, 0,
 					0 , 0.7f, 0, 0.7f, &localA, &localB);
 		 	
 		 		btHingeConstraint* constraint = new btHingeConstraint(*p_bodyA, *p_bodyB, localA, localB);
 		 		constraint->setLimit(- PI_2 , PI_2 );
 		 		m_dynamicWorld->addConstraint(constraint);
 		 		constraint->setDbgDrawSize(0.5f);
 		 		return constraint;
 		 	}
 		 	//Right upper arm - Right lower arm
 		 	else if(p_nameA.compare("Character1_RightArm") == 0 && p_nameB.compare("Character1_RightForeArm") == 0 )
 		 	{
 				CalculateConstraintTransform(p_bodyA, p_bodyB, 
 					-0.0f , -0.00f * OFFSET, 0,
 					0.0f, 0.0f * OFFSET, 0,
 					0 , 0.7f, 0, 0.7f, &localA, &localB);
 		 		
 		 		btHingeConstraint* constraint = new btHingeConstraint(*p_bodyA, *p_bodyB, localA, localB);
 		 		constraint->setLimit(- PI_2 , PI_2 );
 		 		m_dynamicWorld->addConstraint(constraint);
 		 		constraint->setDbgDrawSize(0.5f);
 		 		return constraint;
 		 	}
 		 	//Left lower arm - Left hand
 		 	else if(p_nameA.compare("Character1_LeftForeArm") == 0 && p_nameB.compare("Character1_LeftHand") == 0 )
 		 	{
 				CalculateConstraintTransform(p_bodyA, p_bodyB, 
 					-0.0f , -0.0f * OFFSET, 0,
 					0.0f, 0.0f * OFFSET, 0,
 					0 , 0.7f, 0, 0.7f, &localA, &localB);
 		 		
 		 		btHingeConstraint* constraint = new btHingeConstraint(*p_bodyA, *p_bodyB, localA, localB);
 					constraint->setLimit(- PI_2 , PI_2 );
 		 		m_dynamicWorld->addConstraint(constraint);
 		 		constraint->setDbgDrawSize(0.5f);
 		 		return constraint;
 		 	}
 		 	//Right lower arm - Right hand
 		 	else if(p_nameA.compare("Character1_RightForeArm") == 0 && p_nameB.compare("Character1_RightHand") == 0 )
 		 	{
 				CalculateConstraintTransform(p_bodyA, p_bodyB, 
 					-0.0f , -0.0f * OFFSET, 0,
 					0.0f, 0.0f * OFFSET, 0,
 					0 , 0.7f, 0, 0.7f, &localA, &localB);
 		 		
 		 		btHingeConstraint* constraint = new btHingeConstraint(*p_bodyA, *p_bodyB, localA, localB);
 				constraint->setLimit(- PI_2 , PI_2 );
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
		
		for(int i = 0 ;  i < BodyPart::TOTAL_BONE_AMUNT; i++)
		{
			if(i != BodyPart::HIPS && i !=BodyPart::SPINE)
				m_bodies[i]->applyCentralImpulse(p_velocity* 2);
			else
			{
				m_bodies[i]->applyCentralImpulse(p_velocity );
			}
		}
		/*m_bodies[BodyPart::HIPS]->setLinearVelocity(p_velocity);
		m_bodies[BodyPart::SPINE]->setLinearVelocity(p_velocity);*/
	}

	//The magic function of constraint calculations, basically creates the constraint in the middle between the two bodies with a user defined offset applied
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
		constraintWorld.setRotation(bodyTransformA.getRotation() * btQuaternion(p_quatX, p_quatY, p_quatZ, p_quatW));
		p_transBodyA->setIdentity();
		*p_transBodyA = bodyTransformA.inverse() * constraintWorld;

		p_transBodyB->setIdentity();
		*p_transBodyB = bodyTransformB.inverse() * constraintWorld;
	}

	void Ragdoll::FixPosition( glm::mat4 p_bones[20], aiNode* p_rootNode )
	{
		int myIndex = m_nameToIndex[p_rootNode->mName.data];
		float data[16];
		//My pos
		btTransform trans = m_bodies[myIndex]->getWorldTransform();
		trans.getOpenGLMatrix(data);
		float rootData[16];
		int parentIndex = m_boneToFollow[myIndex];
		//Rootnode(Hips) Pos
		btTransform worldTrans = m_bodies[BodyPart::HIPS]->getWorldTransform();
		float x,y,z,w;
		x = worldTrans.getRotation().w();
		y = worldTrans.getRotation().x();
		z = worldTrans.getRotation().y();
		w = worldTrans.getRotation().z();
		worldTrans.setRotation(btQuaternion(x,y,z,w));
		worldTrans.getOpenGLMatrix(rootData);


		

		//Create a offset matrix to get the model parts better aligned with the shapes (midpoint to midpoint instead of midpoint to a edge)
		btVector3 btAxis = (trans.getRotation().getAxis());
		glm::vec3 axis = glm::vec3(btAxis.x(), btAxis.y(), btAxis.z());
		float angle = trans.getRotation().getAngle();
		glm::vec3 rotatedOffset = glm::rotate(m_boneShapeSize[myIndex], angle , axis);
		glm::mat4 rotatedTranslation = glm::translate(glm::mat4(1.0f), rotatedOffset);


		//Bullet world space to model local space
		m_prevPos[myIndex] =  glm::make_mat4(rootData);
		p_bones[myIndex] = (glm::inverse(m_prevPos[myIndex]) *  ( glm::make_mat4(data))); 	
		p_bones[myIndex] *= rotatedTranslation;
		p_bones[myIndex] *= m_boneOffset[myIndex];
		
	

		for(unsigned int i = 0; i < p_rootNode->mNumChildren; i++)
		{
			//we must go deeper
			FixPosition(p_bones, p_rootNode->mChildren[i]);
		}
	}
}