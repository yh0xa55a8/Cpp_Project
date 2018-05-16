#pragma once

#include "ui_mainWindow.h"
#include <QtWidgets/QMainWindow>
#include <QString>
#include <QFileDialog>
#include <iostream>

class mainWindow : public QMainWindow
{
	Q_OBJECT
public:
	mainWindow(QWidget *parent = Q_NULLPTR);
	void flashImage(const QImage &);
	void openFile();
	//virtual void keyPressEvent(QKeyEvent *);
	QString getFilePath() { return filePath; }
private:
	QString filePath;
	QLabel surface;
	Ui::mainWindowClass ui;
signals:
	void fileOpened();
};
