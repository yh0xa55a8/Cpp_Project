#include "CPU.h"



CPU::CPU(Storage& _sto)
{
	loadOpcode();
	simSto = &_sto;
}

void CPU::loadOpcode()
{	

	//8-bit load
	//LD nn,n
	opcode[0x06] = [&]()->int {byteRegs[B] = readByte_(regPC++); return 8; };
	opcode[0x0E] = [&]()->int {byteRegs[C] = readByte_(regPC++); return 8; };
	opcode[0x16] = [&]()->int {byteRegs[D] = readByte_(regPC++); return 8; };
	opcode[0x1E] = [&]()->int {byteRegs[E] = readByte_(regPC++); return 8; };
	opcode[0x26] = [&]()->int {byteRegs[H] = readByte_(regPC++); return 8; };
	opcode[0x2E] = [&]()->int {byteRegs[L] = readByte_(regPC++); return 8; };
	opcode[0x36] = [&]()->int {writeByte_(doubleRegAddr(H, L), readByte_(regPC++)); return 8; };

	//LD r1,r2
	//未实现的为装载到自身的指令
	opcode[0x40] = [&]()->int {return 4; };
	opcode[0x41] = [&]()->int {byteRegs[B] = byteRegs[C]; return 4; };
	opcode[0x42] = [&]()->int {byteRegs[B] = byteRegs[D]; return 4; };
	opcode[0x43] = [&]()->int {byteRegs[B] = byteRegs[E]; return 4; };
	opcode[0x44] = [&]()->int {byteRegs[B] = byteRegs[H]; return 4; };
	opcode[0x45] = [&]()->int {byteRegs[B] = byteRegs[L]; return 4; };
	opcode[0x46] = [&]()->int {byteRegs[B] = readByte_(doubleRegAddr(H, L)); return 8; };
	opcode[0x47] = [&]()->int {byteRegs[B] = byteRegs[A]; return 4; };
	opcode[0x48] = [&]()->int {byteRegs[C] = byteRegs[B]; return 4; };
	opcode[0x49] = [&]()->int {return 4; };
	opcode[0x4A] = [&]()->int {byteRegs[C] = byteRegs[D]; return 4; };
	opcode[0x4B] = [&]()->int {byteRegs[C] = byteRegs[E]; return 4; };
	opcode[0x4C] = [&]()->int {byteRegs[C] = byteRegs[H]; return 4; };
	opcode[0x4D] = [&]()->int {byteRegs[C] = byteRegs[L]; return 4; };
	opcode[0x4E] = [&]()->int {byteRegs[C] = readByte_(doubleRegAddr(H, L)); return 8; };
	opcode[0x4F] = [&]()->int {byteRegs[C] = byteRegs[A]; return 4; };
	opcode[0x50] = [&]()->int {byteRegs[D] = byteRegs[B]; return 4; };
	opcode[0x51] = [&]()->int {byteRegs[D] = byteRegs[C]; return 4; };
	opcode[0x52] = [&]()->int {return 4; };
	opcode[0x53] = [&]()->int {byteRegs[D] = byteRegs[E]; return 4; };
	opcode[0x54] = [&]()->int {byteRegs[D] = byteRegs[H]; return 4; };
	opcode[0x55] = [&]()->int {byteRegs[D] = byteRegs[L]; return 4; };
	opcode[0x56] = [&]()->int {byteRegs[D] = readByte_(doubleRegAddr(H, L)); return 8; };
	opcode[0x57] = [&]()->int {byteRegs[D] = byteRegs[A]; return 4; };
	opcode[0x58] = [&]()->int {byteRegs[E] = byteRegs[B]; return 4; };
	opcode[0x59] = [&]()->int {byteRegs[E] = byteRegs[C]; return 4; };
	opcode[0x5A] = [&]()->int {byteRegs[E] = byteRegs[D]; return 4; };
	opcode[0x5B] = [&]()->int {return 4; };
	opcode[0x5C] = [&]()->int {byteRegs[E] = byteRegs[H]; return 4; };
	opcode[0x5D] = [&]()->int {byteRegs[E] = byteRegs[L]; return 4; };
	opcode[0x5E] = [&]()->int {byteRegs[E] = readByte_(doubleRegAddr(H, L)); return 8; };
	opcode[0x5F] = [&]()->int {byteRegs[E] = byteRegs[A]; return 4; };
	opcode[0x60] = [&]()->int {byteRegs[H] = byteRegs[B]; return 4; };
	opcode[0x61] = [&]()->int {byteRegs[H] = byteRegs[C]; return 4; };
	opcode[0x62] = [&]()->int {byteRegs[H] = byteRegs[D]; return 4; };
	opcode[0x63] = [&]()->int {byteRegs[H] = byteRegs[E]; return 4; };
	opcode[0x64] = [&]()->int { return 4; };
	opcode[0x65] = [&]()->int {byteRegs[H] = byteRegs[L]; return 4; };
	opcode[0x66] = [&]()->int {byteRegs[H] = readByte_(doubleRegAddr(H, L)); return 8; };
	opcode[0x67] = [&]()->int {byteRegs[H] = byteRegs[A]; return 4; };
	opcode[0x68] = [&]()->int {byteRegs[L] = byteRegs[B]; return 4; };
	opcode[0x69] = [&]()->int {byteRegs[L] = byteRegs[C]; return 4; };
	opcode[0x6A] = [&]()->int {byteRegs[L] = byteRegs[D]; return 4; };
	opcode[0x6B] = [&]()->int {byteRegs[L] = byteRegs[E]; return 4; };
	opcode[0x6C] = [&]()->int {byteRegs[L] = byteRegs[H]; return 4; };
	opcode[0x6D] = [&]()->int { return 4; };
	opcode[0x6E] = [&]()->int {byteRegs[L] = readByte_(doubleRegAddr(H, L)); return 8; };
	opcode[0x6F] = [&]()->int {byteRegs[L] = byteRegs[A]; return 4; };
	opcode[0x70] = [&]()->int {writeByte_(doubleRegAddr(H, L), byteRegs[B]); return 8; };
	opcode[0x71] = [&]()->int {writeByte_(doubleRegAddr(H, L), byteRegs[C]); return 8; };
	opcode[0x72] = [&]()->int {writeByte_(doubleRegAddr(H, L), byteRegs[D]); return 8; };
	opcode[0x73] = [&]()->int {writeByte_(doubleRegAddr(H, L), byteRegs[E]); return 8; };
	opcode[0x74] = [&]()->int {writeByte_(doubleRegAddr(H, L), byteRegs[H]); return 8; };
	opcode[0x75] = [&]()->int {writeByte_(doubleRegAddr(H, L), byteRegs[L]); return 8; };
	opcode[0x77] = [&]()->int {writeByte_(doubleRegAddr(H, L), byteRegs[A]); return 8; };
	opcode[0x78] = [&]()->int {byteRegs[A] = byteRegs[B]; return 8; };
	opcode[0x79] = [&]()->int {byteRegs[A] = byteRegs[C]; return 8; };
	opcode[0x7A] = [&]()->int {byteRegs[A] = byteRegs[D]; return 8; };
	opcode[0x7B] = [&]()->int {byteRegs[A] = byteRegs[E]; return 8; };
	opcode[0x7C] = [&]()->int {byteRegs[A] = byteRegs[H]; return 8; };
	opcode[0x7D] = [&]()->int {byteRegs[A] = byteRegs[L]; return 8; };
	opcode[0x7E] = [&]()->int {byteRegs[A] = readByte_(doubleRegAddr(H, L)); return 8; };
	opcode[0x7F] = [&]()->int {return 4; };

	//LD A,n n=(BC),(DE),(nn),#
	opcode[0x0A] = [&]()->int {byteRegs[A] = readByte_(doubleRegAddr(B, C)); return 8; };
	opcode[0x1A] = [&]()->int {byteRegs[A] = readByte_(doubleRegAddr(D, E)); return 8; };
	opcode[0xFA] = [&]()->int {byteRegs[A] = readByte_(readDouble_(regPC)); regPC += 2; return 16; };
	opcode[0x3E] = [&]()->int {byteRegs[A] = readByte_(regPC++); return 8; };

	//LD n,A n = (BC), (DE), (nn)
	opcode[0x02] = [&]()->int {writeByte_(doubleRegAddr(B, C), byteRegs[A]); return 8; };
	opcode[0x12] = [&]()->int {writeByte_(doubleRegAddr(D, E), byteRegs[A]); return 8; };
	opcode[0xEA] = [&]()->int {writeByte_(readByte_(readDouble_(regPC)), byteRegs[A]); regPC += 2; return 8; };

	//LD A,(C)
	opcode[0xF2] = [&]()->int {byteRegs[A] = readByte_(0xFF00 + byteRegs[C]); return 8; };

	//LD (C),A
	opcode[0xE2] = [&]()->int {writeByte_(0xFF00 + byteRegs[C], byteRegs[A]); return 8; };

	//LD A,(HLD)
	opcode[0x3A] = [&]()->int {opcode[0x7E](); opcode[0x2B](); return 8; };

	//LD (HLD),A
	opcode[0x32] = [&]()->int {opcode[0x77](); opcode[0x2B](); return 8; };

	//LD A,(HLI)
	opcode[0x2A] = [&]()->int {opcode[0x7E](); opcode[0x23](); return 8; };

	//LD (HLI),A 
	opcode[0x22] = [&]()->int {opcode[0x77](); opcode[0x23](); return 8; };

	//LDH (n),A
	opcode[0xE0] = [&]()->int {writeByte_(0xFF00 + readByte_(regPC++), byteRegs[A]); return 12; };

	//LDH A,(n)
	opcode[0xF0] = [&]()->int {byteRegs[A] = readByte_(0xFF00 + readByte_(regPC++)); return 12; };

	//16-bit load
	//LD n,nn n=BC,DE,HL,SP	  nn=#
	opcode[0x01] = [&]()->int {byteRegs[C] = readByte_(regPC++); byteRegs[B] = readByte_(regPC++); return 12; };
	opcode[0x11] = [&]()->int {byteRegs[E] = readByte_(regPC++); byteRegs[D] = readByte_(regPC++); return 12; };
	opcode[0x21] = [&]()->int {byteRegs[L] = readByte_(regPC++); byteRegs[H] = readByte_(regPC++); return 12; };
	opcode[0x31] = [&]()->int {regSP = readDouble_(regPC); regPC += 2; return 12; };

	//LD SP,HL
	opcode[0xF9] = [&]()->int {regSP = byteRegs[L] + (byteRegs[H] << 8); return 8; };

	//LD HL,SP+n
	opcode[0xF8] = [&]()->int {
		byte n = readByte_(regPC++); 
		doubleByte re = n + regSP;
		setFlag(FZ, false);
		setFlag(FN, false);
		setFlag(FH,(n^regSP^re)&0x10); 
		setFlag(FC, (n^regSP^re) & 0x100);
		regSP = re;
		return 12; 
	};

	//LD (nn),SP
	opcode[0x08] = [&]()->int { writeDouble_(readDouble_(regPC), regSP); regPC += 2; return 20; };

	//PUSH nn nn=AF,BC,DE,HL
	opcode[0xF5] = [&]()->int {regPC -= 2; writeDouble_(regSP, doubleReg(A, F)); return 16; };
	opcode[0xC5] = [&]()->int {regPC -= 2; writeDouble_(regSP, doubleReg(B, C)); return 16; };
	opcode[0xD5] = [&]()->int {regPC -= 2; writeDouble_(regSP, doubleReg(D, E)); return 16; };
	opcode[0xE5] = [&]()->int {regPC -= 2; writeDouble_(regSP, doubleReg(H, L)); return 16; };

	//POP nn
	opcode[0xF1] = [&]()->int {byteRegs[F] = readByte_(regSP++); byteRegs[A] = readByte_(regSP++); return 16; };
	opcode[0xC1] = [&]()->int {byteRegs[C] = readByte_(regSP++); byteRegs[B] = readByte_(regSP++); return 16; };
	opcode[0xD1] = [&]()->int {byteRegs[E] = readByte_(regSP++); byteRegs[D] = readByte_(regSP++); return 16; };
	opcode[0xE1] = [&]()->int {byteRegs[L] = readByte_(regSP++); byteRegs[H] = readByte_(regSP++); return 16; };

	//8-bit calculation
	//ADD A,n
	opcode[0x87] = [&]()->int {ADD(byteRegs[A]); return 4; };
	opcode[0x80] = [&]()->int {ADD(byteRegs[B]); return 4; };
	opcode[0x81] = [&]()->int {ADD(byteRegs[C]); return 4; };
	opcode[0x82] = [&]()->int {ADD(byteRegs[D]); return 4; };
	opcode[0x83] = [&]()->int {ADD(byteRegs[E]); return 4; };
	opcode[0x84] = [&]()->int {ADD(byteRegs[H]); return 4; };
	opcode[0x85] = [&]()->int {ADD(byteRegs[L]); return 4; };
	opcode[0x86] = [&]()->int {ADD(readByte_(doubleRegAddr(H, L))); return 8; };
	opcode[0xC6] = [&]()->int {ADD(readByte_(regPC++)); };

	//ADC A,n
	opcode[0x8F] = [&]()->int {ADC(byteRegs[A]); return 4; };
	opcode[0x88] = [&]()->int {ADC(byteRegs[B]); return 4; };
	opcode[0x89] = [&]()->int {ADC(byteRegs[C]); return 4; };
	opcode[0x8A] = [&]()->int {ADC(byteRegs[D]); return 4; };
	opcode[0x8B] = [&]()->int {ADC(byteRegs[E]); return 4; };
	opcode[0x8C] = [&]()->int {ADC(byteRegs[H]); return 4; };
	opcode[0x8D] = [&]()->int {ADC(byteRegs[L]); return 4; };
	opcode[0x8E] = [&]()->int {ADC(readByte_(doubleRegAddr(H, L))); return 8; };
	opcode[0xCE] = [&]()->int {ADC(readByte_(regPC++)); return 8; };

	//SUB A,n
	opcode[0x97] = [&]()->int {SUB(byteRegs[A]); return 4; };
	opcode[0x90] = [&]()->int {SUB(byteRegs[B]); return 4; };
	opcode[0x91] = [&]()->int {SUB(byteRegs[C]); return 4; };
	opcode[0x92] = [&]()->int {SUB(byteRegs[D]); return 4; };
	opcode[0x93] = [&]()->int {SUB(byteRegs[E]); return 4; };
	opcode[0x94] = [&]()->int {SUB(byteRegs[H]); return 4; };
	opcode[0x95] = [&]()->int {SUB(byteRegs[L]); return 4; };
	opcode[0x96] = [&]()->int {SUB(readByte_(doubleRegAddr(H, L))); return 8; };
	opcode[0xD6] = [&]()->int {SUB(readByte_(regPC++)); return 8; };

	//SBC A,n
	opcode[0x9F] = [&]()->int {SBC(byteRegs[A]); return 4; };
	opcode[0x98] = [&]()->int {SBC(byteRegs[B]); return 4; };
	opcode[0x99] = [&]()->int {SBC(byteRegs[C]); return 4; };
	opcode[0x9A] = [&]()->int {SBC(byteRegs[D]); return 4; };
	opcode[0x9B] = [&]()->int {SBC(byteRegs[E]); return 4; };
	opcode[0x9C] = [&]()->int {SBC(byteRegs[H]); return 4; };
	opcode[0x9D] = [&]()->int {SBC(byteRegs[L]); return 4; };
	opcode[0x9E] = [&]()->int {SBC(readByte_(doubleRegAddr(H, L))); return 8; };
	opcode[0xDE] = [&]()->int {SBC(readByte_(regPC++)); return 8; };

	//AND n
	opcode[0xA7] = [&]()->int {AND(byteRegs[A]); return 4; };
	opcode[0xA0] = [&]()->int {AND(byteRegs[B]); return 4; };
	opcode[0xA1] = [&]()->int {AND(byteRegs[C]); return 4; };
	opcode[0xA2] = [&]()->int {AND(byteRegs[D]); return 4; };
	opcode[0xA3] = [&]()->int {AND(byteRegs[E]); return 4; };
	opcode[0xA4] = [&]()->int {AND(byteRegs[H]); return 4; };
	opcode[0xA5] = [&]()->int {AND(byteRegs[L]); return 4; };
	opcode[0xA6] = [&]()->int {AND(readByte_(doubleRegAddr(H, L))); return 8; };
	opcode[0xE6] = [&]()->int {AND(readByte_(regPC++)); return 8; };

	//OR n
	opcode[0xB7] = [&]()->int {OR(byteRegs[A]); return 4; };
	opcode[0xB0] = [&]()->int {OR(byteRegs[B]); return 4; };
	opcode[0xB1] = [&]()->int {OR(byteRegs[C]); return 4; };
	opcode[0xB2] = [&]()->int {OR(byteRegs[D]); return 4; };
	opcode[0xB3] = [&]()->int {OR(byteRegs[E]); return 4; };
	opcode[0xB4] = [&]()->int {OR(byteRegs[H]); return 4; };
	opcode[0xB5] = [&]()->int {OR(byteRegs[L]); return 4; };
	opcode[0xB6] = [&]()->int {OR(readByte_(doubleRegAddr(H, L))); return 8; };
	opcode[0xF6] = [&]()->int {OR(readByte_(regPC++)); return 8; };

	//XOR n
	opcode[0xAF] = [&]()->int {XOR(byteRegs[A]); return 4; };
	opcode[0xA8] = [&]()->int {XOR(byteRegs[B]); return 4; };
	opcode[0xA9] = [&]()->int {XOR(byteRegs[C]); return 4; };
	opcode[0xAA] = [&]()->int {XOR(byteRegs[D]); return 4; };
	opcode[0xAB] = [&]()->int {XOR(byteRegs[E]); return 4; };
	opcode[0xAC] = [&]()->int {XOR(byteRegs[H]); return 4; };
	opcode[0xAD] = [&]()->int {XOR(byteRegs[L]); return 4; };
	opcode[0xAE] = [&]()->int {XOR(readByte_(doubleRegAddr(H, L))); return 8; };
	opcode[0xEE] = [&]()->int {XOR(readByte_(regPC++)); return 8; };

	//CP n
	opcode[0xBF] = [&]()->int {CP(byteRegs[A]); return 4; };
	opcode[0xB8] = [&]()->int {CP(byteRegs[B]); return 4; };
	opcode[0xB9] = [&]()->int {CP(byteRegs[C]); return 4; };
	opcode[0xBA] = [&]()->int {CP(byteRegs[D]); return 4; };
	opcode[0xBB] = [&]()->int {CP(byteRegs[E]); return 4; };
	opcode[0xBC] = [&]()->int {CP(byteRegs[H]); return 4; };
	opcode[0xBD] = [&]()->int {CP(byteRegs[L]); return 4; };
	opcode[0xBE] = [&]()->int {CP(readByte_(doubleRegAddr(H, L))); return 8; };
	opcode[0xFE] = [&]()->int {CP(readByte_(regPC++)); return 8; };

	//INC n
	opcode[0x3C] = [&]()->int {byteRegs[A] = INC(byteRegs[A]); return 4; };
	opcode[0x04] = [&]()->int {byteRegs[B] = INC(byteRegs[B]); return 4; };
	opcode[0x0C] = [&]()->int {byteRegs[C] = INC(byteRegs[C]); return 4; };
	opcode[0x14] = [&]()->int {byteRegs[D] = INC(byteRegs[D]); return 4; };
	opcode[0x1C] = [&]()->int {byteRegs[E] = INC(byteRegs[E]); return 4; };
	opcode[0x24] = [&]()->int {byteRegs[H] = INC(byteRegs[H]); return 4; };
	opcode[0x2C] = [&]()->int {byteRegs[L] = INC(byteRegs[L]); return 4; };
	opcode[0x34] = [&]()->int {writeByte_(doubleRegAddr(H, L), INC(readByte_(doubleRegAddr(H, L)))); return 12; };

	//DEC n
	opcode[0x3D] = [&]()->int {byteRegs[A] = DEC(byteRegs[A]); return 4; };
	opcode[0x05] = [&]()->int {byteRegs[B] = DEC(byteRegs[B]); return 4; };
	opcode[0x0D] = [&]()->int {byteRegs[C] = DEC(byteRegs[C]); return 4; };
	opcode[0x15] = [&]()->int {byteRegs[D] = DEC(byteRegs[D]); return 4; };
	opcode[0x1D] = [&]()->int {byteRegs[E] = DEC(byteRegs[E]); return 4; };
	opcode[0x25] = [&]()->int {byteRegs[H] = DEC(byteRegs[H]); return 4; };
	opcode[0x2D] = [&]()->int {byteRegs[L] = DEC(byteRegs[L]); return 4; };
	opcode[0x35] = [&]()->int {writeByte_(doubleRegAddr(H, L), DEC(readByte_(doubleRegAddr(H, L)))); return 12; };
};