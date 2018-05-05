#include "Cpp_Project.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Cpp_Project w;
	w.show();
	return a.exec();
}
