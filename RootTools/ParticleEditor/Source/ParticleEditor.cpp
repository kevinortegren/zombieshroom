#include "ParticleEditor.h"

ParticleEditor::ParticleEditor(QWidget *parent)
	: QMainWindow(parent), m_running(true), m_showGrid(true)
{
	ui.setupUi(this);
}

ParticleEditor::~ParticleEditor()
{
	delete m_statWidget;
	delete m_particleTab;
	delete m_colorTriangle;
	delete m_colorEndTriangle;
}

void ParticleEditor::ConnectSignalsAndSlots()
{
	connect(ui.actionExit,			SIGNAL(triggered()),						this, SLOT(MenuExit()));
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
	connect(m_colorTriangle,		SIGNAL(colorChanged(const QColor&)),		this, SLOT(ColorChanged(const QColor&)));
	connect(m_colorEndTriangle,		SIGNAL(colorChanged(const QColor&)),		this, SLOT(ColorEndChanged(const QColor&)));
	connect(ui.actionColor_Triangle,SIGNAL(triggered()),						this, SLOT(MenuViewColorTriangle()));
	connect(ui.actionEndColor_Triangle,SIGNAL(triggered()),						this, SLOT(MenuViewColorEndTriangle()));
}

void ParticleEditor::Init()
{
	//Create a status widget window for displaying FPS and other things
	m_statWidget = new StatWidget();
	m_statWidget->hide();

	//Create the tab bar on top of the 3D view, each tab is a Entity with a transform and a particle emitter component
	m_particleTab = new ParticleTab(this);
	m_particleTab->setGeometry(QRect(0, ui.menuBar->height(), 1209, 27));
	m_particleTab->show();

	ui.menuView->setEnabled(true);

	ui.tabWidget->setEnabled(false);
	ui.tabWidget->hide();

	m_selectedEmitterIndex	= -1;
	m_selectedEntityIndex	= -1;
	m_materialIndex			= 0;

	m_fileSystemModel = new QFileSystemModel;
	m_fileSystemModel->setRootPath(QString::fromStdString(m_workingDirectory + "Assets/Textures/"));

	QStringList filters;
	filters << "*.dds";
	m_fileSystemModel->setNameFilters(filters);
	m_fileSystemModel->setNameFilterDisables(false);
	ui.textureTreeView->setModel(m_fileSystemModel);
	ui.textureTreeView->setColumnWidth(0, 200);
	ui.textureTreeView->hideColumn(2);
	ui.textureTreeView->setRootIndex(m_fileSystemModel->index(QString::fromStdString(m_workingDirectory + "Assets/Textures/")));
	
	m_colorTriangle = new QtColorTriangle();
	m_colorTriangle->show();
	m_colorTriangle->setGeometry(100.0f, 100.0f, 200.0f, 200.0f);

	m_colorEndTriangle = new QtColorTriangle();
	m_colorEndTriangle->show();
	m_colorEndTriangle->setGeometry(400.0f, 400, 200.0f, 200.0f);
}

bool ParticleEditor::CheckExit()
{
	return m_running;
}

void ParticleEditor::closeEvent( QCloseEvent *event )
{
	//Give the user a choice to abort the close event with a message box
	QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Exit Particle Editor",
		tr("Sure you want to quit Particle Editor?\nYou won't be able to edit or view particle systems if you do."),
		QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
		QMessageBox::Yes);
	if (resBtn != QMessageBox::Yes)
	{
		event->ignore();
	} 
	else 
	{
		event->accept();
		m_running = false;
	}
	
}

void ParticleEditor::Update( float p_dt )
{
	m_statWidget->Update(p_dt);
	if(m_showGrid)
		DrawGridX(2);
}

void ParticleEditor::MenuExit()
{
	close();
}

void ParticleEditor::MenuNew()
{
	//Add new tab
	m_particleTab->AddTab();

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
		ui.tabWidget->setEnabled(true);
		ui.tabWidget->show();
		m_selectedEntityIndex = 0; 
	}
}

void ParticleEditor::MenuOpen()
{

}

void ParticleEditor::MenuSave()
{

}

