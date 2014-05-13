#include <RootTools/Treenity/Include/Utils.h>

bool Utils::m_instanceFlag = false;
Utils* Utils::m_console = nullptr;

Utils* Utils::GetInstance()
{
	if(!m_instanceFlag)
	{
		m_console = new Utils();
		m_instanceFlag = true;
		return m_console;
	}
	else
	{
		return m_console;
	}
}

Utils::~Utils()
{
	m_instanceFlag = false;
}

void Utils::Write( const QString& p_text )
{
	GetInstance()->WriteToConsole(p_text);
}


Utils::Utils( QWidget* p_parent /*= 0*/ ) : QDockWidget(p_parent)
{
	setAllowedAreas(Qt::BottomDockWidgetArea|Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
	setGeometry(10, 25, 300, 700);
	setWindowTitle("Log");
	hide();
	
	//Create a widget to hold all content
	m_contentWidget = new QWidget();
	
	//Create a text browser
	m_textBrowser = new QTextBrowser(m_contentWidget);

	//Create layout for content
	m_layout = new QVBoxLayout(m_contentWidget);
	m_layout->setContentsMargins(0, 0, 0, 0);
	m_layout->setSpacing(0);

	//Add text browser to layout
	m_layout->addWidget(m_textBrowser);

	//Put the content widget in the dockable window
	setWidget(m_contentWidget);

	//Set up progress dialog 
	m_progressDialog = new QProgressDialog(this);
	connect(&m_futureWatcher, SIGNAL(finished()), m_progressDialog , SLOT(cancel()));
}

void Utils::WriteToConsole( const QString& p_text )
{
	m_textBrowser->append(p_text);
}

void Utils::Show()
{
	show();
}

void Utils::RunWithProgressBar( QFuture<void> p_future )
{
	GetInstance()->Run(p_future);

}

void Utils::Run(  QFuture<void> p_future )
{
	// Start the computation.
	m_futureWatcher.setFuture(p_future);
	m_progressDialog->setWindowTitle("Loading");
	m_progressDialog->setLabelText("Please wait...");
	m_progressDialog->setMinimum(0);
	m_progressDialog->setMaximum(0);
	m_progressDialog->setWindowModality(Qt::WindowModal);
	m_progressDialog->exec();
}

