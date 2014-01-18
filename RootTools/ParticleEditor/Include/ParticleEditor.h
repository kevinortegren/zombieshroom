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

	//MEMBERS
	bool m_running;
	ECS::World* m_world;
	RootEngine::GameSharedContext* m_context;
	std::vector<ECS::Entity*> m_emitterEntities;
	StatWidget* m_statWidget;
	ParticleTab* m_particleTab;
	int m_selectedEmitterIndex;

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
	void SizeStartChanged(double p_val);
	void SizeEndChanged(double p_val);

	
};

