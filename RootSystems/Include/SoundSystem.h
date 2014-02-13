#pragma once

#include <map>
#include <Utility/ECS/Include/World.h>
#include <RootEngine/Include/ModelImporter.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootSystems/Include/RenderingSystem.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <Utility/ECS/Include/Component.h>
#include <Utility/ECS/Include/EntitySystem.h>
#include <RootSystems/Include/Transform.h>
#include "glm/glm.hpp"
#include <RootEngine/Sound/Include/SoundManager.h>

namespace RootForce
{
	struct SoundComponent : public ECS::Component<SoundComponent>
	{
		SoundComponent(): m_volume(0.5f), m_play(false), m_minDist(0.0f), m_maxDist(100.0f), m_soundChannel(nullptr), m_soundAudio(nullptr) {}
		~SoundComponent()
		{
			delete m_soundChannel;
		}

		RootEngine::Sound::SoundAudioInterface* m_soundAudio;
		RootEngine::Sound::SoundChannelInterface* m_soundChannel;
		bool m_play;
		float m_volume;
		float m_minDist;
		float m_maxDist;
	};

	struct SoundSystem : public ECS::EntitySystem
	{
		SoundSystem(ECS::World* p_world, RootEngine::GameSharedContext* p_context);
		~SoundSystem();
		void Init();
		void Begin();
		void ProcessEntity(ECS::Entity* p_entity);
		void End();

	private:

		ECS::ComponentMapper<RootForce::Transform> m_transform;
		ECS::ComponentMapper<RootForce::SoundComponent> m_sound;

		ECS::World*						m_world;
		Logging::LoggingInterface*		m_logger;
		RootEngine::GameSharedContext*	m_context;

	};
}