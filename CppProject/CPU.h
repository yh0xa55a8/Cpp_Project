#pragma once
#include <macros.h>
#include <QObject>
#include <array>
#include <functional>
#include <Storage.h>
class CPU :public QObject
{
	Q_OBJECT
public:
	//initialization
	void init(Storage&);

	//CPU step, return timer cycles used
	int step();
private:
	//enums for interrupt process
	enum {interruptSwitchAdd = 0xFFFF,interruptFlagAdd = 0xFF0F};
	enum interruptType { keyPad = 0x60, serialTrans = 0x58, timerOverflow = 0x50, LCDCStatus = 0x48, Vblank = 0x40 };

	//internal regs for halt and option EI/DI
	bool haltFlag = false;
	bool interruptEnabled = true;
	bool interruptSwitched = false;

	//pointer to IO with RAM
	Storage * simSto;

	//temperary store last option's time
	int deltaTime=0;

	//registers
	std::array<byte, 8> byteRegs{ 0x01,0xB0,0x00,0x13,0x00,0xD8,0x01,0x4D };
	enum bReg { A = 0, F = 1, B = 2, C = 3, D = 4, E = 5, H = 6, L = 7 };
	doubleByte regPC = 0x0100;
	doubleByte regSP = 0xFFFE;

	//instructions
	void loadOpcode();
	std::array<std::function<int()>, 0x100> opcode;
	std::array<std::function<int()>, 0x100> CBopcode;

	//assist funcs for instructions
	bool getBit(int, byte);
	void setBit(int , byte &, bool);
	enum flags { FZ = 7, FN = 6, FH = 5, FC = 4 };
	bool getFlag(flags flag) {return getBit(flag,byteRegs[F]);}
	void setFlag(flags flag, bool val) {setBit(flag, byteRegs[F], val);}
	doubleByte doubleReg(bReg HByte, bReg LByte) { return (byteRegs[HByte] << 8) | byteRegs[LByte]; };
	byte readByte_(doubleByte add) { return simSto->readByte(add); }
	void writeByte_(doubleByte add, byte val) { simSto->writeByte(add, val); }
	doubleByte readDouble_(doubleByte add) { return (readByte_(add + 1) << 8) + readByte_(add); }
	void writeDouble_(doubleByte add, doubleByte val) { writeByte_(add, val & 0x00FF); writeByte_(add + 1, (val & 0xFF00) >> 8); }
	void ADD(byte n);
	void ADC(byte n);
	void SUB(byte n);
	void SBC(byte n);
	void AND(byte n);
	void OR(byte n);
	void XOR(byte n);
	void CP(byte n);
	byte INC(byte n);
	byte DEC(byte n);
	void ADD(doubleByte n);
	void ADD(signedByte& n);
	byte RLC(byte n);
	byte RL(byte n);
	byte RRC(byte n);
	byte RR(byte n);
	byte SLA(byte n);
	byte SRA(byte n);
	byte SRL(byte n);
};

