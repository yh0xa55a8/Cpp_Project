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
signals:
	void requestKey(bool);
public slots:
	void getRom(QString);
public slots:
	void writeKey(byte);
private:
	std::array<byte, 0x10000> storage;
	//VRam
	
	void updateTileSet(doubleByte);
	void updataSpriteSet(doubleByte);
};