void ParticleEditor::ManuSaveAs()
{

}

void ParticleEditor::MenuViewStats()
{
	m_statWidget->show();
}

void ParticleEditor::MenuHelpAbout()
{

}

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
	RootForce::Transform* t = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(entity);
	RootForce::ParticleEmitter* e = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(entity);

	//Create a new particle emitter and push it to the component
	RootForce::ParticleSystemStruct particleSystem;
	e->m_particleSystems.push_back(particleSystem);

	//Set default data 
	e->m_particleSystems[e->m_particleSystems.size()-1].m_system = m_context->m_renderer->CreateParticleSystem();	
	e->m_particleSystems[e->m_particleSystems.size()-1].m_material = m_context->m_resourceManager->GetMaterial("particle" + std::to_string(m_materialIndex++));
	e->m_particleSystems[e->m_particleSystems.size()-1].m_material->m_diffuseMap = m_context->m_resourceManager->LoadTexture("smoke", Render::TextureType::TEXTURE_2D);
	e->m_particleSystems[e->m_particleSystems.size()-1].m_material->m_effect = m_context->m_resourceManager->LoadEffect("Particle/Particle");
	e->m_particleSystems[e->m_particleSystems.size()-1].m_lifeTimeMin = 2.0f;
	e->m_particleSystems[e->m_particleSystems.size()-1].m_lifeTimeMax = 2.0f;
	e->m_particleSystems[e->m_particleSystems.size()-1].m_speedMin = 0.1f;
	e->m_particleSystems[e->m_particleSystems.size()-1].m_speedMax = 0.1f;
	e->m_particleSystems[e->m_particleSystems.size()-1].m_sizeMin	= glm::vec2(0.05f);
	e->m_particleSystems[e->m_particleSystems.size()-1].m_sizeMax	= glm::vec2(0.05f);
	e->m_particleSystems[e->m_particleSystems.size()-1].m_sizeEnd	= glm::vec2(0.0f);
	e->m_particleSystems[e->m_particleSystems.size()-1].m_color		= glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	e->m_particleSystems[e->m_particleSystems.size()-1].m_colorEnd	= glm::vec4(0.0f);
	e->m_particleSystems[e->m_particleSystems.size()-1].m_gravity	= glm::vec3(0.0f, 0.0f, 0.0f);
	e->m_particleSystems[e->m_particleSystems.size()-1].m_direction = glm::vec3(0.0f);
	e->m_particleSystems[e->m_particleSystems.size()-1].m_spread	= 1.0f;
	e->m_particleSystems[e->m_particleSystems.size()-1].m_spawnTime = 0.05f;

	//Map data to params list for buffering
	e->m_particleSystems[e->m_particleSystems.size()-1].m_params[Render::Semantic::POSITION]		= &t->m_position;
	e->m_particleSystems[e->m_particleSystems.size()-1].m_params[Render::Semantic::LIFETIMEMIN]		= &e->m_particleSystems[e->m_particleSystems.size()-1].m_lifeTimeMin;
	e->m_particleSystems[e->m_particleSystems.size()-1].m_params[Render::Semantic::LIFETIMEMAX]		= &e->m_particleSystems[e->m_particleSystems.size()-1].m_lifeTimeMax;
	e->m_particleSystems[e->m_particleSystems.size()-1].m_params[Render::Semantic::SPEEDMIN]		= &e->m_particleSystems[e->m_particleSystems.size()-1].m_speedMin;
	e->m_particleSystems[e->m_particleSystems.size()-1].m_params[Render::Semantic::SPEEDMAX]		= &e->m_particleSystems[e->m_particleSystems.size()-1].m_speedMax;
	e->m_particleSystems[e->m_particleSystems.size()-1].m_params[Render::Semantic::SIZEMIN]			= &e->m_particleSystems[e->m_particleSystems.size()-1].m_sizeMin;
	e->m_particleSystems[e->m_particleSystems.size()-1].m_params[Render::Semantic::SIZEMAX]			= &e->m_particleSystems[e->m_particleSystems.size()-1].m_sizeMax;
	e->m_particleSystems[e->m_particleSystems.size()-1].m_params[Render::Semantic::SIZEEND]			= &e->m_particleSystems[e->m_particleSystems.size()-1].m_sizeEnd;
	e->m_particleSystems[e->m_particleSystems.size()-1].m_params[Render::Semantic::COLOR]			= &e->m_particleSystems[e->m_particleSystems.size()-1].m_color;
	e->m_particleSystems[e->m_particleSystems.size()-1].m_params[Render::Semantic::COLOREND]		= &e->m_particleSystems[e->m_particleSystems.size()-1].m_colorEnd;
	e->m_particleSystems[e->m_particleSystems.size()-1].m_params[Render::Semantic::GRAVITY]			= &e->m_particleSystems[e->m_particleSystems.size()-1].m_gravity;
	e->m_particleSystems[e->m_particleSystems.size()-1].m_params[Render::Semantic::DIRECTION]		= &e->m_particleSystems[e->m_particleSystems.size()-1].m_direction;
	e->m_particleSystems[e->m_particleSystems.size()-1].m_params[Render::Semantic::SPREAD]			= &e->m_particleSystems[e->m_particleSystems.size()-1].m_spread;
	e->m_particleSystems[e->m_particleSystems.size()-1].m_params[Render::Semantic::SPAWNTIME]		= &e->m_particleSystems[e->m_particleSystems.size()-1].m_spawnTime;

	//Add an item representing the emitter to the emitter list with the specified name
	QListWidgetItem* tempItem = new QListWidgetItem(ui.nameEmitterLineEdit->text());
	ui.listWidget->addItem(tempItem);
	m_context->m_logger->LogText(LogTag::TOOLS, LogLevel::DEBUG_PRINT, "Emitter created: %s", ui.nameEmitterLineEdit->text().toStdString().c_str());

	//Enable Delete and rename buttons
	if(m_selectedEmitterIndex == -1)
	{
		ui.deleteEmitterButton->setEnabled(true);
		ui.renameEmitterButton->setEnabled(true);
		EmitterSelected(tempItem);
	}

	//Empty the name field
	ui.nameEmitterLineEdit->setText("");
}

