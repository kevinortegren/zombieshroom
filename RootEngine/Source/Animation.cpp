#include <RootEngine/Include/Animation.h>

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

		void Animation::AddBoneData( unsigned int p_vertexIndex, unsigned int p_boneIndex, float p_weight )
		{
			m_boneData[p_vertexIndex].AddBoneData(p_boneIndex, p_weight);
		}
	}
}