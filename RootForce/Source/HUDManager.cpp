#include <RootForce/Include/HUDManager.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootEngine/InputManager/Include/InputInterface.h>
#include <RootEngine/Script/Include/RootScript.h>

extern RootEngine::GameSharedContext g_engineContext;
extern ECS::World* g_world;

namespace RootForce
{
	HUDManager::HUDManager(bool& p_displayIngameMenu)
		: m_displayIngameMenu(p_displayIngameMenu)
	{}

	void HUDManager::SetHUD(HUD* p_hud)
	{
		m_hud = p_hud;
	}

	void HUDManager::SetMatchStateSystem(MatchStateSystem* p_matchStateSystem)
	{
		m_matchStateSystem = p_matchStateSystem;
	}

	void HUDManager::SetIngameMenu(IngameMenu* p_ingameMenu)
	{
		m_ingameMenu = p_ingameMenu;
	}

	void HUDManager::UpdateHUD()
	{
		if (m_matchStateSystem->IsMatchOver())
		{
			m_hud->SetValue("EndGame", "true" );
		}

		if (m_matchStateSystem->IsMatchOver() || g_engineContext.m_inputSys->GetKeyState(SDL_SCANCODE_TAB) == RootEngine::InputManager::KeyState::DOWN)
		{
			m_hud->SetValue("ShowScore", "true" );
			m_hud->SetScoreList(m_matchStateSystem->GetScoreList());
		}
		else if (g_engineContext.m_inputSys->GetKeyState(SDL_SCANCODE_TAB) == RootEngine::InputManager::KeyState::UP)
		{
			m_hud->SetValue("ShowScore", "false" );
		}
		if(m_displayIngameMenu)
		{
			m_ingameMenu->SetScoreList(m_matchStateSystem->GetScoreList());
		}
		else
		{
			ECS::Entity* player = g_world->GetTagManager()->GetEntityByTag("Player");
			if (!m_matchStateSystem->IsMatchOver())
			{
				PlayerComponent* playerComponent = g_world->GetEntityManager()->GetComponent<PlayerComponent>(player);
				HealthComponent* healthComponent = g_world->GetEntityManager()->GetComponent<HealthComponent>(player);
				PlayerActionComponent* playerActionComponent = g_world->GetEntityManager()->GetComponent<PlayerActionComponent>(player);

				// Update the charge bar
				if (playerComponent->AbilityState != AbilityState::OFF)
				{
					float abilityChargeTime = (float) g_engineContext.m_script->GetGlobalNumber("chargeTime", playerActionComponent->CurrentAbilityEvent.ActiveAbilityScript.C_String());
					float abilityChannelingTime = (float) g_engineContext.m_script->GetGlobalNumber("channelingTime", playerActionComponent->CurrentAbilityEvent.ActiveAbilityScript.C_String());
					float abilityCooldownTime = (float) g_engineContext.m_script->GetGlobalNumber("cooldown", playerActionComponent->CurrentAbilityEvent.ActiveAbilityScript.C_String());

					if(playerActionComponent->CurrentAbilityEvent.Time <= abilityChargeTime)
						m_hud->SetValue("ChargeBarValue", std::to_string(playerActionComponent->CurrentAbilityEvent.Time/abilityChargeTime));
					else if(abilityChannelingTime > 0)
						m_hud->SetValue("ChargeBarValue", std::to_string((abilityChargeTime + abilityChannelingTime - playerActionComponent->CurrentAbilityEvent.Time)/abilityChannelingTime));
					else if(abilityChargeTime > 0) // Make sure the charge bar reaches the end before fading out
						m_hud->SetValue("ChargeBarValue", "1");
				}
				else
				{
					m_hud->SetValue("ChargeBarValue", "0");
				}

				// Update announcement depending on state. Only warmup has a persistent HUD state. See the MatchStateListener implementations for the events.
				ECS::Entity* matchState = g_world->GetTagManager()->GetEntityByTag("MatchState");
				TDMRuleSet* ruleSet = g_world->GetEntityManager()->GetComponent<TDMRuleSet>(matchState);
				KillAnnouncement* killAnnouncement = g_world->GetEntityManager()->GetComponent<KillAnnouncement>(matchState);
				
				if (ruleSet->CurrentState == MatchState::Warmup)
				{
					m_hud->GetView()->BufferJavascript("Announce('Waiting for players...', -1);");
				}
				

				//Update all the data that is displayed in the HUD
				m_hud->SetValue("PlayerScore", std::to_string(playerComponent->Score) );
				m_hud->SetValue("PlayerDeaths", std::to_string(playerComponent->Deaths) );
				m_hud->SetValue("Team1Score",  std::to_string(m_matchStateSystem->GetTeamScore(1)) );
				m_hud->SetValue("Team2Score",  std::to_string(m_matchStateSystem->GetTeamScore(2)) );
				if(healthComponent && playerActionComponent)
				{
					m_hud->SetValue("Health", std::to_string(healthComponent->Health) );
					m_hud->SetValue("IsDead", healthComponent->IsDead?"true":"false" );

					m_hud->SetAbility(1, playerComponent->AbilityScripts[0].Name);
					m_hud->SetAbility(2,  playerComponent->AbilityScripts[1].Name);
					m_hud->SetAbility(3,  playerComponent->AbilityScripts[2].Name);

					m_hud->SetCharges(1, playerComponent->AbilityScripts[0].Charges);
					m_hud->SetCharges(2, playerComponent->AbilityScripts[1].Charges);
					m_hud->SetCharges(3, playerComponent->AbilityScripts[2].Charges);
					
					//g_engineContext.m_logger->LogText(LogTag::GUI, LogLevel::PINK_PRINT, ("current crosshair: " + playerComponent->AbilityScripts[playerActionComponent->SelectedAbility].Crosshair).c_str());
					m_hud->SetCrosshair(playerComponent->AbilityScripts[playerActionComponent->SelectedAbility].Crosshair);

					if(playerComponent->AbilityScripts[0].Cooldown > 0 && playerComponent->AbilityScripts[0].Name.compare("") != 0)
						m_hud->SetCooldown(1, playerComponent->AbilityScripts[0].Cooldown/(float) g_engineContext.m_script->GetGlobalNumber("cooldown", playerComponent->AbilityScripts[0].Name));
					else
						m_hud->SetCooldown(1, 0);
					if(playerComponent->AbilityScripts[1].Cooldown > 0 && playerComponent->AbilityScripts[1].Name.compare("") != 0)
						m_hud->SetCooldown(2, playerComponent->AbilityScripts[1].Cooldown/(float) g_engineContext.m_script->GetGlobalNumber("cooldown", playerComponent->AbilityScripts[1].Name));
					else
						m_hud->SetCooldown(2, 0);
					if(playerComponent->AbilityScripts[2].Cooldown > 0 && playerComponent->AbilityScripts[2].Name.compare("") != 0)
						m_hud->SetCooldown(3, playerComponent->AbilityScripts[2].Cooldown/(float) g_engineContext.m_script->GetGlobalNumber("cooldown", playerComponent->AbilityScripts[2].Name));
					else
						m_hud->SetCooldown(3, 0);
					m_hud->SetSelectedAbility(playerActionComponent->SelectedAbility + 1);
				}
			}
		}

		m_hud->SetValue("TimeLeft", std::to_string((int)m_matchStateSystem->GetTimeLeft()));
		m_hud->Update(); // Executes either the HUD update or ShowScore if the match is over
	}

