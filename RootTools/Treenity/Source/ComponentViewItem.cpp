#include <RootTools/Treenity/Include/ComponentViewItem.h>

ComponentViewItem::ComponentViewItem(const QString& p_title, QWidget* p_item, QWidget* p_parent /*= 0*/ ) : QWidget(p_parent), m_item(p_item)
{
	
	m_label = new QLabel(p_title, this);
	m_label->setStyleSheet("background-color: #D9D9D9;");
	m_layout = new QVBoxLayout(this);
	m_layout->setContentsMargins(0, 0, 0, 0);
	m_layout->addWidget(m_label);
	m_layout->addWidget(p_item);    
	setLayout(m_layout);

	p_item->setVisible(false);
}

ComponentViewItem::~ComponentViewItem()
{
	while(!m_layout->isEmpty())
	{
		delete m_layout->takeAt(0);
	}
	delete m_layout;
}

void ComponentViewItem::mousePressEvent( QMouseEvent* event )
{
	if(m_label->geometry().contains(event->pos()))
		m_item->setVisible(!m_item->isVisible());
}

