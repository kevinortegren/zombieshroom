#pragma once

//Qt includes
#include <QWidget>
#include <RootTools/Treenity/GeneratedFiles/ui_TerrainTool.h>

//Engine includes
#include <RootTools/Treenity/Include/EngineInterface.h>
#include <RootTools/Treenity/Include/EditorInterface.h>

//Other includes
#include <sstream>

#include <RootTools/Treenity/Include/PieMenu.h>

class TerrainDialog : public QWidget
{
	Q_OBJECT
public:
	TerrainDialog(QWidget* p_parent = 0);
	~TerrainDialog();

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

	Ui::TerrainTool ui;

	PieMenu* m_pieMenu;

private slots:
	
	void DisplayData();
	void SetSize(int p_val);
	void SetStrength(double p_val);
	void SetAutoSmooth(bool p_val);
	void SetCircleStyle();
	void SetGridStyle();
	void SetOptions(int p_val);
	void SmoothAll();

	void PieButton1();
	void PieButton2();
	void PieButton3();
};