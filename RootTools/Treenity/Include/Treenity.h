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

class Treenity : public QMainWindow
{
	Q_OBJECT

public:
	Treenity(QWidget *parent = 0);
	~Treenity();

	void SetEngineInterface(EngineInterface* p_engineInterface);

	bool IsRunning();
	void closeEvent(QCloseEvent *event);

	// Called by the engine.
	void EntityCreated(ECS::Entity* p_entity);
	void EntityRemoved(ECS::Entity* p_entity);
	void ComponentCreated(ECS::Entity* p_entity, int p_componentType);
	void ComponentRemoved(ECS::Entity* p_entity, int p_componentType);
private:
	// Component meta-data.
	std::map<int, QString> m_componentNames;

	// Project meta-data.
	std::map<ECS::Entity*, QString> m_entityNames;
	ECS::Entity* m_selectedEntity;

	EngineInterface* m_engineInterface;

	bool m_running;
	Ui::TreenityClass ui;
	Ui::Transform transformUI;
	Ui::Renderable renderableUI;
private slots:
	void CreateEntity();
	void DestroyEntity();
	void RenameEntity();
	void OutlinerSelectEntity();
	void AddRenderable();

	void SelectEntity(ECS::Entity* p_entity);
	void SetupUIForComponent(QWidget* p_widget, int p_componentType);
};

#endif // TREENITY_H