	void HUDManager::MatchStateChanged(MatchState p_newState, MatchState p_previousState)
	{
		if (p_newState == MatchState::CountDown && p_previousState == MatchState::Warmup)
		{
			m_hud->GetView()->BufferJavascript("Announce('5',1);Announce('4',1);Announce('3',1);Announce('2',1);Announce('1',1);");
		}

		if (p_newState == MatchState::Match && p_previousState == MatchState::CountDown)
		{
			m_hud->GetView()->BufferJavascript("Announce('May the roots be with you!',3);");
		}
	}

	void HUDManager::PlayerKilled(KillAnnouncement* p_killAnnouncement, ECS::Entity* p_murderer, ECS::Entity* p_victim)
	{
		ECS::Entity* player = g_world->GetTagManager()->GetEntityByTag("Player");
		PlayerComponent* murdererPlayerComponent = p_murderer ? g_world->GetEntityManager()->GetComponent<PlayerComponent>(p_murderer) : nullptr;
		PlayerComponent* victimPlayerComponent = g_world->GetEntityManager()->GetComponent<PlayerComponent>(p_victim);

		m_hud->GetView()->BufferJavascript("KillAnnouncement('" + (murdererPlayerComponent ? murdererPlayerComponent->Name : std::string()) + "','" + victimPlayerComponent->Name + "', '" + p_killAnnouncement->AbilityName + "');");
		if(p_murderer == player)
			m_hud->GetView()->BufferJavascript("Announce('You have killed " + victimPlayerComponent->Name + "', 3);");
		if(p_victim == player)
			m_hud->GetView()->BufferJavascript("Announce('You have been killed" + (murdererPlayerComponent ? " by " + murdererPlayerComponent->Name : std::string()) + "', 3);");
	}
}