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
	connect(ui.actionEmitter, SIGNAL(triggered()), this, SLOT(MenuCreateEmitter()));
	connect(ui.actionStats, SIGNAL(triggered()), this, SLOT(MenuViewStats()));

	m_statWidget = new StatWidget(ui.tabWidget);
	m_statWidget->hide();
}

ParticleEditor::~ParticleEditor()
{
	delete m_statWidget;
}

bool ParticleEditor::CheckExit()
{
	return m_running;
}

void ParticleEditor::closeEvent( QCloseEvent *event )
{
	QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Exit Particle Editor",
		tr("Sure you want to quit Particle Editor?\nYou won't be able to edit och view particle systems if you do."),
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
	ECS::Entity* p = m_world->GetEntityManager()->CreateEntity();
	RootForce::Transform* t = m_world->GetEntityManager()->CreateComponent<RootForce::Transform>(p);
	RootForce::ParticleEmitter* e = m_world->GetEntityManager()->CreateComponent<RootForce::ParticleEmitter>(p);	

	Render::ParticleSystemDescription desc;
	desc.m_initalPos =glm::vec3(0,0,0);
	desc.m_initalVel = glm::vec3(0,0,0);
	desc.m_size = glm::vec2(0.05f, 0.05f);

	e->m_system = m_context->m_renderer->CreateParticleSystem(desc);	
	e->m_material = m_context->m_resourceManager->GetMaterial("particle");
	e->m_material->m_diffuseMap = m_context->m_resourceManager->LoadTexture("smoke", Render::TextureType::TEXTURE_2D);
	e->m_material->m_effect = m_context->m_resourceManager->LoadEffect("Particle/Particle");

	m_emitterEntities.push_back(p);
	ui.listWidget->addItem(new QListWidgetItem("Hejsan"));
}

void ParticleEditor::SetWorld( ECS::World* p_world )
{
	m_world = p_world;
}

void ParticleEditor::SetContext( RootEngine::GameSharedContext* p_context )
{
	m_context = p_context;
}
