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
	CPU(Storage&);
private:
	bool haltFlag = false;
	bool stopFlag = false;
	bool interruptEnabled = true;
	bool interruptSwitched = false;
	Storage * simSto;

	int timeOfLastInst;
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
	bool getBit(int bit, const byte& n) { return (n >> bit) & 1; }
	void setBit(int bit, byte &n, bool val) {
		if (val) {
			n |= (1 << bit);
		}
		else {
			n &= 0xFF - (1 << bit);
		}
	}
	enum flags { FZ = 7, FN = 6, FH = 5, FC = 4 };
	bool getFlag(flags flag) {return getBit(flag,byteRegs[F]);}
	void setFlag(flags flag, bool val) {setBit(flag, byteRegs[F], val);}
	doubleByte doubleRegAddr(bReg HByte, bReg LByte) { return (byteRegs[LByte] << 4) | byteRegs[HByte]; }
	doubleByte doubleReg(bReg HByte, bReg LByte) { return (byteRegs[HByte] << 4) | byteRegs[LByte]; };
	byte readByte_(doubleByte add) { return simSto->readByte(add); }
	void writeByte_(doubleByte add, byte val) { simSto->writeByte(add, val); }
	doubleByte readDouble_(doubleByte add) { return readByte_(add + 1) << 8 + readByte_(add); }
	void writeDouble_(doubleByte add, doubleByte val) { writeByte_(add, val & 0x00FF); writeByte_(add + 1, (val & 0xFF00) >> 8); }
	void ADD(byte n) {
		byte re = byteRegs[A] + n;
		setFlag(FZ, (re == 0 || re == 0x100));
		setFlag(FN, false);
		setFlag(FH, (byteRegs[A] ^ n^re) & 0x10);
		setFlag(FC, (byteRegs[A] ^ n^re) & 0x100);
		byteRegs[A] = re & 0xFF;
	}
	void ADC(byte n) {
		byte re = byteRegs[A] + n + getFlag(FC);
		setFlag(FZ, (re == 0 || re == 0x100));
		setFlag(FN, false);
		setFlag(FH, (byteRegs[A] ^ n^re) & 0x10);
		setFlag(FC, (byteRegs[A] ^ n^re) & 0x100);
		byteRegs[A] = re & 0xFF;
	}
	void SUB(byte n) {
		byte re = byteRegs[A] - n;
		setFlag(FZ, (re == 0));
		setFlag(FN, true);
		setFlag(FH, (byteRegs[A] ^ n^re) & 0x10);
		setFlag(FC, (byteRegs[A] ^ n^re) & 0x100);
		byteRegs[A] = re & 0xFF;
	}
	void SBC(byte n) {
		byte re = byteRegs[A] - n - getFlag(FC);
		setFlag(FZ, (re == 0));
		setFlag(FN, true);
		setFlag(FH, (byteRegs[A] ^ n^re) & 0x10);
		setFlag(FC, (byteRegs[A] ^ n^re) & 0x100);
		byteRegs[A] = re & 0xFF;
	}
	void AND(byte n) {
		byte re = byteRegs[A] & n;
		setFlag(FZ, (re == 0));
		setFlag(FN, true);
		setFlag(FH, true);
		setFlag(FC, false);
		byteRegs[A] = re & 0xFF;
	}
	void OR(byte n) {
		byte re = byteRegs[A] | n;
		setFlag(FZ, (re == 0));
		setFlag(FN, false);
		setFlag(FH, false);
		setFlag(FC, false);
		byteRegs[A] = re & 0xFF;
	}
	void XOR(byte n) {
		byte re = byteRegs[A] ^ n;
		setFlag(FZ, (re == 0));
		setFlag(FN, false);
		setFlag(FH, false);
		setFlag(FC, false);
		byteRegs[A] = re & 0xFF;
	}
	void CP(byte n) {
		byte re = byteRegs[A] - n;
		setFlag(FZ, (re == 0));
		setFlag(FN, true);
		setFlag(FH, (byteRegs[A] ^ n^re) & 0x10);
		setFlag(FC, (byteRegs[A] ^ n^re) & 0x100);
	}
	byte INC(byte n) {
		byte re = n + 1;
		setFlag(FZ, (re == 0x100));
		setFlag(FN, false);
		setFlag(FH, (n ^re) & 0x10);
		return re & 0xFF;
	}
	byte DEC(byte n) {
		byte re = n - 1;
		setFlag(FZ, (re == 0));
		setFlag(FN, true);
		setFlag(FH, (n ^re) & 0x10);
		return re & 0xFF;
	}
	void ADD(doubleByte n) {
		doubleByte HL = doubleReg(H, L);
		unsigned int re = HL + n;
		setFlag(FN, false);
		setFlag(FH, (HL^n^re) & 0x1000);
		setFlag(FC, (HL^n^re) & 0x10000);
		byteRegs[H] = (re >> 8) & 0xFF;
		byteRegs[L] = re & 0xFF;
	}
	void ADD(signedByte& n) {
		int re = regSP + n;
		setFlag(FZ, false);
		setFlag(FN, false);
		setFlag(FH, (regSP^re) & 0x1000);
		setFlag(FC, re & 0x10000);
		regSP = re & 0xFFFF;
	}
	byte RLC(byte n) {
		setFlag(FN, false);
		setFlag(FH, false);
		setFlag(FC, n >> 7);
		n = (n << 1) + getFlag(FC);
		setFlag(FZ, !n);
		return n;
	}
	byte RL(byte n) {
		setFlag(FN, false);
		setFlag(FH, false);
		bool tmp = getFlag(FC);
		setFlag(FC, n >> 7);
		n = (n << 1) + tmp;
		setFlag(FZ, !n);
		return n;
	}
	byte RRC(byte n) {
		setFlag(FN, false);
		setFlag(FH, false);
		setFlag(FC, n & 1);
		n = (n >> 1) + (static_cast<byte>(getFlag(FC)) << 7);
		setFlag(FZ, !n);
		return n;
	}
	byte RR(byte n) {
		setFlag(FN, false); 
		setFlag(FH, false); 
		bool tmp = getFlag(FC); 
		setFlag(FC, n & 1); 
		n = (n >> 1) + (static_cast<byte>(tmp) << 7); 
		setFlag(FZ, !n);
		return n;
	}
};

