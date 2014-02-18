#include "IngameMenu.h"

namespace RootForce
{

	IngameMenu::IngameMenu( RootEngine::GUISystem::WebView* p_view, RootEngine::GameSharedContext p_context, Keymapper* p_keymapper )
	{
		m_return = false;
		m_exit = false;

		m_view = p_view;
		m_settingsMenu = new SettingsMenu(p_context, p_keymapper);
		p_keymapper->SetMenu(m_view);

		m_view->RegisterJSCallback("Exit", JSDelegate1(this, &IngameMenu::Exit));
		m_view->RegisterJSCallback("Return", JSDelegate1(this, &IngameMenu::Return));

		m_settingsMenu->BindEvents(m_view);
		m_view->Focus();
	}

	IngameMenu::~IngameMenu()
	{
		delete m_settingsMenu;
	}

	void IngameMenu::Return( const Awesomium::JSArray& p_array )
	{
		m_return = true;
	}

	void IngameMenu::Exit( const Awesomium::JSArray& p_array )
	{
		m_exit = true;
	}

	void IngameMenu::Reset()
	{
		m_return = false;
		m_exit = false;
	}

	void IngameMenu::Update()
	{
		m_settingsMenu->Update();
	}

}

