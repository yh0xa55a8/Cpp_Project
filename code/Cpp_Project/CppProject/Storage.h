#pragma once
#include <QObject>
#include <QMessageBox>
#include <array>
#include <fstream>
#include <cassert>
#include "macros.h"
#include "Sprite.h"
#include <Timer.h>

//empty exception class for file open error
class fileOpenException :public std::exception {};

class Storage:public QObject
{
	Q_OBJECT
public:
	//initialization
	void init();

	//timer controlled by storage
	Timer simTimer;

	//Tile and Sprite sets for graphics
	std::array<std::array<std::array<int, 8>, 8>, 384> VRamTileSet;
	std::array<Sprite, 40> SpriteSet;

	//basic data IO
	const byte & readByte(doubleByte);
	void writeByte(doubleByte, byte);
signals:
	//signal for start simThread
	void getRomComp();	

public slots:
	//slot for load ROM from file
	void getRom(QString);

public slots:
	//slot for get input from GUI
	void writeKey(byte,byte);

private:
	//main Ram
	std::array<byte, 0x10000> storage;

	//Input
	std::array<byte, 2> keys;

	//VRam funcs
	void updateTileSet(doubleByte);
	void updataSpriteSet(doubleByte);
};

