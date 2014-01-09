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

		void Animation::BoneTransform(float TimeInSeconds)
		{
			glm::mat4 Identity = glm::mat4(1.0);
			
			float TicksPerSecond = (float)(m_aiImporter->GetScene()->mAnimations[0]->mTicksPerSecond != 0 ? m_aiImporter->GetScene()->mAnimations[0]->mTicksPerSecond : 25.0f);
			float TimeInTicks = TimeInSeconds * TicksPerSecond;
			float AnimationTime = fmod(TimeInTicks, (float)m_aiImporter->GetScene()->mAnimations[0]->mDuration);

			ReadNodeHeirarchy(AnimationTime, m_aiImporter->GetScene()->mRootNode, Identity);

			for (unsigned int i = 0 ; i < m_numBones ; i++) 
			{
				m_bones[i] = m_boneInfo[i].m_finalTransformation;
			}
		}

		void Animation::ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform)
		{    
			std::string NodeName(pNode->mName.data);

			const aiAnimation* pAnimation = m_aiImporter->GetScene()->mAnimations[0];
			
			
			aiMatrix4x4 am = pNode->mTransformation;
			glm::mat4x4 gm = glm::mat4x4();
			gm[0][0] = am.a1; gm[0][1] = am.b1; gm[0][2] = am.c1; gm[0][3] = am.d1;
			gm[1][0] = am.a2; gm[1][1] = am.b2; gm[1][2] = am.c2; gm[1][3] = am.d2;
			gm[2][0] = am.a3; gm[2][1] = am.b3; gm[2][2] = am.c3; gm[2][3] = am.d3;
			gm[3][0] = am.a4; gm[3][1] = am.b4; gm[3][2] = am.c4; gm[3][3] = am.d4;

			glm::mat4 NodeTransformation(gm);

			const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

			if (pNodeAnim) 
			{
				// Interpolate scaling and generate scaling transformation matrix
				aiVector3D Scaling;
				CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
				glm::mat4 ScalingM = glm::translate(glm::mat4(1.0f), glm::vec3(Scaling.x, Scaling.y, Scaling.z));
				// Interpolate rotation and generate rotation transformation matrix
				aiQuaternion RotationQ;
				CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
				aiMatrix3x3 am3 = RotationQ.GetMatrix(); 
				glm::mat3x3 gm3 = glm::mat3x3();
				gm3[0][0] = am3.a1; gm3[0][1] = am3.b1; gm3[0][2] = am3.c1; 
				gm3[1][0] = am3.a2; gm3[1][1] = am3.b2; gm3[1][2] = am3.c2; 
				gm3[2][0] = am3.a3; gm3[2][1] = am3.b3; gm3[2][2] = am3.c3; 
				
				glm::mat4 RotationM = glm::mat4(gm);

				// Interpolate translation and generate translation transformation matrix
				aiVector3D Translation;
				CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
				glm::mat4 TranslationM = glm::translate(glm::mat4(1.0f), glm::vec3(Translation.x, Translation.y, Translation.z)) ;

				// Combine the above transformations
				NodeTransformation = TranslationM * RotationM * ScalingM;
			}

			glm::mat4 GlobalTransformation = ParentTransform * NodeTransformation;

			if (m_boneMapping.find(NodeName) != m_boneMapping.end()) 
			{
				unsigned int BoneIndex = m_boneMapping[NodeName];
				m_boneInfo[BoneIndex].m_finalTransformation = m_globalInverseTransform * GlobalTransformation * m_boneInfo[BoneIndex].m_boneOffset;
			}

			for (unsigned int i = 0 ; i < pNode->mNumChildren ; i++) 
			{
				ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
			}
		}

		unsigned int Animation::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
		{    
			for (unsigned int i = 0 ; i < pNodeAnim->mNumPositionKeys - 1 ; i++) 
			{
				if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) 
				{
					return i;
				}
			}

			assert(0);

			return 0;
		}


		unsigned int Animation::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
		{
			assert(pNodeAnim->mNumRotationKeys > 0);

			for (unsigned int i = 0 ; i < pNodeAnim->mNumRotationKeys - 1 ; i++) 
			{
				if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) 
				{
					return i;
				}
			}

			assert(0);

			return 0;
		}


		unsigned int Animation::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
		{
			assert(pNodeAnim->mNumScalingKeys > 0);

			for (unsigned int i = 0 ; i < pNodeAnim->mNumScalingKeys - 1 ; i++)
			{
				if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) 
				{
					return i;
				}
			}

			assert(0);

			return 0;
		}


		void Animation::CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
		{
			if (pNodeAnim->mNumPositionKeys == 1) 
			{
				Out = pNodeAnim->mPositionKeys[0].mValue;
				return;
			}

			unsigned int PositionIndex = FindPosition(AnimationTime, pNodeAnim);
			unsigned int NextPositionIndex = (PositionIndex + 1);
			assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
			float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
			float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
			assert(Factor >= 0.0f && Factor <= 1.0f);
			const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
			const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
			aiVector3D Delta = End - Start;
			Out = Start + Factor * Delta;
		}


		void Animation::CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
		{
			// we need at least two values to interpolate...
			if (pNodeAnim->mNumRotationKeys == 1) 
			{
				Out = pNodeAnim->mRotationKeys[0].mValue;
				return;
			}

			unsigned int RotationIndex = FindRotation(AnimationTime, pNodeAnim);
			unsigned int NextRotationIndex = (RotationIndex + 1);
			assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
			float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
			float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
			assert(Factor >= 0.0f && Factor <= 1.0f);
			const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
			const aiQuaternion& EndRotationQ   = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;    
			aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
			Out = Out.Normalize();
		}


		void Animation::CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
		{
			if (pNodeAnim->mNumScalingKeys == 1) 
			{
				Out = pNodeAnim->mScalingKeys[0].mValue;
				return;
			}

			unsigned int ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
			unsigned int NextScalingIndex = (ScalingIndex + 1);
			assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
			float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
			float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
			assert(Factor >= 0.0f && Factor <= 1.0f);
			const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
			const aiVector3D& End   = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
			aiVector3D Delta = End - Start;
			Out = Start + Factor * Delta;
		}
		const aiNodeAnim* Animation::FindNodeAnim(const aiAnimation* pAnimation, const std::string NodeName)
		{
			for (unsigned int i = 0 ; i < pAnimation->mNumChannels ; i++) 
			{
				const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

				if (std::string(pNodeAnim->mNodeName.data) == NodeName) 
				{
					return pNodeAnim;
				}
			}

			return NULL;
		}

		

		std::vector<glm::mat4> Animation::GetBones()
		{
			return m_bones;
		}

		BoneInfo* Animation::GetBoneInfo( unsigned int p_index )
		{
			return &m_boneInfo[p_index];
		}

		void Animation::SetAiImporter( std::shared_ptr<Assimp::Importer> p_aiImporter )
		{
			m_bones.resize(m_numBones);
			m_aiImporter = p_aiImporter;
		}

		RootEngine::RootAnimation::VertexBoneData* Animation::GetBoneData(unsigned int p_index)
		{
			return &m_boneData[p_index];
		}

	}
}