#include <RootTools/Treenity/Include/WaterTool.h>

WaterTool::WaterTool( QWidget* p_parent /*= 0*/ ): QDockWidget(p_parent)
{
	QWidget* contents = new QWidget();

	ui.setupUi(contents);

	setAllowedAreas(Qt::BottomDockWidgetArea|Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
	setGeometry(ui.gridLayout->geometry());
	setWindowTitle("Water settings");
	setFloating(true);
	hide();

	setWidget(contents);

	m_colorDialog = new QColorDialog(this);
	m_colorDialog->setWindowModality(Qt::NonModal);

	connect(m_colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(SetWaterColor(const QColor&)));


	//Connect items from UI to methods
	connect(ui.doubleSpinBox_waterSpeed,	SIGNAL(valueChanged(double)),	this, SLOT(SetWaterSpeed(double)));
	connect(ui.doubleSpinBox_waterDepth,	SIGNAL(valueChanged(double)),	this, SLOT(SetWaterDepth(double)));
	connect(ui.checkBox_waterReflections,	SIGNAL(clicked(bool)),			this, SLOT(SetWaterReflections(bool)));
	connect(ui.checkBox_waterRefractions,	SIGNAL(clicked(bool)),			this, SLOT(SetWaterRefractions(bool)));
	connect(ui.checkBox_waterNormals,		SIGNAL(clicked(bool)),			this, SLOT(SetWaterNormals(bool)));
	connect(ui.checkBox_waterPause,			SIGNAL(clicked(bool)),			this, SLOT(SetWaterPause(bool)));
	connect(ui.pushButton_waterColor,		SIGNAL(clicked()),				this, SLOT(OpenColorDialog()));
	connect(ui.pushButton_initWater,		SIGNAL(clicked()),				this, SLOT(InitWater()));
	connect(ui.pushButton_resetWater,		SIGNAL(clicked()),				this, SLOT(ResetWater()));

	
}

WaterTool::~WaterTool()
{

}

void WaterTool::DisplayData()
{
	ui.doubleSpinBox_waterSpeed->blockSignals(true);
	ui.doubleSpinBox_waterDepth->blockSignals(true);

	ui.doubleSpinBox_waterSpeed->setValue((double)m_engineInterface->GetWaterSpeed());
	ui.doubleSpinBox_waterDepth->setValue((double)m_engineInterface->GetWaterDepth());
	ui.checkBox_waterReflections->setChecked(m_engineInterface->GetWaterReflections());
	ui.checkBox_waterRefractions->setChecked(m_engineInterface->GetWaterRefractions());
	ui.checkBox_waterNormals->setChecked(m_engineInterface->GetWaterNormals());
	ui.checkBox_waterPause->setChecked(m_engineInterface->GetWaterPaused());

	glm::vec3 waterCol = m_engineInterface->GetWaterColor();
	m_waterColor.setRgbF(waterCol.x, waterCol.y, waterCol.z);
	if(m_waterColor.isValid()) 
	{
		QString qss = QString("background-color: %1").arg(m_waterColor.name());
		ui.pushButton_waterColor->setStyleSheet(qss);
	}

	ui.doubleSpinBox_waterSpeed->blockSignals(false);
	ui.doubleSpinBox_waterDepth->blockSignals(false);
}

void WaterTool::SetEngineInterface( EngineInterface* p_engineInterface )
{
	m_engineInterface = p_engineInterface;
}

void WaterTool::SetWaterSpeed( double p_speed )
{
	std::stringstream ss;
	ss << "water speed set " << (float)p_speed;
	m_engineInterface->SetWaterParam(&ss);
}

void WaterTool::SetWaterDepth( double p_depth )
{
	std::stringstream ss;
	ss << "water depth " << (float)p_depth;
	m_engineInterface->SetWaterParam(&ss);
}

void WaterTool::SetWaterRefractions( bool p_active )
{
	std::stringstream ss;
	ss << "water refr";
	m_engineInterface->SetWaterParam(&ss);
}

void WaterTool::SetWaterReflections( bool p_active )
{
	std::stringstream ss;
	ss << "water refl";
	m_engineInterface->SetWaterParam(&ss);
}

void WaterTool::SetWaterNormals( bool p_active )
{
	std::stringstream ss;
	ss << "water norm";
	m_engineInterface->SetWaterParam(&ss);
}

void WaterTool::SetWaterPause(bool p_active)
{
	std::stringstream ss;
	ss << "water pause";
	m_engineInterface->SetWaterParam(&ss);
}

void WaterTool::SetWaterColor(const QColor& p_color)
{
	m_engineInterface->SetWaterColor(glm::vec3(p_color.redF(), p_color.greenF(), p_color.blueF()));
	m_waterColor = p_color;
	if(p_color.isValid()) 
	{
		QString qss = QString("background-color: %1").arg(p_color.name());
		ui.pushButton_waterColor->setStyleSheet(qss);
	}
}

void WaterTool::ResetWater()
{
	std::stringstream ss;
	ss << "water reset";
	m_engineInterface->SetWaterParam(&ss);
}

void WaterTool::InitWater()
{
	std::stringstream ss;
	ss << "water init";
	m_engineInterface->SetWaterParam(&ss);
}

void WaterTool::Show()
{
	DisplayData();
	show();
}

void WaterTool::OpenColorDialog()
{
	m_colorDialog->colorSelected(m_waterColor);
	m_colorDialog->show();
}
