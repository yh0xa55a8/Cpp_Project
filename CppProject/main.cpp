#include "mainWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	mainWindow w;
	w.show();
	SimThread thread;
	thread.start();
	QEventLoop fileLoop;
	QObject::connect(&w, SIGNAL(fileOpened(QString)), &(thread.getSto()), SLOT(getRom(QString)));
	QObject::connect(&w, SIGNAL(fileOpened(QString)), &fileLoop, SLOT(quit()));
	fileLoop.exec();	
	
	return a.exec();
}
