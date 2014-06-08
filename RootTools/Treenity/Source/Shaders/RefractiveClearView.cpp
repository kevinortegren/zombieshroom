#include <RootTools/Treenity/Include/Shaders/RefractiveClearView.h>

extern RootEngine::GameSharedContext g_engineContext;

RefractiveClearShaderView::RefractiveClearShaderView(QWidget* p_parent)
	: AbstractShaderView(p_parent)
{
	ui.setupUi(this);
}

void RefractiveClearShaderView::SetEngineInterface(EngineInterface* p_engineInterface)
{
	AbstractShaderView::SetEngineInterface(p_engineInterface);
}

void RefractiveClearShaderView::Display()
{
	
}
