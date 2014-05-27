#include <RootTools/Treenity/Include/ComponentViewItem.h>

ComponentViewItem::ComponentViewItem(AbstractComponentView* p_item, QWidget* p_parent /*= 0*/ ) : QWidget(p_parent), m_item(p_item)
{
	m_header = new ComponentViewHeader(p_item->GetComponentName(), this);
	p_item->setVisible(true);
	m_layout = new QVBoxLayout(this);
	m_layout->setContentsMargins(0, 0, 0, 0);
	m_layout->addWidget(m_header);
	m_layout->addWidget(p_item);
	m_layout->setSizeConstraint(QLayout::SetNoConstraint);
	setLayout(m_layout);

	connect(m_header, SIGNAL(showView()),	this, SLOT(ShowComponentView()));
	connect(m_header, SIGNAL(hideView()),	this, SLOT(HideComponentView()));
	connect(m_header, SIGNAL(remove()),		this, SLOT(RemoveComponent()));
}

ComponentViewItem::~ComponentViewItem()
{
	while(!m_layout->isEmpty())
	{
		m_layout->takeAt(0);
	}

	m_item->setParent(nullptr);
	delete m_header;
	delete m_layout;
}

AbstractComponentView* ComponentViewItem::GetItem()
{
	return m_item;
}

void ComponentViewItem::ShowComponentView()
{
	m_item->setVisible(true);
}

void ComponentViewItem::HideComponentView()
{
	m_item->setVisible(false);
}

void ComponentViewItem::RemoveComponent()
{
	ECS::Entity* entity = *m_item->GetEditorInterface()->GetSelection().begin();

	m_item->Delete(entity);
}
