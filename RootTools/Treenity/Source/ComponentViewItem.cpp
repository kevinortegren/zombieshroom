#include <RootTools/Treenity/Include/ComponentViewItem.h>

ComponentViewItem::ComponentViewItem(AbstractComponentView* p_item, QWidget* p_parent /*= 0*/ ) : QWidget(p_parent), m_item(p_item)
{
	m_label = new QLabel(p_item->GetComponentName(), this);
	m_label->setStyleSheet("background-color: #D9D9D9;color: #000000;");
	m_layout = new QVBoxLayout(this);
	m_layout->setContentsMargins(0, 0, 0, 0);
	m_layout->addWidget(m_label);
	m_layout->addWidget(p_item);
	m_layout->setSizeConstraint(QLayout::SetNoConstraint);
	setLayout(m_layout);

	//p_item->setVisible(false);
}

ComponentViewItem::~ComponentViewItem()
{
	while(!m_layout->isEmpty())
	{
		m_layout->takeAt(0);
	}

	m_item->setParent(nullptr);
	delete m_label;
	delete m_layout;
}

void ComponentViewItem::mousePressEvent( QMouseEvent* event )
{
	if(m_label->geometry().contains(event->pos()))
	{
		m_item->setVisible(!m_item->isVisible());
	}
}

AbstractComponentView* ComponentViewItem::GetItem()
{
	return m_item;
}

