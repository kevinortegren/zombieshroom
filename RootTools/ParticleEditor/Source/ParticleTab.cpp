#include <RootTools/ParticleEditor/Include/ParticleTab.h>

ParticleTab::ParticleTab(QWidget *parent)
	: QTabWidget(parent)
{
	setObjectName(QStringLiteral("tabWidget_2"));
	setTabsClosable(false);
	setMovable(false);
	connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(RemoveTab(int)));
	m_numTabs = 0;
}

ParticleTab::~ParticleTab()
{

}

void ParticleTab::closeEvent( QCloseEvent *event )
{
	event->ignore();
}

void ParticleTab::AddTab()
{
	std::string str = "Untitled project " + std::to_string(m_numTabs);
	m_numTabs++;
	QWidget* widget = new QWidget(this);
	m_tabs[str] = widget;
	addTab(widget, QString::fromStdString(str));
}

void ParticleTab::RemoveTab( int p_index )
{
	QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Close project",
		tr("Close project?\n"),
		QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
		QMessageBox::Yes);
	if (resBtn == QMessageBox::Yes)
	{
		if(m_tabs.size() != 0)
		{
			removeTab(p_index);
		}
	} 
	else 
	{
	}
	
}
