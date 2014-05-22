#include <RootTools/Treenity/Include/ComponentViewItem.h>

ComponentViewItem::ComponentViewItem(AbstractComponentView* p_item, QWidget* p_parent /*= 0*/ ) : QWidget(p_parent), m_item(p_item)
{
	m_headerButton = new QPushButton(p_item->GetComponentName(), this);
	m_headerButton->setStyleSheet("QPushButton {background-color: #666666; padding-top: 3px; padding-bottom: 3px; }");
	m_layout = new QVBoxLayout(this);
	m_layout->setContentsMargins(0, 0, 0, 0);
	m_layout->addWidget(m_headerButton);
	m_layout->addWidget(p_item);
	m_layout->setSizeConstraint(QLayout::SetNoConstraint);
	setLayout(m_layout);

	connect(m_headerButton, SIGNAL(clicked()), this, SLOT(HeaderClicked()));
}

ComponentViewItem::~ComponentViewItem()
{
	while(!m_layout->isEmpty())
	{
		m_layout->takeAt(0);
	}

	m_item->setParent(nullptr);
	delete m_headerButton;
	delete m_layout;
}

AbstractComponentView* ComponentViewItem::GetItem()
{
	return m_item;
}

void ComponentViewItem::HeaderClicked()
{
	m_item->setVisible(!m_item->isVisible());
}

