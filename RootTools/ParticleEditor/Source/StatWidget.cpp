#include <RootTools/ParticleEditor/Include/StatWidget.h>

StatWidget::StatWidget(QWidget *parent)
	: QDockWidget(parent)
{
	setObjectName(QStringLiteral("dockWidget"));
	setGeometry(QRect(0, 100, 271, 291));
	setAllowedAreas(Qt::NoDockWidgetArea);

	//m_widget = new QWidget(this);
	//m_widget->setObjectName(QStringLiteral("m_widget"));
	//m_widget->show();

	m_fpsLCD = new QLCDNumber(this);
	m_fpsLCD->setObjectName(QStringLiteral("m_fpsLCD"));
	m_fpsLCD->setGeometry(QRect(110, 10, 81, 31));
	m_fpsLCD->show();

	m_labelLCD = new QLabel(this);
	m_labelLCD->setObjectName(QStringLiteral("m_labelLCD"));
	m_labelLCD->setGeometry(QRect(60, 20, 51, 21));
	m_labelLCD->setText("FPS: ");
	m_labelLCD->show();
}


StatWidget::~StatWidget()
{
	delete m_labelLCD;
	delete m_fpsLCD;
	//delete m_widget;
}

void StatWidget::Update( float p_dt )
{
	int fps = (int)(1.0f/p_dt);
	m_fpsLCD->display(fps);
}

void StatWidget::SetStatStruct( StatStruct* p_statStruct )
{
	m_statStruct = p_statStruct;
}

void StatWidget::closeEvent( QCloseEvent *event )
{
	hide();
	event->ignore();
}