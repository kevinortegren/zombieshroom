#include <RootTools/Treenity/Include/ComponentViewHeader.h>

ComponentViewHeader::ComponentViewHeader(const QString& p_name, QWidget* p_parent)
	: QWidget(p_parent)
{
	ui.setupUi(this);
	ui.label_componentName->setText(p_name);

	ui.toolButton_collapse->setArrowType(Qt::ArrowType::UpArrow);

	m_contextMenu = new QMenu(this);
	QAction* remove = new QAction("Remove Component", this);
	connect(remove, SIGNAL(triggered()), this, SLOT(MenuActionRemove()));
	m_contextMenu->addAction(remove);

	ui.toolButton_remove->setMenu(m_contextMenu);
	ui.toolButton_remove->setPopupMode(QToolButton::InstantPopup);

	connect(ui.toolButton_collapse, SIGNAL(pressed()), this, SLOT(CollapseButtonPress()));
	//connect(ui.toolButton_remove,	SIGNAL(pressed()), this, SLOT(RemoveButtonPress()));
	//connect(m_contextMenu, SIGNAL(QMenu::aboutToShow()), this, SLOT(Poulate()));
}

void ComponentViewHeader::CollapseButtonPress()
{
	if(ui.toolButton_collapse->arrowType() == Qt::ArrowType::DownArrow)
	{
		ui.toolButton_collapse->setArrowType(Qt::ArrowType::UpArrow);
		emit showView();
	}
	else if(ui.toolButton_collapse->arrowType() == Qt::ArrowType::UpArrow)
	{
		ui.toolButton_collapse->setArrowType(Qt::ArrowType::DownArrow);
		emit hideView();
	}	
}

void ComponentViewHeader::RemoveButtonPress()
{
	m_contextMenu->exec();
}

void ComponentViewHeader::MenuActionRemove()
{
	emit remove();
}
