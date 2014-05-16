#include <RootTools/Treenity/Include/Components/ScriptView.h>

ScriptView::ScriptView( QWidget* p_parent /*= 0*/ )
	: AbstractComponentView(p_parent)
	, m_name("Script")
{
	ui.setupUi(this);
	
	ui.lineEdit_name->AddDropFilter("lua");

	connect(ui.lineEdit_name,	SIGNAL(editingFinished()),	this,		SLOT(NameChanged()));
	connect(ui.toolButton,		SIGNAL(clicked()),			this,		SLOT(BrowseScript()));

}

const QString& ScriptView::GetComponentName() const
{
	return m_name;
}

void ScriptView::DisplayEntity( ECS::Entity* p_entity )
{
	ui.lineEdit_name->setText(QString::fromStdString(m_engineInterface->GetScript(p_entity)));
}

void ScriptView::NameChanged()
{
	if (m_editorInterface->GetSelection().size() == 1)
	{
		ECS::Entity* selectedEntity = *m_editorInterface->GetSelection().begin();

		m_engineInterface->SetScript(selectedEntity, ui.lineEdit_name->text().toStdString());
	}
}

void ScriptView::BrowseScript()
{
	if (m_editorInterface->GetSelection().size() == 1)
	{
		ECS::Entity* selectedEntity = *m_editorInterface->GetSelection().begin();
		ui.lineEdit_name->setText(QString::fromStdString(m_engineInterface->GetScript(selectedEntity)));
	}
}

