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
#include <RootTools/Treenity/Include/Components/TransformView.h>
#include <RootTools/Treenity/Include/EditorInterface.h>

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

	// Implemented from EditorInterface
	void Select(ECS::Entity* p_entity);
	void AddToSelection(ECS::Entity* p_entity);
	void ClearSelection();
	const std::set<ECS::Entity*>& GetSelection() const;
private:
	// Component meta-data.
	std::map<int, QString> m_componentNames;

	// Project meta-data.
	QString m_currentProjectFile;
	ProjectManager* m_projectManager;
	std::set<ECS::Entity*> m_selectedEntities;

	EngineInterface* m_engineInterface;

	bool m_running;
	Ui::TreenityClass ui;

	// Utility
	void UpdateOnSelection();

	ComponentView* m_compView;
	std::map<int, AbstractComponentView*> m_componentViews;
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
	
};

#endif // TREENITY_H
