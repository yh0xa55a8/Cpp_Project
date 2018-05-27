#pragma once
#include <QObject>
#include <QMessageBox>
#include <array>
#include <fstream>
#include <cassert>
#include "macros.h"
#include "Sprite.h"

class fileOpenException :public std::exception {};

class Storage:public QObject
{
	Q_OBJECT
public:
	void init();
	std::array<std::array<std::array<int, 8>, 8>, 384> VRamTileSet;
	std::array<Sprite, 40> SpriteSet;
	const byte & readByte(doubleByte);
	void writeByte(doubleByte, byte);
signals:
	void getRomComp();
public slots:
	void getRom(QString);
public slots:
	void writeKey(byte,byte);
private:
	std::array<byte, 0x10000> storage;
	//VRam
	std::array<byte, 2> keys;
	void updateTileSet(doubleByte);
	void updataSpriteSet(doubleByte);
};

