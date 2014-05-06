#include <RootTools/Treenity/Include/Components/AbstractComponentView.h>

AbstractComponentView::AbstractComponentView(QWidget* p_parent)
	: QWidget(p_parent)
	, m_engineInterface(nullptr)
	, m_editorInterface(nullptr)
{}

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
