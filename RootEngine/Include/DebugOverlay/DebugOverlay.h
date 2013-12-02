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
		void AddHTML(const char* p_html, bool p_leftSide = true);
		void Clear();

	private:
		Awesomium::WebView* m_view;
	};
}