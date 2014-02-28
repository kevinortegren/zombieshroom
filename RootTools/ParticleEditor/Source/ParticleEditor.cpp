#include "ParticleEditor.h"

ParticleEditor::ParticleEditor(QWidget *parent)
	: QMainWindow(parent), m_running(true), m_showGrid(true), m_showAxis(true), m_firstDrag(true)
{
	ui.setupUi(this);
	ui.aboutWidget->hide();
	ui.helpWidget->hide();
}

ParticleEditor::~ParticleEditor()
{
	delete m_particleTab;
	delete m_colorTriangle;
	delete m_colorEndTriangle;
	delete m_fileSystemModel;
	delete m_fileSystemModelModel;
	delete m_fileSystemModelModelTex;
}

void ParticleEditor::ConnectSignalsAndSlots()
{
	connect(ui.actionExit,			SIGNAL(triggered()),						this, SLOT(MenuExit()));
	connect(ui.actionHelp,			SIGNAL(triggered()),						this, SLOT(MenuHelpHelp()));
	connect(ui.actionAbout,			SIGNAL(triggered()),						this, SLOT(MenuHelpAbout()));
	connect(ui.actionNew,			SIGNAL(triggered()),						this, SLOT(MenuNew()));
	connect(ui.actionOpen,			SIGNAL(triggered()),						this, SLOT(MenuOpen()));
	connect(ui.actionSave,			SIGNAL(triggered()),						this, SLOT(MenuSave()));
	connect(ui.actionSave_As,		SIGNAL(triggered()),						this, SLOT(ManuSaveAs()));
	connect(ui.newEmitterButton,	SIGNAL(clicked()),							this, SLOT(NewEmitter()));
	connect(ui.actionStats,			SIGNAL(triggered()),						this, SLOT(MenuViewStats()));
	connect(ui.deleteEmitterButton, SIGNAL(clicked()),							this, SLOT(DeleteEmitter()));
	connect(ui.renameEmitterButton, SIGNAL(clicked()),							this, SLOT(RenameEmitter()));
	connect(ui.listWidget,			SIGNAL(itemClicked(QListWidgetItem*)),		this, SLOT(EmitterSelected(QListWidgetItem*)));
	connect(ui.actionGrid_2,		SIGNAL(triggered()),						this, SLOT(GridToggled()));
	connect(ui.posSpinBoxX,			SIGNAL(valueChanged(double)),				this, SLOT(PositionXChanged(double)));
	connect(ui.posSpinBoxY,			SIGNAL(valueChanged(double)),				this, SLOT(PositionYChanged(double)));
	connect(ui.posSpinBoxZ,			SIGNAL(valueChanged(double)),				this, SLOT(PositionZChanged(double)));
	connect(ui.sizeMinSpinBoxX,		SIGNAL(valueChanged(double)),				this, SLOT(SizeMinXChanged(double)));
	connect(ui.sizeMaxSpinBoxX,		SIGNAL(valueChanged(double)),				this, SLOT(SizeMaxXChanged(double)));
	connect(ui.sizeEndSpinBoxX,		SIGNAL(valueChanged(double)),				this, SLOT(SizeEndXChanged(double)));
	connect(ui.lifeTimeMinSpinBox,	SIGNAL(valueChanged(double)),				this, SLOT(LifeTimeMinChanged(double)));
	connect(ui.lifeTimeMaxSpinBox,	SIGNAL(valueChanged(double)),				this, SLOT(LifeTimeMaxChanged(double)));
	connect(ui.gravitySpinBoxX,		SIGNAL(valueChanged(double)),				this, SLOT(GravityXChanged(double)));
	connect(ui.gravitySpinBoxY,		SIGNAL(valueChanged(double)),				this, SLOT(GravityYChanged(double)));
	connect(ui.gravitySpinBoxZ,		SIGNAL(valueChanged(double)),				this, SLOT(GravityZChanged(double)));
	connect(ui.directionSpinBoxX,	SIGNAL(valueChanged(double)),				this, SLOT(DirectionXChanged(double)));
	connect(ui.directionSpinBoxY,	SIGNAL(valueChanged(double)),				this, SLOT(DirectionYChanged(double)));
	connect(ui.directionSpinBoxZ,	SIGNAL(valueChanged(double)),				this, SLOT(DirectionZChanged(double)));
	connect(ui.speedMinSpinBox,		SIGNAL(valueChanged(double)),				this, SLOT(SpeedMinChanged(double)));
	connect(ui.speedMaxSpinBox,		SIGNAL(valueChanged(double)),				this, SLOT(SpeedMaxChanged(double)));
	connect(ui.spreadSpinBox,		SIGNAL(valueChanged(double)),				this, SLOT(SpreadChanged(double)));
	connect(ui.spawnTimeSpinBox,	SIGNAL(valueChanged(double)),				this, SLOT(SpawnTimeChanged(double)));
	connect(ui.textureTreeView,		SIGNAL(doubleClicked(const QModelIndex&)),	this, SLOT(TextureDoubleClicked(const QModelIndex&)));
	connect(ui.modelTreeView,		SIGNAL(doubleClicked(const QModelIndex&)),	this, SLOT(ModelDoubleClicked(const QModelIndex&)));
	connect(ui.modeltexTreeView,	SIGNAL(doubleClicked(const QModelIndex&)),	this, SLOT(ModelTexDoubleClicked(const QModelIndex&)));
	connect(m_colorTriangle,		SIGNAL(colorChanged(const QColor&)),		this, SLOT(ColorChanged(const QColor&)));
	connect(m_colorEndTriangle,		SIGNAL(colorChanged(const QColor&)),		this, SLOT(ColorEndChanged(const QColor&)));
	connect(ui.actionColor_Triangle,SIGNAL(triggered()),						this, SLOT(MenuViewColorTriangle()));
	connect(ui.actionEndColor_Triangle,SIGNAL(triggered()),						this, SLOT(MenuViewColorEndTriangle()));
	connect(ui.gridSpaceSpinBox,	SIGNAL(valueChanged(double)),				this, SLOT(GridSizeChanged(double)));
	connect(ui.colorAlphaSlider,	SIGNAL(sliderMoved(int)),					this, SLOT(colorAlphaSliderChanged(int)));
	connect(ui.endcolorAlphaSlider,	SIGNAL(sliderMoved(int)),					this, SLOT(endColorAlphaSliderChanged(int)));
	connect(ui.templateComboBox,	SIGNAL(currentIndexChanged(int)),			this, SLOT(TemplateChanged(int)));
	connect(ui.orbitRadiusSpinBox,  SIGNAL(valueChanged(double)),				this, SLOT(OrbitRadiusChanged(double)));
	connect(ui.orbitSpeedSpinBox,  SIGNAL(valueChanged(double)),				this, SLOT(OrbitSpeedChanged(double)));
	connect(ui.spreadSlider,		SIGNAL(sliderMoved(int)),					this, SLOT(SpreadSliderChanged(int)));
	connect(ui.bgColorComboBox,		SIGNAL(currentIndexChanged(int)),			this, SLOT(BackgroundColorChanged(int)));
	connect(ui.removeObjectButton,	SIGNAL(clicked()),							this, SLOT(RemoveObjectButton()));
}

