#include "ParticleEditor.h"

ParticleEditor::ParticleEditor(QWidget *parent)
	: QMainWindow(parent), m_running(true), m_showGrid(false)
{
	ui.setupUi(this);
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(MenuExit()));
	connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(MenuHelpAbout()));
	connect(ui.actionNew, SIGNAL(triggered()), this, SLOT(MenuNew()));
	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(MenuOpen()));
	connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(MenuSave()));
	connect(ui.actionSave_As, SIGNAL(triggered()), this, SLOT(ManuSaveAs()));
	connect(ui.newEmitterButton, SIGNAL(clicked()), this, SLOT(MenuCreateEmitter()));
	connect(ui.actionStats, SIGNAL(triggered()), this, SLOT(MenuViewStats()));
	connect(ui.deleteEmitterButton, SIGNAL(clicked()), this, SLOT(DeleteEmitter()));
	connect(ui.renameEmitterButton, SIGNAL(clicked()), this, SLOT(RenameEmitter()));
	connect(ui.listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(EmitterSelected(QListWidgetItem*)));
	connect(ui.actionGrid_2, SIGNAL(triggered()), this, SLOT(GridToggled()));
	connect(ui.posSpinBoxX, SIGNAL(valueChanged(double)), this, SLOT(PositionXChanged(double)));
	connect(ui.posSpinBoxY, SIGNAL(valueChanged(double)), this, SLOT(PositionYChanged(double)));
	connect(ui.posSpinBoxZ, SIGNAL(valueChanged(double)), this, SLOT(PositionZChanged(double)));
	connect(ui.sizeMinSpinBoxX, SIGNAL(valueChanged(double)), this, SLOT(SizeMinXChanged(double)));
	connect(ui.sizeMinSpinBoxY, SIGNAL(valueChanged(double)), this, SLOT(SizeMinYChanged(double)));
	connect(ui.sizeMaxSpinBoxX, SIGNAL(valueChanged(double)), this, SLOT(SizeMaxXChanged(double)));
	connect(ui.sizeMaxSpinBoxY, SIGNAL(valueChanged(double)), this, SLOT(SizeMaxYChanged(double)));
	connect(ui.sizeEndSpinBoxX, SIGNAL(valueChanged(double)), this, SLOT(SizeEndXChanged(double)));
	connect(ui.sizeEndSpinBoxY, SIGNAL(valueChanged(double)), this, SLOT(SizeEndYChanged(double)));
	connect(ui.lifeTimeMinSpinBox, SIGNAL(valueChanged(double)), this, SLOT(LifeTimeMinChanged(double)));
	connect(ui.lifeTimeMaxSpinBox, SIGNAL(valueChanged(double)), this, SLOT(LifeTimeMaxChanged(double)));
	connect(ui.gravitySpinBoxX, SIGNAL(valueChanged(double)), this, SLOT(GravityXChanged(double)));
	connect(ui.gravitySpinBoxY, SIGNAL(valueChanged(double)), this, SLOT(GravityYChanged(double)));
	connect(ui.gravitySpinBoxZ, SIGNAL(valueChanged(double)), this, SLOT(GravityZChanged(double)));
	connect(ui.directionSpinBoxX, SIGNAL(valueChanged(double)), this, SLOT(DirectionXChanged(double)));
	connect(ui.directionSpinBoxY, SIGNAL(valueChanged(double)), this, SLOT(DirectionYChanged(double)));
	connect(ui.directionSpinBoxZ, SIGNAL(valueChanged(double)), this, SLOT(DirectionZChanged(double)));
	connect(ui.speedMinSpinBox, SIGNAL(valueChanged(double)), this, SLOT(SpeedMinChanged(double)));
	connect(ui.speedMaxSpinBox, SIGNAL(valueChanged(double)), this, SLOT(SpeedMaxChanged(double)));
	connect(ui.spreadSpinBox, SIGNAL(valueChanged(double)), this, SLOT(SpreadChanged(double)));
	connect(ui.spawnTimeSpinBox, SIGNAL(valueChanged(double)), this, SLOT(SpawnTimeChanged(double)));

	m_statWidget = new StatWidget();
	m_statWidget->hide();

	m_particleTab = new ParticleTab(this);
	m_particleTab->setGeometry(QRect(0, ui.menuBar->height(), 1209, 27));
	m_particleTab->show();

	ui.menuView->setEnabled(false);

	m_selectedEmitterIndex = 0;
}

