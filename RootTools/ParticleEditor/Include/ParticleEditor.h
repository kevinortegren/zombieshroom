#ifndef PARTICLEEDITOR_H
#define PARTICLEEDITOR_H

#include <QtWidgets/QMainWindow>
#include <RootTools/ParticleEditor/GeneratedFiles/ui_ParticleEditor.h>
#include <QtWidgets/QMessageBox>
#include <QtGui/QCloseEvent>

class ParticleEditor : public QMainWindow
{
	Q_OBJECT

public:
	ParticleEditor(QWidget *parent = 0);
	~ParticleEditor();
	bool CheckExit();

	Ui::ParticleEditorClass ui;
private:
	void closeEvent(QCloseEvent *event);
	bool m_running;
private slots:
	void MenuExit();

	
};

#endif // PARTICLEEDITOR_H
