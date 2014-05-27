#include <RootTools/Treenity/Include/ComponentViewHeader.h>

ComponentViewHeader::ComponentViewHeader(const QString& p_name, QWidget* p_parent)
	: QWidget(p_parent)
{
	ui.setupUi(this);

	ui.label_componentName->setText(p_name);

	connect(ui.toolButton_collapse, SIGNAL(pressed()), this, SLOT(CollapseButtonPress()));
	connect(ui.toolButton_remove, SIGNAL(pressed()), this, SLOT(RemoveButtonPress()));
}

void ComponentViewHeader::CollapseButtonPress()
{
	if(ui.toolButton_collapse->arrowType() == Qt::ArrowType::DownArrow)
	{
		ui.toolButton_collapse->setArrowType(Qt::ArrowType::UpArrow);
		emit show();
	}
	else if(ui.toolButton_collapse->arrowType() == Qt::ArrowType::UpArrow)
	{
		ui.toolButton_collapse->setArrowType(Qt::ArrowType::DownArrow);
		emit hide();
	}	
}

void ComponentViewHeader::RemoveButtonPress()
{
	emit remove();
}