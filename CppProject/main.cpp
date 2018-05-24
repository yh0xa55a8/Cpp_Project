#include "mainWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a{ argc, argv };
	mainWindow w;
	w.show();
	SimThread thread;
	QObject::connect(&w, SIGNAL(fileOpened(QString)), &(thread.getSto()), SLOT(getRom(QString)));
	QObject::connect(&w, SIGNAL(romClosed()), &thread, SLOT(requestInterruption()));
	QObject::connect(&(thread.getSto()), SIGNAL(getRomComp()), &thread, SLOT(start()));
	return a.exec();
}
