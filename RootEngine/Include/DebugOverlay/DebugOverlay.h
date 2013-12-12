#ifndef COMPILE_LEVEL_EDITOR
#pragma once
#include <string>
#include <Awesomium/WebCore.h>
#include "DebugOverlayInterface.h"

namespace RootEngine
{
	

	class DebugOverlay : public DebugOverlayInterface
	{
	public:
		DebugOverlay();
		~DebugOverlay();

		void SetView(Awesomium::WebView* p_view);
		void AddHTMLToBuffer(const char* p_html, TextColor::TextColor p_color, bool p_leftSide = true);
		void RenderOverlay();

	private:
		Awesomium::WebView* m_view;
		std::string m_leftOverlay;
		std::string m_rightOverlay;
	};
}
#endif