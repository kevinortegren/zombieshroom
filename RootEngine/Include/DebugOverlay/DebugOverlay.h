#pragma once
#include <string>

namespace Awesomium
{
	class WebView; // Define that a class exists to avoid WebSocket and WebSocket2 conflicts caused by mixed includes
}

namespace RootEngine
{
	class DebugOverlayInterface abstract
	{
	public:
		virtual void SetView(Awesomium::WebView* p_view) = 0;
		virtual void AddHTML(std::string p_html, bool p_leftSide = true) = 0;
		virtual void Clear() = 0;
	};

	class DebugOverlay : public DebugOverlayInterface
	{
	public:
		DebugOverlay();
		~DebugOverlay();

		void SetView(Awesomium::WebView* p_view);
		void AddHTML(std::string p_html, bool p_leftSide = true);
		void Clear();

	private:
		Awesomium::WebView* m_view;
	};
}