ParticleEditor::~ParticleEditor()
{
	delete m_statWidget;
	delete m_particleTab;
}

bool ParticleEditor::CheckExit()
{
	return m_running;
}

void ParticleEditor::closeEvent( QCloseEvent *event )
{
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
		DrawGrid(2);
}

void ParticleEditor::MenuExit()
{
	close();
}

void ParticleEditor::MenuNew()
{
	m_particleTab->AddTab();
	ui.menuView->setEnabled(true);
	ui.newEmitterButton->setEnabled(true);

	ECS::Entity* p = m_world->GetEntityManager()->CreateEntity();
	RootForce::Transform* t = m_world->GetEntityManager()->CreateComponent<RootForce::Transform>(p);
	t->m_position = glm::vec3(0,0,0);
	RootForce::ParticleEmitter* e = m_world->GetEntityManager()->CreateComponent<RootForce::ParticleEmitter>(p);
	e->m_particleSystems.reserve(5);
	m_emitterEntities.push_back(p);
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

void ParticleEditor::MenuCreateEmitter()
{
	if(0 == QString::compare(ui.nameEmitterLineEdit->text(), ""))
	{
		QMessageBox msg;
		msg.setText("Invalid name");
		msg.setIcon(QMessageBox::Information);
		msg.setStandardButtons(QMessageBox::Ok);
		msg.setDefaultButton(QMessageBox::Ok);
		QSpacerItem* horizontalSpacer = new QSpacerItem(200, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
		QGridLayout* layout = (QGridLayout*)msg.layout();
		layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
		msg.show();
		msg.exec();
		return;
	}

	ui.deleteEmitterButton->setEnabled(true);
	ui.renameEmitterButton->setEnabled(true);

	ECS::Entity* entity = m_emitterEntities.at(0);
	RootForce::Transform* t = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(entity);
	RootForce::ParticleEmitter* e = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(entity);

	RootForce::ParticleSystemStruct particleSystem;
	e->m_particleSystems.push_back(particleSystem);

	e->m_particleSystems[e->m_particleSystems.size()-1].m_system = m_context->m_renderer->CreateParticleSystem();	
	e->m_particleSystems[e->m_particleSystems.size()-1].m_material = m_context->m_resourceManager->GetMaterial("particle");
	e->m_particleSystems[e->m_particleSystems.size()-1].m_material->m_diffuseMap = m_context->m_resourceManager->LoadTexture("smoke", Render::TextureType::TEXTURE_2D);
	e->m_particleSystems[e->m_particleSystems.size()-1].m_material->m_effect = m_context->m_resourceManager->LoadEffect("Particle/Particle");
	e->m_particleSystems[e->m_particleSystems.size()-1].m_lifeTimeMin = 2.0f;
	e->m_particleSystems[e->m_particleSystems.size()-1].m_lifeTimeMax = 2.0f;
	e->m_particleSystems[e->m_particleSystems.size()-1].m_speedMin = 0.1f;
	e->m_particleSystems[e->m_particleSystems.size()-1].m_speedMax = 0.1f;
	e->m_particleSystems[e->m_particleSystems.size()-1].m_sizeMin = glm::vec2(0.05f);
	e->m_particleSystems[e->m_particleSystems.size()-1].m_sizeMax = glm::vec2(0.05f);
	e->m_particleSystems[e->m_particleSystems.size()-1].m_sizeEnd = glm::vec2(1.0f);
	e->m_particleSystems[e->m_particleSystems.size()-1].m_color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	e->m_particleSystems[e->m_particleSystems.size()-1].m_colorEnd = glm::vec4(1.0f);
	e->m_particleSystems[e->m_particleSystems.size()-1].m_gravity = glm::vec3(0.0f, 0.0f, 0.0f);
	e->m_particleSystems[e->m_particleSystems.size()-1].m_direction = glm::vec3(0.0f);
	e->m_particleSystems[e->m_particleSystems.size()-1].m_spread = 1.0f;
	e->m_particleSystems[e->m_particleSystems.size()-1].m_spawnTime = 0.05f;

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

	ui.listWidget->addItem(new QListWidgetItem(ui.nameEmitterLineEdit->text()));
	m_context->m_logger->LogText(LogTag::TOOLS, LogLevel::DEBUG_PRINT, "Emitter created: %s", ui.nameEmitterLineEdit->text().toStdString().c_str());
	ui.nameEmitterLineEdit->setText("");
}

void ParticleEditor::SetWorld( ECS::World* p_world )
{
	m_world = p_world;
}

void ParticleEditor::SetContext( RootEngine::GameSharedContext* p_context )
{
	m_context = p_context;
}

void ParticleEditor::DeleteEmitter()
{
	QModelIndexList indexes = ui.listWidget->selectionModel()->selectedIndexes();
	if(indexes.size() == 0)
		return;

	int index = indexes.at(0).row();

	ECS::Entity* entity = m_emitterEntities.at(0);
	RootForce::ParticleEmitter* e = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(entity);

	e->m_particleSystems.erase(e->m_particleSystems.begin() + index);

	QListWidgetItem* deleteItem =  ui.listWidget->takeItem(index);
	m_context->m_logger->LogText(LogTag::TOOLS, LogLevel::DEBUG_PRINT, "Emitter deleted: %s", deleteItem->text().toStdString().c_str());
	delete deleteItem;
	if (ui.listWidget->count() == 0)
	{
		ui.deleteEmitterButton->setEnabled(false);
		ui.renameEmitterButton->setEnabled(false);
	}
}

void ParticleEditor::RenameEmitter()
{
	if(0 == QString::compare(ui.nameEmitterLineEdit->text(), ""))
	{
		QMessageBox msg;
		msg.setText("Name is not valid\n");
		msg.show();
		msg.exec();
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

	ECS::Entity* entity = m_emitterEntities.at(0);
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
	ui.sizeMinSpinBoxY->setValue(pe->m_particleSystems[m_selectedEmitterIndex].m_sizeMin.y);
	ui.sizeMaxSpinBoxX->setValue(pe->m_particleSystems[m_selectedEmitterIndex].m_sizeMax.x);
	ui.sizeMaxSpinBoxY->setValue(pe->m_particleSystems[m_selectedEmitterIndex].m_sizeMax.y);
	ui.sizeEndSpinBoxX->setValue(pe->m_particleSystems[m_selectedEmitterIndex].m_sizeEnd.x);
	ui.sizeEndSpinBoxY->setValue(pe->m_particleSystems[m_selectedEmitterIndex].m_sizeEnd.y);
	//Color
	std::string colorBut = "* { background-color: rgb(" + std::to_string(pe->m_particleSystems[m_selectedEmitterIndex].m_color.r) + "," + std::to_string(pe->m_particleSystems[m_selectedEmitterIndex].m_color.g) + "," + std::to_string(pe->m_particleSystems[m_selectedEmitterIndex].m_color.b) + ") }";
	ui.colorButton->setStyleSheet(QString::fromStdString(colorBut));
	std::string colorEndBut = "* { background-color: rgb(" + std::to_string(pe->m_particleSystems[m_selectedEmitterIndex].m_colorEnd.r) + "," + std::to_string(pe->m_particleSystems[m_selectedEmitterIndex].m_colorEnd.g) + "," + std::to_string(pe->m_particleSystems[m_selectedEmitterIndex].m_colorEnd.b) + ") }";
	ui.colorButtonEnd->setStyleSheet(QString::fromStdString(colorEndBut));
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
	RootForce::Transform* t = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_emitterEntities.at(0));
	t->m_position.x = (float)p_x;
}

void ParticleEditor::PositionYChanged( double p_y )
{
	RootForce::Transform* t = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_emitterEntities.at(0));
	t->m_position.y = (float)p_y;
}

void ParticleEditor::PositionZChanged( double p_z )
{
	RootForce::Transform* t = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_emitterEntities.at(0));
	t->m_position.z = (float)p_z;
}

