#include <RootTools/Treenity/Include/ComponentViewHeader.h>

ComponentViewHeader::ComponentViewHeader(const QString& p_name, QWidget* p_parent)
	: QFrame(p_parent)
{
	ui.setupUi(this);
	ui.label_componentName->setText(p_name);

	m_contextMenu = new QMenu(this);
	QAction* remove = new QAction("Remove Component", this);
	connect(remove, SIGNAL(triggered()), this, SLOT(MenuActionRemove()));
	m_contextMenu->addAction(remove);

	ui.toolButton_remove->setMenu(m_contextMenu);
	ui.toolButton_remove->setPopupMode(QToolButton::InstantPopup);

	connect(ui.toolButton_collapse, SIGNAL(pressed()), this, SLOT(CollapseButtonPress()));	
	//connect(ui.toolButton_remove, SIGNAL(pressed()), this, SLOT(RemoveButtonPress()));

	//setFrameStyle(QFrame::StyledPanel);
	//setLineWidth(1);
}

void ComponentViewHeader::CollapseButtonPress()
{
	if(ui.toolButton_collapse->arrowType() == Qt::ArrowType::RightArrow)
	{
		ui.toolButton_collapse->setArrowType(Qt::ArrowType::DownArrow);
		emit showView();
	}
	else if(ui.toolButton_collapse->arrowType() == Qt::ArrowType::DownArrow)
	{
		ui.toolButton_collapse->setArrowType(Qt::ArrowType::RightArrow);
		emit hideView();
	}	
}

void ComponentViewHeader::RemoveButtonPress()
{
	m_contextMenu->popup(this->mapToGlobal(ui.toolButton_remove->pos() + QPoint(0, ui.toolButton_remove->geometry().height())));
}

void ComponentViewHeader::MenuActionRemove()
{
	emit remove();
}

void ComponentViewHeader::SetArrowShape(Qt::ArrowType p_type)
{
	ui.toolButton_collapse->setArrowType(p_type);
}