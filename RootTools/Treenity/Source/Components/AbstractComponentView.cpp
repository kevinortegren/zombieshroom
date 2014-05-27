#include <RootTools/Treenity/Include/Components/AbstractComponentView.h>

AbstractComponentView::AbstractComponentView(QWidget* p_parent)
	: QFrame(p_parent)
	, m_engineInterface(nullptr)
	, m_editorInterface(nullptr)
	, m_receiveUpdates(true)
	, m_shown(false)
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

void AbstractComponentView::SetReceiveUpdates(bool p_value)
{
	m_receiveUpdates = p_value;
}

bool AbstractComponentView::IsReceivingUpdates() const
{
	return m_receiveUpdates;
}

EditorInterface* AbstractComponentView::GetEditorInterface()
{
	return m_editorInterface;
}

EngineInterface* AbstractComponentView::GetEngineInterface()
{
	return m_engineInterface;
}

void AbstractComponentView::Delete(ECS::Entity* p_entity)
{
	emit deleted(p_entity);
}

void AbstractComponentView::Show(bool p_value)
{
	m_shown = p_value;
}