void ParticleEditor::Init()
{
	//Create the tab bar on top of the 3D view, each tab is a Entity with a transform and a particle emitter component
	m_particleTab = new ParticleTab(this);
	m_particleTab->setGeometry(QRect(0, ui.menuBar->height(), 999, 27));
	m_particleTab->show();

	ui.menuView->setEnabled(true);

	ui.tabWidget->setEnabled(false);
	ui.tabWidget->hide();

	m_selectedEmitterIndex	= -1;
	m_selectedEntityIndex	= -1;
	m_materialIndex			= 0;
	m_gridSpace				= 0.5f;
	m_samples				= 0;
	m_collectedTime			= 0.0f;

	m_fileSystemModelModel = new QFileSystemModel;
	m_fileSystemModelModel->setRootPath(QString::fromStdString(m_workingDirectory + "Assets/Models/"));
	m_fileSystemModel = new QFileSystemModel;
	m_fileSystemModel->setRootPath(QString::fromStdString(m_workingDirectory + "Assets/Textures/"));
	m_fileSystemModelModelTex = new QFileSystemModel;
	m_fileSystemModelModelTex->setRootPath(QString::fromStdString(m_workingDirectory + "Assets/Textures/"));
	//Texture browser
	QStringList filters;
	filters << "*.dds";
	m_fileSystemModel->setNameFilters(filters);
	m_fileSystemModel->setNameFilterDisables(false);
	ui.textureTreeView->setModel(m_fileSystemModel);
	ui.textureTreeView->setColumnWidth(0, 200);
	ui.textureTreeView->hideColumn(2);
	ui.textureTreeView->setRootIndex(m_fileSystemModel->index(QString::fromStdString(m_workingDirectory + "Assets/Textures/")));
	//Model texture browser
	m_fileSystemModelModelTex->setNameFilters(filters);
	m_fileSystemModelModelTex->setNameFilterDisables(false);
	ui.modeltexTreeView->setModel(m_fileSystemModelModelTex);
	ui.modeltexTreeView->setColumnWidth(0, 200);
	ui.modeltexTreeView->hideColumn(2);
	ui.modeltexTreeView->setRootIndex(m_fileSystemModelModelTex->index(QString::fromStdString(m_workingDirectory + "Assets/Textures/")));
	//Model browser
	QStringList filtersModel;
	filtersModel << "*.dae";
	m_fileSystemModelModel->setNameFilters(filtersModel);
	m_fileSystemModelModel->setNameFilterDisables(false);
	ui.modelTreeView->setModel(m_fileSystemModelModel);
	ui.modelTreeView->setColumnWidth(0, 200);
	ui.modelTreeView->hideColumn(2);
	ui.modelTreeView->setRootIndex(m_fileSystemModelModel->index(QString::fromStdString(m_workingDirectory + "Assets/Models/")));
	
	m_colorTriangle = new QtColorTriangle(ui.colorDockWidget);
	m_colorTriangle->show();
	m_colorTriangle->setGeometry(0, 20, 200.0f, 180.0f);

	m_colorEndTriangle = new QtColorTriangle(ui.endcolorDockWidget);
	m_colorEndTriangle->show();
	m_colorEndTriangle->setGeometry(0, 20, 200.0f, 180.0f);

	ui.endcolorDockWidget->hide();
	ui.colorDockWidget->hide();

	//Add new tab
	m_particleTab->AddTab();

	m_savePath = "";

	ECS::Entity* renderModel = m_world->GetEntityManager()->CreateEntity();
	m_modelTrans = m_world->GetEntityManager()->CreateComponent<RootForce::Transform>(renderModel);
	m_modelTrans->m_scale = glm::vec3(0.0f);
	m_model = m_world->GetEntityManager()->CreateComponent<RootForce::Renderable>(renderModel);
	m_model->m_material = g_engineContext.m_renderer->CreateMaterial("ModelMaterial");
	m_model->m_model = g_engineContext.m_resourceManager->LoadCollada("Primitives/sphere");
	m_model->m_material->m_textures[Render::TextureSemantic::DIFFUSE] = g_engineContext.m_resourceManager->LoadTexture("blockMana", Render::TextureType::TextureType::TEXTURE_2D);
	m_model->m_material->m_effect = g_engineContext.m_resourceManager->LoadEffect("Mesh");

	m_inverseProjection = glm::inverse(glm::perspectiveFov<float>(45.0f, (float)ui.frame->width(), (float)ui.frame->height(), 0.1f, 100.0f));

	Saved();

	m_axisAABB[0] = AxisBoundingBox(glm::vec3(0.1f, -0.1f, -0.1f), glm::vec3(0.5f, 0.1f, 0.1f));//X
	m_axisAABB[1] = AxisBoundingBox(glm::vec3(-0.1f, 0.1f, -0.1f), glm::vec3(0.1f, 0.5f, 0.1f));//Y
	m_axisAABB[2] = AxisBoundingBox(glm::vec3(-0.1f, -0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.5f));//Z
}

bool ParticleEditor::CheckExit()
{
	return m_running;
}

void ParticleEditor::closeEvent( QCloseEvent *event )
{
	if(SaveWarningDialog() == QMessageBox::Cancel)
		event->ignore();
	else
	{
		event->accept();
		m_running = false;
	}
}

void ParticleEditor::Update( float p_dt )
{
	m_samples++;
	m_collectedTime += p_dt;
	if(m_collectedTime >= 0.5f)
	{
		int fps = (int)(1.0f/(m_collectedTime/(float)m_samples));
		ui.FPSlcdNumber->display(fps);
		m_samples = 0;
		m_collectedTime = 0;
	}

	if(m_showGrid)
		DrawGridX(m_gridSpace, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));

	if(m_showAxis)
		DrawPositionAxis();
}

#pragma region File menu actions
void ParticleEditor::MenuExit()
{
	close();
}

void ParticleEditor::MenuNew()
{
	
	if(SaveWarningDialog() == QMessageBox::Cancel)
		return;

	ClearScene();

	//Enable the New Emitter button 
	ui.newEmitterButton->setEnabled(true);

	//Create the Entity with transform and particle components
	ECS::Entity* p = m_world->GetEntityManager()->CreateEntity();
	RootForce::Transform* t = m_world->GetEntityManager()->CreateComponent<RootForce::Transform>(p);
	t->m_position = glm::vec3(0,0,0);
	RootForce::ParticleEmitter* e = m_world->GetEntityManager()->CreateComponent<RootForce::ParticleEmitter>(p);

	//Reserve 5 slots for particle emitters in this entity
	e->m_particleSystems.reserve(5);

	//Push back this Entity in the local entity vector
	m_emitterEntities.push_back(p);

	//Show property pages if the first is created
	if(m_selectedEntityIndex == -1)
	{
		m_selectedEntityIndex = 0; 
	}
	m_particleTab->setTabText(0, "Untitled project 0");
	Saved();
}

void ParticleEditor::MenuOpen()
{
	if(SaveWarningDialog() == QMessageBox::Cancel)
		return;

	QString fullFileName = QFileDialog::getOpenFileName(this, tr("Open file"), "", tr("PARTICLE-file (*.particle)"));
	if(fullFileName.compare("") == 0)
		return;

	ClearScene();
	//Enable the New Emitter button 
	ui.newEmitterButton->setEnabled(true);

	//Create the Entity with transform and particle components
	ECS::Entity* p = m_world->GetEntityManager()->CreateEntity();
	RootForce::Transform* t = m_world->GetEntityManager()->CreateComponent<RootForce::Transform>(p);
	t->m_position = glm::vec3(0,0,0);
	RootForce::ParticleEmitter* e = m_world->GetEntityManager()->CreateComponent<RootForce::ParticleEmitter>(p);

	//Push back this Entity in the local entity vector
	m_emitterEntities.push_back(p);

	//Show property pages if the first is created
	if(m_selectedEntityIndex == -1)
	{
		m_selectedEntityIndex = 0; 
	}

	e->m_particleSystems = m_context->m_resourceManager->LoadParticleEmitter(fullFileName.toStdString(), true);

	for (unsigned int i = 0; i < e->m_particleSystems.size(); i++)
	{
		e->m_systems.push_back(m_context->m_renderer->CreateParticleSystem());
		//Add an item representing the emitter to the emitter list with the specified name
		QListWidgetItem* tempItem = new QListWidgetItem(QString::fromStdString(e->m_particleSystems[i]->m_name));
		ui.listWidget->addItem(tempItem);
		m_context->m_logger->LogText(LogTag::TOOLS, LogLevel::DEBUG_PRINT, "Emitter created: %s", ui.nameEmitterLineEdit->text().toStdString().c_str());

		//Enable Delete and rename buttons
		if(m_selectedEmitterIndex == -1)
		{
			ui.deleteEmitterButton->setEnabled(true);
			ui.renameEmitterButton->setEnabled(true);
			ui.tabWidget->setEnabled(true);
			ui.tabWidget->show();
			ui.colorDockWidget->show();
			ui.endcolorDockWidget->show();
			EmitterSelected(tempItem);
			ui.listWidget->setCurrentItem(tempItem);
		}
	}

	QFileInfo fileinfo(fullFileName);
	m_particleTab->setTabText(0, fileinfo.baseName());
	Saved();
}

