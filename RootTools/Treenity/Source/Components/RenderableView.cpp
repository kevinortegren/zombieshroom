#include <RootTools/Treenity/Include/Components/RenderableView.h>

RenderableView::RenderableView(QWidget* p_parent)
	: AbstractComponentView(p_parent)
	, m_name("Renderable")
{
	ui.setupUi(this);

	connect(ui.lineEdit_materialName,		SIGNAL(editingFinished()), this,		SLOT(MaterialNameChanged()));
	connect(ui.lineEdit_modelName,			SIGNAL(editingFinished()), this,		SLOT(ModelNameChanged()));
}

const QString& RenderableView::GetComponentName() const
{
	return m_name;
}

void RenderableView::DisplayEntity(ECS::Entity* p_entity)
{
	ui.lineEdit_modelName->setText(QString::fromUtf8(m_engineInterface->GetRenderableModelName(p_entity).c_str()));
	ui.lineEdit_materialName->setText(QString::fromUtf8(m_engineInterface->GetRenderableMaterialName(p_entity).c_str()));
}

void RenderableView::MaterialNameChanged()
{
	QString p_materialName = ui.lineEdit_materialName->text();
	if (m_editorInterface->GetSelection().size() == 1)
	{
		ECS::Entity* selectedEntity = *m_editorInterface->GetSelection().begin();

		std::string materialName = m_engineInterface->GetRenderableMaterialName(selectedEntity);
		materialName = p_materialName.toStdString();
		m_engineInterface->SetRenderableMaterialName(selectedEntity, materialName);
	}
}

void RenderableView::ModelNameChanged()
{
	QString p_modelName = ui.lineEdit_modelName->text();
	if (m_editorInterface->GetSelection().size() == 1)
	{
		ECS::Entity* selectedEntity = *m_editorInterface->GetSelection().begin();

		std::string modelName = m_engineInterface->GetRenderableModelName(selectedEntity);
		modelName = p_modelName.toStdString();
		m_engineInterface->SetRenderableModelName(selectedEntity, modelName);
	}
}