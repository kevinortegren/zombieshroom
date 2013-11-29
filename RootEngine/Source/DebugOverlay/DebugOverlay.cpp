#include "DebugOverlay/DebugOverlay.h"

#include <Awesomium/WebCore.h>
#include <Awesomium/STLHelpers.h>

namespace RootEngine
{
	DebugOverlay::DebugOverlay()
	{
	}

	DebugOverlay::~DebugOverlay()
	{
	}

	void DebugOverlay::SetView(Awesomium::WebView* p_view)
	{
		m_view = p_view;
	}

	void DebugOverlay::AddHTML(std::string p_html, bool p_leftSide)
	{
		Awesomium::JSValue window = m_view->ExecuteJavascriptWithResult(Awesomium::WSLit("window"), Awesomium::WSLit(""));

		if (window.IsObject()) {
		  Awesomium::JSArray args;
		  args.Push(Awesomium::JSValue(p_html.c_str()));
		  args.Push(Awesomium::JSValue(p_leftSide));

		  window.ToObject().Invoke(Awesomium::WSLit("AddHTML"), args);
		}
	}

	void DebugOverlay::Clear()
	{
		Awesomium::JSValue window = m_view->ExecuteJavascriptWithResult(Awesomium::WSLit("window"), Awesomium::WSLit(""));

		if (window.IsObject()) {
		  Awesomium::JSArray args;

		  window.ToObject().Invoke(Awesomium::WSLit("Clear"), args);
		}
	}
}