void ParticleEditor::MenuSave()
{
	if(m_savePath.compare("") == 0)
		ManuSaveAs();
	else
		ExportParticle(QString::fromStdString(m_savePath));
}

void ParticleEditor::ManuSaveAs()
{
	ExportParticle(QFileDialog::getSaveFileName(this, tr("Save file"), "", tr("PARTICLE-file (*.particle)")));
}

void ParticleEditor::MenuViewStats()
{
	ui.dockWidget->show();
}

void ParticleEditor::MenuHelpAbout()
{
	ui.aboutWidget->show();
}
#pragma endregion

#pragma region Emitter management buttons
void ParticleEditor::NewEmitter()
{
	//If name entry is empty, don't rename
	if(0 == QString::compare(ui.nameEmitterLineEdit->text(), "") || 0 == QString::compare(ui.nameEmitterLineEdit->text(), "thomas"))
	{
		ShowMessageBox("Invalid name!");
		return;
	}

	//Get current particle entity and get its components
	ECS::Entity* entity = m_emitterEntities.at(0);
	RootForce::Transform* t			= m_world->GetEntityManager()->GetComponent<RootForce::Transform>(entity);
	RootForce::ParticleEmitter* e	= m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(entity);

	//Create a new particle emitter and push it to the component
	e->m_particleSystems.push_back(new RootEngine::ParticleSystemStruct());
	e->m_systems.push_back(m_context->m_renderer->CreateParticleSystem());

	//Set default data 
	e->m_particleSystems[e->m_particleSystems.size()-1]->m_material = m_context->m_renderer->CreateMaterial("particle" + std::to_string(m_materialIndex++));
	e->m_particleSystems[e->m_particleSystems.size()-1]->m_material->m_textures[Render::TextureSemantic::DIFFUSE] = m_context->m_resourceManager->LoadTexture("smoke", Render::TextureType::TEXTURE_2D);
	e->m_particleSystems[e->m_particleSystems.size()-1]->m_material->m_effect = m_context->m_resourceManager->LoadEffect("Particle/ParticleAdditive");
	e->m_particleSystems[e->m_particleSystems.size()-1]->m_position		= glm::vec3(0.0f);
	e->m_particleSystems[e->m_particleSystems.size()-1]->m_lifeTimeMin	= 2.0f;
	e->m_particleSystems[e->m_particleSystems.size()-1]->m_lifeTimeMax	= 2.0f;
	e->m_particleSystems[e->m_particleSystems.size()-1]->m_speedMin		= 0.1f;
	e->m_particleSystems[e->m_particleSystems.size()-1]->m_speedMax		= 0.1f;
	e->m_particleSystems[e->m_particleSystems.size()-1]->m_sizeMin		= glm::vec2(0.05f);
	e->m_particleSystems[e->m_particleSystems.size()-1]->m_sizeMax		= glm::vec2(0.05f);
	e->m_particleSystems[e->m_particleSystems.size()-1]->m_sizeEnd		= glm::vec2(0.0f);
	e->m_particleSystems[e->m_particleSystems.size()-1]->m_color		= glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	e->m_particleSystems[e->m_particleSystems.size()-1]->m_colorEnd		= glm::vec4(0.0f);
	e->m_particleSystems[e->m_particleSystems.size()-1]->m_gravity		= glm::vec3(0.0f, 0.0f, 0.0f);
	e->m_particleSystems[e->m_particleSystems.size()-1]->m_direction	= glm::vec3(0.0f);
	e->m_particleSystems[e->m_particleSystems.size()-1]->m_spread		= 0.0f;
	e->m_particleSystems[e->m_particleSystems.size()-1]->m_orbitSpeed	= 1.0f;
	e->m_particleSystems[e->m_particleSystems.size()-1]->m_orbitRadius	= 0.5f;
	e->m_particleSystems[e->m_particleSystems.size()-1]->m_spawnTime	= 0.05f;
	e->m_particleSystems[e->m_particleSystems.size()-1]->m_name			= ui.nameEmitterLineEdit->text().toStdString();
	e->m_particleSystems[e->m_particleSystems.size()-1]->m_template		= 0;

	//Map data to params list for buffering
	e->m_particleSystems[e->m_particleSystems.size()-1]->m_params[Render::Semantic::POSITION]		= &e->m_particleSystems[e->m_particleSystems.size()-1]->m_position;
	e->m_particleSystems[e->m_particleSystems.size()-1]->m_params[Render::Semantic::LIFETIMEMIN]	= &e->m_particleSystems[e->m_particleSystems.size()-1]->m_lifeTimeMin;
	e->m_particleSystems[e->m_particleSystems.size()-1]->m_params[Render::Semantic::LIFETIMEMAX]	= &e->m_particleSystems[e->m_particleSystems.size()-1]->m_lifeTimeMax;
	e->m_particleSystems[e->m_particleSystems.size()-1]->m_params[Render::Semantic::SPEEDMIN]		= &e->m_particleSystems[e->m_particleSystems.size()-1]->m_speedMin;
	e->m_particleSystems[e->m_particleSystems.size()-1]->m_params[Render::Semantic::SPEEDMAX]		= &e->m_particleSystems[e->m_particleSystems.size()-1]->m_speedMax;
	e->m_particleSystems[e->m_particleSystems.size()-1]->m_params[Render::Semantic::SIZEMIN]		= &e->m_particleSystems[e->m_particleSystems.size()-1]->m_sizeMin;
	e->m_particleSystems[e->m_particleSystems.size()-1]->m_params[Render::Semantic::SIZEMAX]		= &e->m_particleSystems[e->m_particleSystems.size()-1]->m_sizeMax;
	e->m_particleSystems[e->m_particleSystems.size()-1]->m_params[Render::Semantic::SIZEEND]		= &e->m_particleSystems[e->m_particleSystems.size()-1]->m_sizeEnd;
	e->m_particleSystems[e->m_particleSystems.size()-1]->m_params[Render::Semantic::COLOR]			= &e->m_particleSystems[e->m_particleSystems.size()-1]->m_color;
	e->m_particleSystems[e->m_particleSystems.size()-1]->m_params[Render::Semantic::COLOREND]		= &e->m_particleSystems[e->m_particleSystems.size()-1]->m_colorEnd;
	e->m_particleSystems[e->m_particleSystems.size()-1]->m_params[Render::Semantic::GRAVITY]		= &e->m_particleSystems[e->m_particleSystems.size()-1]->m_gravity;
	e->m_particleSystems[e->m_particleSystems.size()-1]->m_params[Render::Semantic::DIRECTION]		= &e->m_particleSystems[e->m_particleSystems.size()-1]->m_direction;
	e->m_particleSystems[e->m_particleSystems.size()-1]->m_params[Render::Semantic::SPREAD]			= &e->m_particleSystems[e->m_particleSystems.size()-1]->m_spread;
	e->m_particleSystems[e->m_particleSystems.size()-1]->m_params[Render::Semantic::SPAWNTIME]		= &e->m_particleSystems[e->m_particleSystems.size()-1]->m_spawnTime;
	e->m_particleSystems[e->m_particleSystems.size()-1]->m_params[Render::Semantic::TRANSPOSITION]	= &t->m_position;

	//Add an item representing the emitter to the emitter list with the specified name
	QListWidgetItem* tempItem = new QListWidgetItem(ui.nameEmitterLineEdit->text());
	ui.listWidget->addItem(tempItem);
	m_context->m_logger->LogText(LogTag::TOOLS, LogLevel::DEBUG_PRINT, "Emitter created: %s", ui.nameEmitterLineEdit->text().toStdString().c_str());

	//Enable Delete and rename buttons
	if(m_selectedEmitterIndex == -1)
	{
		ui.deleteEmitterButton->setEnabled(true);
		ui.renameEmitterButton->setEnabled(true);
		ui.tabWidget->setEnabled(true);
		ui.tabWidget->show();
		ui.colorDockWidget->show();
		ui.endcolorDockWidget->show();
		EmitterSelected(tempItem);
		ui.listWidget->setCurrentItem(tempItem);
	}

	//Empty the name field
	ui.nameEmitterLineEdit->setText("");
	Changed();
}

