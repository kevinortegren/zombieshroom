
#include <RootEngine/Physics/Include/Ragdoll.h>
#include <glm/gtc/type_ptr.hpp>
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
		CreateBody(p_bones, p_rootNode, p_nameToIndex, p_transform, p_pos );

	}

	btRigidBody* Ragdoll::CreateBody(glm::mat4 p_bones[20], aiNode* p_rootNode, std::map<std::string, int>  p_nameToIndex, glm::mat4 p_transform, const btVector3& p_pos )
	{
		btRigidBody* body;
		btCylinderShape* shape = CreateBone(p_rootNode->mName.data);
		if(shape != nullptr)
		{
			btVector3 inertia = btVector3(0,0,0);
			shape->calculateLocalInertia(1, inertia);
			int index = p_nameToIndex[p_rootNode->mName.data];
			btTransform trans;
			const float* data = glm::value_ptr(p_bones[index]);
			trans.setFromOpenGLMatrix(data);
			std::string test= p_rootNode->mName.data;
			trans.setOrigin(trans.getOrigin());
			btDefaultMotionState* motionstate = new btDefaultMotionState(trans);
			body = new btRigidBody(1.0f, motionstate, shape, inertia);
			m_bodies[index] = body;
			m_dynamicWorld->addRigidBody(body);
			//int index = p_rootNode->mName;

			for(unsigned int i = 0; i < p_rootNode->mNumChildren; i++)
			{
				float PI_2 = 1.57079632679489661923f;
				btRigidBody* childbody = CreateBody(p_bones, p_rootNode->mChildren[i], p_nameToIndex, p_transform, p_pos);
				if(childbody != nullptr)
				{
					btTransform localA, localB;
					localA.setIdentity(); localB.setIdentity();
					localA.getBasis().setEulerZYX(0,0,-PI_2*5); localA.setOrigin(body->getWorldTransform().getOrigin());
					localB.getBasis().setEulerZYX(0,0,-PI_2*5); localB.setOrigin(childbody->getWorldTransform().getOrigin());
					btConeTwistConstraint* constraint = new btConeTwistConstraint(*body, *childbody, localA, localB);
					constraint->setLimit(PI_2,PI_2, 0);
					m_dynamicWorld->addConstraint(constraint);
					constraint->setDbgDrawSize(0.5f);
					
				}
				//Add a constraint here somewhere ala constraint(mybody, m_bodies[myindex + 1 + i], variables , variable)
			}
			return body;
		}
		for(unsigned int i = 0; i < p_rootNode->mNumChildren; i++)
			CreateBody(p_bones, p_rootNode->mChildren[i], p_nameToIndex, p_transform, p_pos);
	
		return nullptr;
	}

	btCylinderShape* Ragdoll::CreateBone( std::string p_name )
	{
		
		if(p_name.compare("Character1_Hips") == 0)
			return new btCylinderShape(btVector3(0.7f,0.2f,0.7f));
		else if(p_name.compare("Character1_LeftArm") == 0)
			return new btCylinderShape(btVector3(0.05f,0.3f,0.05f));
		else if(p_name.compare("Character1_LeftFoot") == 0)
			return new btCylinderShape(btVector3(0.1f,0.04f,0.1f));
		else if(p_name.compare("Character1_LeftForeArm") == 0)
			return new btCylinderShape(btVector3(0.05f,0.3f,0.05f));
		else if(p_name.compare("Character1_LeftHand") == 0)
			return new btCylinderShape(btVector3(0.1f,0.04f,0.1f));
		else if(p_name.compare("Character1_LeftLeg") == 0)
			return new btCylinderShape(btVector3(0.1f,0.3f,0.1f));
		else if(p_name.compare("Character1_LeftUpLeg") == 0)
			return new btCylinderShape(btVector3(0.1f,0.3f,0.1f));
		else if(p_name.compare("Character1_RightArm") == 0)
			return new btCylinderShape(btVector3(0.05f,0.3f,0.05f));
		else if(p_name.compare("Character1_RightFoot") == 0)
			return new btCylinderShape(btVector3(0.1f,0.04f,0.1f));
		else if(p_name.compare("Character1_RightForeArm") == 0)
			return new btCylinderShape(btVector3(0.05f,0.3f,0.05f));
		else if(p_name.compare("Character1_RightHand") == 0)
			return new btCylinderShape(btVector3(0.1f,0.04f,0.1f));
		else if(p_name.compare("Character1_RightLeg") == 0)
			return new btCylinderShape(btVector3(0.1f,0.3f,0.1f));
		else if(p_name.compare("Character1_RightUpLeg") == 0)
			return new btCylinderShape(btVector3(0.1f,0.3f,0.1f));
		else if(p_name.compare("Character1_Spine") == 0)
			return new btCylinderShape(btVector3(0.7f,1.0f,0.7f));

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
			m_bodies[i]->getWorldTransform().getOpenGLMatrix(data);
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

}