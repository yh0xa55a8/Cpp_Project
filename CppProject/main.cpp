#include "mainWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	mainWindow w;
	w.show();
	std::u16string filePath;
	QEventLoop fileLoop;
	QObject::connect(&w, SIGNAL(fileOpened()), &fileLoop, SLOT(quit()));
	fileLoop.exec();
	filePath = w.getFilePath().toStdU16String();
	return a.exec();
}