void ParticleEditor::DeleteEmitter()
{
	QListWidgetItem* tempItem = ui.listWidget->currentItem();
	if(!tempItem)
		return;

	int index = ui.listWidget->row(tempItem);

	ECS::Entity* entity = m_emitterEntities.at(m_selectedEntityIndex);
	RootForce::ParticleEmitter* e = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(entity);

	//delete e->m_particleSystems.at(index);
	e->m_particleSystems.erase(e->m_particleSystems.begin() + index);

	//Deletes the item
	ui.listWidget->takeItem(index);
	
	//Print info
	m_context->m_logger->LogText(LogTag::TOOLS, LogLevel::DEBUG_PRINT, "Emitter deleted: %s", tempItem->text().toStdString().c_str());

	//Must manually delete items from memory as Qt does not
	delete tempItem;

	if (ui.listWidget->count() == 0)
	{
		ui.deleteEmitterButton->setEnabled(false);
		ui.renameEmitterButton->setEnabled(false);
		ui.tabWidget->setEnabled(false);
		ui.tabWidget->hide();
		ui.colorDockWidget->hide();
		ui.endcolorDockWidget->hide();
		m_selectedEmitterIndex = -1;
	}
	else
	{
		//Select first emitter
		EmitterSelected(ui.listWidget->item(0));
	}
	Changed();
}

void ParticleEditor::RenameEmitter()
{
	if(0 == QString::compare(ui.nameEmitterLineEdit->text(), ""))
	{
		ShowMessageBox("Invalid name!");
		return;
	}

	QListWidgetItem* temp = ui.listWidget->currentItem();

	if(temp)
	{
		m_context->m_logger->LogText(LogTag::TOOLS, LogLevel::DEBUG_PRINT, "Emitter renamed from '%s' to '%s'", temp->text().toStdString().c_str(), ui.nameEmitterLineEdit->text().toStdString().c_str() );
		temp->setText(ui.nameEmitterLineEdit->text());
		ui.nameEmitterLineEdit->setText("");
	}
	Changed();
}

void ParticleEditor::EmitterSelected( QListWidgetItem* p_item)
{
	m_selectedEmitterIndex = ui.listWidget->row(p_item);
	m_context->m_logger->LogText(LogTag::TOOLS, LogLevel::DEBUG_PRINT, "Emitter selected: %d", m_selectedEmitterIndex);

	ECS::Entity* entity = m_emitterEntities.at(m_selectedEntityIndex);
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(entity);
	
	//Position
	ui.posSpinBoxX->setValue(pe->m_particleSystems[m_selectedEmitterIndex]->m_position.x);
	ui.posSpinBoxY->setValue(pe->m_particleSystems[m_selectedEmitterIndex]->m_position.y);
	ui.posSpinBoxZ->setValue(pe->m_particleSystems[m_selectedEmitterIndex]->m_position.z);
	//Lifetime
	ui.lifeTimeMinSpinBox->setValue(pe->m_particleSystems[m_selectedEmitterIndex]->m_lifeTimeMin);
	ui.lifeTimeMaxSpinBox->setValue(pe->m_particleSystems[m_selectedEmitterIndex]->m_lifeTimeMax);
	//Speed
	ui.speedMinSpinBox->setValue(pe->m_particleSystems[m_selectedEmitterIndex]->m_speedMin);
	ui.speedMaxSpinBox->setValue(pe->m_particleSystems[m_selectedEmitterIndex]->m_speedMax);
	//Size
	ui.sizeMinSpinBoxX->setValue(pe->m_particleSystems[m_selectedEmitterIndex]->m_sizeMin.x);
	ui.sizeMaxSpinBoxX->setValue(pe->m_particleSystems[m_selectedEmitterIndex]->m_sizeMax.x);
	ui.sizeEndSpinBoxX->setValue(pe->m_particleSystems[m_selectedEmitterIndex]->m_sizeEnd.x);
	
	//Start color
	float red	= pe->m_particleSystems[m_selectedEmitterIndex]->m_color.r*255.0f;
	float green = pe->m_particleSystems[m_selectedEmitterIndex]->m_color.g*255.0f;
	float blue	= pe->m_particleSystems[m_selectedEmitterIndex]->m_color.b*255.0f;
	float alpha	= pe->m_particleSystems[m_selectedEmitterIndex]->m_color.a*255.0f;

	QColor col(red, green, blue, alpha);
	m_colorTriangle->setColor(col);
	ui.colorAlphaSlider->setValue(alpha);
	ui.colorSpinBoxA->setValue(alpha);
	//End color
	red		= pe->m_particleSystems[m_selectedEmitterIndex]->m_colorEnd.r*255.0f;
	green	= pe->m_particleSystems[m_selectedEmitterIndex]->m_colorEnd.g*255.0f;
	blue	= pe->m_particleSystems[m_selectedEmitterIndex]->m_colorEnd.b*255.0f;
	alpha	= pe->m_particleSystems[m_selectedEmitterIndex]->m_colorEnd.a*255.0f;

	QColor colEnd(red, green, blue, alpha);
	m_colorEndTriangle->setColor(colEnd);
	ui.endcolorAlphaSlider->setValue(alpha);
	ui.endcolorSpinBoxA->setValue(alpha);
	//Gravity
	ui.gravitySpinBoxX->setValue(pe->m_particleSystems[m_selectedEmitterIndex]->m_gravity.x);
	ui.gravitySpinBoxY->setValue(pe->m_particleSystems[m_selectedEmitterIndex]->m_gravity.y);
	ui.gravitySpinBoxZ->setValue(pe->m_particleSystems[m_selectedEmitterIndex]->m_gravity.z);
	//Direction
	ui.directionSpinBoxX->setValue(pe->m_particleSystems[m_selectedEmitterIndex]->m_direction.x);
	ui.directionSpinBoxY->setValue(pe->m_particleSystems[m_selectedEmitterIndex]->m_direction.y);
	ui.directionSpinBoxZ->setValue(pe->m_particleSystems[m_selectedEmitterIndex]->m_direction.z);
	//Spread
	ui.spreadSpinBox->setValue(pe->m_particleSystems[m_selectedEmitterIndex]->m_spread*(180.0f/glm::pi<double>()));
	//Spawntime
	ui.spawnTimeSpinBox->setValue(pe->m_particleSystems[m_selectedEmitterIndex]->m_spawnTime);
	//Orbit
	ui.orbitRadiusSpinBox->setValue(pe->m_particleSystems[m_selectedEmitterIndex]->m_orbitRadius);
	ui.orbitSpeedSpinBox->setValue(pe->m_particleSystems[m_selectedEmitterIndex]->m_orbitSpeed);
	TemplateChanged(pe->m_particleSystems[m_selectedEmitterIndex]->m_template);
	ui.templateComboBox->setCurrentIndex(pe->m_particleSystems[m_selectedEmitterIndex]->m_template);
}
#pragma endregion

