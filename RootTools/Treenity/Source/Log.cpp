#include <RootTools/Treenity/Include/Log.h>

bool Log::m_instanceFlag = false;
Log* Log::m_console = nullptr;

Log* Log::GetInstance()
{
	if(!m_instanceFlag)
	{
		m_console = new Log();
		m_instanceFlag = true;
		return m_console;
	}
	else
	{
		return m_console;
	}
}

Log::~Log()
{
	m_instanceFlag = false;
}

void Log::Write( const QString& p_text )
{
	GetInstance()->WriteToConsole(p_text);
}


Log::Log( QWidget* p_parent /*= 0*/ ) : QDockWidget(p_parent)
{

	//QPalette palette;
	//palette.setColor(QPalette::Window, QColor(53,53,53));
	//palette.setColor(QPalette::WindowText, Qt::white);
	//palette.setColor(QPalette::Base, QColor(15,15,15));
	//palette.setColor(QPalette::AlternateBase, QColor(53,53,53));
	//palette.setColor(QPalette::ToolTipBase, Qt::white);
	//palette.setColor(QPalette::ToolTipText, Qt::white);
	//palette.setColor(QPalette::Text, Qt::white);
	//palette.setColor(QPalette::Button, QColor(53,53,53));
	//palette.setColor(QPalette::ButtonText, Qt::white);
	//palette.setColor(QPalette::BrightText, Qt::red);
	//palette.setColor(QPalette::Highlight, QColor(255,140,0));
	//palette.setColor(QPalette::HighlightedText, Qt::black);
	//setPalette(palette);

	setAllowedAreas(Qt::BottomDockWidgetArea|Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
	setGeometry(10, 25, 300, 700);
	setWindowTitle("Log");
	hide();
	
	//Create a widget to hold all content
	m_contentWidget = new QWidget();
	
	//Create a text browser
	m_textBrowser = new QTextBrowser(m_contentWidget);

	//Add input field
	m_lineEdit = new QLineEdit(m_contentWidget);

	//Create layout for content
	m_layout = new QVBoxLayout(m_contentWidget);
	m_layout->setContentsMargins(0, 0, 0, 0);
	m_layout->setSpacing(0);

	//Add text browser to layout
	m_layout->addWidget(m_textBrowser);
	m_layout->addWidget(m_lineEdit);
	

	//Put the content widget in the dockable window
	setWidget(m_contentWidget);
}

void Log::WriteToConsole( const QString& p_text )
{
	m_textBrowser->append(p_text);
}

void Log::Show()
{
	show();
}
