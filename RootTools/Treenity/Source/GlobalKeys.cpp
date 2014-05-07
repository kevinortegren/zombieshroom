#include <RootTools/Treenity/Include/GlobalKeys.h>

std::map<int, SDL_Scancode> GlobalKeys::m_qtModToSDLScancode;

void GlobalKeys::InitializeKeyMap()
{
	m_qtModToSDLScancode[Qt::ShiftModifier] = SDL_SCANCODE_LSHIFT;
	m_qtModToSDLScancode[Qt::AltModifier] = SDL_SCANCODE_LALT;
}

void GlobalKeys::Update()
{
	Qt::KeyboardModifiers modifers = QApplication::keyboardModifiers();
	for(auto itr = m_modifers.begin(); itr != m_modifers.end(); ++itr)
	{
		if(m_modifersDown[(*itr)] == true)
		{
			if((modifers & (*itr)) == 0)
			{
				SDL_Event keyEvent;
				keyEvent.type = SDL_KEYUP;
				keyEvent.key.keysym.scancode = m_qtModToSDLScancode[(*itr)];
				keyEvent.key.repeat = false;
				SDL_PushEvent(&keyEvent);

				m_modifersDown[(*itr)] = false;
			}
		}
		else
		{
			if((modifers & (*itr)) != 0)
			{
				SDL_Event keyEvent;
				keyEvent.type = SDL_KEYDOWN;
				keyEvent.key.keysym.scancode = m_qtModToSDLScancode[(*itr)];
				keyEvent.key.repeat = false;
				SDL_PushEvent(&keyEvent);

				m_modifersDown[(*itr)] = true;
			}
		}
	}
}

void GlobalKeys::RegisterModifier(Qt::KeyboardModifiers p_modifer)
{
	m_modifers.push_back(p_modifer);
}

void GlobalKeys::UnregisterModifier(Qt::KeyboardModifiers p_modifer)
{
	auto itr = std::find(m_modifers.begin(), m_modifers.end(), p_modifer);
	m_modifers.erase(itr);
}
