#pragma once

#include <QObject>
#include <QImage>
#include <QColor>
#include "Storage.h"

class GPU : public QObject
{
	Q_OBJECT
public:
	void init(Storage&, std::array<std::array<std::array<int, 8>, 8>, 384>&);
	void step(int);
signals:
	void freshImage(QImage);
private:
	byte readByte_(doubleByte add) { return simSto->readByte(add); }
	void writeByte_(doubleByte add, byte val) { simSto->writeByte(add, val); }
	enum { addLCDC = 0xFF40,addSTAT = 0xFF41,addSCY=0xFF42,addSCX=0xFF43,addLY=0xFF44,addBGP = 0xFF47,addOBP0=0xFF48,addOBP1=0xFF49,addWX=0xFF4A,addWY=0xFF4B};
	QImage imageBuffer { 160,144, QImage::Format_RGB16 };
	Storage* simSto;
	std::array<std::array<std::array<int, 8>, 8>, 384>* TileSet;
	int GPUclock=0;
	enum modes {Hb=0,Vb=1,OAM=2,VRAM=3};
	modes mode=Hb;
	void freshLine();
};
