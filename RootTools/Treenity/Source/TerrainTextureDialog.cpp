#include <RootTools/Treenity/Include/TerrainTextureDialog.h>

TerrainTextureDialog::TerrainTextureDialog( QWidget* p_parent /*= 0*/ ): QDockWidget(p_parent)
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
	connect(ui.spinBox_size,		SIGNAL(valueChanged(int)),	this, SLOT(SetSize(int)));
	connect(ui.spinBox_opacity,		SIGNAL(valueChanged(int)),	this, SLOT(SetOpacity(int)));
	connect(ui.pushButton,			SIGNAL(clicked()),			this, SLOT(RedClicked()));
	connect(ui.pushButton_2,		SIGNAL(clicked()),			this, SLOT(GreenClicked()));
	connect(ui.pushButton_3,		SIGNAL(clicked()),			this, SLOT(BlueClicked()));
}

TerrainTextureDialog::~TerrainTextureDialog()
{

}

void TerrainTextureDialog::DisplayData()
{
	ui.spinBox_size->blockSignals(true);
	ui.spinBox_opacity->blockSignals(true);	
	ui.pushButton->blockSignals(true);
	ui.pushButton_2->blockSignals(true);
	ui.pushButton_3->blockSignals(true);

	ui.spinBox_size->setValue(m_editorInterface->GetTerrainTextureBrush()->GetSize());
	ui.spinBox_opacity->setValue((int)(100 * m_editorInterface->GetTerrainTextureBrush()->GetOpacity()));

	ui.spinBox_size->blockSignals(false);
	ui.spinBox_opacity->blockSignals(false);	
	ui.pushButton->blockSignals(false);
	ui.pushButton_2->blockSignals(false);
	ui.pushButton_3->blockSignals(false);
}

void TerrainTextureDialog::SetEngineInterface( EngineInterface* p_engineInterface )
{
	m_engineInterface = p_engineInterface;
}

void TerrainTextureDialog::SetEditorInterface( EditorInterface* p_editorInterFace )
{
	m_editorInterface = p_editorInterFace;
}

void TerrainTextureDialog::Show()
{
	DisplayData();
	show();
}

void TerrainTextureDialog::SetSize( int p_val )
{
	m_editorInterface->GetTerrainTextureBrush()->SetSize(p_val);
}

void TerrainTextureDialog::SetOpacity( int p_val )
{
	m_editorInterface->GetTerrainTextureBrush()->SetOpacity((float)p_val / 100.0f);
}

void TerrainTextureDialog::RedClicked()
{
	m_editorInterface->GetTerrainTextureBrush()->SetChannel(0);
}

void TerrainTextureDialog::GreenClicked()
{
	m_editorInterface->GetTerrainTextureBrush()->SetChannel(1);
}

void TerrainTextureDialog::BlueClicked()
{
	m_editorInterface->GetTerrainTextureBrush()->SetChannel(2);
}
