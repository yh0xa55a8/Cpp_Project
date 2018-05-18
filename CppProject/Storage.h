#pragma once
#include <QObject>
#include <array>
#include <fstream>
#include <cassert>
#include "macros.h"

class fileOpenException :public std::exception {};

class Storage:public QObject
{
	Q_OBJECT
public:
	Storage();
	const byte & readByte(const doubleByte address) { return storage[address]; };
	void writeByte(const doubleByte& address, byte info) { storage[address] = info; }
public slots:
	void getRom(QString);
private:
	std::array<byte, 0x10000> storage;
};