#pragma region Attributes changed
void ParticleEditor::PositionXChanged( double p_x )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));
	pe->m_particleSystems[m_selectedEmitterIndex]->m_position.x = (float)p_x;
	Changed();
}

void ParticleEditor::PositionYChanged( double p_y )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));
	pe->m_particleSystems[m_selectedEmitterIndex]->m_position.y = (float)p_y;
	Changed();
}

void ParticleEditor::PositionZChanged( double p_z )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));
	pe->m_particleSystems[m_selectedEmitterIndex]->m_position.z = (float)p_z;
	Changed();
}

void ParticleEditor::SizeMinXChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));
	pe->m_particleSystems[m_selectedEmitterIndex]->m_sizeMin.x = (float)p_val;
	pe->m_particleSystems[m_selectedEmitterIndex]->m_sizeMin.y = (float)p_val;
	Changed();
}

void ParticleEditor::SizeMaxXChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));
	pe->m_particleSystems[m_selectedEmitterIndex]->m_sizeMax.x = (float)p_val;
	pe->m_particleSystems[m_selectedEmitterIndex]->m_sizeMax.y = (float)p_val;
	Changed();
}


void ParticleEditor::SizeEndXChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));
	pe->m_particleSystems[m_selectedEmitterIndex]->m_sizeEnd.x = (float)p_val;
	pe->m_particleSystems[m_selectedEmitterIndex]->m_sizeEnd.y = (float)p_val;
	Changed();
}

void ParticleEditor::LifeTimeMinChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));
	pe->m_particleSystems[m_selectedEmitterIndex]->m_lifeTimeMin = (float)p_val;
	Changed();
}

void ParticleEditor::LifeTimeMaxChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));
	pe->m_particleSystems[m_selectedEmitterIndex]->m_lifeTimeMax = (float)p_val;
	Changed();
}

void ParticleEditor::GravityXChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));
	pe->m_particleSystems[m_selectedEmitterIndex]->m_gravity.x = (float)p_val;
	Changed();
}

void ParticleEditor::GravityYChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));
	pe->m_particleSystems[m_selectedEmitterIndex]->m_gravity.y = (float)p_val;
	Changed();
}

void ParticleEditor::GravityZChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));
	pe->m_particleSystems[m_selectedEmitterIndex]->m_gravity.z = (float)p_val;
	Changed();
}

void ParticleEditor::DirectionXChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));
	pe->m_particleSystems[m_selectedEmitterIndex]->m_direction.x = (float)p_val;
	Changed();
}

void ParticleEditor::DirectionYChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));
	pe->m_particleSystems[m_selectedEmitterIndex]->m_direction.y = (float)p_val;
	Changed();
}

void ParticleEditor::DirectionZChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));
	pe->m_particleSystems[m_selectedEmitterIndex]->m_direction.z = (float)p_val;
	Changed();
}

void ParticleEditor::SpeedMinChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));
	pe->m_particleSystems[m_selectedEmitterIndex]->m_speedMin = (float)p_val;
	Changed();
}

void ParticleEditor::SpeedMaxChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));
	pe->m_particleSystems[m_selectedEmitterIndex]->m_speedMax = (float)p_val;
	Changed();
}

void ParticleEditor::SpreadChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));
	pe->m_particleSystems[m_selectedEmitterIndex]->m_spread = (float)p_val*(glm::pi<float>()/180.0f);
	ui.spreadSlider->setValue((int)p_val);
	Changed();
}

void ParticleEditor::SpawnTimeChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));
	pe->m_particleSystems[m_selectedEmitterIndex]->m_spawnTime = (float)p_val;
	Changed();
}

void ParticleEditor::ColorChanged( const QColor& p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));

	float red	= (float)p_val.red()/255.0f;
	float green = (float)p_val.green()/255.0f;
	float blue	= (float)p_val.blue()/255.0f;
	//Set particle data
	pe->m_particleSystems[m_selectedEmitterIndex]->m_color.r = red;
	pe->m_particleSystems[m_selectedEmitterIndex]->m_color.g = green;
	pe->m_particleSystems[m_selectedEmitterIndex]->m_color.b = blue;
	//Set UI
	ui.colorSpinBoxR->setValue(red);
	ui.colorSpinBoxG->setValue(green);
	ui.colorSpinBoxB->setValue(blue);
	Changed();
}

void ParticleEditor::ColorEndChanged( const QColor& p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));

	float red	= (float)p_val.red()/255.0f;
	float green = (float)p_val.green()/255.0f;
	float blue	= (float)p_val.blue()/255.0f;
	//Set particle data
	pe->m_particleSystems[m_selectedEmitterIndex]->m_colorEnd.r = red;
	pe->m_particleSystems[m_selectedEmitterIndex]->m_colorEnd.g = green;
	pe->m_particleSystems[m_selectedEmitterIndex]->m_colorEnd.b = blue;
	//Set UI
	ui.endcolorSpinBoxR->setValue(red);
	ui.endcolorSpinBoxG->setValue(green);
	ui.endcolorSpinBoxB->setValue(blue);
	Changed();
}

void ParticleEditor::colorAlphaSliderChanged( int p_val )
{
	float alpha = (float)p_val/255.0f;
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));
	ui.colorSpinBoxA->setValue(alpha);
	pe->m_particleSystems[m_selectedEmitterIndex]->m_color.a = alpha;
	Changed();
}

void ParticleEditor::endColorAlphaSliderChanged( int p_val )
{
	float alpha = (float)p_val/255.0f;
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));
	ui.endcolorSpinBoxA->setValue(alpha);
	pe->m_particleSystems[m_selectedEmitterIndex]->m_colorEnd.a = alpha;
	Changed();
}

void ParticleEditor::OrbitRadiusChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));
	pe->m_particleSystems[m_selectedEmitterIndex]->m_orbitRadius = (float)p_val;
	Changed();
}

void ParticleEditor::OrbitSpeedChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));
	pe->m_particleSystems[m_selectedEmitterIndex]->m_orbitSpeed = (float)p_val;
	Changed();
}


void ParticleEditor::SpreadSliderChanged( int p_val )
{
	ui.spreadSpinBox->setValue((double)p_val);
	Changed();
}

void ParticleEditor::BackgroundColorChanged( int p_value )
{
	switch (p_value)
	{
	case 0:
		glClearColor(0,0,0,1);
		break;
	case 1:
		glClearColor(0.5,0.5,0.5,1);
		break;
	case 2:
		glClearColor(1,1,1,1);
		break;
	case 3:
		glClearColor(1,0,0,1);
		break;
	case 4:
		glClearColor(0,1,0,1);
		break;
	case 5:
		glClearColor(0,0,1,1);
		break;
	default:
		break;
	}
}

#pragma endregion

void ParticleEditor::MenuViewColorTriangle()
{
	if(m_selectedEmitterIndex != -1)
		ui.colorDockWidget->show();
}

