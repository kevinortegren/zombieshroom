#include <RootTools/Treenity/Include/Components/RenderableView.h>
#include <QPixmapCache>
#include <qimagereader.h>
#include <qimage.h>
#include <QtConcurrent/QtConcurrent>
#include <RootTools/Treenity/Include/Utils.h>
#include <qfiledialog.h>

#include <RootTools/Treenity/Include/MaterialExporter.h>
#include <RootSystems/Include/RenderingSystem.h>

RenderableView::RenderableView(QWidget* p_parent)
	: AbstractComponentView(p_parent)
	, m_name("Renderable")
{
	ui.setupUi(this);

	ui.lineEdit_modelName->AddDropFilter("dae");

	// Create shader views.
	m_shaderViews.resize(1);
	m_shaderViews[0] = new DiffuseShaderView();

	/*ui.label_image_diffuse->AddDropFilter("dds");
	ui.label_image_diffuse_1->AddDropFilter("dds");
	ui.label_image_specular->AddDropFilter("dds");
	ui.label_image_specular_1->AddDropFilter("dds");
	ui.label_image_glow->AddDropFilter("dds");
	ui.label_image_glow_1->AddDropFilter("dds");
	ui.label_image_normal_1->AddDropFilter("dds");*/


	connect(ui.lineEdit_materialName,		SIGNAL(editingFinished()),			this,		SLOT(MaterialNameChanged()));
	connect(ui.lineEdit_modelName,			SIGNAL(editingFinished()),			this,		SLOT(ModelNameChanged()));
	connect(ui.pushButton_newMaterial,		SIGNAL(pressed()),					this,		SLOT(NewMaterial()));
	connect(ui.comboBox_shaderSelect,		SIGNAL(currentIndexChanged(int)),	this,		SLOT(ShaderChanged(int)));

	/*connect(ui.label_image_diffuse,			SIGNAL(dropped(const QString&)),		   this,		SLOT(DiffuseTextureDropped(const QString&)));
	connect(ui.label_image_diffuse_1,		SIGNAL(dropped(const QString&)),		   this,		SLOT(DiffuseTextureDropped(const QString&)));
	connect(ui.label_image_specular,		SIGNAL(dropped(const QString&)),		   this,		SLOT(SpecularTextureDropped(const QString&)));
	connect(ui.label_image_specular_1,		SIGNAL(dropped(const QString&)),		   this,		SLOT(SpecularTextureDropped(const QString&)));
	connect(ui.label_image_glow,			SIGNAL(dropped(const QString&)),		   this,		SLOT(GlowTextureDropped(const QString&)));
	connect(ui.label_image_glow_1,			SIGNAL(dropped(const QString&)),		   this,		SLOT(GlowTextureDropped(const QString&)));
	connect(ui.label_image_normal_1,		SIGNAL(dropped(const QString&)),		   this,		SLOT(NormalTextureDropped(const QString&)));*/

}

const QString& RenderableView::GetComponentName() const
{
	return m_name;
}

void RenderableView::DisplayEntity(ECS::Entity* p_entity)
{
	ui.lineEdit_modelName->setText(QString::fromUtf8(m_engineInterface->GetRenderableModelName(p_entity).c_str()));
	ui.lineEdit_materialName->setText(QString::fromUtf8(m_engineInterface->GetRenderableMaterialName(p_entity).c_str()));

	Render::Material* material = m_engineInterface->GetMaterial(ui.lineEdit_materialName->text().toStdString());

	m_currentEntity = p_entity;
	m_currentMaterial = material;
	
	/*if(material->m_effect != nullptr)
	{
		// Get effect name.
		const std::string effectName = m_engineInterface->GetMaterialEffectName(m_currentMaterial);

		// Switch to correct page.
		if(effectName == "Mesh")
		{
			ui.stackedWidget_shaderProperties->setCurrentIndex(0);
			QtConcurrent::run(this, &RenderableView::DisplayDiffuse, material);
		}
		else if(effectName == "Mesh_NormalMap")
		{
			ui.stackedWidget_shaderProperties->setCurrentIndex(1);
			QtConcurrent::run(this, &RenderableView::DisplayDiffuseNormal, material);
		}
		else
		{
			Utils::GetInstance()->Write("No widget implemented for effect" + QString::fromStdString(effectName));

			ui.label_image_diffuse->setPixmap(QPixmap());
			ui.label_image_specular->setPixmap(QPixmap());
			ui.label_image_glow->setPixmap(QPixmap());
			ui.label_image_diffuse_1->setPixmap(QPixmap());
			ui.label_image_specular_1->setPixmap(QPixmap());
			ui.label_image_normal_1->setPixmap(QPixmap());
			ui.label_image_glow_1->setPixmap(QPixmap());
		}
	}*/
}

