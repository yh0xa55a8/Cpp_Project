#pragma once
#include <QObject>
#include <QMessageBox>
#include <array>
#include <fstream>
#include <cassert>
#include "macros.h"

class fileOpenException :public std::exception {};

class Storage:public QObject
{
	Q_OBJECT
public:
	void init();
	const byte & readByte(const doubleByte address) { return storage[address]; };
	void writeByte(const doubleByte& address, byte info) { storage[address] = info; }
signals:
	void getRomComp();
public slots:
	void getRom(QString);
private:
	std::array<byte, 0x10000> storage;
	
};

