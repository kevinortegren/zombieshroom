#include "Treenity.h"
#include <QtWidgets\qmessagebox.h>
#include <RootTools/Treenity/Include/Log.h>

Treenity::Treenity(QWidget *parent)
	: QMainWindow(parent), m_running(true)
{
	ui.setupUi(this);

	//Set some logging init things
	Log::GetInstance()->setParent(this);
	Log::GetInstance()->setFloating(true);

	QActionGroup* group = new QActionGroup(ui.toolBar);

	group->addAction(ui.actionTranslate);
	group->addAction(ui.actionRotate);
	group->addAction(ui.actionResize);

	ui.actionTranslate->setChecked(true);

	connect(ui.actionAdd_entity,	SIGNAL(triggered()), this,					SLOT(CreateEntity()));
	connect(ui.actionLog,			SIGNAL(triggered()), Log::GetInstance(),	SLOT(Show()));
}

Treenity::~Treenity()
{

}

void Treenity::SetEngineInterface(EngineInterface* p_engineInterface)
{
	m_engineInterface = p_engineInterface;
}

bool Treenity::IsRunning()
{
	return m_running;
}

void Treenity::closeEvent(QCloseEvent *event)
{
	m_running = false;
}

void Treenity::CreateEntity()
{
	ECS::Entity* e = m_engineInterface->CreateEntity();
	Log::Write("Entity created! ID: " + QString::number(e->GetId()));
}