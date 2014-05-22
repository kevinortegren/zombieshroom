#pragma once

#include <SDL2/SDL.h>
#include <Qt/QtWidgets/qwidget.h>

static void InitialiseKeymap();

static const int KEYMAP_SIZE = 256;
static SDL_Scancode s_keymap[KEYMAP_SIZE];

static SDL_Scancode GetScanCodeFromQtKey(int key)
{
	// 7-bit common ASCII can be sent right away.
	if(key >= Qt::Key_Space && key <= Qt::Key_AsciiTilde) 
	{		
		if(key >= Qt::Key_A && key <= Qt::Key_Z)
			key += 32;

		return SDL_GetScancodeFromKey(key);
	} 
	else // misc keys & modifiers needs to be identified.
	{
		int v = key&0x000000FF;
		return s_keymap[v];
	}
}

static void InitialiseKeymap()
{
	// Initialize the key map with default values.
	memset(s_keymap, 0, sizeof(s_keymap));

	// Assign individual values.
	s_keymap[Qt::Key_Alt&0x000000FF] = SDL_SCANCODE_LALT;
	s_keymap[Qt::Key_Left&0x000000FF] = SDL_SCANCODE_LEFT;
	s_keymap[Qt::Key_Right&0x000000FF] = SDL_SCANCODE_RIGHT;
	s_keymap[Qt::Key_Up&0x000000FF] = SDL_SCANCODE_UP;
	s_keymap[Qt::Key_Down&0x000000FF] = SDL_SCANCODE_DOWN;


	/*Qt::Key_Escape	0x01000000	 
		Qt::Key_Tab	0x01000001	 
		Qt::Key_Backtab	0x01000002	 
		Qt::Key_Backspace	0x01000003	 
		Qt::Key_Return	0x01000004	 
		Qt::Key_Enter	0x01000005	Typically located on the keypad.
		Qt::Key_Insert	0x01000006	 
		Qt::Key_Delete	0x01000007	 
		Qt::Key_Pause	0x01000008	The Pause/Break key (Note: Not anything to do with pausing media)
		Qt::Key_Print	0x01000009	 
		Qt::Key_SysReq	0x0100000a	 
		Qt::Key_Clear	0x0100000b	 
		Qt::Key_Home	0x01000010	 
		Qt::Key_End	0x01000011	 
		Qt::Key_Left	0x01000012	 
		Qt::Key_Up	0x01000013	 
		Qt::Key_Right	0x01000014	 
		Qt::Key_Down	0x01000015	 
		Qt::Key_PageUp	0x01000016	 
		Qt::Key_PageDown	0x01000017	 
		Qt::Key_Shift	0x01000020	 
		Qt::Key_Control	0x01000021	On Mac OS X, this corresponds to the Command keys.
		Qt::Key_Meta	0x01000022	On Mac OS X, this corresponds to the Control keys. On Windows keyboards, this key is mapped to the Windows key.
		Qt::Key_Alt	0x01000023	 
		Qt::Key_AltGr	0x01001103	On Windows, when the KeyDown event for this key is sent, the Ctrl+Alt modifiers are also set.
		Qt::Key_CapsLock	0x01000024	 
		Qt::Key_NumLock	0x01000025	 
		Qt::Key_ScrollLock	0x01000026	 
		Qt::Key_F1	0x01000030	 
		Qt::Key_F2	0x01000031	 
		Qt::Key_F3	0x01000032	 
		Qt::Key_F4	0x01000033	 
		Qt::Key_F5	0x01000034	 
		Qt::Key_F6	0x01000035	 
		Qt::Key_F7	0x01000036	 
		Qt::Key_F8	0x01000037	 
		Qt::Key_F9	0x01000038	 
		Qt::Key_F10	0x01000039	 
		Qt::Key_F11	0x0100003a	 
		Qt::Key_F12	0x0100003b */

}
