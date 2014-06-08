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
	connect(ui.checkBox_autoSmooth,			SIGNAL(clicked(bool)),				this, SLOT(SetAutoSmooth(bool)));
	connect(ui.checkBox_smoothOnly,			SIGNAL(clicked(bool)),				this, SLOT(SetSmoothOnly(bool)));
	connect(ui.checkBox_flat,				SIGNAL(clicked(bool)),				this, SLOT(SetFlat(bool)));
	connect(ui.comboBox_brushStyle,			SIGNAL(currentIndexChanged(int)),	this, SLOT(SetStyle(int)));
	

}

TerrainDialog::~TerrainDialog()
{

}

void TerrainDialog::DisplayData()
{
	ui.spinBox_size->blockSignals(true);
	ui.doubleSpinBox_strength->blockSignals(true);	
	ui.comboBox_brushStyle->blockSignals(true);

	ui.spinBox_size->setValue(m_editorInterface->GetBrushManager()->GetCurrentBrush()->GetSize());
	ui.doubleSpinBox_strength->setValue(m_editorInterface->GetBrushManager()->GetCurrentBrush()->GetStrength());		
	ui.checkBox_autoSmooth->setChecked(m_editorInterface->GetBrushManager()->GetCurrentBrush()->GetAutoSmooth());			
	ui.checkBox_smoothOnly->setChecked(m_editorInterface->GetBrushManager()->GetCurrentBrush()->GetSmoothOnly());			
	ui.checkBox_flat->setChecked(m_editorInterface->GetBrushManager()->GetCurrentBrush()->GetFlat());			
	ui.comboBox_brushStyle->setCurrentIndex(m_editorInterface->GetBrushManager()->GetActiveMaterialIndex());

	ui.spinBox_size->blockSignals(false);
	ui.doubleSpinBox_strength->blockSignals(false);
		ui.comboBox_brushStyle->blockSignals(false);
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
	m_editorInterface->GetBrushManager()->GetCurrentBrush()->SetSize(p_val);
}

void TerrainDialog::SetStrength( double p_val )
{
	m_editorInterface->GetBrushManager()->GetCurrentBrush()->SetStrength(p_val);
}

void TerrainDialog::SetAutoSmooth( bool p_val )
{
	m_editorInterface->GetBrushManager()->GetCurrentBrush()->SetAutoSmooth(p_val);
}

void TerrainDialog::SetSmoothOnly( bool p_val )
{
	m_editorInterface->GetBrushManager()->GetCurrentBrush()->SetSmoothOnly(p_val);
}

void TerrainDialog::SetFlat( bool p_val )
{
	m_editorInterface->GetBrushManager()->GetCurrentBrush()->SetFlat(p_val);
}

void TerrainDialog::SmoothAll()
{
	//TODO
}

void TerrainDialog::SetStyle( int p_val )
{
	m_editorInterface->GetBrushManager()->SetActiveMaterial(p_val);
}
