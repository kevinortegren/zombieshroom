#pragma once

#include <QtWidgets/QMainWindow>
#include <RootTools/ParticleEditor/GeneratedFiles/ui_ParticleEditor.h>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFileSystemModel>
#include <QtCore/QString>
#include <QtGui/QCloseEvent>
#include <RootSystems/Include/Components.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <Utility/ECS/Include/World.h>
#include <vector>
#include <RootTools/ParticleEditor/Include/ParticleTab.h>
#include <QtColorTriangle/QtColorTriangle>
#include <QtWidgets/QFileDialog>
#include <QtCore/QTextStream>
#include <yaml-cpp\yaml.h>
#include <RootForce/Include/ComponentImporter.h>


class ParticleEditor : public QMainWindow
{
	Q_OBJECT

public:
	ParticleEditor(QWidget *parent = 0);
	~ParticleEditor();
	//SETTERS
	void SetWorld(ECS::World* p_world);
	void SetContext(RootEngine::GameSharedContext* p_context);
	void SetWorkingDirectory(std::string p_dir);
	void SetAimingDevice(ECS::Entity* p_aimingDevice);
	void SetLookAtSpinBox(glm::vec3 p_pos);
	//GETTERS

	//METHODS
	bool CheckExit();
	void Update(float p_dt);
	void Init();
	void ConnectSignalsAndSlots();

	//MEMBERS
	Ui::ParticleEditorClass ui;
private:
	//METHODS
	void closeEvent(QCloseEvent *event);
	void DrawGridX(float p_spacing, glm::vec4 p_color);
	
	void ShowMessageBox(QString p_msg);

	//MEMBERS
	ECS::World* m_world;
	RootEngine::GameSharedContext* m_context;
	std::vector<ECS::Entity*> m_emitterEntities;
	ParticleTab* m_particleTab;
	QFileSystemModel* m_fileSystemModel;
	QtColorTriangle* m_colorTriangle;
	QtColorTriangle* m_colorEndTriangle;
	std::string m_workingDirectory;
	RootForce::Transform* m_aimingDeviceTransform;

	int m_selectedEmitterIndex;
	int m_selectedEntityIndex;

	int m_materialIndex;
	int m_samples;

	bool m_showGrid;
	bool m_running;

	float m_gridSpace;
	float m_collectedTime;

private slots:
	void MenuNew();
	void MenuOpen();
	void MenuSave();
	void ManuSaveAs();
	void MenuExit();
	void MenuViewStats();
	void MenuHelpAbout();
	void MenuViewColorTriangle();
	void MenuViewColorEndTriangle();
	void NewEmitter();
	void DeleteEmitter();
	void RenameEmitter();
	void EmitterSelected(QListWidgetItem*);
	void PositionXChanged(double p_x);
	void PositionYChanged(double p_y);
	void PositionZChanged(double p_z);
	void SizeMinXChanged(double p_val);
	void SizeMaxXChanged(double p_val);
	void SizeEndXChanged(double p_val);
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
	void ColorChanged(const QColor& p_val);
	void ColorEndChanged(const QColor& p_val);
	void TextureDoubleClicked(const QModelIndex& p_index);
	void GridToggled();
	void GridSizeChanged(double p_val);
	void FocusButtonClicked();
	void colorAlphaSliderChanged(int p_val);
	void endColorAlphaSliderChanged(int p_val);
};

