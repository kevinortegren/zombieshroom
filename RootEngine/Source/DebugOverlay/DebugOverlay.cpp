#ifndef COMPILE_LEVEL_EDITOR
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

	RootEngine::GUISystem::WebView* DebugOverlay::GetView()
	{
		return m_view;
	}

	void DebugOverlay::SetView(RootEngine::GUISystem::WebView* p_view)
	{
		m_view = p_view;
	}

	void DebugOverlay::AddHTMLToBuffer( const char* p_html, TextColor::TextColor p_color, bool p_leftSide /*= true*/ )
	{
		std::string command; // Compile a string-function call in form of AddHTML(p_html, p_leftSide);
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
		command += "</a> <br /> ";

		if(p_leftSide)
			m_leftOverlay += command;
		else
			m_rightOverlay += command;
	}

	void DebugOverlay::RenderOverlay()
	{
		std::string command; // Compile a string-function call in form of AddHTML(p_html, p_leftSide);
		command = "AddHTML(\"";
		command += m_leftOverlay;
		command += "\",true);";
		m_view->BufferJavascript(command);

		command = "AddHTML(\"";
		command += m_rightOverlay;
		command += "\",false);";
		m_view->BufferJavascript(command);

		m_view->Update();
		m_leftOverlay = "";
		m_rightOverlay = "";
	}
}

#endif