void ParticleEditor::SizeMinXChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(0));
	pe->m_particleSystems[m_selectedEmitterIndex].m_sizeMin.x = (float)p_val;
}

void ParticleEditor::SizeMinYChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(0));
	pe->m_particleSystems[m_selectedEmitterIndex].m_sizeMin.y = (float)p_val;
}

void ParticleEditor::SizeMaxXChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(0));
	pe->m_particleSystems[m_selectedEmitterIndex].m_sizeMax.x = (float)p_val;
}

void ParticleEditor::SizeMaxYChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(0));
	pe->m_particleSystems[m_selectedEmitterIndex].m_sizeMax.y = (float)p_val;
}

void ParticleEditor::SizeEndXChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(0));
	pe->m_particleSystems[m_selectedEmitterIndex].m_sizeEnd.x = (float)p_val;
}

void ParticleEditor::SizeEndYChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(0));
	pe->m_particleSystems[m_selectedEmitterIndex].m_sizeEnd.y = (float)p_val;
}

void ParticleEditor::LifeTimeMinChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(0));
	pe->m_particleSystems[m_selectedEmitterIndex].m_lifeTimeMin = (float)p_val;
}

void ParticleEditor::LifeTimeMaxChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(0));
	pe->m_particleSystems[m_selectedEmitterIndex].m_lifeTimeMax = (float)p_val;
}

