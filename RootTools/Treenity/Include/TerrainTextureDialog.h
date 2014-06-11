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

#include <RootTools/Treenity/Include/PieMenu.h>

class TerrainTextureDialog : public QWidget
{
	Q_OBJECT
public:
	TerrainTextureDialog(QWidget* p_parent = 0);
	~TerrainTextureDialog();

	void SetEngineInterface(EngineInterface* p_engineInterface);
	void SetEditorInterface(EditorInterface* p_editorInterFace);

	bool MouseClick();
	bool MouseRelease();
	bool MouseScroll(int p_scrollDelta);

public slots:
	void Show();

private:
	EngineInterface* m_engineInterface;
	EditorInterface* m_editorInterface;

	Ui::TerrainTextureTool ui;

	PieMenu* m_pieMenu;

private slots:
	
	void DisplayData();
	void SetSize(int p_val);
	void SetOpacity(int p_val);
	void RedClicked();
	void GreenClicked();
	void BlueClicked();
};