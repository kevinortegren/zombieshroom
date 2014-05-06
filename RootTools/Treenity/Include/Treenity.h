#ifndef TREENITY_H
#define TREENITY_H

#include <map>
#include <QtWidgets/QMainWindow>
#include "ui_Treenity.h"
#include <RootTools/Treenity/GeneratedFiles/ui_TransformComponent.h>
#include <RootTools/Treenity/GeneratedFiles/ui_RenderableComponent.h>
#include <Utility/ECS/include/World.h>
#include <RootTools/Treenity/Include/EngineInterface.h>
#include <RootTools/Treenity/Include/EntityOutliner.h>
#include <RootTools/Treenity/Include/ProjectManager.h>
#include <RootTools/Treenity/Include/ComponentView.h>
#include <RootTools/Treenity/Include/ComponentViewItem.h>
class Treenity : public QMainWindow
{
	Q_OBJECT

public:
	Treenity(QWidget *parent = 0);
	~Treenity();

	void SetEngineInterface(EngineInterface* p_engineInterface);
	void SetProjectManager(ProjectManager* p_projectManager);
	void CreateNewScene();

	void UpdateWindowTitle();

	bool IsRunning();
	void closeEvent(QCloseEvent *event);

	// Called by the engine.
	void EntityCreated(ECS::Entity* p_entity);
	void EntityRemoved(ECS::Entity* p_entity);
	void ComponentCreated(ECS::Entity* p_entity, int p_componentType);
	void ComponentRemoved(ECS::Entity* p_entity, int p_componentType);
	void TagAdded(ECS::Entity* p_entity, const std::string& p_tag);
	void TagRemoved(ECS::Entity* p_entity, const std::string& p_tag);
	void EntityAddedToGroup(ECS::Entity* p_entity, const std::string& p_group);
	void EntityRemovedFromGroup(ECS::Entity* p_entity, const std::string& p_group);
	void CreateOpenGLContext();
//protected:
	//void keyPressEvent( QKeyEvent *k );
	//void keyReleaseEvent( QKeyEvent *k);

private:
	// Component meta-data.
	std::map<int, QString> m_componentNames;

	// Project meta-data.
	QString m_currentProjectFile;
	ProjectManager* m_projectManager;
	ECS::Entity* m_selectedEntity;

	EngineInterface* m_engineInterface;

	bool m_running;
	Ui::TreenityClass ui;
	Ui::Transform transformUI;
	Ui::Renderable renderableUI;

	// Utility
	void SelectEntity(ECS::Entity* p_entity);

	// Component property panel
	void SetupUIForComponent(QWidget* p_widget, int p_componentType);
	QWidget* GetComponentToolboxItemByType(int p_componentType);

	ComponentView* m_compView;

private slots:
	void New();
	void OpenProject();
	void Save();
	void SaveAs();
	void CreateEntity();
	void DestroyEntity();
	void RenameEntity();
	void OutlinerSelectEntity();
	void AddRenderable();
	void TransformPositionXChanged(double p_value);
	void TransformPositionYChanged(double p_value);
	void TransformPositionZChanged(double p_value);
	void TransformOrientationXChanged(double p_value);
	void TransformOrientationYChanged(double p_value);
	void TransformOrientationZChanged(double p_value);
	void TransformScaleXChanged(double p_value);
	void TransformScaleYChanged(double p_value);
	void TransformScaleZChanged(double p_value);
};

#endif // TREENITY_H
