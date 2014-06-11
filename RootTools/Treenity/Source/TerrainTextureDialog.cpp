#include <RootTools/Treenity/Include/TerrainTextureDialog.h>


TerrainTextureDialog::TerrainTextureDialog( QWidget* p_parent /*= 0*/ ): QWidget(p_parent)
{
	ui.setupUi(this);

	hide();

	m_pieMenu = new PieMenu(nullptr);

	//Connect items from UI to methods
	connect(ui.spinBox_size,				SIGNAL(valueChanged(int)),	this, SLOT(SetSize(int)));
	connect(ui.spinBox_opacity,				SIGNAL(valueChanged(int)),	this, SLOT(SetOpacity(int)));
	connect(ui.horizontalSlider_opacity,	SIGNAL(sliderMoved(int)),	this, SLOT(SetOpacity(int)));
	connect(ui.pushButton,					SIGNAL(clicked()),			this, SLOT(RedClicked()));
	connect(ui.pushButton_2,				SIGNAL(clicked()),			this, SLOT(GreenClicked()));
	connect(ui.pushButton_3,				SIGNAL(clicked()),			this, SLOT(BlueClicked()));

	PiePiece* tempPiece;
	tempPiece = m_pieMenu->addPiece("Resources/blueSquare.png", "Blue Texture");
	connect(tempPiece, SIGNAL(clicked()), this, SLOT(BlueClicked()));
	tempPiece = m_pieMenu->addPiece("Resources/greenSquare.png", "Green Texture");
	connect(tempPiece, SIGNAL(clicked()), this, SLOT(GreenClicked()));
	tempPiece = m_pieMenu->addPiece("Resources/redSquare.png", "Red Texture");
	connect(tempPiece, SIGNAL(clicked()), this, SLOT(RedClicked()));
}

TerrainTextureDialog::~TerrainTextureDialog()
{
	delete m_pieMenu;
}

void TerrainTextureDialog::DisplayData()
{
	ui.spinBox_size->blockSignals(true);
	ui.spinBox_opacity->blockSignals(true);	
	ui.pushButton->blockSignals(true);
	ui.pushButton_2->blockSignals(true);
	ui.pushButton_3->blockSignals(true);
	ui.horizontalSlider_opacity->blockSignals(true);

	ui.spinBox_size->setValue(m_editorInterface->GetTerrainTextureBrush()->GetSize());
	ui.spinBox_opacity->setValue((int)(100 * m_editorInterface->GetTerrainTextureBrush()->GetOpacity()));
	ui.horizontalSlider_opacity->setValue((int)(100 * m_editorInterface->GetTerrainTextureBrush()->GetOpacity()));

	ui.spinBox_size->blockSignals(false);
	ui.spinBox_opacity->blockSignals(false);	
	ui.pushButton->blockSignals(false);
	ui.pushButton_2->blockSignals(false);
	ui.pushButton_3->blockSignals(false);
	ui.horizontalSlider_opacity->blockSignals(false);
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
	
	ui.horizontalSlider_opacity->blockSignals(true);
	ui.spinBox_opacity->blockSignals(true);

	ui.horizontalSlider_opacity->setValue(p_val);
	ui.spinBox_opacity->setValue(p_val);

	ui.horizontalSlider_opacity->blockSignals(false);
	ui.spinBox_opacity->blockSignals(false);
}

void TerrainTextureDialog::RedClicked()
{
	ui.pushButton->blockSignals(true);
	ui.pushButton->setChecked(true);
	m_editorInterface->GetTerrainTextureBrush()->SetChannel(0);
	ui.pushButton->blockSignals(false);

}

void TerrainTextureDialog::GreenClicked()
{

	ui.pushButton_2->blockSignals(true);
	ui.pushButton_2->setChecked(true);
	m_editorInterface->GetTerrainTextureBrush()->SetChannel(1);
	ui.pushButton_2->blockSignals(false);

}

void TerrainTextureDialog::BlueClicked()
{

	ui.pushButton_3->blockSignals(true);
	ui.pushButton_3->setChecked(true);
	m_editorInterface->GetTerrainTextureBrush()->SetChannel(2);
	ui.pushButton_3->blockSignals(false);
}

bool TerrainTextureDialog::MouseClick()
{
	if(isVisible())
	{
		m_pieMenu->showMenu();

		return true;
	}

	return false;
}

bool TerrainTextureDialog::MouseRelease()
{
	if(isVisible())
	{
		if (m_pieMenu->canSee())
			m_pieMenu->closeMenu();
		return true;
	}

	return false;
}

bool TerrainTextureDialog::MouseScroll( int p_scrollDelta )
{
	if(isVisible())
	{
		ui.spinBox_size->setValue(ui.spinBox_size->value() + p_scrollDelta / 100);
		return true;
	}

	return false;
}