void ParticleEditor::MenuViewColorEndTriangle()
{
	if(m_selectedEmitterIndex != -1)
		ui.endcolorDockWidget->show();
}

#pragma region Grid
void ParticleEditor::DrawGridX(float p_spacing, glm::vec4 p_color)
{
	for (int i = -5; i < 6; i++)
	{
		if(i == 0)
		{
			m_context->m_renderer->AddLine(glm::vec3((float)(-5*p_spacing), 0, 0), glm::vec3((float)(5*p_spacing), 0, 0), glm::vec4(1.0f));
			m_context->m_renderer->AddLine(glm::vec3(0, 0, (float)(-5*p_spacing)), glm::vec3(0, 0, (float)(5*p_spacing)), glm::vec4(1.0f));
		}
		else
		{
			m_context->m_renderer->AddLine(glm::vec3((float)(-5*p_spacing), 0, (float)(i*p_spacing)), glm::vec3((float)(5*p_spacing), 0, (float)(i*p_spacing)), p_color);
			m_context->m_renderer->AddLine(glm::vec3((float)(i*p_spacing), 0, (float)(-5*p_spacing)), glm::vec3((float)(i*p_spacing), 0, (float)(5*p_spacing)), p_color);
		}
	}
}

void ParticleEditor::GridToggled()
{
	m_showGrid = true;
	ui.gridDockWidget->show();
	m_context->m_logger->LogText(LogTag::TOOLS, LogLevel::DEBUG_PRINT, "Grid showing");
}

void ParticleEditor::GridSizeChanged( double p_val )
{
	//Change grid spacing
	m_gridSpace = (float)p_val;
}
#pragma endregion

void ParticleEditor::TextureDoubleClicked( const QModelIndex& p_index )
{
	//If no emitter selected, abort
	if(m_selectedEmitterIndex == -1)
		return;

	//Read file info of double clicked file and determine if valid
	QFileInfo fileInfo = m_fileSystemModel->fileInfo(p_index);
	if(fileInfo.suffix().compare("dds") != 0)
	{
		m_context->m_logger->LogText(LogTag::TOOLS, LogLevel::WARNING, "Texture must be .dds! Tried to select %s", fileInfo.fileName().toStdString().c_str() );
		//ShowMessageBox("Texture must be .dds!");
		return;
	}
	m_context->m_logger->LogText(LogTag::TOOLS, LogLevel::DEBUG_PRINT, "Selected new texture: %s", fileInfo.fileName().toStdString().c_str());
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));
	pe->m_particleSystems[m_selectedEmitterIndex]->m_material->m_textures[Render::TextureSemantic::DIFFUSE] = m_context->m_resourceManager->LoadTexture(fileInfo.baseName().toStdString().c_str(), Render::TextureType::TEXTURE_2D);
	Changed();
}

void ParticleEditor::ModelDoubleClicked( const QModelIndex& p_index )
{
	//If no emitter selected, abort
	if(m_selectedEmitterIndex == -1)
		return;

	//Read file info of double clicked file and determine if valid
	QFileInfo fileInfo = m_fileSystemModelModel->fileInfo(p_index);
	if(fileInfo.suffix().compare("dae") != 0)
	{
		m_context->m_logger->LogText(LogTag::TOOLS, LogLevel::WARNING, "Model must be .dae! Tried to select %s", fileInfo.fileName().toStdString().c_str() );
		//ShowMessageBox("Texture must be .dds!");
		return;
	}
	m_context->m_logger->LogText(LogTag::TOOLS, LogLevel::DEBUG_PRINT, "Selected new model: %s", fileInfo.fileName().toStdString().c_str());
	m_model->m_model = g_engineContext.m_resourceManager->LoadCollada(fileInfo.baseName().toStdString().c_str());
	m_modelTrans->m_scale = glm::vec3(1.0f);
}

void ParticleEditor::ModelTexDoubleClicked( const QModelIndex& p_index )
{
	//If no emitter selected, abort
	if(m_selectedEmitterIndex == -1)
		return;

	//Read file info of double clicked file and determine if valid
	QFileInfo fileInfo = m_fileSystemModelModelTex->fileInfo(p_index);
	if(fileInfo.suffix().compare("dds") != 0)
	{
		m_context->m_logger->LogText(LogTag::TOOLS, LogLevel::WARNING, "Texture must be .dds! Tried to select %s", fileInfo.fileName().toStdString().c_str() );
		//ShowMessageBox("Texture must be .dds!");
		return;
	}
	m_context->m_logger->LogText(LogTag::TOOLS, LogLevel::DEBUG_PRINT, "Selected new texture: %s", fileInfo.fileName().toStdString().c_str());
	m_model->m_material->m_textures[Render::TextureSemantic::DIFFUSE] = m_context->m_resourceManager->LoadTexture(fileInfo.baseName().toStdString().c_str(), Render::TextureType::TEXTURE_2D);
}

glm::vec3 ParticleEditor::GetSelectedPosition()
{
	return glm::vec3((float)ui.posSpinBoxX->value(), (float)ui.posSpinBoxY->value(), (float)ui.posSpinBoxZ->value());
}

