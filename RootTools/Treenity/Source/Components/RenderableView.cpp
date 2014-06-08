#include <RootTools/Treenity/Include/Components/RenderableView.h>
#include <QPixmapCache>
#include <qimagereader.h>
#include <qimage.h>
#include <QtConcurrent/QtConcurrent>
#include <RootTools/Treenity/Include/Utils.h>
#include <QInputDialog.h>

#include <RootTools/Treenity/Include/MaterialExporter.h>
#include <RootTools/Treenity/Include/MaterialImporter.h>

#include <RootSystems/Include/RenderingSystem.h>

#include <RootTools/Treenity/Include/Shaders/BlendShaderView.h>
#include <RootTools/Treenity/Include/Shaders/DiffuseNormalView.h>
#include <RootTools/Treenity/Include/Shaders/DiffuseNormalTransView.h>
#include <RootTools/Treenity/Include/Shaders/RefractiveClearView.h>
#include <RootTools/Treenity/Include/Shaders/RefractiveView.h>

RenderableView::RenderableView(QWidget* p_parent)
	: AbstractComponentView(p_parent)
	, m_name("Renderable")
	, m_currentView(nullptr)
{
	ui.setupUi(this);
	ui.lineEdit_modelName->AddDropFilter("dae");
	ui.lineEdit_materialName->AddDropFilter("material");

	// Create shader views.
	m_shaderViews.push_back(new DiffuseShaderView());
	m_shaderViews.push_back(new DiffuseNormalShaderView());
	m_shaderViews.push_back(new DiffuseNormalTransShaderView());
	m_shaderViews.push_back(new RefractiveShaderView());	
	m_shaderViews.push_back(new RefractiveClearShaderView());
	m_shaderViews.push_back(new BlendShaderView());
	
	// Map root engine effect names to shader views.
	m_effectToShaderIndex["Mesh"] = 0;
	m_effectToShaderIndex["Mesh_NormalMap"] = 1;
	m_effectToShaderIndex["Mesh_Normal_Trans"] = 2;
	m_effectToShaderIndex["Mesh_Refractive"] = 3;
	m_effectToShaderIndex["Mesh_Refractive_Clear"] = 4;
	m_effectToShaderIndex["Mesh_Blend"] = 5;
	
	connect(ui.lineEdit_materialName,		SIGNAL(editingFinished()),			this,		SLOT(MaterialNameChanged()));
	connect(ui.lineEdit_modelName,			SIGNAL(editingFinished()),			this,		SLOT(ModelNameChanged()));
	connect(ui.pushButton_newMaterial,		SIGNAL(pressed()),					this,		SLOT(NewMaterial()));
	connect(ui.comboBox_shaderSelect,		SIGNAL(activated(int)),				this,		SLOT(ShaderChanged(int)));
}

const QString& RenderableView::GetComponentName() const
{
	return m_name;
}

void RenderableView::SetEngineInterface(EngineInterface* p_engineInterface)
{
	// Set interface in shader views.
	for(auto shaderViews : m_shaderViews)
		shaderViews->SetEngineInterface(p_engineInterface);

	AbstractComponentView::SetEngineInterface(p_engineInterface);
}

void RenderableView::DisplayEntity(ECS::Entity* p_entity)
{
	ui.lineEdit_modelName->setText(QString::fromUtf8(m_engineInterface->GetRenderableModelName(p_entity).c_str()));
	ui.lineEdit_materialName->setText(QString::fromUtf8(m_engineInterface->GetRenderableMaterialName(p_entity).c_str()));

	m_currentMaterial = m_engineInterface->GetMaterial(ui.lineEdit_materialName->text().toStdString());
	
	if(m_currentMaterial->m_effect != nullptr)
	{
		// Get effect name.
		const std::string effectName = m_engineInterface->GetMaterialEffectName(m_currentMaterial);

		if(m_effectToShaderIndex.find(effectName) != m_effectToShaderIndex.end())
		{
			ui.comboBox_shaderSelect->setCurrentIndex(m_effectToShaderIndex[effectName]);

			ShaderChanged(m_effectToShaderIndex[effectName]);
		}
		else
		{
			Clear();
		}
	}
}

void RenderableView::NewMaterial()
{
	bool ok;
	QString name = QInputDialog::getText(this, tr("Enter Material Name"), tr("Name:"), QLineEdit::Normal, "DefaultMaterial", &ok);

	if (ok && !name.isEmpty())
	{
		// Create new material.
		Render::Material* material = m_engineInterface->GetMaterial(name.toStdString());
		material->m_effect = m_engineInterface->GetEffect("Mesh");

		const std::string savePath = m_engineInterface->GetContentPath() + "Assets\\Materials\\" + name.toStdString() + ".material";

		MaterialExporter e;
		e.Export(material, savePath);

		m_engineInterface->SetRenderableMaterialName(*m_editorInterface->GetSelection().begin(), name.toStdString());

		DisplayEntity(*m_editorInterface->GetSelection().begin());
	}
}

void RenderableView::MaterialNameChanged()
{
	QString p_materialName = ui.lineEdit_materialName->text();
	if(p_materialName.size() == 0)
		return;

	if (m_editorInterface->GetSelection().size() == 1)
	{
		ECS::Entity* selectedEntity = *m_editorInterface->GetSelection().begin();

		std::string materialName = m_engineInterface->GetRenderableMaterialName(selectedEntity);
		materialName = p_materialName.toStdString();

		const std::string loadPath = m_engineInterface->GetContentPath() + "Assets\\Materials\\" + materialName + ".material";

		MaterialImporter i;
		if(i.Import(loadPath) == nullptr)
			return;

		m_engineInterface->SetRenderableMaterialName(*m_editorInterface->GetSelection().begin(), materialName);

		DisplayEntity(selectedEntity);
	}	
}

void RenderableView::ModelNameChanged()
{
	QString p_modelName = ui.lineEdit_modelName->text();
	if(p_modelName.size() == 0)
		return;

	if (m_editorInterface->GetSelection().size() == 1)
	{
		ECS::Entity* selectedEntity = *m_editorInterface->GetSelection().begin();

		std::string modelName = m_engineInterface->GetRenderableModelName(selectedEntity);
		modelName = p_modelName.toStdString();
		m_engineInterface->SetRenderableModelName(selectedEntity, modelName);
	}
}

void RenderableView::ShaderChanged(int index)
{
	Clear();

	if((size_t)index < m_shaderViews.size())
	{
		std::string effectName;
		for(auto name : m_effectToShaderIndex)
		{
			if(name.second == index)
			{
				effectName = name.first;
				break;
			}
		}

		m_currentMaterial->m_effect = m_engineInterface->GetEffect(effectName);
		m_shaderViews[index]->SetMaterialName(ui.lineEdit_materialName->text());
		m_shaderViews[index]->SetMaterial(m_currentMaterial);
		m_shaderViews[index]->Display();

		//MAgic
		// Remove last spacer item if present.
		int count = ui.verticalLayout_2->count();
		if (count > 1) {
			ui.verticalLayout_2->removeItem(ui.verticalLayout_2->itemAt(count - 1));
		}

		ui.verticalLayout_2->addWidget(m_shaderViews[index]);

		//MAgic
		ui.verticalLayout_2->addStretch();

		m_currentView = m_shaderViews[index];
	}
}

void RenderableView::Clear()
{
	QLayoutItem *wItem;
	while ((wItem = ui.verticalLayout_2->takeAt(0)) != 0)
	{
		if (wItem->widget())
			wItem->widget()->setParent(NULL);
		delete wItem;
	}
}


