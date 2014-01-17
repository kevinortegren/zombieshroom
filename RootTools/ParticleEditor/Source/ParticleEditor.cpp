#include "ParticleEditor.h"

ParticleEditor::ParticleEditor(QWidget *parent)
	: QMainWindow(parent), m_running(true)
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

	m_statWidget = new StatWidget();
	m_statWidget->hide();

	m_particleTab = new ParticleTab(this);
	m_particleTab->setGeometry(QRect(0, ui.menuBar->height(), 1209, 27));
	m_particleTab->show();

	ui.menuView->setEnabled(false);
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

	ECS::Entity* p = m_world->GetEntityManager()->CreateEntity();
	RootForce::Transform* t = m_world->GetEntityManager()->CreateComponent<RootForce::Transform>(p);
	RootForce::ParticleEmitter* e = m_world->GetEntityManager()->CreateComponent<RootForce::ParticleEmitter>(p);	

	Render::ParticleSystemDescription desc;
	desc.m_initalPos = glm::vec3(0,0,0);
	desc.m_initalVel = glm::vec3(0,0,0);
	desc.m_size = glm::vec2(0.05f, 0.05f);

	RootForce::ParticleSystemStruct particleSystem;
	particleSystem.m_system = m_context->m_renderer->CreateParticleSystem(desc);	
	particleSystem.m_material = m_context->m_resourceManager->GetMaterial("particle");
	particleSystem.m_material->m_diffuseMap = m_context->m_resourceManager->LoadTexture("smoke", Render::TextureType::TEXTURE_2D);
	particleSystem.m_material->m_effect = m_context->m_resourceManager->LoadEffect("Particle/Particle");

	particleSystem.m_params[Render::Semantic::POSITION] = &t->m_position;

	m_emitterEntities.push_back(p);
	ui.listWidget->addItem(new QListWidgetItem(ui.nameEmitterLineEdit->text()));
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

void ParticleEditor::DeleteEmitter(  )
{
	QModelIndexList indexes = ui.listWidget->selectionModel()->selectedIndexes();
	if(indexes.size() == 0)
		return;

	int index = indexes.at(0).row();
	m_world->GetEntityManager()->RemoveAllComponents(m_emitterEntities.at(index));
	m_world->GetEntityManager()->RemoveEntity(m_emitterEntities.at(index));
	m_emitterEntities.erase(m_emitterEntities.begin()+index);
	delete ui.listWidget->takeItem(index);

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
		ui.listWidget->currentItem()->setText(ui.nameEmitterLineEdit->text());
		ui.nameEmitterLineEdit->setText("");
	}
}
