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

private:
	Ui::TreenityClass ui;
};

#endif // TREENITY_H
