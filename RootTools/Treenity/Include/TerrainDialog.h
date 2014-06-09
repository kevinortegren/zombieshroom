#pragma once

//Qt includes
#include <QWidget>
#include <QDockWidget>
#include <RootTools/Treenity/GeneratedFiles/ui_TerrainTool.h>

//Engine includes
#include <RootTools/Treenity/Include/EngineInterface.h>
#include <RootTools/Treenity/Include/EditorInterface.h>

//Other includes
#include <sstream>

class TerrainDialog : public QDockWidget
{
	Q_OBJECT
public:
	TerrainDialog(QWidget* p_parent = 0);
	~TerrainDialog();

	void SetEngineInterface(EngineInterface* p_engineInterface);
	void SetEditorInterface(EditorInterface* p_editorInterFace);

public slots:
	void Show();

private:
	EngineInterface* m_engineInterface;
	EditorInterface* m_editorInterface;

	Ui::TerrainTool ui;

private slots:
	
	void DisplayData();
	void SetSize(int p_val);
	void SetStrength(double p_val);
	void SetAutoSmooth(bool p_val);
	void SetStyle(int p_val);
	void SetOptions(int p_val);
	void SmoothAll();
};