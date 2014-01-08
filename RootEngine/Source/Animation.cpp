#include <RootEngine/Include/Animation.h>

namespace RootEngine
{
	namespace RootAnimation
	{
		Animation::Animation(unsigned int p_vertSize) : m_numBones(0)
		{
			m_boneData.resize(p_vertSize);
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

		void Animation::AddAnimationKeyFrames( unsigned int p_start, unsigned int p_stop )
		{
			m_animationKeyFrames.push_back(glm::vec2((float)p_start, (float)p_stop));
		}

		void Animation::BoneTransform(float TimeInSeconds, std::vector<glm::mat4>& Transforms)
		{
			glm::mat4 Identity = glm::mat4(1.0);
			

			float TicksPerSecond = (float)(m_aiScene->mAnimations[0]->mTicksPerSecond != 0 ? m_aiScene->mAnimations[0]->mTicksPerSecond : 25.0f);
			float TimeInTicks = TimeInSeconds * TicksPerSecond;
			float AnimationTime = fmod(TimeInTicks, (float)m_aiScene->mAnimations[0]->mDuration);

			ReadNodeHeirarchy(AnimationTime, m_aiScene->mRootNode, Identity);

			Transforms.resize(m_numBones);

			for (unsigned int i = 0 ; i < m_numBones ; i++) {
				Transforms[i] = m_boneInfo[i].m_finalTransformation;
			}
		}

		void Animation::SetAiScene( aiScene* p_aiScene )
		{
			m_aiScene = p_aiScene;
		}

	}
}