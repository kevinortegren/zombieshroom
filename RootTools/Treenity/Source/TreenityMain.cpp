#include "Treenity.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Treenity w;
	w.show();
	return a.exec();
}