void RenderableView::NewMaterial()
{
	QString fileName = QFileDialog::getSaveFileName(this, "Save Material", "", "Material File (*.material)");
	if (fileName != "")
	{
		// Get material.
		QFileInfo info(fileName);
		QString baseName = info.completeBaseName();

		// Create new material.
		Render::Material* material = m_engineInterface->GetMaterial(baseName.toStdString());
		material->m_effect = m_engineInterface->GetEffect("Mesh");

		// Export a file.
		MaterialExporter e;
		e.Export(material, fileName.toStdString());

		m_engineInterface->SetRenderableMaterialName(*m_editorInterface->GetSelection().begin(), baseName.toStdString());

		DisplayEntity(m_currentEntity);
	}
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

void RenderableView::ShaderChanged(int index)
{
	if((size_t)index < m_shaderViews.size())
	{
		m_shaderViews[0]->show();
		ui.verticalLayout_2->addWidget(m_shaderViews[0]);
		//ui.verticalLayout_2->addStretch();
	}
}

void RenderableView::DiffuseTextureDropped(const QString& p_path)
{
	m_currentMaterial->m_textures[Render::TextureSemantic::DIFFUSE] = m_engineInterface->GetTexture(p_path.toStdString());
}

void RenderableView::SpecularTextureDropped(const QString& p_path)
{
	m_currentMaterial->m_textures[Render::TextureSemantic::SPECULAR] = m_engineInterface->GetTexture(p_path.toStdString());
}

void RenderableView::GlowTextureDropped(const QString& p_path)
{
	m_currentMaterial->m_textures[Render::TextureSemantic::GLOW] = m_engineInterface->GetTexture(p_path.toStdString());
}

void RenderableView::NormalTextureDropped(const QString& p_path)
{
	m_currentMaterial->m_textures[Render::TextureSemantic::NORMAL] = m_engineInterface->GetTexture(p_path.toStdString()); 
}

QPixmap RenderableView::GetPixmap(Render::TextureInterface* p_texture)
{
	std::string fullTexturePath = m_engineInterface->GetContentPath() + "Assets\\Textures\\" +  m_engineInterface->GetTextureName(p_texture) + ".dds";
			
	std::replace( fullTexturePath.begin(), fullTexturePath.end(), '\\', '/');
			
	// find textures.
	int limit = QPixmapCache::cacheLimit();
	QPixmap pm;
	if (!QPixmapCache::find(QString::fromStdString(fullTexturePath), &pm))
	{
		QImageReader reader(QString::fromStdString(fullTexturePath));

		if(!reader.canRead())
		{
			return pm;
		}

		QImage image = reader.read();
		if(image.isNull())
		{
			return pm;
		}

		pm = QPixmap::fromImage(image);

		QPixmapCache::insert(QString::fromStdString(fullTexturePath), pm);
	}

	return pm;
}

void RenderableView::DisplayDiffuse(Render::Material* p_material)
{
	/*ui.label_image_diffuse->setPixmap(QPixmap());
	ui.label_image_specular->setPixmap(QPixmap());
	ui.label_image_glow->setPixmap(QPixmap());

	for(auto itr = p_material->m_textures.begin(); itr != p_material->m_textures.end(); ++itr)
	{
		if((*itr).first == Render::TextureSemantic::DIFFUSE)
		{
			QPixmap pm = GetPixmap((*itr).second);			
			ui.label_image_diffuse->setPixmap(pm.scaled(ui.label_image_diffuse->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
		}
		else if((*itr).first == Render::TextureSemantic::SPECULAR)
		{
			QPixmap pm = GetPixmap((*itr).second);
			ui.label_image_specular->setPixmap(pm.scaled(ui.label_image_specular->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
		}
		else if((*itr).first == Render::TextureSemantic::GLOW)
		{
			QPixmap pm = GetPixmap((*itr).second);
			ui.label_image_glow->setPixmap(pm.scaled(ui.label_image_glow->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
		}
	}*/
}

void RenderableView::DisplayDiffuseNormal(Render::Material* p_material)
{	
	/*ui.label_image_diffuse_1->setPixmap(QPixmap());
	ui.label_image_specular_1->setPixmap(QPixmap());
	ui.label_image_normal_1->setPixmap(QPixmap());
	ui.label_image_glow_1->setPixmap(QPixmap());

	for(auto itr = p_material->m_textures.begin(); itr != p_material->m_textures.end(); ++itr)
	{
		if((*itr).first == Render::TextureSemantic::DIFFUSE)
		{
			QPixmap pm = GetPixmap((*itr).second);			
			ui.label_image_diffuse_1->setPixmap(pm.scaled(ui.label_image_diffuse_1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
		}
		else if((*itr).first == Render::TextureSemantic::SPECULAR)
		{
			QPixmap pm = GetPixmap((*itr).second);		
			ui.label_image_specular_1->setPixmap(pm.scaled(ui.label_image_specular_1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
		}
		else if((*itr).first == Render::TextureSemantic::NORMAL)
		{
			QPixmap pm = GetPixmap((*itr).second);	
			ui.label_image_normal_1->setPixmap(pm.scaled(ui.label_image_normal_1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
		}
		else if((*itr).first == Render::TextureSemantic::GLOW)
		{
			QPixmap pm = GetPixmap((*itr).second);		
			ui.label_image_glow_1->setPixmap(pm.scaled(ui.label_image_glow_1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
		}
	}*/
}