void ParticleEditor::GravityXChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(0));
	pe->m_particleSystems[m_selectedEmitterIndex].m_gravity.x = (float)p_val;
}

void ParticleEditor::GravityYChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(0));
	pe->m_particleSystems[m_selectedEmitterIndex].m_gravity.y = (float)p_val;
}

void ParticleEditor::GravityZChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(0));
	pe->m_particleSystems[m_selectedEmitterIndex].m_gravity.z = (float)p_val;
}

void ParticleEditor::DirectionXChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(0));
	pe->m_particleSystems[m_selectedEmitterIndex].m_direction.x = (float)p_val;
}

void ParticleEditor::DirectionYChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(0));
	pe->m_particleSystems[m_selectedEmitterIndex].m_direction.y = (float)p_val;
}

void ParticleEditor::DirectionZChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(0));
	pe->m_particleSystems[m_selectedEmitterIndex].m_direction.z = (float)p_val;
}

void ParticleEditor::SpeedMinChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(0));
	pe->m_particleSystems[m_selectedEmitterIndex].m_speedMin = (float)p_val;
}

void ParticleEditor::SpeedMaxChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(0));
	pe->m_particleSystems[m_selectedEmitterIndex].m_speedMax = (float)p_val;
}

void ParticleEditor::SpreadChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(0));
	pe->m_particleSystems[m_selectedEmitterIndex].m_spread = (float)p_val;
}

void ParticleEditor::SpawnTimeChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(0));
	pe->m_particleSystems[m_selectedEmitterIndex].m_spawnTime = (float)p_val;
}

void ParticleEditor::ColorRChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(0));
	pe->m_particleSystems[m_selectedEmitterIndex].m_color.r = (float)p_val;
}

void ParticleEditor::ColorGChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(0));
	pe->m_particleSystems[m_selectedEmitterIndex].m_color.g = (float)p_val;
}

void ParticleEditor::ColorBChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(0));
	pe->m_particleSystems[m_selectedEmitterIndex].m_color.b = (float)p_val;
}

void ParticleEditor::ColorEndRChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(0));
	pe->m_particleSystems[m_selectedEmitterIndex].m_colorEnd.r = (float)p_val;
}

void ParticleEditor::ColorEndGChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(0));
	pe->m_particleSystems[m_selectedEmitterIndex].m_colorEnd.g = (float)p_val;
}

void ParticleEditor::ColorEndBChanged( double p_val )
{
	RootForce::ParticleEmitter* pe = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(m_emitterEntities.at(0));
	pe->m_particleSystems[m_selectedEmitterIndex].m_colorEnd.b = (float)p_val;
}

void ParticleEditor::DrawGrid(int p_spacing)
{
	for (int i = -5; i < 6; i++)
	{
		m_context->m_renderer->AddLine(glm::vec3((float)(-5*p_spacing), 0, (float)(i*p_spacing)), glm::vec3((float)(5*p_spacing), 0, (float)(i*p_spacing)), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
		m_context->m_renderer->AddLine(glm::vec3((float)(i*p_spacing), 0, (float)(-5*p_spacing)), glm::vec3((float)(i*p_spacing), 0, (float)(5*p_spacing)), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
	}
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



