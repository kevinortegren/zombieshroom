#include "ParticleEditor.h"

ParticleEditor::ParticleEditor(QWidget *parent)
	: QMainWindow(parent), m_running(true)
{
	ui.setupUi(this);
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(MenuExit()));
}

ParticleEditor::~ParticleEditor()
{

}

bool ParticleEditor::CheckExit()
{
	return m_running;
}

void ParticleEditor::closeEvent( QCloseEvent *event )
{
	QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Exit Particle Editor",
		tr("Are you sure?\n"),
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

void ParticleEditor::MenuExit()
{
	close();
}
