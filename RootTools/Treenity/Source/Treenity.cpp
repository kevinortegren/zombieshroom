#include "Treenity.h"
#include <QtWidgets\qmessagebox.h>

Treenity::Treenity(QWidget *parent)
	: QMainWindow(parent), m_running(true)
{
	ui.setupUi(this);

	connect(ui.pushButton_createEntity, SIGNAL(clicked()), this, SLOT(CreateEntity()));
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

	QMessageBox::warning(this, QString::number(e->GetId()), "Entity id in the title!", QMessageBox::NoButton, QMessageBox::NoButton);
}