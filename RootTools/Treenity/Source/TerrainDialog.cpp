#include <RootTools/Treenity/Include/TerrainDialog.h>

TerrainDialog::TerrainDialog( QWidget* p_parent /*= 0*/ ): QWidget(p_parent)
{
	ui.setupUi(this);

	hide();

	m_pieMenu = new PieMenu(nullptr);

	//Connect items from UI to methods
	connect(ui.spinBox_size,				SIGNAL(valueChanged(int)),			this, SLOT(SetSize(int)));
	connect(ui.doubleSpinBox_strength,		SIGNAL(valueChanged(double)),		this, SLOT(SetStrength(double)));
	connect(ui.checkBox_autosmooth,			SIGNAL(clicked(bool)),				this, SLOT(SetAutoSmooth(bool)));
	connect(ui.buttonCircle,				SIGNAL(clicked()),					this, SLOT(SetCircleStyle()));
	connect(ui.buttonGrid,					SIGNAL(clicked()),					this, SLOT(SetGridStyle()));
	connect(ui.comboBox_options,			SIGNAL(currentIndexChanged(int)),	this, SLOT(SetOptions(int)));

	PiePiece* tempPiece;
	tempPiece = m_pieMenu->addPiece("Resources/defaultBrush.png", "Default");
	connect(tempPiece, SIGNAL(clicked()), this, SLOT(PieButton1()));
	tempPiece = m_pieMenu->addPiece("Resources/plateauBrush.png", "Plateau");
	connect(tempPiece, SIGNAL(clicked()), this, SLOT(PieButton2()));
	tempPiece = m_pieMenu->addPiece("Resources/smoothOnlyBrush.png", "Smooth only");
	connect(tempPiece, SIGNAL(clicked()), this, SLOT(PieButton3()));
}

TerrainDialog::~TerrainDialog()
{

}

void TerrainDialog::DisplayData()
{
	ui.spinBox_size->blockSignals(true);
	ui.doubleSpinBox_strength->blockSignals(true);	
	ui.buttonCircle->blockSignals(true);
	ui.buttonGrid->blockSignals(true);
	ui.comboBox_options->blockSignals(true);

	ui.spinBox_size->setValue(m_editorInterface->GetTerrainGeometryBrush()->GetSize());
	ui.doubleSpinBox_strength->setValue(m_editorInterface->GetTerrainGeometryBrush()->GetStrength());		
	ui.checkBox_autosmooth->setChecked(m_editorInterface->GetTerrainGeometryBrush()->GetAutoSmooth());					
	int matID = m_editorInterface->GetTerrainGeometryBrush()->GetActiveMaterialIndex();
	if(matID == 0)
	{
		ui.buttonCircle->setChecked(true);
		ui.buttonGrid->setChecked(false);
	}
	else if(matID == 1)
	{
		ui.buttonGrid->setChecked(true);
		ui.buttonCircle->setChecked(false);
	}

	ui.comboBox_options->setCurrentIndex(m_editorInterface->GetTerrainGeometryBrush()->GetOptionIndex());

	ui.spinBox_size->blockSignals(false);
	ui.doubleSpinBox_strength->blockSignals(false);
	ui.buttonCircle->blockSignals(false);
	ui.buttonGrid->blockSignals(false);
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

void TerrainDialog::SetOptions( int p_val )
{
	m_editorInterface->GetTerrainGeometryBrush()->SetOptionIndex(p_val);

	m_editorInterface->GetTerrainGeometryBrush()->SetSmoothOnly(false);
	m_editorInterface->GetTerrainGeometryBrush()->SetPlateau(false);

	if(p_val == 1)
	{
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

void TerrainDialog::SetCircleStyle()
{
	m_editorInterface->GetTerrainGeometryBrush()->SetActiveMaterial(0);
}

void TerrainDialog::SetGridStyle()
{
	m_editorInterface->GetTerrainGeometryBrush()->SetActiveMaterial(1);
}

bool TerrainDialog::MouseClick()
{
	if(isVisible())
	{
		m_pieMenu->showMenu();

		return true;
	}

	return false;
}

bool TerrainDialog::MouseRelease()
{
	if(isVisible())
	{
		if (m_pieMenu->canSee())
			m_pieMenu->closeMenu();
		return true;
	}

	return false;
}

bool TerrainDialog::MouseScroll( int p_scrollDelta )
{
	if(isVisible())
	{
		ui.spinBox_size->setValue(ui.spinBox_size->value() + p_scrollDelta / 100);
		return true;
	}

	return false;
}

void TerrainDialog::PieButton1()
{
	ui.comboBox_options->setCurrentIndex(0);
}

void TerrainDialog::PieButton2()
{
	ui.comboBox_options->setCurrentIndex(2);

}

void TerrainDialog::PieButton3()
{
	ui.comboBox_options->setCurrentIndex(3);

}
