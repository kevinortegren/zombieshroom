#include "IngameMenu.h"

namespace RootForce
{

	IngameMenu::IngameMenu( Awesomium::WebView* p_view, RootEngine::GUISystem::DispatcherInterface* p_dispatcher, RootEngine::GameSharedContext p_context )
	{
		m_return = false;
		m_exit = false;

		m_view = p_view;
		m_context = p_context;
		m_settingsMenu = new SettingsMenu(p_context);
		Awesomium::JSValue result = m_view->CreateGlobalJavascriptObject(Awesomium::WSLit("IngameMenu"));

		if(result.IsObject() && result.ToObject().type() != Awesomium::kJSObjectType_Local)
		{
			p_dispatcher->Bind(result.ToObject(), Awesomium::WSLit("Exit"), JSDelegate(this, &IngameMenu::Exit));
			p_dispatcher->Bind(result.ToObject(), Awesomium::WSLit("Return"), JSDelegate(this, &IngameMenu::Return));
		}
		m_settingsMenu->BindEvents(m_view, p_dispatcher, result);
		m_view->set_js_method_handler(p_dispatcher);
		m_view->Focus();
	}

	IngameMenu::~IngameMenu()
	{
		delete m_settingsMenu;
	}

	void IngameMenu::Return( Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array )
	{
		m_return = true;
	}

	void IngameMenu::Exit( Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array )
	{
		m_exit = true;
	}

	void IngameMenu::Reset()
	{
		m_return = false;
		m_exit = false;
	}

}

