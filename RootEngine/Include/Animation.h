#pragma once

#include <vector>
#include <map>
#include <string>
#include <glm/glm.hpp>

namespace RootEngine
{
	namespace RootAnimation
	{

		#define NUM_BONES_PER_VERTEX 4
		
		struct BoneInfo
		{
			glm::mat4x4 m_boneOffset;
			glm::mat4x4 m_finalTransformation;        
		
			BoneInfo()
			{
				m_boneOffset = glm::mat4x4(0.0f);
				m_finalTransformation = glm::mat4x4(0.0f);          
			}
		};
		
		struct VertexBoneData
		{        
			unsigned int m_IDList[NUM_BONES_PER_VERTEX];
			float m_weightList[NUM_BONES_PER_VERTEX];
		
			VertexBoneData()
			{
				Reset();
			};
		
			void Reset()
			{
				std::fill_n(m_IDList, NUM_BONES_PER_VERTEX, 0);
				std::fill_n(m_weightList, NUM_BONES_PER_VERTEX, 0.0f);
			}
		
			void AddBoneData(unsigned int p_boneID, float p_weight)
			{
				for (unsigned int i = 0 ; i < NUM_BONES_PER_VERTEX ; i++) {
					if (m_weightList[i] == 0.0f) {
						m_IDList[i]     = p_boneID;
						m_weightList[i] = p_weight;
						return;
					}        
				}
		
				// Should never get here! If it does, there are more bones per vertex than allowed.
				assert(0);
			}
		};

		class AnimationInterface
		{
			public:
				virtual bool BoneExists(std::string p_boneName) = 0;
				virtual void PushBoneInfo(RootEngine::RootAnimation::BoneInfo p_boneInfo) = 0;
				virtual unsigned int GetNumBones() = 0;
				virtual void SetNumBones(unsigned int p_numBones) = 0;
				virtual void MapBone(std::string p_boneName, unsigned int p_index) = 0;
				virtual unsigned int GetIndexFromBoneName(std::string p_boneName) = 0;
				virtual void AddBoneData(unsigned int p_vertexIndex, unsigned int p_boneIndex, float p_weight) = 0;
				virtual void AddAnimationKeyFrames(unsigned int p_start, unsigned int p_stop) = 0;
			};

			class Animation : public AnimationInterface
			{
			public:
				Animation(unsigned int p_vertSize);
				~Animation();

				bool BoneExists(std::string p_boneName);
				void PushBoneInfo(RootEngine::RootAnimation::BoneInfo p_boneInfo);
				void MapBone(std::string p_boneName, unsigned int p_index);
				unsigned int GetIndexFromBoneName(std::string p_boneName);
				unsigned int GetNumBones();
				void SetNumBones(unsigned int p_numBones);
				void AddBoneData(unsigned int p_vertexIndex, unsigned int p_boneIndex, float p_weight);
				void AddAnimationKeyFrames(unsigned int p_start, unsigned int p_stop);

			private:
			//Map bone name to index
			std::map<std::string, unsigned int> m_boneMapping;
			std::vector<RootEngine::RootAnimation::BoneInfo> m_boneInfo;
			std::vector<RootEngine::RootAnimation::VertexBoneData> m_boneData;
			std::vector<glm::vec2> m_animationKeyFrames;
			glm::mat4x4 m_globalInverseTransform;
			unsigned int m_numBones;


		};
	}
}