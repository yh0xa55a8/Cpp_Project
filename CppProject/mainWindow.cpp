#include "mainWindow.h"

mainWindow::mainWindow(QWidget *parent)
	: QMainWindow{ parent, Qt::WindowCloseButtonHint }
{
	ui.setupUi(this);
	QObject::connect(ui.actionExit, &QAction::triggered, &QApplication::quit);
	QObject::connect(ui.actionOpen, &QAction::triggered, this, &mainWindow::openFile);
	QImage image{ 160, 144, QImage::Format_RGB16 };
	image.fill(Qt::black);
	this->flashImage(image);
	ui.label->grabKeyboard();
}
//���ڹ���,ͬʱ��������ʾͼ���ui.label��Ϊȫ��

void mainWindow::flashImage(const QImage &image) {
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

void mainWindow::keyPressEvent(QKeyEvent *k)
{
	keyStatus[static_cast<Qt::Key>(k->key())] = true;
}

void mainWindow::keyReleaseEvent(QKeyEvent *k)
{
	keyStatus[static_cast<Qt::Key>(k->key())] = false;
}
//������̰���/�ͷ��¼