#include "mainWindow.h"

mainWindow::mainWindow(QWidget *parent)
	: QMainWindow(parent, Qt::WindowCloseButtonHint)
{
	ui.setupUi(this);
	QObject::connect(ui.actionExit, &QAction::triggered, &QApplication::quit);
	QObject::connect(ui.actionOpen, &QAction::triggered, this, &mainWindow::openFile);
	keyStatus[Qt::Key_Up] = false;
	QImage image{ 160, 144, QImage::Format_RGB16 };
	image.fill(Qt::black);
	this->flashImage(image);
	ui.label->grabKeyboard();
}
//窗口构造,同时将用于显示图像的ui.label设为全黑

void mainWindow::flashImage(const QImage &image) {
	ui.label->setPixmap(QPixmap::fromImage(image));
}
//使用Qimage为参数更新窗口中的图像

void mainWindow::openFile() {
	emit fileOpened(QFileDialog::getOpenFileName(this, tr("open file"), "/", tr("GB ROM file(*.gb)")));
}
//调用框架自带的QFileDialog选取文件同时发送fileOpend信号

void mainWindow::keyPressEvent(QKeyEvent *k)
{
	keyStatus[static_cast<Qt::Key>(k->key())] = true;
}

void mainWindow::keyReleaseEvent(QKeyEvent *k)
{
	keyStatus[static_cast<Qt::Key>(k->key())] = false;
}
//处理键盘按下/释放事件
