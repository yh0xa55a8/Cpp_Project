#pragma once

#include "ui_mainWindow.h"
#include <QtWidgets/QMainWindow>
#include <QString>
#include <QFileDialog>
#include <iostream>
#include <map>
#include <QKeyEvent>
#include <QCloseEvent>
#include "SimThread.h"
class mainWindow : public QMainWindow
{
	Q_OBJECT
public:
	mainWindow(QWidget *parent = Q_NULLPTR);

signals:
	void fileOpened(QString);
signals:
	void romClosed();
signals:
	void keys(byte);
public slots:
	void freshImage(const QImage);
public slots:
	void sendKey(bool);
private:
	void openFile();
	void closeRom();
	void showAbout() { QMessageBox::about(nullptr, "GameBoy Simulator", "An OOP course project.\nProgrammed by 1752704 and 1752086\n"); }
	std::map<Qt::Key, bool> keyStatus;
	Ui::mainWindowClass ui;
	void keyPressEvent(QKeyEvent *);
	void keyReleaseEvent(QKeyEvent *);
};
