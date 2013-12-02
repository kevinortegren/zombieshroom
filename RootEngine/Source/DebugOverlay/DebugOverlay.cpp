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

	void DebugOverlay::AddHTML(const char* p_html, bool p_leftSide)
	{
		std::string command; // Compile a string-function call in form of AddHTML(p_html, p_leftSide);
		command = "AddHTML(\"";
		command += p_html;
		command += "\", ";
		command += (p_leftSide?"true":"false");
		command += ");";
		m_view->ExecuteJavascript(Awesomium::WSLit(command.c_str()), Awesomium::WSLit(""));
	}

	void DebugOverlay::Clear()
	{
		m_view->ExecuteJavascript(Awesomium::WSLit("Clear();"), Awesomium::WSLit(""));
	}
}