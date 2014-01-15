#ifndef COMPILE_LEVEL_EDITOR
#include <RootEngine/Include/Animation.h>
#include <iostream>
namespace RootEngine
{
	namespace RootAnimation
	{
		Animation::Animation() : m_numBones(0)
		{
		}

		Animation::~Animation()
		{
		}

		bool Animation::BoneExists( std::string p_boneName )
		{
			if(m_boneMapping.find(p_boneName) == m_boneMapping.end())
				return false;
			else
				return true;
		}
	
		void Animation::PushBoneInfo( RootEngine::RootAnimation::BoneInfo p_boneInfo )
		{
			m_boneInfo.push_back(p_boneInfo);
		}

		unsigned int Animation::GetNumBones()
		{
			return m_numBones;
		}

		void Animation::SetNumBones( unsigned int p_numBones )
		{
			m_numBones = p_numBones;
		}

		void Animation::MapBone( std::string p_boneName, unsigned int p_index )
		{
			m_boneMapping[p_boneName] = p_index;
		}

		unsigned int Animation::GetIndexFromBoneName( std::string p_boneName )
		{
			return m_boneMapping[p_boneName];
		}


		BoneInfo* Animation::GetBoneInfo( unsigned int p_index )
		{
			return &m_boneInfo[p_index];
		}

		void Animation::SetAiImporter( std::shared_ptr<Assimp::Importer> p_aiImporter )
		{
			m_aiImporter = p_aiImporter;
			aiMatrix4x4 am = m_aiImporter->GetScene()->mRootNode->mChildren[0]->mTransformation;
			glm::mat4x4 gm = glm::mat4x4();
			memcpy(&gm[0][0], &am[0][0], sizeof(aiMatrix4x4));
			m_globalInverseTransform = glm::transpose(gm);
		}

		const aiScene* Animation::GetScene()
		{
			return m_aiImporter->GetScene();
		}

		glm::mat4x4 Animation::GetGlobalInverseTransform()
		{
			return m_globalInverseTransform;
		}

		glm::mat4x4 Animation::GetBoneOffset( unsigned int p_index )
		{
			return m_boneInfo[p_index].m_boneOffset;
		}

	}
}
#endif