void ParticleEditor::DeleteEmitter()
{
	QListWidgetItem* tempItem = ui.listWidget->currentItem();
	if(!tempItem)
		return;

	int index = ui.listWidget->row(tempItem);

	ECS::Entity* entity = m_emitterEntities.at(m_selectedEntityIndex);
	RootForce::ParticleEmitter* e = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(entity);

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
		m_selectedEmitterIndex = -1;
	}
	else
	{
		//Select first emitter
		EmitterSelected(ui.listWidget->item(0));
	}
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
}

void ParticleEditor::EmitterSelected( QListWidgetItem* p_item)
{
	m_selectedEmitterIndex = ui.listWidget->row(p_item);
	m_context->m_logger->LogText(LogTag::TOOLS, LogLevel::DEBUG_PRINT, "Emitter selected: %d", m_selectedEmitterIndex);

	ECS::Entity* entity = m_emitterEntities.at(m_selectedEntityIndex);
	RootForce::Transform* t = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(entity);
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(entity);
	
	//Position
	ui.posSpinBoxX->setValue(t->m_position.x);
	ui.posSpinBoxY->setValue(t->m_position.y);
	ui.posSpinBoxZ->setValue(t->m_position.z);
	//Lifetime
	ui.lifeTimeMinSpinBox->setValue(pe->m_particleSystems[m_selectedEmitterIndex].m_lifeTimeMin);
	ui.lifeTimeMaxSpinBox->setValue(pe->m_particleSystems[m_selectedEmitterIndex].m_lifeTimeMax);
	//Speed
	ui.speedMinSpinBox->setValue(pe->m_particleSystems[m_selectedEmitterIndex].m_speedMin);
	ui.speedMaxSpinBox->setValue(pe->m_particleSystems[m_selectedEmitterIndex].m_speedMax);
	//Size
	ui.sizeMinSpinBoxX->setValue(pe->m_particleSystems[m_selectedEmitterIndex].m_sizeMin.x);
	ui.sizeMaxSpinBoxX->setValue(pe->m_particleSystems[m_selectedEmitterIndex].m_sizeMax.x);
	ui.sizeEndSpinBoxX->setValue(pe->m_particleSystems[m_selectedEmitterIndex].m_sizeEnd.x);
	
	//Color
	QColor col(pe->m_particleSystems[m_selectedEmitterIndex].m_color.r*255.0f, pe->m_particleSystems[m_selectedEmitterIndex].m_color.g*255.0f, pe->m_particleSystems[m_selectedEmitterIndex].m_color.b*255.0f, pe->m_particleSystems[m_selectedEmitterIndex].m_color.a*255.0f);
	m_colorTriangle->setColor(col);
	
	QColor colEnd(pe->m_particleSystems[m_selectedEmitterIndex].m_colorEnd.r*255.0f, pe->m_particleSystems[m_selectedEmitterIndex].m_colorEnd.g*255.0f, pe->m_particleSystems[m_selectedEmitterIndex].m_colorEnd.b*255.0f, pe->m_particleSystems[m_selectedEmitterIndex].m_colorEnd.a*255.0f);
	m_colorEndTriangle->setColor(colEnd);

	//Gravity
	ui.gravitySpinBoxX->setValue(pe->m_particleSystems[m_selectedEmitterIndex].m_gravity.x);
	ui.gravitySpinBoxY->setValue(pe->m_particleSystems[m_selectedEmitterIndex].m_gravity.y);
	ui.gravitySpinBoxZ->setValue(pe->m_particleSystems[m_selectedEmitterIndex].m_gravity.z);
	//Direction
	ui.directionSpinBoxX->setValue(pe->m_particleSystems[m_selectedEmitterIndex].m_direction.x);
	ui.directionSpinBoxY->setValue(pe->m_particleSystems[m_selectedEmitterIndex].m_direction.y);
	ui.directionSpinBoxZ->setValue(pe->m_particleSystems[m_selectedEmitterIndex].m_direction.z);
	//Spread
	ui.spreadSpinBox->setValue(pe->m_particleSystems[m_selectedEmitterIndex].m_spread);
	//Spawntime
	ui.spawnTimeSpinBox->setValue(pe->m_particleSystems[m_selectedEmitterIndex].m_spawnTime);
}

