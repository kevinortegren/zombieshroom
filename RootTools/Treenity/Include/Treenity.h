#ifndef TREENITY_H
#define TREENITY_H

#include <map>
#include <QtWidgets/QMainWindow>
#include "ui_Treenity.h"
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
	void EntityDestroyed(ECS::Entity* p_entity);
private:
	// Project meta-data.
	std::map<ECS::Entity*, QString> m_entityNames;
	ECS::Entity* m_selectedEntity;

	EngineInterface* m_engineInterface;

	bool m_running;
	Ui::TreenityClass ui;
private slots:
	void CreateEntity();
	void DestroyEntity();
	void RenameEntity();
	void OutlinerSelectEntity();
};

#endif // TREENITY_H
