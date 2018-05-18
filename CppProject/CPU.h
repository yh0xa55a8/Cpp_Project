#pragma once
#include <macros.h>
#include <QObject>
#include <array>
#include <functional>
#include <Storage.h>
class CPU:public QObject
{
	Q_OBJECT
public:
	CPU(Storage&);
private:
	Storage * sto;

	int timeOfLastInst;
	//registers
	std::array<byte, 8> byteRegs{0x01,0xB0,0x00,0x13,0x00,0xD8,0x01,0x4D};
	enum bReg {A=0,F=1,B=2,C=3,D=4,E=5,H=6,L=7};
	doubleByte regPC{0x0100};
	doubleByte regSP{0xFFFE};
	//instructions
	void loadOpcode();
	std::array<std::function<int()>, 0x100> opcode;
	std::array<std::function<int()>, 0x100> CBopcode;
};

