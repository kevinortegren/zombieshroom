#pragma once
#include <string>

namespace Awesomium
{
	class WebView; // Define that a class exists to avoid WebSocket and WebSocket2 conflicts caused by mixed includes
}

namespace RootEngine
{
	namespace TextColor
	{
		enum TextColor
		{
			RED = 0,
			YELLOW,
			GREEN,
			WHITE,
			GRAY,
			BLACK
		};
	}

	class DebugOverlayInterface abstract
	{
	public:
		// Initialize debug overlay to a loaded Awesomium view
		virtual void SetView(Awesomium::WebView* p_view) = 0;
		// Javascript call: Add HTML code (or plain text) to left or right side of the debug overlay

		virtual void AddHTMLToBuffer(const char* p_html, TextColor::TextColor p_color = TextColor::WHITE, bool p_leftSide = true) = 0;
		virtual void RenderOverlay() = 0;
	};
}