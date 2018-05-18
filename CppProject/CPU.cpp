#include "CPU.h"



CPU::CPU(Storage& _sto)
{
	loadOpcode();
	sto = &_sto;
}

void CPU::loadOpcode()
{
	auto regHLAddr = [&] ()->doubleByte{return (byteRegs[L] << 4) | byteRegs[H]; };
	auto regHL = [&]()->doubleByte {return (byteRegs[H] << 4) | byteRegs[L]; };

	//8-bit load
	//LD nn,n
	opcode[0x06] = [&]()->int {byteRegs[B] = sto->readByte(regPC++); return 8; };
	opcode[0x0E] = [&]()->int {byteRegs[C] = sto->readByte(regPC++); return 8; };
	opcode[0x16] = [&]()->int {byteRegs[D] = sto->readByte(regPC++); return 8; };
	opcode[0x1E] = [&]()->int {byteRegs[E] = sto->readByte(regPC++); return 8; };
	opcode[0x26] = [&]()->int {byteRegs[H] = sto->readByte(regPC++); return 8; };
	opcode[0x2E] = [&]()->int {byteRegs[L] = sto->readByte(regPC++); return 8; };
	opcode[0x36] = [&]()->int {sto->writeByte(regHLAddr(), sto->readByte(regPC++)); return 8; };

	//LD r1,r2
	//未实现的为装载到自身的指令
	opcode[0x40] = [&]()->int {return 4; };
	opcode[0x41] = [&]()->int {byteRegs[B] = byteRegs[C]; return 4; };
	opcode[0x42] = [&]()->int {byteRegs[B] = byteRegs[D]; return 4; };
	opcode[0x43] = [&]()->int {byteRegs[B] = byteRegs[E]; return 4; };
	opcode[0x44] = [&]()->int {byteRegs[B] = byteRegs[H]; return 4; };
	opcode[0x45] = [&]()->int {byteRegs[B] = byteRegs[L]; return 4; };
	opcode[0x46] = [&]()->int {byteRegs[B] = sto->readByte(regHLAddr()); return 8; };
	opcode[0x47] = [&]()->int {byteRegs[B] = byteRegs[A]; return 4; };
	opcode[0x48] = [&]()->int {byteRegs[C] = byteRegs[B]; return 4; };
	opcode[0x49] = [&]()->int {return 4; };
	opcode[0x4A] = [&]()->int {byteRegs[C] = byteRegs[D]; return 4; };
	opcode[0x4B] = [&]()->int {byteRegs[C] = byteRegs[E]; return 4; };
	opcode[0x4C] = [&]()->int {byteRegs[C] = byteRegs[H]; return 4; };
	opcode[0x4D] = [&]()->int {byteRegs[C] = byteRegs[L]; return 4; };
	opcode[0x4E] = [&]()->int {byteRegs[C] = sto->readByte(regHLAddr()); return 8; };
	opcode[0x4F] = [&]()->int {byteRegs[C] = byteRegs[A]; return 4; };
	opcode[0x50] = [&]()->int {byteRegs[D] = byteRegs[B]; return 4; };
	opcode[0x51] = [&]()->int {byteRegs[D] = byteRegs[C]; return 4; };
	opcode[0x52] = [&]()->int {return 4; };
	opcode[0x53] = [&]()->int {byteRegs[D] = byteRegs[E]; return 4; };
	opcode[0x54] = [&]()->int {byteRegs[D] = byteRegs[H]; return 4; };
	opcode[0x55] = [&]()->int {byteRegs[D] = byteRegs[L]; return 4; };
	opcode[0x56] = [&]()->int {byteRegs[D] = sto->readByte(regHLAddr()); return 8; };
	opcode[0x57] = [&]()->int {byteRegs[D] = byteRegs[A]; return 4; };
	opcode[0x58] = [&]()->int {byteRegs[E] = byteRegs[B]; return 4; };
	opcode[0x59] = [&]()->int {byteRegs[E] = byteRegs[C]; return 4; };
	opcode[0x5A] = [&]()->int {byteRegs[E] = byteRegs[D]; return 4; };
	opcode[0x5B] = [&]()->int {return 4; };
	opcode[0x5C] = [&]()->int {byteRegs[E] = byteRegs[H]; return 4; };
	opcode[0x5D] = [&]()->int {byteRegs[E] = byteRegs[L]; return 4; };
	opcode[0x5E] = [&]()->int {byteRegs[E] = sto->readByte(regHLAddr()); return 8; };
	opcode[0x5F] = [&]()->int {byteRegs[E] = byteRegs[A]; return 4; };
	opcode[0x60] = [&]()->int {byteRegs[H] = byteRegs[B]; return 4; };
	opcode[0x61] = [&]()->int {byteRegs[H] = byteRegs[C]; return 4; };
	opcode[0x62] = [&]()->int {byteRegs[H] = byteRegs[D]; return 4; };
	opcode[0x63] = [&]()->int {byteRegs[H] = byteRegs[E]; return 4; };
	opcode[0x64] = [&]()->int { return 4; };
	opcode[0x65] = [&]()->int {byteRegs[H] = byteRegs[L]; return 4; };
	opcode[0x66] = [&]()->int {byteRegs[H] = sto->readByte(regHLAddr()); return 8; };
	opcode[0x67] = [&]()->int {byteRegs[H] = byteRegs[A]; return 4; };
	opcode[0x68] = [&]()->int {byteRegs[L] = byteRegs[B]; return 4; };
	opcode[0x69] = [&]()->int {byteRegs[L] = byteRegs[C]; return 4; };
	opcode[0x6A] = [&]()->int {byteRegs[L] = byteRegs[D]; return 4; };
	opcode[0x6B] = [&]()->int {byteRegs[L] = byteRegs[E]; return 4; };
	opcode[0x6C] = [&]()->int {byteRegs[L] = byteRegs[H]; return 4; };
	opcode[0x6D] = [&]()->int { return 4; };
	opcode[0x6E] = [&]()->int {byteRegs[L] = sto->readByte(regHLAddr()); return 8; };
	opcode[0x6F] = [&]()->int {byteRegs[L] = byteRegs[A]; return 4; };
	opcode[0x70] = [&]()->int {sto->writeByte(regHLAddr(), byteRegs[B]); return 8; };
	opcode[0x71] = [&]()->int {sto->writeByte(regHLAddr(), byteRegs[C]); return 8; };
	opcode[0x72] = [&]()->int {sto->writeByte(regHLAddr(), byteRegs[D]); return 8; };
	opcode[0x73] = [&]()->int {sto->writeByte(regHLAddr(), byteRegs[E]); return 8; };
	opcode[0x74] = [&]()->int {sto->writeByte(regHLAddr(), byteRegs[H]); return 8; };
	opcode[0x75] = [&]()->int {sto->writeByte(regHLAddr(), byteRegs[L]); return 8; };
	opcode[0x77] = [&]()->int {sto->writeByte(regHLAddr(), byteRegs[A]); return 8; };
};