void ParticleEditor::PositionXChanged( double p_x )
{
	RootForce::Transform* t = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_emitterEntities.at(m_selectedEntityIndex));
	t->m_position.x = (float)p_x;
}

void ParticleEditor::PositionYChanged( double p_y )
{
	RootForce::Transform* t = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_emitterEntities.at(m_selectedEntityIndex));
	t->m_position.y = (float)p_y;
}

void ParticleEditor::PositionZChanged( double p_z )
{
	RootForce::Transform* t = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_emitterEntities.at(m_selectedEntityIndex));
	t->m_position.z = (float)p_z;
}

void ParticleEditor::SizeMinXChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));
	pe->m_particleSystems[m_selectedEmitterIndex].m_sizeMin.x = (float)p_val;
	pe->m_particleSystems[m_selectedEmitterIndex].m_sizeMin.y = (float)p_val;
}

void ParticleEditor::SizeMaxXChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));
	pe->m_particleSystems[m_selectedEmitterIndex].m_sizeMax.x = (float)p_val;
	pe->m_particleSystems[m_selectedEmitterIndex].m_sizeMax.y = (float)p_val;
}


void ParticleEditor::SizeEndXChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));
	pe->m_particleSystems[m_selectedEmitterIndex].m_sizeEnd.x = (float)p_val;
	pe->m_particleSystems[m_selectedEmitterIndex].m_sizeEnd.y = (float)p_val;
}

void ParticleEditor::LifeTimeMinChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));
	pe->m_particleSystems[m_selectedEmitterIndex].m_lifeTimeMin = (float)p_val;
}

void ParticleEditor::LifeTimeMaxChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));
	pe->m_particleSystems[m_selectedEmitterIndex].m_lifeTimeMax = (float)p_val;
}

void ParticleEditor::GravityXChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));
	pe->m_particleSystems[m_selectedEmitterIndex].m_gravity.x = (float)p_val;
}