void ParticleEditor::ShowMessageBox( QString p_msg )
{
	QMessageBox msg;
	msg.setText(p_msg);
	msg.setIcon(QMessageBox::Information);
	msg.setStandardButtons(QMessageBox::Ok);
	msg.setDefaultButton(QMessageBox::Ok);
	QSpacerItem* horizontalSpacer = new QSpacerItem(200, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
	QGridLayout* layout = (QGridLayout*)msg.layout();
	layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
	msg.show();
	msg.exec();
}


void ParticleEditor::SetWorld( ECS::World* p_world )
{
	m_world = p_world;
}

void ParticleEditor::SetContext( RootEngine::GameSharedContext* p_context )
{
	m_context = p_context;
}

void ParticleEditor::SetWorkingDirectory( std::string p_dir )
{
	m_workingDirectory = p_dir;
}

void ParticleEditor::SetAimingDevice( ECS::Entity* p_aimingDevice )
{
	m_aimingDeviceTransform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(p_aimingDevice);
}

void ParticleEditor::SetLookAtSpinBox( glm::vec3 p_pos )
{
	//update lookat info
	ui.lookAtSpinBoxX->setValue(p_pos.x);
	ui.lookAtSpinBoxY->setValue(p_pos.y);
	ui.lookAtSpinBoxZ->setValue(p_pos.z);
}

void ParticleEditor::ClearScene()
{
	for(auto itr = m_emitterEntities.begin(); itr != m_emitterEntities.end(); ++itr)
	{
		m_world->GetEntityManager()->RemoveAllComponents((*itr));
		m_world->GetEntityManager()->RemoveEntity((*itr));
	}
	m_emitterEntities.clear();

	m_selectedEmitterIndex = -1;
	m_selectedEntityIndex = -1;

	ui.endcolorDockWidget->hide();
	ui.colorDockWidget->hide();
	ui.tabWidget->hide();

	while(ui.listWidget->count() > 0)
	{
		delete ui.listWidget->takeItem(0);
	}
}

void ParticleEditor::ExportParticle( QString p_fullFilePath )
{
	if(p_fullFilePath.compare("") == 0)
		return;

	ECS::Entity* entity = m_emitterEntities.at(0);
	RootForce::ParticleEmitter* e = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(entity);
	QFile file(p_fullFilePath);
	YAML::Emitter emitter;

	emitter << YAML::BeginSeq;

	for(auto itr = e->m_particleSystems.begin(); itr != e->m_particleSystems.end(); ++itr)
	{
		// Export Particle systems
		emitter << YAML::BeginMap;
		emitter << YAML::Key << "POSITION"		<< YAML::Value << YAML::Flow << YAML::BeginSeq << (*itr)->m_position.x << (*itr)->m_position.y << (*itr)->m_position.z << YAML::EndSeq; 
		emitter << YAML::Key << "LIFETIMEMIN"	<< YAML::Value << (*itr)->m_lifeTimeMin;
		emitter << YAML::Key << "LIFETIMEMAX"	<< YAML::Value << (*itr)->m_lifeTimeMax;
		emitter << YAML::Key << "SPEEDMIN"		<< YAML::Value << (*itr)->m_speedMin;
		emitter << YAML::Key << "SPEEDMAX"		<< YAML::Value << (*itr)->m_speedMax;
		emitter << YAML::Key << "SIZEMIN"		<< YAML::Value << YAML::Flow << YAML::BeginSeq << (*itr)->m_sizeMin.x	<< (*itr)->m_sizeMin.y << YAML::EndSeq; 
		emitter << YAML::Key << "SIZEMAX"		<< YAML::Value << YAML::Flow << YAML::BeginSeq << (*itr)->m_sizeMax.x	<< (*itr)->m_sizeMax.y << YAML::EndSeq; 
		emitter << YAML::Key << "SIZEEND"		<< YAML::Value << YAML::Flow << YAML::BeginSeq << (*itr)->m_sizeEnd.x	<< (*itr)->m_sizeEnd.y << YAML::EndSeq; 
		emitter << YAML::Key << "COLOR"			<< YAML::Value << YAML::Flow << YAML::BeginSeq << (*itr)->m_color.r		<< (*itr)->m_color.g		<< (*itr)->m_color.b		<< (*itr)->m_color.a << YAML::EndSeq; 
		emitter << YAML::Key << "COLOREND"		<< YAML::Value << YAML::Flow << YAML::BeginSeq << (*itr)->m_colorEnd.r	<< (*itr)->m_colorEnd.g		<< (*itr)->m_colorEnd.b		<< (*itr)->m_colorEnd.a << YAML::EndSeq; 
		emitter << YAML::Key << "GRAVITY"		<< YAML::Value << YAML::Flow << YAML::BeginSeq << (*itr)->m_gravity.x	<< (*itr)->m_gravity.y		<< (*itr)->m_gravity.z		<< YAML::EndSeq; 
		emitter << YAML::Key << "DIRECTION"		<< YAML::Value << YAML::Flow << YAML::BeginSeq << (*itr)->m_direction.x << (*itr)->m_direction.y	<< (*itr)->m_direction.z	<< YAML::EndSeq; 
		emitter << YAML::Key << "SPREAD"		<< YAML::Value << (*itr)->m_spread;
		emitter << YAML::Key << "SPAWNTIME"		<< YAML::Value << (*itr)->m_spawnTime;
		emitter << YAML::Key << "ORBITSPEED"	<< YAML::Value << (*itr)->m_orbitSpeed;
		emitter << YAML::Key << "ORBITRADIUS"	<< YAML::Value << (*itr)->m_orbitRadius;
		emitter << YAML::Key << "TEMPLATE"		<< YAML::Value << (*itr)->m_template;
		emitter << YAML::Key << "TEXTURE"		<< YAML::Value << m_context->m_resourceManager->ResolveStringFromTexture((*itr)->m_material->m_textures[Render::TextureSemantic::DIFFUSE]);
		emitter << YAML::Key << "EFFECT"		<< YAML::Value << m_context->m_resourceManager->ResolveStringFromEffect((*itr)->m_material->m_effect);
		emitter << YAML::Key << "NAME"			<< YAML::Value << (*itr)->m_name;
		emitter << YAML::EndMap;
	}

	emitter << YAML::EndSeq;

	//Write exported yaml to file
	if(file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream out(&file);
		out << emitter.c_str();
		file.close();
	}
	m_savePath = p_fullFilePath.toStdString();
	QFileInfo fileinfo(p_fullFilePath);
	m_particleTab->setTabText(0, fileinfo.baseName());
	Saved();
}

bool ParticleEditor::IsSaved()
{
	if(m_savePath.compare("") == 0)
		return false;
	else
		return true;
}

QMessageBox::StandardButton ParticleEditor::SaveWarningDialog()
{
	if(m_changed)
	{
		if(m_selectedEmitterIndex != -1)
		{
			QMessageBox msgBox;
			msgBox.setText("The document has been modified.");
			msgBox.setInformativeText("Do you want to save your changes?");
			msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
			msgBox.setDefaultButton(QMessageBox::Save);
			int ret = msgBox.exec();

			switch (ret) {
			case QMessageBox::Save:
				{
					MenuSave();
					return QMessageBox::Save;
				}
				break;
			case QMessageBox::Discard:
				{
					return QMessageBox::Discard;
				}
				break;
			case QMessageBox::Cancel:
				{
					return QMessageBox::Cancel;
				}
				break;
			default:
				// should never be reached
				break;
			}
		}
	}
	else
	{
		return QMessageBox::Save;
	}
}

void ParticleEditor::Changed()
{
	m_changed = true;
}

void ParticleEditor::MenuHelpHelp()
{
	ui.helpWidget->show();
}

void ParticleEditor::Saved()
{
	m_changed = false;
}

void ParticleEditor::TemplateChanged( int p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));

	if(p_val == 0)
	{
		ResetTemplates();
		pe->m_particleSystems[m_selectedEmitterIndex]->m_material->m_effect = m_context->m_resourceManager->LoadEffect("Particle/ParticleAdditive");
		pe->m_particleSystems[m_selectedEmitterIndex]->m_template = p_val;
	}
	else if(p_val == 1)
	{
		ResetTemplates();
		pe->m_particleSystems[m_selectedEmitterIndex]->m_params[Render::Semantic::ORBITSPEED]  = &pe->m_particleSystems[m_selectedEmitterIndex]->m_orbitSpeed;
		pe->m_particleSystems[m_selectedEmitterIndex]->m_params[Render::Semantic::ORBITRADIUS] = &pe->m_particleSystems[m_selectedEmitterIndex]->m_orbitRadius;
		pe->m_particleSystems[m_selectedEmitterIndex]->m_material->m_effect = m_context->m_resourceManager->LoadEffect("Particle/ParticleOrbit");
		pe->m_particleSystems[m_selectedEmitterIndex]->m_template = p_val;
	}
	Changed();
}

void ParticleEditor::ResetTemplates()
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));
	if(pe->m_particleSystems[m_selectedEmitterIndex]->m_params.find(Render::Semantic::ORBITSPEED) != pe->m_particleSystems[m_selectedEmitterIndex]->m_params.end())
		pe->m_particleSystems[m_selectedEmitterIndex]->m_params.erase(Render::Semantic::ORBITSPEED);
	if(pe->m_particleSystems[m_selectedEmitterIndex]->m_params.find(Render::Semantic::ORBITRADIUS) != pe->m_particleSystems[m_selectedEmitterIndex]->m_params.end())
		pe->m_particleSystems[m_selectedEmitterIndex]->m_params.erase(Render::Semantic::ORBITRADIUS);
}

void ParticleEditor::RemoveObjectButton()
{
	m_modelTrans->m_scale = glm::vec3(0.0f);
}

