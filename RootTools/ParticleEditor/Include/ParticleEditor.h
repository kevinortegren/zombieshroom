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
	int CheckRayVsObject(glm::ivec2 p_mousePos, glm::vec3 p_camPos, glm::mat4 p_viewMatrix);
	float CheckRayVsAABB(glm::vec3 p_rayDir, glm::vec3 p_rayOrigin, glm::vec3 p_bound1, glm::vec3 p_bound2);
	void DragEmitter(int p_axis, glm::ivec2 p_mousePos, glm::vec3 p_camPos, glm::mat4 p_viewMatrix);
	glm::vec3 GetSelectedPosition();
	void OpenParticleFile(std::string p_filePath);
	//MEMBERS
	Ui::ParticleEditorClass ui;
private:
	struct AxisBoundingBox
	{
		AxisBoundingBox(){}
		AxisBoundingBox(glm::vec3 p_lower, glm::vec3 p_upper) : m_lower(p_lower), m_upper(p_upper)
		{}
		glm::vec3 m_lower;
		glm::vec3 m_upper;
	};
	struct PointOnPlane
	{
		PointOnPlane(){}
		PointOnPlane(glm::vec3 p_point, bool p_hit) : Point(p_point), Hit(p_hit){}
		
		glm::vec3 Point;
		bool Hit;
	};
	//METHODS
	void closeEvent(QCloseEvent *event);
	void DrawGridX(float p_spacing, glm::vec4 p_color);
	void DrawPositionAxis();
	void ClearScene();
	void ShowMessageBox(QString p_msg);
	void ExportParticle(QString p_fullFilePath);
	bool IsSaved();
	void Changed();
	void Saved();
	void ResetTemplates();
	float GetDragOffset(float p_pointOnAxis, float p_pointOfEmitter);
	PointOnPlane GetPointOnPlane(glm::vec3 p_camPos, glm::vec3 p_worldCamPos, glm::vec3 p_rayDir);
	bool CalculateMaxParticles();
	void OpenParticleFileQ(QString p_filePath);
	QMessageBox::StandardButton SaveWarningDialog();

	//MEMBERS
	ECS::World* m_world;
	RootEngine::GameSharedContext* m_context;
	std::vector<ECS::Entity*> m_emitterEntities;
	ParticleTab* m_particleTab;
	QFileSystemModel* m_fileSystemModel;
	QFileSystemModel* m_fileSystemModelModel;
	QFileSystemModel* m_fileSystemModelModelTex;
	QtColorTriangle* m_colorTriangle;
	QtColorTriangle* m_colorEndTriangle;
	std::string m_workingDirectory;
	RootForce::Transform* m_aimingDeviceTransform;
	RootForce::Renderable* m_model;
	RootForce::Transform* m_modelTrans;

	int m_selectedEmitterIndex;
	int m_selectedEntityIndex;

	int m_materialIndex;
	int m_samples;

	bool m_showGrid;
	bool m_showAxis;
	bool m_running;
	bool m_firstDrag;

	float m_dragOffset;
	float m_gridSpace;
	float m_collectedTime;

	std::string m_savePath;

	bool m_changed;

	glm::mat4 m_inverseProjection;

	AxisBoundingBox m_axisAABB[3];

private slots:
	void MenuNew();
	void MenuOpen();
	void MenuSave();
	void ManuSaveAs();
	void MenuExit();
	void MenuViewStats();
	void MenuHelpAbout();
	void MenuHelpHelp();
	void MenuViewColorTriangle();
	void MenuViewColorEndTriangle();
	void NewEmitter();
	void OpenNewEmitterWidget();
	void OpenNewRenameWidget();
	void DeleteEmitter();
	void RenameEmitter();
	void DuplicateEmitter();
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
	void colorAlphaSliderChanged(int p_val);
	void endColorAlphaSliderChanged(int p_val);
	void BlendingChanged(int p_val);
	void RelativeChanged(int p_val);
	void OrbitRadiusChanged(double p_val);
	void OrbitSpeedChanged(double p_val);
	void SpreadSliderChanged(int p_val);
	void ModelDoubleClicked(const QModelIndex& p_index);
	void ModelTexDoubleClicked(const QModelIndex& p_index);
	void BackgroundColorChanged(int p_value);
	void RemoveObjectButton();
	void RotationSpeedMinChanged(double p_val);
	void RotationSpeedMaxChanged(double p_val);
	void MaxPerFrameChanged(double p_val);
	void ResetParticleSystem();
};

