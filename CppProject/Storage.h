#pragma once
#include <QObject>
#include <array>
#include <fstream>
#include <cassert>
#include "macros.h"
class Storage:public QObject
{
	Q_OBJECT
public:
	Storage();
	~Storage();
	
public slots:
	void getRom(QString);
private:
	std::array<byte, 0x10000> storage;
};

