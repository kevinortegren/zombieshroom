#include "AbilityEditor.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	AbilityEditor w;
	w.Init();
	w.show();
	return a.exec();
}