void ParticleEditor::GravityYChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));
	pe->m_particleSystems[m_selectedEmitterIndex].m_gravity.y = (float)p_val;
}

void ParticleEditor::GravityZChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));
	pe->m_particleSystems[m_selectedEmitterIndex].m_gravity.z = (float)p_val;
}

void ParticleEditor::DirectionXChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));
	pe->m_particleSystems[m_selectedEmitterIndex].m_direction.x = (float)p_val;
}

void ParticleEditor::DirectionYChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));
	pe->m_particleSystems[m_selectedEmitterIndex].m_direction.y = (float)p_val;
}

void ParticleEditor::DirectionZChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));
	pe->m_particleSystems[m_selectedEmitterIndex].m_direction.z = (float)p_val;
}

void ParticleEditor::SpeedMinChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));
	pe->m_particleSystems[m_selectedEmitterIndex].m_speedMin = (float)p_val;
}

void ParticleEditor::SpeedMaxChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));
	pe->m_particleSystems[m_selectedEmitterIndex].m_speedMax = (float)p_val;
}

void ParticleEditor::SpreadChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));
	pe->m_particleSystems[m_selectedEmitterIndex].m_spread = (float)p_val;
}

void ParticleEditor::SpawnTimeChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));
	pe->m_particleSystems[m_selectedEmitterIndex].m_spawnTime = (float)p_val;
}

void ParticleEditor::ColorChanged( const QColor& p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));
	pe->m_particleSystems[m_selectedEmitterIndex].m_color.r = (float)p_val.red()/255.0f;
	pe->m_particleSystems[m_selectedEmitterIndex].m_color.g = (float)p_val.green()/255.0f;
	pe->m_particleSystems[m_selectedEmitterIndex].m_color.b = (float)p_val.blue()/255.0f;
}

void ParticleEditor::ColorEndChanged( const QColor& p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(m_selectedEntityIndex));
	pe->m_particleSystems[m_selectedEmitterIndex].m_colorEnd.r = (float)p_val.red()/255.0f;
	pe->m_particleSystems[m_selectedEmitterIndex].m_colorEnd.g = (float)p_val.green()/255.0f;
	pe->m_particleSystems[m_selectedEmitterIndex].m_colorEnd.b = (float)p_val.blue()/255.0f;
}

void ParticleEditor::MenuViewColorTriangle()
{
	m_colorTriangle->show();
}

void ParticleEditor::MenuViewColorEndTriangle()
{
	m_colorEndTriangle->show();
}

void ParticleEditor::DrawGridX(int p_spacing)
{//TODO: IMPLEMENT ORIGO LINES IN ANOTHER COLOR
	for (int i = -5; i < 6; i++)
	{
		m_context->m_renderer->AddLine(glm::vec3((float)(-5*p_spacing), 0, (float)(i*p_spacing)), glm::vec3((float)(5*p_spacing), 0, (float)(i*p_spacing)), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
		m_context->m_renderer->AddLine(glm::vec3((float)(i*p_spacing), 0, (float)(-5*p_spacing)), glm::vec3((float)(i*p_spacing), 0, (float)(5*p_spacing)), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
	}
}

void ParticleEditor::DrawGridY( int p_spacing )
{
	//TODO: IMPLEMENT 
}

void ParticleEditor::DrawGridZ( int p_spacing )
{
	//TODO: IMPLEMENT 
}

void ParticleEditor::GridToggled()
{
	if(m_showGrid)
	{
		m_showGrid = false;
		m_context->m_logger->LogText(LogTag::TOOLS, LogLevel::DEBUG_PRINT, "Grid toggled off");
	}
	else
	{
		m_showGrid = true;
		m_context->m_logger->LogText(LogTag::TOOLS, LogLevel::DEBUG_PRINT, "Grid toggled on");
	}
}

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
	pe->m_particleSystems[m_selectedEmitterIndex].m_material->m_diffuseMap = m_context->m_resourceManager->LoadTexture(fileInfo.baseName().toStdString().c_str(), Render::TextureType::TEXTURE_2D);
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


