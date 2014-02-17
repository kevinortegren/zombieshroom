#pragma once
#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/Network/NetworkComponents.h>
#include <RootSystems/Include/Script.h>
#include <RakNet/RakPeerInterface.h>

namespace RootForce
{
	struct TimerComponent : ECS::Component<TimerComponent> 
	{
		float TimeLeft;
		bool TimeUp;
		TimerComponent()
		{
			TimeLeft = 0.0f;
			TimeUp = false;
		}
	};

	class TimerSystem : public ECS::EntitySystem
	{
	public:
		TimerSystem(ECS::World* p_world)
			: ECS::EntitySystem(p_world), m_serverPeer(nullptr)
		{
			SetUsage<TimerComponent>();
			SetUsage<Network::NetworkComponent>();
			SetUsage<Script>();
		}

		void Init();
		void ProcessEntity(ECS::Entity* p_entity);
		void SetServerPeer(RakNet::RakPeerInterface* p_serverPeer);

	private:
		ECS::ComponentMapper<TimerComponent> m_timers;
		ECS::ComponentMapper<Network::NetworkComponent> m_networks;
		ECS::ComponentMapper<Script> m_scripts;

		RakNet::RakPeerInterface* m_serverPeer;
	};
}