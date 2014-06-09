#pragma once

//Qt includes
#include <QWidget>
#include <QDockWidget>
#include <RootTools/Treenity/GeneratedFiles/ui_TerrainTextureTool.h>

//Engine includes
#include <RootTools/Treenity/Include/EngineInterface.h>
#include <RootTools/Treenity/Include/EditorInterface.h>

//Other includes
#include <sstream>

class TerrainTextureDialog : public QDockWidget
{
	Q_OBJECT
public:
	TerrainTextureDialog(QWidget* p_parent = 0);
	~TerrainTextureDialog();

	void SetEngineInterface(EngineInterface* p_engineInterface);
	void SetEditorInterface(EditorInterface* p_editorInterFace);

public slots:
	void Show();

private:
	EngineInterface* m_engineInterface;
	EditorInterface* m_editorInterface;

	Ui::TerrainTextureTool ui;

private slots:
	
	void DisplayData();
	void SetSize(int p_val);
	void SetOpacity(int p_val);
	void RedClicked();
	void GreenClicked();
	void BlueClicked();
};