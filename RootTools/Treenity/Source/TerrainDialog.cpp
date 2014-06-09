#include <RootTools/Treenity/Include/TerrainDialog.h>

TerrainDialog::TerrainDialog( QWidget* p_parent /*= 0*/ ): QDockWidget(p_parent)
{
	QWidget* contents = new QWidget();

	ui.setupUi(contents);

	setAllowedAreas(Qt::BottomDockWidgetArea|Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
	setGeometry(ui.gridLayout->geometry());
	setWindowTitle("Terrain sculpt");
	setFloating(true);
	hide();

	setWidget(contents);

	//Connect items from UI to methods
	connect(ui.spinBox_size,				SIGNAL(valueChanged(int)),			this, SLOT(SetSize(int)));
	connect(ui.doubleSpinBox_strength,		SIGNAL(valueChanged(double)),		this, SLOT(SetStrength(double)));
	connect(ui.checkBox_autosmooth,			SIGNAL(clicked(bool)),				this, SLOT(SetAutoSmooth(bool)));
	connect(ui.comboBox_brushStyle,			SIGNAL(currentIndexChanged(int)),	this, SLOT(SetStyle(int)));
	connect(ui.comboBox_options,			SIGNAL(currentIndexChanged(int)),	this, SLOT(SetOptions(int)));

}

TerrainDialog::~TerrainDialog()
{

}

void TerrainDialog::DisplayData()
{
	ui.spinBox_size->blockSignals(true);
	ui.doubleSpinBox_strength->blockSignals(true);	
	ui.comboBox_brushStyle->blockSignals(true);
	ui.comboBox_options->blockSignals(true);

	ui.spinBox_size->setValue(m_editorInterface->GetTerrainGeometryBrush()->GetSize());
	ui.doubleSpinBox_strength->setValue(m_editorInterface->GetTerrainGeometryBrush()->GetStrength());		
	ui.checkBox_autosmooth->setChecked(m_editorInterface->GetTerrainGeometryBrush()->GetAutoSmooth());					
	ui.comboBox_brushStyle->setCurrentIndex(m_editorInterface->GetTerrainGeometryBrush()->GetActiveMaterialIndex());
	ui.comboBox_options->setCurrentIndex(m_editorInterface->GetTerrainGeometryBrush()->GetOptionIndex());

	ui.spinBox_size->blockSignals(false);
	ui.doubleSpinBox_strength->blockSignals(false);
	ui.comboBox_brushStyle->blockSignals(false);
	ui.comboBox_options->blockSignals(false);
}

void TerrainDialog::SetEngineInterface( EngineInterface* p_engineInterface )
{
	m_engineInterface = p_engineInterface;
}

void TerrainDialog::SetEditorInterface( EditorInterface* p_editorInterFace )
{
	m_editorInterface = p_editorInterFace;
}

void TerrainDialog::Show()
{
	DisplayData();
	show();
}

void TerrainDialog::SetSize( int p_val )
{
	m_editorInterface->GetTerrainGeometryBrush()->SetSize(p_val);
}

void TerrainDialog::SetStrength( double p_val )
{
	m_editorInterface->GetTerrainGeometryBrush()->SetStrength(p_val);
}

void TerrainDialog::SetAutoSmooth( bool p_val )
{
	m_editorInterface->GetTerrainGeometryBrush()->SetAutoSmooth(p_val);
}

void TerrainDialog::SmoothAll()
{
	//TODO
}

void TerrainDialog::SetStyle( int p_val )
{
	m_editorInterface->GetTerrainGeometryBrush()->SetActiveMaterial(p_val);
}

void TerrainDialog::SetOptions( int p_val )
{
	m_editorInterface->GetTerrainGeometryBrush()->SetOptionIndex(p_val);

	m_editorInterface->GetTerrainGeometryBrush()->SetFlat(false);
	m_editorInterface->GetTerrainGeometryBrush()->SetSmoothOnly(false);
	m_editorInterface->GetTerrainGeometryBrush()->SetPlateau(false);

	if(p_val == 1)
	{
		m_editorInterface->GetTerrainGeometryBrush()->SetFlat(true);
	}
	else if (p_val == 2)
	{
		m_editorInterface->GetTerrainGeometryBrush()->SetPlateau(true);
	}
	else if (p_val == 3)
	{
		m_editorInterface->GetTerrainGeometryBrush()->SetSmoothOnly(true);
	}
	
}
