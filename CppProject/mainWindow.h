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
	void openFile();
signals:
	void fileOpened(QString); 
public slots:
	void flashImage(const QImage &);
private:
	std::map<Qt::Key, bool> keyStatus;
	Ui::mainWindowClass ui;
	void keyPressEvent(QKeyEvent *);
	void keyReleaseEvent(QKeyEvent *);
};
