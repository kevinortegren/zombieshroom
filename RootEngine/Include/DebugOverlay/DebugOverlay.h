#ifndef COMPILE_LEVEL_EDITOR
#pragma once
#include <string>
#include <RootEngine/GUI/Include/WebView.h>
#include "DebugOverlayInterface.h"

namespace RootEngine
{
	

	class DebugOverlay : public DebugOverlayInterface
	{
	public:
		DebugOverlay();
		~DebugOverlay();

		RootEngine::GUISystem::WebView* GetView();
		void SetView(RootEngine::GUISystem::WebView* p_view);

		void AddHTMLToBuffer(const char* p_html, TextColor::TextColor p_color, bool p_leftSide = true);
		void RenderOverlay();

	private:
		RootEngine::GUISystem::WebView* m_view;
		std::string m_leftOverlay;
		std::string m_rightOverlay;
	};
}
#endif
