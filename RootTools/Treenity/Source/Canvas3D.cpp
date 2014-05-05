#include "Canvas3D.h"
#include <iostream>

#include <SDL2/SDL_main.h>
#include <SDL2/SDL_syswm.h> 
#include <SDL2/SDL_hints.h>

#include <RootEngine/Include/GameSharedContext.h>
#include <RootEngine/Render/Include/Renderer.h>

extern RootEngine::GameSharedContext g_engineContext;

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
		return s_keymap[key&0x000000FF];
	}
}

Canvas3D::Canvas3D( QWidget* p_parent /*= 0*/ ) : QWidget(p_parent)
{
	//Flicker fix
	setAttribute(Qt::WA_PaintOnScreen);
	setUpdatesEnabled(false);

	//Standard setup
	//setGeometry(QRect(2, 28, 1173, 899));
	setFocusPolicy(Qt::ClickFocus);
	show();
	setEnabled(true);
	setMouseTracking(true);

		//Create temporary SDL window to set up correct pixel format and SDL_WINDOW_OPENGL flag
	SDL_Window* sdlwindow = SDL_CreateWindow(
		"Root Particle",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width(),
		height(),
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);

	std::cout << "Width: " << width() << " Height: " << height() << std::endl;
	std::cout << "G Width: " << geometry().width() << "G Height: " << geometry().height() << std::endl;

	//Format string with adress to temporary SDL window
	//char buffer [10];
	//sprintf_s(buffer, "%p", sdlwindow);
	//Set SDL_HINT_VIDEO_WINDOW_SHARE_PIXEL_FORMAT hint with adresst to temporary window
	//SDL_SetHint(SDL_HINT_VIDEO_WINDOW_SHARE_PIXEL_FORMAT, buffer );

	//Get Qt WId from the frame wherer we want to render. Cast to HWND
	HWND qtWindowHandle = (HWND)winId();
	//SDL_CreateWindowFrom takes an already created native window handle and builds an SDL_Window for us
	m_window = std::shared_ptr<SDL_Window>(SDL_CreateWindowFrom((void*)qtWindowHandle), SDL_DestroyWindow);

	SDL_DestroyWindow(sdlwindow);

	if (m_window == nullptr) 
	{
		std::cout << SDL_GetError() << std::endl;
		throw std::runtime_error("Failed to create window");
	}

	InitialiseKeymap();
}

Canvas3D::~Canvas3D()
{

}

void Canvas3D::resizeEvent( QResizeEvent * event)
{
	if(g_engineContext.m_renderer == nullptr)
	{
		SDL_SetWindowSize(m_window.get(), event->size().width(), event->size().height());
	}
	else
	{
		g_engineContext.m_renderer->SetResolution(false, event->size().width(), event->size().height());
	}
	

	std::cout << "Resize Width: " << event->size().width() << "Resize Height: " << event->size().height() << std::endl;
}

void Canvas3D::enterEvent( QEvent * event )
{
	setFocus(Qt::TabFocusReason);
}

void Canvas3D::CreateOpenGLContext()
{	
	// Setup the SDL context
	g_engineContext.m_renderer->SetupSDLContext(m_window.get());
}

void Canvas3D::wheelEvent(QWheelEvent* event)
{
	SDL_Event scrollEvent;
	scrollEvent.type = SDL_MOUSEWHEEL;
	scrollEvent.wheel.y = event->delta() / 100;
	SDL_PushEvent(&scrollEvent);
}

void Canvas3D::keyPressEvent( QKeyEvent *k )
{
	SDL_Event keyEvent;
	keyEvent.type = SDL_KEYDOWN;
	keyEvent.key.keysym.scancode = GetScanCodeFromQtKey(k->key());
	keyEvent.key.repeat = false;
	SDL_PushEvent(&keyEvent);
}

void Canvas3D::keyReleaseEvent( QKeyEvent *k )
{
	SDL_Event keyEvent;
	keyEvent.type = SDL_KEYUP;
	keyEvent.key.keysym.scancode = GetScanCodeFromQtKey(k->key());
	keyEvent.key.repeat = false;
	SDL_PushEvent(&keyEvent);
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
