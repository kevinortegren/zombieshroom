#include "Treenity.h"
#include <QtWidgets\qmessagebox.h>

#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Include/GameSharedContext.h>
extern RootEngine::GameSharedContext g_engineContext;

Treenity::Treenity(QWidget *parent)
	: QMainWindow(parent), m_running(true)
{
	ui.setupUi(this);

	QActionGroup* group = new QActionGroup(ui.toolBar);

	group->addAction(ui.actionTranslate);
	group->addAction(ui.actionRotate);
	group->addAction(ui.actionResize);

	ui.actionTranslate->setChecked(true);

	connect(ui.actionAdd_entity, SIGNAL(triggered()), this, SLOT(CreateEntity()));
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

void Treenity::CreateOpenGLContext()
{
	ui.widget_canvas3D->CreateOpenGLContext();
	g_engineContext.m_logger->LogText(LogTag::TOOLS, LogLevel::START_PRINT, "Creating OpenGL context in treenity.");
}

void Treenity::CreateEntity()
{
	ECS::Entity* e = m_engineInterface->CreateEntity();

//	QMessageBox::warning(this, QString::number(e->GetId()), "Entity id in the title!", QMessageBox::NoButton, QMessageBox::NoButton);
}