#include <RootTools/ParticleEditor/Include/StatWidget.h>

StatWidget::StatWidget(QWidget *parent)
	: QWidget(parent)
{
	m_samples = 0;
	m_collectedTime = 0.0f;

	setObjectName(QStringLiteral("dockWidget"));
	setGeometry(QRect(0, 100, 271, 291));

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
}

void StatWidget::Update( float p_dt )
{
	
	m_samples++;
	m_collectedTime += p_dt;
	if(m_collectedTime >= 0.5f)
	{
		int fps = (int)(1.0f/(m_collectedTime/(float)m_samples));
		m_fpsLCD->display(fps);
		m_samples = 0;
		m_collectedTime = 0;
	}
	
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