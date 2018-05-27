#include "mainWindow.h"

mainWindow::mainWindow(QWidget *parent)
	: QMainWindow{ parent, Qt::WindowCloseButtonHint }
{
	ui.setupUi(this);
	QObject::connect(ui.actionExit, &QAction::triggered, &QApplication::quit);
	QObject::connect(ui.actionOpen, &QAction::triggered, this, &mainWindow::openFile);
	QObject::connect(ui.actionClose, &QAction::triggered, this, &mainWindow::closeRom);
	QObject::connect(ui.actionAbout, &QAction::triggered, this,&mainWindow::showAbout);
	QImage image{ 160, 144, QImage::Format_RGB16 };
	image.fill(Qt::black);
	this->freshImage(image);
	ui.label->grabKeyboard();
}
//窗口构造,同时将用于显示图像的ui.label设为全黑

void mainWindow::freshImage(const QImage image) {
	ui.label->setPixmap(QPixmap::fromImage(image));
}
//使用Qimage为参数更新窗口中的图像

void mainWindow::openFile() {
	auto tmpPath = QFileDialog::getOpenFileName(this, tr("open file"), "/", tr("GB ROM file(*.gb)"));
	if (tmpPath != "") {
		emit fileOpened(tmpPath);
	}//避免用户点击取消后报错
}
//调用框架自带的QFileDialog选取文件同时发送fileOpend信号

void mainWindow::closeRom() {
	QImage image{ 160, 144, QImage::Format_RGB16 };
	image.fill(Qt::black);
	this->freshImage(image);
	emit romClosed();
}

void mainWindow::keyPressEvent(QKeyEvent *k)
{
	keyStatus[static_cast<Qt::Key>(k->key())] = true;
}

void mainWindow::keyReleaseEvent(QKeyEvent *k)
{
	Qt::Key tmp = static_cast<Qt::Key>(k->key());
	keyStatus[static_cast<Qt::Key>(k->key())] = false;
}
//处理键盘按下/释放事件

void mainWindow::sendKey(bool row) {
	byte status = 0;
	if (row) {
		status = (((((keyStatus[Qt::Key_Enter] << 1) + keyStatus[Qt::Key_Space]) << 1) + keyStatus[Qt::Key_X]) << 1) + keyStatus[Qt::Key_Z];
	}
	else {
		status = (((((keyStatus[Qt::Key_Down] << 1) + keyStatus[Qt::Key_Up]) << 1) + keyStatus[Qt::Key_Left]) << 1) + keyStatus[Qt::Key_Right];
	}
	emit keys(status);
	keyStatus[Qt::Key_Enter] = false;
	keyStatus[Qt::Key_Space] = false;
	keyStatus[Qt::Key_X] = false;
	keyStatus[Qt::Key_Z] = false;
	keyStatus[Qt::Key_Down] = false;
	keyStatus[Qt::Key_Up] = false;
	keyStatus[Qt::Key_Left] = false;
	keyStatus[Qt::Key_Right] = false;
}