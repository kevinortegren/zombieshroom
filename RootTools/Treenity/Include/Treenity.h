#ifndef TREENITY_H
#define TREENITY_H

#include <map>
#include <QtWidgets/QMainWindow>
#include "ui_Treenity.h"
#include <RootTools/Treenity/GeneratedFiles/ui_TransformComponent.h>
#include <RootTools/Treenity/GeneratedFiles/ui_RenderableComponent.h>
#include <RootTools/Treenity/GeneratedFiles/ui_PhysicsComponent.h>
#include <Utility/ECS/include/World.h>
#include <RootTools/Treenity/Include/EngineInterface.h>
#include <RootTools/Treenity/Include/EntityOutliner.h>
#include <RootTools/Treenity/Include/ProjectManager.h>
#include <RootTools/Treenity/Include/ComponentView.h>
#include <RootTools/Treenity/Include/ComponentViewItem.h>
#include <RootTools/Treenity/Include/Components/TransformView.h>
#include <RootTools/Treenity/Include/Components/PhysicsView.h>
#include <RootTools/Treenity/Include/Components/RenderableView.h>
#include <RootTools/Treenity/Include/Components/WaterColliderView.h>
#include <RootTools/Treenity/Include/Components/ScriptView.h>
#include <RootTools/Treenity/Include/Components/CollisionResponderView.h>
#include <RootTools/Treenity/Include/EditorInterface.h>
#include <RootTools/Treenity/Include/AssetManagerWidget.h>
#include <RootTools/Treenity/Include/ToolManager.h>


class Treenity : public QMainWindow, public EditorInterface
{
	Q_OBJECT

public:
	Treenity(QWidget *parent = 0);
	~Treenity();

	void SetEngineInterface(EngineInterface* p_engineInterface);
	void SetProjectManager(ProjectManager* p_projectManager);
	void CreateNewScene();
	void UpdateWindowTitle();

	void InitializeTools(ECS::World* p_world);

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
	Ui::TreenityClass& GetUi();
	//QPoint GetCanvasCenter() const;

	// Implemented from EditorInterface
	void SelectInOutliner(const std::set<ECS::Entity*>& p_entities);
	void Select(ECS::Entity* p_entity);
	void Select(const std::set<ECS::Entity*>& p_entities);
	void AddToSelection(ECS::Entity* p_entity);
	void ClearSelection();
	const std::set<ECS::Entity*>& GetSelection() const;
	void RenameEntity(ECS::Entity* p_entity, const QString& p_name);

	//Called after RootEngine has been loaded from TreenityMain!
	void Init();

	// Called every frame by the main loop.
	void Update(float p_dt);

	ToolManager m_toolManager;

private:

	// Component meta-data.
	std::map<int, QString> m_componentNames;

	// Project meta-data.
	QString m_currentProjectFile;
	ProjectManager* m_projectManager;
	std::set<ECS::Entity*> m_selectedEntities;

	// Editor state
	bool m_running;

	EngineInterface* m_engineInterface;
	Ui::TreenityClass ui;

	// Utility
	void UpdateOnSelection();

	ComponentView* m_compView;
	std::map<int, AbstractComponentView*> m_componentViews;

	//Asset management
	AssetManagerWidget* m_assetManagerWidget;
protected:
	void keyPressEvent(QKeyEvent* event);
	void keyReleaseEvent(QKeyEvent* event);

private slots:
	void New();
	void OpenProject();
	void Save();
	void SaveAs();
	void Play();
	void CreateEntity();
	void DestroyEntity();
	void RenameEntity();
	void AddRenderable();
	void AddPhysics();
	void AddWaterCollider();
	void AddScriptComponent();
	void AddCollisionResponder();
	
};

#endif // TREENITY_H
