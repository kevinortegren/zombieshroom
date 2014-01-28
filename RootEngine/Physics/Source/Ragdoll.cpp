
#include <RootEngine/Physics/Include/Ragdoll.h>

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

	void Ragdoll::BuildRagdoll( glm::mat4 p_bones[20], aiNode* p_rootNode , std::map<std::string, int>  p_nameToIndex)
	{
		//here be dragons nu ‰r det fel i min hj‰rna
	
		
	}

	void Ragdoll::CreateBody(glm::mat4 p_bones[20], aiNode* p_rootNode)
	{
		btRigidBody* body;
		//int index = p_rootNode->mName;


	}

}