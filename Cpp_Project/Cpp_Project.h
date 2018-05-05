#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Cpp_Project.h"

class Cpp_Project : public QMainWindow
{
	Q_OBJECT

public:
	Cpp_Project(QWidget *parent = Q_NULLPTR);

private:
	Ui::Cpp_ProjectClass ui;
};
