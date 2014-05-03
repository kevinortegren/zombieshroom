#include <RootTools/Treenity/Include/ComponentView.h>

ComponentView::ComponentView( QWidget* p_parent /*= 0*/ ) : QWidget(p_parent), m_layout(new QVBoxLayout())
{
	setLayout(m_layout);
}

ComponentView::~ComponentView()
{

}

void ComponentView::AddItem( ComponentViewItem* p_item )
{
	// Remove last spacer item if present.
	int count = m_layout->count();
	if (count > 1) {
		m_layout->removeItem(m_layout->itemAt(count - 1));
	}

	// Add item and make sure it stretches the remaining space.
	m_layout->addWidget(p_item);    
	m_layout->addStretch();
}

void ComponentView::RemoveItems()
{
	while(!m_layout->isEmpty())
	{
		delete m_layout->takeAt(0);
	}
}

