#ifndef TREENITY_H
#define TREENITY_H

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

	void CreateOpenGLContext();

private:
	
	EngineInterface* m_engineInterface;

	bool m_running;
	Ui::TreenityClass ui;
	EntityOutliner* m_entityOutliner;

private slots:

	void CreateEntity();

};

#endif // TREENITY_H
