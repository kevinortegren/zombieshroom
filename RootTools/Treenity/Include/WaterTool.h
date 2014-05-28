#pragma once

//Qt includes
#include <QWidget>
#include <QDockWidget>
#include <QColorDialog>
#include <RootTools/Treenity/GeneratedFiles/ui_WaterTool.h>

//Engine includes
#include <RootTools/Treenity/Include/EngineInterface.h>

//Other includes
#include <sstream>

class WaterTool : public QDockWidget
{
	Q_OBJECT
public:
	WaterTool(QWidget* p_parent = 0);
	~WaterTool();

	void SetEngineInterface(EngineInterface* p_engineInterface);

public slots:
	void Show();

private:
	EngineInterface* m_engineInterface;
	QColorDialog* m_colorDialog;
	QColor m_waterColor;

	Ui::WaterUI ui;


private slots:
	
	void SetWaterSpeed(double p_speed);
	void SetWaterDepth(double p_depth);
	void SetWaterRefractions(bool p_active);
	void SetWaterReflections(bool p_active);
	void SetWaterNormals(bool p_active);
	void SetWaterPause(bool p_active);
	void SetWaterColor(const QColor& p_color);
	void ResetWater();
	void InitWater();
	void DisplayData();
	void OpenColorDialog();
};