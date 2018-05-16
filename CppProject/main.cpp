#include "mainWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	mainWindow w;
	w.show();
	QEventLoop fileLoop;
	QObject::connect(&w, SIGNAL(fileOpened(QString)), &fileLoop, SLOT(quit()));
	//QObject::connect(&w, SIGNAL(fileOpened(Qstring)), &thread1, SLOT(openfile(QString)));�˴�δ�����ӵ�ģ����ѭ�������̴߳����ļ���
	fileLoop.exec();	
	return a.exec();
}
