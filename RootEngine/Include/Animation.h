#pragma once
#ifndef COMPILE_LEVEL_EDITOR

#include <vector>
#include <map>
#include <memory>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>  
#include <RootEngine/Include/Logging/Logging.h>
namespace RootEngine
{
	namespace RootAnimation
	{
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

		struct AnimClip 
		{
			double m_startTime;
			double m_duration;
			unsigned int m_startFrame;
			unsigned int m_stopFrame;
		};
		
		class AnimationInterface
		{
			public:
				virtual bool BoneExists(std::string p_boneName) = 0;
				virtual void PushBoneInfo(RootEngine::RootAnimation::BoneInfo p_boneInfo) = 0;
				virtual void SetAiImporter(std::shared_ptr<Assimp::Importer> p_aiImporter) = 0;
				virtual void SetNumBones(unsigned int p_numBones) = 0;
				virtual void MapBone(std::string p_boneName, unsigned int p_index) = 0;
				virtual unsigned int GetNumBones() = 0;
				virtual unsigned int GetIndexFromBoneName(std::string p_boneName) = 0;
				virtual BoneInfo* GetBoneInfo(unsigned int p_index) = 0;
				virtual const aiScene* GetScene() = 0;
				virtual glm::mat4x4 GetGlobalInverseTransform() = 0;
				virtual glm::mat4x4 GetBoneOffset(unsigned int p_index) = 0;
				virtual void SplitAnimation() = 0;
				virtual AnimClip* GetAnimClip(unsigned int p_index) = 0;
				virtual ~AnimationInterface(){};
			};

			class Animation : public AnimationInterface
			{
			public:
				Animation(Logging* p_logging);
				~Animation();

				bool BoneExists(std::string p_boneName);
				void PushBoneInfo(RootEngine::RootAnimation::BoneInfo p_boneInfo);
				void MapBone(std::string p_boneName, unsigned int p_index);
				void SetNumBones(unsigned int p_numBones);
				void SetAiImporter(std::shared_ptr<Assimp::Importer> p_aiImporter);
				unsigned int GetIndexFromBoneName(std::string p_boneName);
				unsigned int GetNumBones();
				BoneInfo* GetBoneInfo(unsigned int p_index);
				const aiScene* GetScene();
				glm::mat4x4 GetGlobalInverseTransform();
				glm::mat4x4 GetBoneOffset(unsigned int p_index);
				void SplitAnimation();
				AnimClip* GetAnimClip(unsigned int p_index);

			private:
			//Map bone name to index
			std::map<std::string, unsigned int>					m_boneMapping;
			std::vector<RootEngine::RootAnimation::BoneInfo>	m_boneInfo;
			std::vector<AnimClip>								m_animClips;
			glm::mat4x4		m_globalInverseTransform;
			unsigned int	m_numBones;
			Logging*		m_logging;
			std::shared_ptr<Assimp::Importer> m_aiImporter;
			
		};
	}
}

#endif
