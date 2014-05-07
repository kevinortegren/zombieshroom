#pragma once

#include <QtWidgets/QApplication>
#include <map>
#include <vector>
#include <SDL2/SDL.h>

class GlobalKeys
{
public:
	void Update();
	void RegisterModifier(Qt::KeyboardModifiers p_modifer);
	void UnregisterModifier(Qt::KeyboardModifiers p_modifer);

	static void InitializeKeyMap();

private:

	std::vector<int> m_modifers;
	std::map<int, bool> m_modifersDown;

	static std::map<int, SDL_Scancode> m_qtModToSDLScancode;
};