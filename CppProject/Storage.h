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
	const byte & readByte(doubleByte);
	void writeByte(doubleByte, byte);
signals:
	void getRomComp();
public slots:
	void getRom(QString);
private:
	std::array<byte, 0x10000> storage;
	//VRam
	std::array<std::array<std::array<int, 8>, 8>, 384> VRamTileSet;
	void updateTileSet(doubleByte);
};

