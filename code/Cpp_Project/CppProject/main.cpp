#include "mainWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a{ argc, argv };
	mainWindow w;
	w.show();
	SimThread thread;
	QObject::connect(&w, SIGNAL(fileOpened(QString)), &(thread.simSto), SLOT(getRom(QString)));
	QObject::connect(&w, SIGNAL(romClosed()), &thread, SLOT(restart()));
	QObject::connect(&(thread.simSto), SIGNAL(getRomComp()), &thread, SLOT(start()));
	QObject::connect(&w, SIGNAL(keys(byte,byte)), &(thread.simSto), SLOT(writeKey(byte,byte)));
	QObject::connect(&(thread.simGPU), SIGNAL(freshImage(QImage)), &w, SLOT(freshImage(QImage)));
	return a.exec();
}
