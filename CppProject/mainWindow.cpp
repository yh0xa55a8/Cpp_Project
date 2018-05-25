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
//���ڹ���,ͬʱ��������ʾͼ���ui.label��Ϊȫ��

void mainWindow::freshImage(const QImage image) {
	ui.label->setPixmap(QPixmap::fromImage(image));
}
//ʹ��QimageΪ�������´����е�ͼ��

void mainWindow::openFile() {
	auto tmpPath = QFileDialog::getOpenFileName(this, tr("open file"), "/", tr("GB ROM file(*.gb)"));
	if (tmpPath != "") {
		emit fileOpened(tmpPath);
	}//�����û����ȡ���󱨴�
}
//���ÿ���Դ���QFileDialogѡȡ�ļ�ͬʱ����fileOpend�ź�

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
	keyStatus[static_cast<Qt::Key>(k->key())] = false;
}
//������̰���/�ͷ��¼