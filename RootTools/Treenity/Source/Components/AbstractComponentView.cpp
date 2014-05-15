#include <RootTools/Treenity/Include/Components/AbstractComponentView.h>

AbstractComponentView::AbstractComponentView(QWidget* p_parent)
	: QFrame(p_parent)
	, m_engineInterface(nullptr)
	, m_editorInterface(nullptr)
{
	setVisible(false);
	setFrameStyle(QFrame::StyledPanel);
	setLineWidth(1);
}

AbstractComponentView::~AbstractComponentView()
{}


void AbstractComponentView::SetEngineInterface(EngineInterface* p_engineInterface)
{
	m_engineInterface = p_engineInterface;
}

void AbstractComponentView::SetEditorInterface(EditorInterface* p_editorInterface)
{
	m_editorInterface = p_editorInterface;
}
