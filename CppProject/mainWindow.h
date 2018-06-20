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
	void fileOpened(QString);		//send path to Storage to load
signals:
	void romClosed();					//for close ROM or whole application
signals:
	void keys(byte,byte);				//send input to Storage
public slots:
	void freshImage(const QImage);
private:
	//functions for menu buttons
	void openFile();
	void closeRom();				
	void showAbout() { QMessageBox::about(nullptr, "GameBoy Simulator", "An OOP course project.\nProgrammed by 1752704 and 1752086\n"); }
	
	Ui::mainWindowClass ui;

	//for keyboard input
	std::map<Qt::Key, bool> keyStatus;
	void keyPressEvent(QKeyEvent *);
	void keyReleaseEvent(QKeyEvent *);
};
