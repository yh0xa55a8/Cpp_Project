#include "mainWindow.h"

mainWindow::mainWindow(QWidget *parent)
	: QMainWindow(parent, Qt::WindowCloseButtonHint)
{
	ui.setupUi(this);
	QObject::connect(ui.actionExit, &QAction::triggered, &QApplication::quit);
	QObject::connect(ui.actionOpen, &QAction::triggered, this, &mainWindow::openFile);
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
	this->filePath = QFileDialog::getOpenFileName(this, tr("open file"), "/", tr("GB ROM file(*.gb)"));
	emit fileOpened();
}
//调用框架自带的QFileDialog选取文件并将文件路径保存在filePath,同时发送fileOpend信号

