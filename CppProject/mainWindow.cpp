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

void mainWindow::freshImage(const QImage image) {
	ui.label->setPixmap(QPixmap::fromImage(image));
}

void mainWindow::openFile() {
	auto tmpPath = QFileDialog::getOpenFileName(this, tr("open file"), "", tr("GB ROM file(*.gb)"));
	if (tmpPath != "") {
		emit fileOpened(tmpPath);
	}//avoid error when fileDialog window is closed directly
}

void mainWindow::closeRom() {
	QImage image{ 160, 144, QImage::Format_RGB16 };
	image.fill(Qt::black);
	this->freshImage(image);
	emit romClosed();
}

void mainWindow::keyPressEvent(QKeyEvent *k)
{
	keyStatus[static_cast<Qt::Key>(k->key())] = true;
	byte r1 = 0, r2 = 0;
	r1 = (((((keyStatus[Qt::Key_Return] << 1) + keyStatus[Qt::Key_Space]) << 1) + keyStatus[Qt::Key_X]) << 1) + keyStatus[Qt::Key_Z];
	r2 = (((((keyStatus[Qt::Key_Down] << 1) + keyStatus[Qt::Key_Up]) << 1) + keyStatus[Qt::Key_Left]) << 1) + keyStatus[Qt::Key_Right];
	emit keys(r1, r2);
}

void mainWindow::keyReleaseEvent(QKeyEvent *k)
{
	keyStatus[static_cast<Qt::Key>(k->key())] = false;
	byte r1 = 0, r2 = 0;
	r1 = (((((keyStatus[Qt::Key_Return] << 1) + keyStatus[Qt::Key_Space]) << 1) + keyStatus[Qt::Key_X]) << 1) + keyStatus[Qt::Key_Z];
	r2 = (((((keyStatus[Qt::Key_Down] << 1) + keyStatus[Qt::Key_Up]) << 1) + keyStatus[Qt::Key_Left]) << 1) + keyStatus[Qt::Key_Right];
	emit keys(r1, r2);
}