int ParticleEditor::CheckRayVsObject( glm::ivec2 p_mousePos, glm::vec3 p_camPos, glm::mat4 p_viewMatrix)
{
	if(m_selectedEntityIndex == -1)
		return 0;

	ECS::Entity* entity = m_emitterEntities.at(0);
	RootForce::ParticleEmitter* e = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(entity);

	float radius = 0.2f;
	float radiusSphere2 = radius*radius;

	float closestDist = 999999.0f;
	int closestEmitter = -1;

	//Calculate NDC coords
	float x = (2.0f * -p_mousePos.x) / (float)ui.frame->width() - 1.0f;
	float y = (2.0f * p_mousePos.y) / (float)ui.frame->height() + 1.0f;
	//View space coords
	glm::vec4 rayView = m_inverseProjection * glm::vec4(x, y, -1.0f, 1.0f);
	rayView = glm::vec4(rayView.x, rayView.y, -1.0f, 0.0f);
	//World space coords
	glm::vec4 rW = (glm::inverse(p_viewMatrix) * rayView);
	glm::vec3 rayWorld = glm::normalize(glm::vec3(rW.x, rW.y, rW.z));

	//Test Axis selection
	for(int i = 1; i <= 3; ++i)
	{
		float collisionDistance = CheckRayVsAABB(rayWorld, p_camPos, GetSelectedPosition() + m_axisAABB[i-1].m_lower, GetSelectedPosition() + m_axisAABB[i-1].m_upper);
		if(collisionDistance < closestDist && collisionDistance >= 0.0f)
		{
			closestEmitter = (int)i;
			closestDist = collisionDistance;
		}
	}
	if(closestEmitter > -1)
	{
		if(closestEmitter == 1)
			m_context->m_logger->LogText(LogTag::TOOLS, LogLevel::DEBUG_PRINT, "X axis selected");
		if(closestEmitter == 2)
			m_context->m_logger->LogText(LogTag::TOOLS, LogLevel::DEBUG_PRINT, "Y axis selected");
		if(closestEmitter == 3)
			m_context->m_logger->LogText(LogTag::TOOLS, LogLevel::DEBUG_PRINT, "Z axis selected");
		m_firstDrag = true;
		return closestEmitter;
	}
	
	//Test emitter selection
	closestDist = 999999.0f;
	closestEmitter = -1;
	for(unsigned i = 0; i < e->m_particleSystems.size(); ++i)
	{
		//Test ray vs shpere
		glm::vec3 L = e->m_particleSystems.at(i)->m_position - p_camPos;
		float tca = glm::dot(L, rayWorld);
		if(tca < 0)
		{
			continue;
		}
		float d2 = glm::dot(L, L) - tca * tca;
		if(d2 > radiusSphere2)
		{
			continue;
		}
		float thc = glm::sqrt(radiusSphere2 - d2);
		float t0 = tca - thc;

		if(t0 < closestDist)
		{
			closestEmitter = (int)i;
			closestDist = t0;
		}
	}

	if(closestEmitter > -1)
	{
		EmitterSelected(ui.listWidget->item(closestEmitter));
		ui.listWidget->setCurrentItem(ui.listWidget->item(closestEmitter));
	}

	return 0;
}

void ParticleEditor::DrawPositionAxis()
{
	glm::vec3 position = GetSelectedPosition();
	//X
	m_context->m_renderer->AddLine(position, position + glm::vec3(0.5f, 0.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	//Y
	m_context->m_renderer->AddLine(position, position + glm::vec3(0.0f, 0.5f, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	//Z
	m_context->m_renderer->AddLine(position, position + glm::vec3(0.0f, 0.0f, 0.5f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
}

void ParticleEditor::DragEmitter( int p_axis, glm::ivec2 p_mousePos, glm::vec3 p_camPos, glm::mat4 p_viewMatrix )
{
	//Calculate NDC coords
	float x = (2.0f * -p_mousePos.x) / (float)ui.frame->width() - 1.0f;
	float y = (2.0f * p_mousePos.y) / (float)ui.frame->height() + 1.0f;
	//View space coords
	glm::vec4 rayView = m_inverseProjection * glm::vec4(x, y, -1.0f, 1.0f);
	rayView = glm::vec4(rayView.x, rayView.y, -1.0f, 0.0f);
	//World space coords
	glm::vec4 rW = (glm::inverse(p_viewMatrix) * rayView);
	glm::vec3 rayWorld = glm::normalize(glm::vec3(rW.x, rW.y, rW.z));


	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));

	switch (p_axis)
	{
	case 1: //x
		{
			glm::vec3 camPos = glm::vec3(GetSelectedPosition().x, p_camPos.y, p_camPos.z);
			PointOnPlane pointOnPlane = GetPointOnPlane(camPos, p_camPos, rayWorld);
			if(pointOnPlane.Hit)
				ui.posSpinBoxX->setValue((double)pointOnPlane.Point.x - (double)GetDragOffset(pointOnPlane.Point.x, ui.posSpinBoxX->value()));
		}
		break;
	case 2: //Y
		{
			glm::vec3 camPos = glm::vec3(p_camPos.x, GetSelectedPosition().y, p_camPos.z);
			PointOnPlane pointOnPlane = GetPointOnPlane(camPos, p_camPos, rayWorld);
			if(pointOnPlane.Hit)
				ui.posSpinBoxY->setValue((double)pointOnPlane.Point.y - (double)GetDragOffset(pointOnPlane.Point.y, ui.posSpinBoxY->value()));
		}
		break;
	case 3: //Z
		{
			glm::vec3 camPos = glm::vec3(p_camPos.x, p_camPos.y, GetSelectedPosition().z);
			PointOnPlane pointOnPlane = GetPointOnPlane(camPos, p_camPos, rayWorld);
			if(pointOnPlane.Hit)
				ui.posSpinBoxZ->setValue((double)pointOnPlane.Point.z - (double)GetDragOffset(pointOnPlane.Point.z, ui.posSpinBoxZ->value()));
		}
		break;
	default:
		break;
	}
}

float ParticleEditor::CheckRayVsAABB( glm::vec3 p_rayDir, glm::vec3 p_rayOrigin, glm::vec3 p_bound1, glm::vec3 p_bound2 )
{
	glm::vec3 invdir = 1.0f / p_rayDir;

	float t1 = (p_bound1.x - p_rayOrigin.x)*invdir.x;
	float t2 = (p_bound2.x - p_rayOrigin.x)*invdir.x;
	float t3 = (p_bound1.y - p_rayOrigin.y)*invdir.y;
	float t4 = (p_bound2.y - p_rayOrigin.y)*invdir.y;
	float t5 = (p_bound1.z - p_rayOrigin.z)*invdir.z;
	float t6 = (p_bound2.z - p_rayOrigin.z)*invdir.z;

	float tmin = glm::max(glm::max(glm::min(t1, t2), glm::min(t3, t4)), glm::min(t5, t6));
	float tmax = glm::min(glm::min(glm::max(t1, t2), glm::max(t3, t4)), glm::max(t5, t6));

	if(tmax < 0)
		return -1.0f;
	if(tmin > tmax)
		return -1.0f;

	return tmin;
}

float ParticleEditor::GetDragOffset( float p_pointOnAxis, float p_pointOfEmitter )
{
	if(m_firstDrag)
	{
		m_dragOffset = p_pointOnAxis - p_pointOfEmitter;
		m_firstDrag = false;
	}
	return m_dragOffset;
	
}

ParticleEditor::PointOnPlane ParticleEditor::GetPointOnPlane( glm::vec3 p_camPos, glm::vec3 p_worldCamPos, glm::vec3 p_rayDir )
{
	float t;
	bool hit = true;

	glm::vec3 normalFlip = glm::normalize(GetSelectedPosition() - p_camPos);

	float denom = glm::dot(normalFlip, (GetSelectedPosition() - p_worldCamPos));

	if(denom > 1e-6)
	{
		t = denom / glm::dot(normalFlip, p_rayDir);
		if(t < 0.0f)
		{
			m_context->m_logger->LogText(LogTag::TOOLS, LogLevel::DEBUG_PRINT, "t < 0.0f");
			hit = false; 
		}
	}
	else
		hit = false; 

	return PointOnPlane(p_worldCamPos + (p_rayDir * t), hit);
}






