#include <RootTools/Treenity/Include/ComponentView.h>

ComponentView::ComponentView( QWidget* p_parent /*= 0*/ ) : QWidget(p_parent), m_layout(new QVBoxLayout())
{
	setLayout(m_layout);
	m_layout->setContentsMargins(0, 0, 0, 0);
	m_layout->setSizeConstraint(QLayout::SetNoConstraint);
}

ComponentView::~ComponentView()
{

}

void ComponentView::AddItem( ComponentViewItem* p_item )
{
	//Don't add duplicate views
	for (int i = 0; i < m_layout->count(); ++i)
	{
		ComponentViewItem* temp = (ComponentViewItem*)m_layout->itemAt(i)->widget();
		if(temp)
		{
			if (temp->GetItem()->GetComponentName() == p_item->GetItem()->GetComponentName())
			{
				return;
			}
		}
	}

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
	QLayoutItem* item;
	while ( ( item = m_layout->takeAt(0) ) != nullptr )
	{
		delete item->widget();
		delete item;
	}
}

int ComponentView::Count()
{
	return m_layout->count();
}

ComponentViewItem* ComponentView::GetItemByName( const QString& p_name )
{
	for (int i = 0; i < m_layout->count(); ++i)
	{
		ComponentViewItem* temp = (ComponentViewItem*)m_layout->itemAt(i)->widget();
		if (temp->GetItem()->GetComponentName() == p_name)
		{
			return temp;
		}
	}

	return nullptr;
}

void ComponentView::RemoveItem( const QString& p_name )
{
	for (int i = 0; i < m_layout->count(); ++i)
	{
		QLayoutItem* item = m_layout->itemAt(i);
		ComponentViewItem* temp = (ComponentViewItem*)item->widget();
		if (temp->GetItem()->GetComponentName() == p_name)
		{
			m_layout->removeItem(item);
			delete temp;
			delete item;
			break;
		}
	}
}

