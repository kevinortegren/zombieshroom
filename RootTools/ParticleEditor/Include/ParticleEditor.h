#pragma once

#include <QtWidgets/QMainWindow>
#include <RootTools/ParticleEditor/GeneratedFiles/ui_ParticleEditor.h>
#include <QtWidgets/QMessageBox>
#include <QtCore/QString>
#include <QtGui/QCloseEvent>
#include <RootSystems/Include/Components.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <Utility/ECS/Include/World.h>
#include <vector>
#include <RootTools/ParticleEditor/Include/StatWidget.h>
#include <RootTools/ParticleEditor/Include/ParticleTab.h>

class ParticleEditor : public QMainWindow
{
	Q_OBJECT

public:
	ParticleEditor(QWidget *parent = 0);
	~ParticleEditor();
	//SETTERS
	void SetWorld(ECS::World* p_world);
	void SetContext(RootEngine::GameSharedContext* p_context);

	//GETTERS

	//METHODS
	bool CheckExit();
	void Update(float p_dt);

	//MEMBERS
	Ui::ParticleEditorClass ui;
private:
	//METHODS
	void closeEvent(QCloseEvent *event);
	void DrawGrid(int p_spacing);
	//MEMBERS
	bool m_running;
	ECS::World* m_world;
	RootEngine::GameSharedContext* m_context;
	std::vector<ECS::Entity*> m_emitterEntities;
	StatWidget* m_statWidget;
	ParticleTab* m_particleTab;
	int m_selectedEmitterIndex;
	bool m_showGrid;

private slots:
	void MenuNew();
	void MenuOpen();
	void MenuSave();
	void ManuSaveAs();
	void MenuExit();
	void MenuViewStats();
	void MenuHelpAbout();
	void MenuCreateEmitter();
	void DeleteEmitter();
	void RenameEmitter();
	void EmitterSelected(QListWidgetItem*);
	void PositionXChanged(double p_x);
	void PositionYChanged(double p_y);
	void PositionZChanged(double p_z);
	void SizeMinXChanged(double p_val);
	void SizeMinYChanged(double p_val);
	void SizeMaxXChanged(double p_val);
	void SizeMaxYChanged(double p_val);
	void SizeEndXChanged(double p_val);
	void SizeEndYChanged(double p_val);
	void LifeTimeMinChanged(double p_val);
	void LifeTimeMaxChanged(double p_val);
	void GravityXChanged(double p_val);
	void GravityYChanged(double p_val);
	void GravityZChanged(double p_val);
	void DirectionXChanged(double p_val);
	void DirectionYChanged(double p_val);
	void DirectionZChanged(double p_val);
	void SpeedMinChanged(double p_val);
	void SpeedMaxChanged(double p_val);
	void SpreadChanged(double p_val);
	void SpawnTimeChanged(double p_val);
	void ColorRChanged(double p_val);
	void ColorGChanged(double p_val);
	void ColorBChanged(double p_val);
	void ColorEndRChanged(double p_val);
	void ColorEndGChanged(double p_val);
	void ColorEndBChanged(double p_val);
	void GridToggled();
};

