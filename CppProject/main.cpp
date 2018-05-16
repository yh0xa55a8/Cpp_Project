#include "mainWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	mainWindow w;
	w.show();
	QEventLoop fileLoop;
	QObject::connect(&w, SIGNAL(fileOpened(QString)), &fileLoop, SLOT(quit()));
	//QObject::connect(&w, SIGNAL(fileOpened(Qstring)), &thread1, SLOT(openfile(QString)));此处未来连接到模拟器循环所在线程处理文件打开
	fileLoop.exec();	
	return a.exec();
}
