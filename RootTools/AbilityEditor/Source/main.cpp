#include "AbilityEditor.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	AbilityEditor w;
	w.show();
	return a.exec();
}
