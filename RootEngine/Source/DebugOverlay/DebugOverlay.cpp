#include "DebugOverlay/DebugOverlay.h"

#ifndef COMPILE_LEVEL_EDITOR

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

	void DebugOverlay::AddHTML(const char* p_html, TextColor::TextColor p_color, bool p_leftSide)
	{
		std::string command; // Compile a string-function call in form of AddHTML(p_html, p_leftSide);
		command = "AddHTML(\"";
		switch(p_color)
		{
		case TextColor::RED:
			command += "<a class='red'>";
			break;
		case TextColor::YELLOW:
			command += "<a class='yellow'>";
			break;
		case TextColor::WHITE:
			command += "<a class='white'>";
			break;
		case TextColor::GREEN:
			command += "<a class='green'>";
			break;
		case TextColor::GRAY:
			command += "<a class='gray'>";
			break;
		case TextColor::BLACK:
			command += "<a class='black'>";
			break;
		default:
			command += "<a>";
		}
		command += p_html;
		command += "</a>\", ";
		command += (p_leftSide?"true":"false");
		command += ");";
		m_view->ExecuteJavascript(Awesomium::WSLit(command.c_str()), Awesomium::WSLit(""));
	}

	void DebugOverlay::Clear()
	{
		m_view->ExecuteJavascript(Awesomium::WSLit("Clear();"), Awesomium::WSLit(""));
	}
}

#endif
