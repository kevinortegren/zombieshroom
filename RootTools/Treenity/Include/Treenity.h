#ifndef TREENITY_H
#define TREENITY_H

#include <QtWidgets/QMainWindow>
#include "ui_Treenity.h"

class Treenity : public QMainWindow
{
	Q_OBJECT

public:
	Treenity(QWidget *parent = 0);
	~Treenity();

	bool IsRunning();
	void closeEvent(QCloseEvent *event);

private:
	bool m_running;
	Ui::TreenityClass ui;
};

#endif // TREENITY_H
