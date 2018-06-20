#include "CPU.h"

void CPU::init(Storage &_sto)
{
	loadOpcode();
	simSto = &_sto;
	byteRegs={0x01,0xB0,0x00,0x13,0x00,0xD8,0x01,0x4D };
	regPC = 0x0100;
	regSP = 0xFFFE;
	deltaTime = 0;
}

int CPU::step() {
	byte IE = readByte_(interruptSwitchAdd);
	byte IF = readByte_(interruptFlagAdd);
	byte IStatus = IE & IF;
	//for interrupt proocess
	if (IStatus&&interruptEnabled) {
		if (haltFlag) {
			haltFlag = false;
		}
		auto processInterrupt = [&](interruptType intr) {
			interruptEnabled = false;
			regSP -= 2;
			writeDouble_(regSP, regPC);
			regPC = static_cast<doubleByte>(intr);
		};
		for (int i = 4; i >= 0; i--) {
			if (getBit(i, IStatus)) {
				setBit(i, IF, false);
				writeByte_(interruptFlagAdd, IF);
				interruptType type;
				switch (i) {
				case 4:
					type = keyPad; break;
				case 3:
					type = serialTrans; break;
				case 2:
					type = timerOverflow; break;
				case 1:
					type = LCDCStatus; break;
				case 0:
					type = Vblank; break;
				}
				processInterrupt(type);
				deltaTime = 12;
			}
		}
	}
	else {
		if (haltFlag) {
			return 4;
		}
		else {
			deltaTime = opcode[readByte_(regPC++)]();
		}
	}
	if (interruptSwitched) {
		interruptEnabled = !interruptEnabled;
		interruptSwitched = false;
	}
	return deltaTime;
}

void CPU::loadOpcode()
{
	//8bit-load
	//LD nn,n
	opcode[0x06] = [&]()->int {byteRegs[B] = readByte_(regPC++); return 8; };
	opcode[0x0E] = [&]()->int {byteRegs[C] = readByte_(regPC++); return 8; };
	opcode[0x16] = [&]()->int {byteRegs[D] = readByte_(regPC++); return 8; };
	opcode[0x1E] = [&]()->int {byteRegs[E] = readByte_(regPC++); return 8; };
	opcode[0x26] = [&]()->int {byteRegs[H] = readByte_(regPC++); return 8; };
	opcode[0x2E] = [&]()->int {byteRegs[L] = readByte_(regPC++); return 8; };
	opcode[0x36] = [&]()->int {writeByte_(doubleReg(H, L), readByte_(regPC++)); return 12; };

	//LD r1,r2
	//load to self are omitted
	opcode[0x40] = [&]()->int {return 4; };
	opcode[0x41] = [&]()->int {byteRegs[B] = byteRegs[C]; return 4; };
	opcode[0x42] = [&]()->int {byteRegs[B] = byteRegs[D]; return 4; };
	opcode[0x43] = [&]()->int {byteRegs[B] = byteRegs[E]; return 4; };
	opcode[0x44] = [&]()->int {byteRegs[B] = byteRegs[H]; return 4; };
	opcode[0x45] = [&]()->int {byteRegs[B] = byteRegs[L]; return 4; };
	opcode[0x46] = [&]()->int {byteRegs[B] = readByte_(doubleReg(H, L)); return 8; };
	opcode[0x47] = [&]()->int {byteRegs[B] = byteRegs[A]; return 4; };
	opcode[0x48] = [&]()->int {byteRegs[C] = byteRegs[B]; return 4; };
	opcode[0x49] = [&]()->int {return 4; };
	opcode[0x4A] = [&]()->int {byteRegs[C] = byteRegs[D]; return 4; };
	opcode[0x4B] = [&]()->int {byteRegs[C] = byteRegs[E]; return 4; };
	opcode[0x4C] = [&]()->int {byteRegs[C] = byteRegs[H]; return 4; };
	opcode[0x4D] = [&]()->int {byteRegs[C] = byteRegs[L]; return 4; };
	opcode[0x4E] = [&]()->int {byteRegs[C] = readByte_(doubleReg(H, L)); return 8; };
	opcode[0x4F] = [&]()->int {byteRegs[C] = byteRegs[A]; return 4; };
	opcode[0x50] = [&]()->int {byteRegs[D] = byteRegs[B]; return 4; };
	opcode[0x51] = [&]()->int {byteRegs[D] = byteRegs[C]; return 4; };
	opcode[0x52] = [&]()->int {return 4; };
	opcode[0x53] = [&]()->int {byteRegs[D] = byteRegs[E]; return 4; };
	opcode[0x54] = [&]()->int {byteRegs[D] = byteRegs[H]; return 4; };
	opcode[0x55] = [&]()->int {byteRegs[D] = byteRegs[L]; return 4; };
	opcode[0x56] = [&]()->int {byteRegs[D] = readByte_(doubleReg(H, L)); return 8; };
	opcode[0x57] = [&]()->int {byteRegs[D] = byteRegs[A]; return 4; };
	opcode[0x58] = [&]()->int {byteRegs[E] = byteRegs[B]; return 4; };
	opcode[0x59] = [&]()->int {byteRegs[E] = byteRegs[C]; return 4; };
	opcode[0x5A] = [&]()->int {byteRegs[E] = byteRegs[D]; return 4; };
	opcode[0x5B] = [&]()->int {return 4; };
	opcode[0x5C] = [&]()->int {byteRegs[E] = byteRegs[H]; return 4; };
	opcode[0x5D] = [&]()->int {byteRegs[E] = byteRegs[L]; return 4; };
	opcode[0x5E] = [&]()->int {byteRegs[E] = readByte_(doubleReg(H, L)); return 8; };
	opcode[0x5F] = [&]()->int {byteRegs[E] = byteRegs[A]; return 4; };
	opcode[0x60] = [&]()->int {byteRegs[H] = byteRegs[B]; return 4; };
	opcode[0x61] = [&]()->int {byteRegs[H] = byteRegs[C]; return 4; };
	opcode[0x62] = [&]()->int {byteRegs[H] = byteRegs[D]; return 4; };
	opcode[0x63] = [&]()->int {byteRegs[H] = byteRegs[E]; return 4; };
	opcode[0x64] = [&]()->int { return 4; };
	opcode[0x65] = [&]()->int {byteRegs[H] = byteRegs[L]; return 4; };
	opcode[0x66] = [&]()->int {byteRegs[H] = readByte_(doubleReg(H, L)); return 8; };
	opcode[0x67] = [&]()->int {byteRegs[H] = byteRegs[A]; return 4; };
	opcode[0x68] = [&]()->int {byteRegs[L] = byteRegs[B]; return 4; };
	opcode[0x69] = [&]()->int {byteRegs[L] = byteRegs[C]; return 4; };
	opcode[0x6A] = [&]()->int {byteRegs[L] = byteRegs[D]; return 4; };
	opcode[0x6B] = [&]()->int {byteRegs[L] = byteRegs[E]; return 4; };
	opcode[0x6C] = [&]()->int {byteRegs[L] = byteRegs[H]; return 4; };
	opcode[0x6D] = [&]()->int { return 4; };
	opcode[0x6E] = [&]()->int {byteRegs[L] = readByte_(doubleReg(H, L)); return 8; };
	opcode[0x6F] = [&]()->int {byteRegs[L] = byteRegs[A]; return 4; };
	opcode[0x70] = [&]()->int {writeByte_(doubleReg(H, L), byteRegs[B]); return 8; };
	opcode[0x71] = [&]()->int {writeByte_(doubleReg(H, L), byteRegs[C]); return 8; };
	opcode[0x72] = [&]()->int {writeByte_(doubleReg(H, L), byteRegs[D]); return 8; };
	opcode[0x73] = [&]()->int {writeByte_(doubleReg(H, L), byteRegs[E]); return 8; };
	opcode[0x74] = [&]()->int {writeByte_(doubleReg(H, L), byteRegs[H]); return 8; };
	opcode[0x75] = [&]()->int {writeByte_(doubleReg(H, L), byteRegs[L]); return 8; };
	opcode[0x77] = [&]()->int {writeByte_(doubleReg(H, L), byteRegs[A]); return 8; };
	opcode[0x78] = [&]()->int {byteRegs[A] = byteRegs[B]; return 4; };
	opcode[0x79] = [&]()->int {byteRegs[A] = byteRegs[C]; return 4; };
	opcode[0x7A] = [&]()->int {byteRegs[A] = byteRegs[D]; return 4; };
	opcode[0x7B] = [&]()->int {byteRegs[A] = byteRegs[E]; return 4; };
	opcode[0x7C] = [&]()->int {byteRegs[A] = byteRegs[H]; return 4; };
	opcode[0x7D] = [&]()->int {byteRegs[A] = byteRegs[L]; return 4; };
	opcode[0x7E] = [&]()->int {byteRegs[A] = readByte_(doubleReg(H, L)); return 8; };
	opcode[0x7F] = [&]()->int {return 4; };

	//LD A,n n=(BC),(DE),(nn),#
	opcode[0x0A] = [&]()->int {byteRegs[A] = readByte_(doubleReg(B, C)); return 8; };
	opcode[0x1A] = [&]()->int {byteRegs[A] = readByte_(doubleReg(D, E)); return 8; };
	opcode[0xFA] = [&]()->int {byteRegs[A] = readByte_(readDouble_(regPC)); regPC += 2; return 16; };
	opcode[0x3E] = [&]()->int {byteRegs[A] = readByte_(regPC++); return 8; };

	//LD n,A n = (BC), (DE), (nn)
	opcode[0x02] = [&]()->int {writeByte_(doubleReg(B, C), byteRegs[A]); return 8; };
	opcode[0x12] = [&]()->int {writeByte_(doubleReg(D, E), byteRegs[A]); return 8; };
	opcode[0xEA] = [&]()->int {writeByte_(readDouble_(regPC), byteRegs[A]); regPC += 2; return 16; };

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
			signedByte& x = reinterpret_cast<signedByte&>(n);
			doubleByte re = x + regSP;
			setFlag(FZ, false);
			setFlag(FN, false);
			setFlag(FH, (x^regSP^re) & 0x10);
			setFlag(FC, (x^regSP^re) & 0x100);
			byteRegs[H] = (re >> 8) & 0xFF;
			byteRegs[L] = re & 0xFF;
			return 12;
		};

	//LD (nn),SP
	opcode[0x08] = [&]()->int { writeDouble_(readDouble_(regPC), regSP); regPC += 2; return 20; };

	//PUSH nn nn=AF,BC,DE,HL
	opcode[0xF5] = [&]()->int {regSP -= 2; writeDouble_(regSP, doubleReg(A, F)); return 16; };
	opcode[0xC5] = [&]()->int {regSP -= 2; writeDouble_(regSP, doubleReg(B, C)); return 16; };
	opcode[0xD5] = [&]()->int {regSP -= 2; writeDouble_(regSP, doubleReg(D, E)); return 16; };
	opcode[0xE5] = [&]()->int {regSP -= 2; writeDouble_(regSP, doubleReg(H, L)); return 16; };

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
	opcode[0x86] = [&]()->int {ADD(readByte_(doubleReg(H, L))); return 8; };
	opcode[0xC6] = [&]()->int {ADD(readByte_(regPC++)); return 8; };

	//ADC A,n
	opcode[0x8F] = [&]()->int {ADC(byteRegs[A]); return 4; };
	opcode[0x88] = [&]()->int {ADC(byteRegs[B]); return 4; };
	opcode[0x89] = [&]()->int {ADC(byteRegs[C]); return 4; };
	opcode[0x8A] = [&]()->int {ADC(byteRegs[D]); return 4; };
	opcode[0x8B] = [&]()->int {ADC(byteRegs[E]); return 4; };
	opcode[0x8C] = [&]()->int {ADC(byteRegs[H]); return 4; };
	opcode[0x8D] = [&]()->int {ADC(byteRegs[L]); return 4; };
	opcode[0x8E] = [&]()->int {ADC(readByte_(doubleReg(H, L))); return 8; };
	opcode[0xCE] = [&]()->int {ADC(readByte_(regPC++)); return 8; };

	//SUB A,n
	opcode[0x97] = [&]()->int {SUB(byteRegs[A]); return 4; };
	opcode[0x90] = [&]()->int {SUB(byteRegs[B]); return 4; };
	opcode[0x91] = [&]()->int {SUB(byteRegs[C]); return 4; };
	opcode[0x92] = [&]()->int {SUB(byteRegs[D]); return 4; };
	opcode[0x93] = [&]()->int {SUB(byteRegs[E]); return 4; };
	opcode[0x94] = [&]()->int {SUB(byteRegs[H]); return 4; };
	opcode[0x95] = [&]()->int {SUB(byteRegs[L]); return 4; };
	opcode[0x96] = [&]()->int {SUB(readByte_(doubleReg(H, L))); return 8; };
	opcode[0xD6] = [&]()->int {SUB(readByte_(regPC++)); return 8; };

	//SBC A,n
	opcode[0x9F] = [&]()->int {SBC(byteRegs[A]); return 4; };
	opcode[0x98] = [&]()->int {SBC(byteRegs[B]); return 4; };
	opcode[0x99] = [&]()->int {SBC(byteRegs[C]); return 4; };
	opcode[0x9A] = [&]()->int {SBC(byteRegs[D]); return 4; };
	opcode[0x9B] = [&]()->int {SBC(byteRegs[E]); return 4; };
	opcode[0x9C] = [&]()->int {SBC(byteRegs[H]); return 4; };
	opcode[0x9D] = [&]()->int {SBC(byteRegs[L]); return 4; };
	opcode[0x9E] = [&]()->int {SBC(readByte_(doubleReg(H, L))); return 8; };
	opcode[0xDE] = [&]()->int {SBC(readByte_(regPC++)); return 8; };

	//AND n
	opcode[0xA7] = [&]()->int {AND(byteRegs[A]); return 4; };
	opcode[0xA0] = [&]()->int {AND(byteRegs[B]); return 4; };
	opcode[0xA1] = [&]()->int {AND(byteRegs[C]); return 4; };
	opcode[0xA2] = [&]()->int {AND(byteRegs[D]); return 4; };
	opcode[0xA3] = [&]()->int {AND(byteRegs[E]); return 4; };
	opcode[0xA4] = [&]()->int {AND(byteRegs[H]); return 4; };
	opcode[0xA5] = [&]()->int {AND(byteRegs[L]); return 4; };
	opcode[0xA6] = [&]()->int {AND(readByte_(doubleReg(H, L))); return 8; };
	opcode[0xE6] = [&]()->int {AND(readByte_(regPC++)); return 8; };

	//OR n
	opcode[0xB7] = [&]()->int {OR(byteRegs[A]); return 4; };
	opcode[0xB0] = [&]()->int {OR(byteRegs[B]); return 4; };
	opcode[0xB1] = [&]()->int {OR(byteRegs[C]); return 4; };
	opcode[0xB2] = [&]()->int {OR(byteRegs[D]); return 4; };
	opcode[0xB3] = [&]()->int {OR(byteRegs[E]); return 4; };
	opcode[0xB4] = [&]()->int {OR(byteRegs[H]); return 4; };
	opcode[0xB5] = [&]()->int {OR(byteRegs[L]); return 4; };
	opcode[0xB6] = [&]()->int {OR(readByte_(doubleReg(H, L))); return 8; };
	opcode[0xF6] = [&]()->int {OR(readByte_(regPC++)); return 8; };

	//XOR n
	opcode[0xAF] = [&]()->int {XOR(byteRegs[A]); return 4; };
	opcode[0xA8] = [&]()->int {XOR(byteRegs[B]); return 4; };
	opcode[0xA9] = [&]()->int {XOR(byteRegs[C]); return 4; };
	opcode[0xAA] = [&]()->int {XOR(byteRegs[D]); return 4; };
	opcode[0xAB] = [&]()->int {XOR(byteRegs[E]); return 4; };
	opcode[0xAC] = [&]()->int {XOR(byteRegs[H]); return 4; };
	opcode[0xAD] = [&]()->int {XOR(byteRegs[L]); return 4; };
	opcode[0xAE] = [&]()->int {XOR(readByte_(doubleReg(H, L))); return 8; };
	opcode[0xEE] = [&]()->int {XOR(readByte_(regPC++)); return 8; };

	//CP n
	opcode[0xBF] = [&]()->int {CP(byteRegs[A]); return 4; };
	opcode[0xB8] = [&]()->int {CP(byteRegs[B]); return 4; };
	opcode[0xB9] = [&]()->int {CP(byteRegs[C]); return 4; };
	opcode[0xBA] = [&]()->int {CP(byteRegs[D]); return 4; };
	opcode[0xBB] = [&]()->int {CP(byteRegs[E]); return 4; };
	opcode[0xBC] = [&]()->int {CP(byteRegs[H]); return 4; };
	opcode[0xBD] = [&]()->int {CP(byteRegs[L]); return 4; };
	opcode[0xBE] = [&]()->int {CP(readByte_(doubleReg(H, L))); return 8; };
	opcode[0xFE] = [&]()->int {CP(readByte_(regPC++)); return 8; };

	//INC n
	opcode[0x3C] = [&]()->int {byteRegs[A] = INC(byteRegs[A]); return 4; };
	opcode[0x04] = [&]()->int {byteRegs[B] = INC(byteRegs[B]); return 4; };
	opcode[0x0C] = [&]()->int {byteRegs[C] = INC(byteRegs[C]); return 4; };
	opcode[0x14] = [&]()->int {byteRegs[D] = INC(byteRegs[D]); return 4; };
	opcode[0x1C] = [&]()->int {byteRegs[E] = INC(byteRegs[E]); return 4; };
	opcode[0x24] = [&]()->int {byteRegs[H] = INC(byteRegs[H]); return 4; };
	opcode[0x2C] = [&]()->int {byteRegs[L] = INC(byteRegs[L]); return 4; };
	opcode[0x34] = [&]()->int {writeByte_(doubleReg(H, L), INC(readByte_(doubleReg(H, L)))); return 12; };

	//DEC n
	opcode[0x3D] = [&]()->int {byteRegs[A] = DEC(byteRegs[A]); return 4; };
	opcode[0x05] = [&]()->int {byteRegs[B] = DEC(byteRegs[B]); return 4; };
	opcode[0x0D] = [&]()->int {byteRegs[C] = DEC(byteRegs[C]); return 4; };
	opcode[0x15] = [&]()->int {byteRegs[D] = DEC(byteRegs[D]); return 4; };
	opcode[0x1D] = [&]()->int {byteRegs[E] = DEC(byteRegs[E]); return 4; };
	opcode[0x25] = [&]()->int {byteRegs[H] = DEC(byteRegs[H]); return 4; };
	opcode[0x2D] = [&]()->int {byteRegs[L] = DEC(byteRegs[L]); return 4; };
	opcode[0x35] = [&]()->int {writeByte_(doubleReg(H, L), DEC(readByte_(doubleReg(H, L)))); return 12; };

	//16-bit calculation
	//ADD HL,n
	opcode[0x09] = [&]()->int {ADD(doubleReg(B, C)); return 8; };
	opcode[0x19] = [&]()->int {ADD(doubleReg(D, E)); return 8; };
	opcode[0x29] = [&]()->int {ADD(doubleReg(H, L)); return 8; };
	opcode[0x39] = [&]()->int {ADD(regSP); return 8; };

	//ADD SP,n
	opcode[0xE8] = [&]()->int {byte&& tmp = readByte_(regPC++); ADD(reinterpret_cast<signedByte&>(tmp)); return 16; };

	//INC nn
	opcode[0x03] = [&]()->int {if (++byteRegs[C] == 0)byteRegs[B]++; return 8; };
	opcode[0x13] = [&]()->int {if (++byteRegs[E] == 0)byteRegs[D]++; return 8; };
	opcode[0x23] = [&]()->int {if (++byteRegs[L] == 0)byteRegs[H]++; return 8; };
	opcode[0x33] = [&]()->int {regSP++; return 8; };

	//DEC nn
	opcode[0x0B] = [&]()->int {if (--byteRegs[C] == 0xFF)byteRegs[B]--; return 8; };
	opcode[0x1B] = [&]()->int {if (--byteRegs[E] == 0xFF)byteRegs[D]--; return 8; };
	opcode[0x2B] = [&]()->int {if (--byteRegs[L] == 0xFF)byteRegs[H]--; return 8; };
	opcode[0x3B] = [&]()->int {regSP--; return 8; };

	//Miscellaneous
	//DAA	
	opcode[0x27] = [&]()->int {
		byte n = 0;
		doubleByte re;
		if (byteRegs[A] & 0x0F > 0b1001) {
			n += 0x06;
		}
		if ((byteRegs[A] >> 4) > 0b1001) {
			n += 0x60;
		}
		re = byteRegs[A] + n;
		setFlag(FZ, re == 0);
		setFlag(FH, false);
		setFlag(FC, re & 0x100);
		byteRegs[A] = re & 0xFF;
		return 4;
	};

	//SWAP n
	CBopcode[0x37] = [&]()->int {byteRegs[A] = (byteRegs[A] >> 4) + (byteRegs[A] << 4); setFlag(FZ, !byteRegs[A]); setFlag(FN, false); setFlag(FH, false); setFlag(FC, false); return 8; };
	CBopcode[0x30] = [&]()->int {byteRegs[B] = (byteRegs[B] >> 4) + (byteRegs[B] << 4); setFlag(FZ, !byteRegs[B]); setFlag(FN, false); setFlag(FH, false); setFlag(FC, false); return 8; };
	CBopcode[0x31] = [&]()->int {byteRegs[C] = (byteRegs[C] >> 4) + (byteRegs[C] << 4); setFlag(FZ, !byteRegs[C]); setFlag(FN, false); setFlag(FH, false); setFlag(FC, false); return 8; };
	CBopcode[0x32] = [&]()->int {byteRegs[D] = (byteRegs[D] >> 4) + (byteRegs[D] << 4); setFlag(FZ, !byteRegs[D]); setFlag(FN, false); setFlag(FH, false); setFlag(FC, false); return 8; };
	CBopcode[0x33] = [&]()->int {byteRegs[E] = (byteRegs[E] >> 4) + (byteRegs[E] << 4); setFlag(FZ, !byteRegs[E]); setFlag(FN, false); setFlag(FH, false); setFlag(FC, false); return 8; };
	CBopcode[0x34] = [&]()->int {byteRegs[H] = (byteRegs[H] >> 4) + (byteRegs[H] << 4); setFlag(FZ, !byteRegs[H]); setFlag(FN, false); setFlag(FH, false); setFlag(FC, false); return 8; };
	CBopcode[0x35] = [&]()->int {byteRegs[L] = (byteRegs[L] >> 4) + (byteRegs[L] << 4); setFlag(FZ, !byteRegs[L]); setFlag(FN, false); setFlag(FH, false); setFlag(FC, false); return 8; };
	CBopcode[0x36] = [&]()->int {byte tmp = readByte_(doubleReg(H, L)); tmp = (tmp >> 4) + (tmp << 4); writeByte_(doubleReg(H, L), tmp); setFlag(FZ, !tmp); setFlag(FN, false); setFlag(FH, false); setFlag(FC, false); return 16; };

	//CPL
	opcode[0x2F] = [&]()->int {byteRegs[A] = ~byteRegs[A]; setFlag(FN, true); setFlag(FH, true); return 4; };

	//CCL
	opcode[0x3F] = [&]()->int {setFlag(FC, !getFlag(FC)); setFlag(FN, false); setFlag(FH, false); return 4; };

	//SCF
	opcode[0x37] = [&]()->int {setFlag(FC, true); setFlag(FN, false); setFlag(FH, false); return 4; };

	//NOP
	opcode[0x00] = [&]()->int {return 4; };

	//HALT
	opcode[0x76] = [&]()->int {haltFlag = true; return 4; };

	//STOP
	opcode[0x10] = [&]()->int {regPC++; haltFlag = true; return 4; };

	//DI and EI
	opcode[0xF3] = [&]()->int {if(interruptEnabled)interruptSwitched = true; return 4; };
	opcode[0xFB] = [&]()->int {if (!interruptEnabled)interruptSwitched = true; return 4; };

	//scrolls and shifts
	//RLCA
	opcode[0x07] = [&]()->int {byteRegs[A] = RLC(byteRegs[A]); return 4; };

	//RLA
	opcode[0x17] = [&]()->int {byteRegs[A] = RL(byteRegs[A]); return 4;  };

	//RRCA
	opcode[0x0F] = [&]()->int {byteRegs[A] = RRC(byteRegs[A]); return 4; };

	//RRA
	opcode[0x1F] = [&]()->int {byteRegs[A] = RR(byteRegs[A]); return 4; };

	//RLC n
	CBopcode[0x07] = [&]()->int {byteRegs[A] = RLC(byteRegs[A]); return 8; };
	CBopcode[0x00] = [&]()->int {byteRegs[B] = RLC(byteRegs[B]); return 8; };
	CBopcode[0x01] = [&]()->int {byteRegs[C] = RLC(byteRegs[C]); return 8; };
	CBopcode[0x02] = [&]()->int {byteRegs[D] = RLC(byteRegs[D]); return 8; };
	CBopcode[0x03] = [&]()->int {byteRegs[E] = RLC(byteRegs[E]); return 8; };
	CBopcode[0x04] = [&]()->int {byteRegs[H] = RLC(byteRegs[H]); return 8; };
	CBopcode[0x05] = [&]()->int {byteRegs[L] = RLC(byteRegs[L]); return 8; };
	CBopcode[0x06] = [&]()->int {writeByte_(doubleReg(H, L), RLC(readByte_(doubleReg(H, L)))); return 16; };

	//RL n
	CBopcode[0x17] = [&]()->int {byteRegs[A] = RL(byteRegs[A]); return 8; };
	CBopcode[0x10] = [&]()->int {byteRegs[B] = RL(byteRegs[B]); return 8; };
	CBopcode[0x11] = [&]()->int {byteRegs[C] = RL(byteRegs[C]); return 8; };
	CBopcode[0x12] = [&]()->int {byteRegs[D] = RL(byteRegs[D]); return 8; };
	CBopcode[0x13] = [&]()->int {byteRegs[E] = RL(byteRegs[E]); return 8; };
	CBopcode[0x14] = [&]()->int {byteRegs[H] = RL(byteRegs[H]); return 8; };
	CBopcode[0x15] = [&]()->int {byteRegs[L] = RL(byteRegs[L]); return 8; };
	CBopcode[0x16] = [&]()->int {writeByte_(doubleReg(H, L), RL(readByte_(doubleReg(H, L)))); return 16; };

	//RRC n
	CBopcode[0x0F] = [&]()->int {byteRegs[A] = RRC(byteRegs[A]); return 8; };
	CBopcode[0x08] = [&]()->int {byteRegs[B] = RRC(byteRegs[B]); return 8; };
	CBopcode[0x09] = [&]()->int {byteRegs[C] = RRC(byteRegs[C]); return 8; };
	CBopcode[0x0A] = [&]()->int {byteRegs[D] = RRC(byteRegs[D]); return 8; };
	CBopcode[0x0B] = [&]()->int {byteRegs[E] = RRC(byteRegs[E]); return 8; };
	CBopcode[0x0C] = [&]()->int {byteRegs[H] = RRC(byteRegs[H]); return 8; };
	CBopcode[0x0D] = [&]()->int {byteRegs[L] = RRC(byteRegs[L]); return 8; };
	CBopcode[0x0E] = [&]()->int {writeByte_(doubleReg(H, L), RRC(readByte_(doubleReg(H, L)))); return 16; };

	//RR n
	CBopcode[0x1F] = [&]()->int {byteRegs[A] = RR(byteRegs[A]); return 8; };
	CBopcode[0x18] = [&]()->int {byteRegs[B] = RR(byteRegs[B]); return 8; };
	CBopcode[0x19] = [&]()->int {byteRegs[C] = RR(byteRegs[C]); return 8; };
	CBopcode[0x1A] = [&]()->int {byteRegs[D] = RR(byteRegs[D]); return 8; };
	CBopcode[0x1B] = [&]()->int {byteRegs[E] = RR(byteRegs[E]); return 8; };
	CBopcode[0x1C] = [&]()->int {byteRegs[H] = RR(byteRegs[H]); return 8; };
	CBopcode[0x1D] = [&]()->int {byteRegs[L] = RR(byteRegs[L]); return 8; };
	CBopcode[0x1E] = [&]()->int {writeByte_(doubleReg(H, L), RR(readByte_(doubleReg(H, L)))); return 16; };

	//SLA n
	CBopcode[0x27] = [&]()->int {byteRegs[A] = SLA(byteRegs[A]); return 8; };
	CBopcode[0x20] = [&]()->int {byteRegs[B] = SLA(byteRegs[B]); return 8; };
	CBopcode[0x21] = [&]()->int {byteRegs[C] = SLA(byteRegs[C]); return 8; };
	CBopcode[0x22] = [&]()->int {byteRegs[D] = SLA(byteRegs[D]); return 8; };
	CBopcode[0x23] = [&]()->int {byteRegs[E] = SLA(byteRegs[E]); return 8; };
	CBopcode[0x24] = [&]()->int {byteRegs[H] = SLA(byteRegs[H]); return 8; };
	CBopcode[0x25] = [&]()->int {byteRegs[L] = SLA(byteRegs[L]); return 8; };
	CBopcode[0x26] = [&]()->int {writeByte_(doubleReg(H, L), SLA(readByte_(doubleReg(H, L)))); return 16; };

	//SRA n
	CBopcode[0x2F] = [&]()->int {byteRegs[A] = SRA(byteRegs[A]); return 8; };
	CBopcode[0x28] = [&]()->int {byteRegs[B] = SRA(byteRegs[B]); return 8; };
	CBopcode[0x29] = [&]()->int {byteRegs[C] = SRA(byteRegs[C]); return 8; };
	CBopcode[0x2A] = [&]()->int {byteRegs[D] = SRA(byteRegs[D]); return 8; };
	CBopcode[0x2B] = [&]()->int {byteRegs[E] = SRA(byteRegs[E]); return 8; };
	CBopcode[0x2C] = [&]()->int {byteRegs[H] = SRA(byteRegs[H]); return 8; };
	CBopcode[0x2D] = [&]()->int {byteRegs[L] = SRA(byteRegs[L]); return 8; };
	CBopcode[0x2E] = [&]()->int {writeByte_(doubleReg(H, L), SRA(readByte_(doubleReg(H, L)))); return 16; };

	//SRL n
	CBopcode[0x3F] = [&]()->int {byteRegs[A] = SRL(byteRegs[A]); return 8; };
	CBopcode[0x38] = [&]()->int {byteRegs[B] = SRL(byteRegs[B]); return 8; };
	CBopcode[0x39] = [&]()->int {byteRegs[C] = SRL(byteRegs[C]); return 8; };
	CBopcode[0x3A] = [&]()->int {byteRegs[D] = SRL(byteRegs[D]); return 8; };
	CBopcode[0x3B] = [&]()->int {byteRegs[E] = SRL(byteRegs[E]); return 8; };
	CBopcode[0x3C] = [&]()->int {byteRegs[H] = SRL(byteRegs[H]); return 8; };
	CBopcode[0x3D] = [&]()->int {byteRegs[L] = SRL(byteRegs[L]); return 8; };
	CBopcode[0x3E] = [&]()->int {writeByte_(doubleReg(H, L), SRL(readByte_(doubleReg(H, L)))); return 16; };

	//bit ops
	//BIT b,r
	//BIT b,A
	CBopcode[0x47] = [&]()->int {setFlag(FZ, !getBit(0, byteRegs[A])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x4F] = [&]()->int {setFlag(FZ, !getBit(1, byteRegs[A])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x57] = [&]()->int {setFlag(FZ, !getBit(2, byteRegs[A])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x5F] = [&]()->int {setFlag(FZ, !getBit(3, byteRegs[A])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x67] = [&]()->int {setFlag(FZ, !getBit(4, byteRegs[A])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x6F] = [&]()->int {setFlag(FZ, !getBit(5, byteRegs[A])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x77] = [&]()->int {setFlag(FZ, !getBit(6, byteRegs[A])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x7F] = [&]()->int {setFlag(FZ, !getBit(7, byteRegs[A])); setFlag(FN, false); setFlag(FH, true); return 8; };

	//BIT b,B
	CBopcode[0x40] = [&]()->int {setFlag(FZ, !getBit(0, byteRegs[B])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x48] = [&]()->int {setFlag(FZ, !getBit(1, byteRegs[B])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x50] = [&]()->int {setFlag(FZ, !getBit(2, byteRegs[B])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x58] = [&]()->int {setFlag(FZ, !getBit(3, byteRegs[B])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x60] = [&]()->int {setFlag(FZ, !getBit(4, byteRegs[B])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x68] = [&]()->int {setFlag(FZ, !getBit(5, byteRegs[B])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x70] = [&]()->int {setFlag(FZ, !getBit(6, byteRegs[B])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x78] = [&]()->int {setFlag(FZ, !getBit(7, byteRegs[B])); setFlag(FN, false); setFlag(FH, true); return 8; };

	//BIT b,C
	CBopcode[0x41] = [&]()->int {setFlag(FZ, !getBit(0, byteRegs[C])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x49] = [&]()->int {setFlag(FZ, !getBit(1, byteRegs[C])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x51] = [&]()->int {setFlag(FZ, !getBit(2, byteRegs[C])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x59] = [&]()->int {setFlag(FZ, !getBit(3, byteRegs[C])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x61] = [&]()->int {setFlag(FZ, !getBit(4, byteRegs[C])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x69] = [&]()->int {setFlag(FZ, !getBit(5, byteRegs[C])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x71] = [&]()->int {setFlag(FZ, !getBit(6, byteRegs[C])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x79] = [&]()->int {setFlag(FZ, !getBit(7, byteRegs[C])); setFlag(FN, false); setFlag(FH, true); return 8; };

	//BIT b,D
	CBopcode[0x42] = [&]()->int {setFlag(FZ, !getBit(0, byteRegs[D])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x4A] = [&]()->int {setFlag(FZ, !getBit(1, byteRegs[D])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x52] = [&]()->int {setFlag(FZ, !getBit(2, byteRegs[D])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x5A] = [&]()->int {setFlag(FZ, !getBit(3, byteRegs[D])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x62] = [&]()->int {setFlag(FZ, !getBit(4, byteRegs[D])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x6A] = [&]()->int {setFlag(FZ, !getBit(5, byteRegs[D])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x72] = [&]()->int {setFlag(FZ, !getBit(6, byteRegs[D])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x7A] = [&]()->int {setFlag(FZ, !getBit(7, byteRegs[D])); setFlag(FN, false); setFlag(FH, true); return 8; };

	//BIT b,E
	CBopcode[0x43] = [&]()->int {setFlag(FZ, !getBit(0, byteRegs[E])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x4B] = [&]()->int {setFlag(FZ, !getBit(1, byteRegs[E])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x53] = [&]()->int {setFlag(FZ, !getBit(2, byteRegs[E])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x5B] = [&]()->int {setFlag(FZ, !getBit(3, byteRegs[E])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x63] = [&]()->int {setFlag(FZ, !getBit(4, byteRegs[E])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x6B] = [&]()->int {setFlag(FZ, !getBit(5, byteRegs[E])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x73] = [&]()->int {setFlag(FZ, !getBit(6, byteRegs[E])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x7B] = [&]()->int {setFlag(FZ, !getBit(7, byteRegs[E])); setFlag(FN, false); setFlag(FH, true); return 8; };

	//BIT b,H
	CBopcode[0x44] = [&]()->int {setFlag(FZ, !getBit(0, byteRegs[H])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x4C] = [&]()->int {setFlag(FZ, !getBit(1, byteRegs[H])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x54] = [&]()->int {setFlag(FZ, !getBit(2, byteRegs[H])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x5C] = [&]()->int {setFlag(FZ, !getBit(3, byteRegs[H])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x64] = [&]()->int {setFlag(FZ, !getBit(4, byteRegs[H])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x6C] = [&]()->int {setFlag(FZ, !getBit(5, byteRegs[H])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x74] = [&]()->int {setFlag(FZ, !getBit(6, byteRegs[H])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x7C] = [&]()->int {setFlag(FZ, !getBit(7, byteRegs[H])); setFlag(FN, false); setFlag(FH, true); return 8; };

	//BIT b,L
	CBopcode[0x45] = [&]()->int {setFlag(FZ, !getBit(0, byteRegs[L])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x4D] = [&]()->int {setFlag(FZ, !getBit(1, byteRegs[L])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x55] = [&]()->int {setFlag(FZ, !getBit(2, byteRegs[L])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x5D] = [&]()->int {setFlag(FZ, !getBit(3, byteRegs[L])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x65] = [&]()->int {setFlag(FZ, !getBit(4, byteRegs[L])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x6D] = [&]()->int {setFlag(FZ, !getBit(5, byteRegs[L])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x75] = [&]()->int {setFlag(FZ, !getBit(6, byteRegs[L])); setFlag(FN, false); setFlag(FH, true); return 8; };
	CBopcode[0x7D] = [&]()->int {setFlag(FZ, !getBit(7, byteRegs[L])); setFlag(FN, false); setFlag(FH, true); return 8; };

	//BIT b,(HL)
	CBopcode[0x46] = [&]()->int {setFlag(FZ, !getBit(0, readByte_(doubleReg(H, L)))); setFlag(FN, false); setFlag(FH, true); return 16; };
	CBopcode[0x4E] = [&]()->int {setFlag(FZ, !getBit(1, readByte_(doubleReg(H, L)))); setFlag(FN, false); setFlag(FH, true); return 16; };
	CBopcode[0x56] = [&]()->int {setFlag(FZ, !getBit(2, readByte_(doubleReg(H, L)))); setFlag(FN, false); setFlag(FH, true); return 16; };
	CBopcode[0x5E] = [&]()->int {setFlag(FZ, !getBit(3, readByte_(doubleReg(H, L)))); setFlag(FN, false); setFlag(FH, true); return 16; };
	CBopcode[0x66] = [&]()->int {setFlag(FZ, !getBit(4, readByte_(doubleReg(H, L)))); setFlag(FN, false); setFlag(FH, true); return 16; };
	CBopcode[0x6E] = [&]()->int {setFlag(FZ, !getBit(5, readByte_(doubleReg(H, L)))); setFlag(FN, false); setFlag(FH, true); return 16; };
	CBopcode[0x76] = [&]()->int {setFlag(FZ, !getBit(6, readByte_(doubleReg(H, L)))); setFlag(FN, false); setFlag(FH, true); return 16; };
	CBopcode[0x7E] = [&]()->int {setFlag(FZ, !getBit(7, readByte_(doubleReg(H, L)))); setFlag(FN, false); setFlag(FH, true); return 16; };

	//SET b,r
	//SET b,A
	CBopcode[0xC7] = [&]()->int {setBit(0, byteRegs[A], true); return 8; };
	CBopcode[0xCF] = [&]()->int {setBit(1, byteRegs[A], true);  return 8; };
	CBopcode[0xD7] = [&]()->int {setBit(2, byteRegs[A], true); return 8; };
	CBopcode[0xDF] = [&]()->int {setBit(3, byteRegs[A], true); return 8; };
	CBopcode[0xE7] = [&]()->int {setBit(4, byteRegs[A], true); return 8; };
	CBopcode[0xEF] = [&]()->int {setBit(5, byteRegs[A], true); return 8; };
	CBopcode[0xF7] = [&]()->int {setBit(6, byteRegs[A], true); return 8; };
	CBopcode[0xFF] = [&]()->int {setBit(7, byteRegs[A], true); return 8; };

	//SET b,B
	CBopcode[0xC0] = [&]()->int {setBit(0, byteRegs[B], true); return 8; };
	CBopcode[0xC8] = [&]()->int {setBit(1, byteRegs[B], true); return 8; };
	CBopcode[0xD0] = [&]()->int {setBit(2, byteRegs[B], true); return 8; };
	CBopcode[0xD8] = [&]()->int {setBit(3, byteRegs[B], true); return 8; };
	CBopcode[0xE0] = [&]()->int {setBit(4, byteRegs[B], true); return 8; };
	CBopcode[0xE8] = [&]()->int {setBit(5, byteRegs[B], true); return 8; };
	CBopcode[0xF0] = [&]()->int {setBit(6, byteRegs[B], true); return 8; };
	CBopcode[0xF8] = [&]()->int {setBit(7, byteRegs[B], true); return 8; };

	//SET b,C
	CBopcode[0xC1] = [&]()->int {setBit(0, byteRegs[C], true); return 8; };
	CBopcode[0xC9] = [&]()->int {setBit(1, byteRegs[C], true); return 8; };
	CBopcode[0xD1] = [&]()->int {setBit(2, byteRegs[C], true); return 8; };
	CBopcode[0xD9] = [&]()->int {setBit(3, byteRegs[C], true); return 8; };
	CBopcode[0xE1] = [&]()->int {setBit(4, byteRegs[C], true); return 8; };
	CBopcode[0xE9] = [&]()->int {setBit(5, byteRegs[C], true); return 8; };
	CBopcode[0xF1] = [&]()->int {setBit(6, byteRegs[C], true); return 8; };
	CBopcode[0xF9] = [&]()->int {setBit(7, byteRegs[C], true); return 8; };

	//SET b,D
	CBopcode[0xC2] = [&]()->int {setBit(0, byteRegs[D], true); return 8; };
	CBopcode[0xCA] = [&]()->int {setBit(1, byteRegs[D], true); return 8; };
	CBopcode[0xD2] = [&]()->int {setBit(2, byteRegs[D], true); return 8; };
	CBopcode[0xDA] = [&]()->int {setBit(3, byteRegs[D], true); return 8; };
	CBopcode[0xE2] = [&]()->int {setBit(4, byteRegs[D], true); return 8; };
	CBopcode[0xEA] = [&]()->int {setBit(5, byteRegs[D], true); return 8; };
	CBopcode[0xF2] = [&]()->int {setBit(6, byteRegs[D], true); return 8; };
	CBopcode[0xFA] = [&]()->int {setBit(7, byteRegs[D], true); return 8; };

	//SET b,E
	CBopcode[0xC3] = [&]()->int {setBit(0, byteRegs[E], true); return 8; };
	CBopcode[0xCB] = [&]()->int {setBit(1, byteRegs[E], true); return 8; };
	CBopcode[0xD3] = [&]()->int {setBit(2, byteRegs[E], true); return 8; };
	CBopcode[0xDB] = [&]()->int {setBit(3, byteRegs[E], true); return 8; };
	CBopcode[0xE3] = [&]()->int {setBit(4, byteRegs[E], true); return 8; };
	CBopcode[0xEB] = [&]()->int {setBit(5, byteRegs[E], true); return 8; };
	CBopcode[0xF3] = [&]()->int {setBit(6, byteRegs[E], true); return 8; };
	CBopcode[0xFB] = [&]()->int {setBit(7, byteRegs[E], true); return 8; };

	//SET b,H
	CBopcode[0xC4] = [&]()->int {setBit(0, byteRegs[H], true); return 8; };
	CBopcode[0xCC] = [&]()->int {setBit(1, byteRegs[H], true); return 8; };
	CBopcode[0xD4] = [&]()->int {setBit(2, byteRegs[H], true); return 8; };
	CBopcode[0xDC] = [&]()->int {setBit(3, byteRegs[H], true); return 8; };
	CBopcode[0xE4] = [&]()->int {setBit(4, byteRegs[H], true); return 8; };
	CBopcode[0xEC] = [&]()->int {setBit(5, byteRegs[H], true); return 8; };
	CBopcode[0xF4] = [&]()->int {setBit(6, byteRegs[H], true); return 8; };
	CBopcode[0xFC] = [&]()->int {setBit(7, byteRegs[H], true); return 8; };

	//SET b,L
	CBopcode[0xC5] = [&]()->int {setBit(0, byteRegs[L], true); return 8; };
	CBopcode[0xCD] = [&]()->int {setBit(1, byteRegs[L], true); return 8; };
	CBopcode[0xD5] = [&]()->int {setBit(2, byteRegs[L], true); return 8; };
	CBopcode[0xDD] = [&]()->int {setBit(3, byteRegs[L], true); return 8; };
	CBopcode[0xE5] = [&]()->int {setBit(4, byteRegs[L], true); return 8; };
	CBopcode[0xED] = [&]()->int {setBit(5, byteRegs[L], true); return 8; };
	CBopcode[0xF5] = [&]()->int {setBit(6, byteRegs[L], true); return 8; };
	CBopcode[0xFD] = [&]()->int {setBit(7, byteRegs[L], true); return 8; };

	//SET b,(HL)
	CBopcode[0xC6] = [&]()->int {byte tmp = readByte_(doubleReg(H, L)); setBit(0, tmp, true); writeByte_(doubleReg(H, L), tmp); return 16; };
	CBopcode[0xCE] = [&]()->int {byte tmp = readByte_(doubleReg(H, L)); setBit(1, tmp, true); writeByte_(doubleReg(H, L), tmp); return 16; };
	CBopcode[0xD6] = [&]()->int {byte tmp = readByte_(doubleReg(H, L)); setBit(2, tmp, true); writeByte_(doubleReg(H, L), tmp); return 16; };
	CBopcode[0xDE] = [&]()->int {byte tmp = readByte_(doubleReg(H, L)); setBit(3, tmp, true); writeByte_(doubleReg(H, L), tmp); return 16; };
	CBopcode[0xE6] = [&]()->int {byte tmp = readByte_(doubleReg(H, L)); setBit(4, tmp, true); writeByte_(doubleReg(H, L), tmp); return 16; };
	CBopcode[0xEE] = [&]()->int {byte tmp = readByte_(doubleReg(H, L)); setBit(5, tmp, true); writeByte_(doubleReg(H, L), tmp); return 16; };
	CBopcode[0xF6] = [&]()->int {byte tmp = readByte_(doubleReg(H, L)); setBit(6, tmp, true); writeByte_(doubleReg(H, L), tmp); return 16; };
	CBopcode[0xFE] = [&]()->int {byte tmp = readByte_(doubleReg(H, L)); setBit(7, tmp, true); writeByte_(doubleReg(H, L), tmp); return 16; };

	//RES b,r
	//RES b,A
	CBopcode[0x87] = [&]()->int {setBit(0, byteRegs[A], false); return 8; };
	CBopcode[0x8F] = [&]()->int {setBit(1, byteRegs[A], false);  return 8; };
	CBopcode[0x97] = [&]()->int {setBit(2, byteRegs[A], false); return 8; };
	CBopcode[0x9F] = [&]()->int {setBit(3, byteRegs[A], false); return 8; };
	CBopcode[0xA7] = [&]()->int {setBit(4, byteRegs[A], false); return 8; };
	CBopcode[0xAF] = [&]()->int {setBit(5, byteRegs[A], false); return 8; };
	CBopcode[0xB7] = [&]()->int {setBit(6, byteRegs[A], false); return 8; };
	CBopcode[0xBF] = [&]()->int {setBit(7, byteRegs[A], false); return 8; };

	//RES b,B
	CBopcode[0x80] = [&]()->int {setBit(0, byteRegs[B], false); return 8; };
	CBopcode[0x88] = [&]()->int {setBit(1, byteRegs[B], false); return 8; };
	CBopcode[0x90] = [&]()->int {setBit(2, byteRegs[B], false); return 8; };
	CBopcode[0x98] = [&]()->int {setBit(3, byteRegs[B], false); return 8; };
	CBopcode[0xA0] = [&]()->int {setBit(4, byteRegs[B], false); return 8; };
	CBopcode[0xA8] = [&]()->int {setBit(5, byteRegs[B], false); return 8; };
	CBopcode[0xB0] = [&]()->int {setBit(6, byteRegs[B], false); return 8; };
	CBopcode[0xB8] = [&]()->int {setBit(7, byteRegs[B], false); return 8; };

	//RES b,C
	CBopcode[0x81] = [&]()->int {setBit(0, byteRegs[C], false); return 8; };
	CBopcode[0x89] = [&]()->int {setBit(1, byteRegs[C], false); return 8; };
	CBopcode[0x91] = [&]()->int {setBit(2, byteRegs[C], false); return 8; };
	CBopcode[0x99] = [&]()->int {setBit(3, byteRegs[C], false); return 8; };
	CBopcode[0xA1] = [&]()->int {setBit(4, byteRegs[C], false); return 8; };
	CBopcode[0xA9] = [&]()->int {setBit(5, byteRegs[C], false); return 8; };
	CBopcode[0xB1] = [&]()->int {setBit(6, byteRegs[C], false); return 8; };
	CBopcode[0xB9] = [&]()->int {setBit(7, byteRegs[C], false); return 8; };

	//RES b,D
	CBopcode[0x82] = [&]()->int {setBit(0, byteRegs[D], false); return 8; };
	CBopcode[0x8A] = [&]()->int {setBit(1, byteRegs[D], false); return 8; };
	CBopcode[0x92] = [&]()->int {setBit(2, byteRegs[D], false); return 8; };
	CBopcode[0x9A] = [&]()->int {setBit(3, byteRegs[D], false); return 8; };
	CBopcode[0xA2] = [&]()->int {setBit(4, byteRegs[D], false); return 8; };
	CBopcode[0xAA] = [&]()->int {setBit(5, byteRegs[D], false); return 8; };
	CBopcode[0xB2] = [&]()->int {setBit(6, byteRegs[D], false); return 8; };
	CBopcode[0xBA] = [&]()->int {setBit(7, byteRegs[D], false); return 8; };

	//RES b,E
	CBopcode[0x83] = [&]()->int {setBit(0, byteRegs[E], false); return 8; };
	CBopcode[0x8B] = [&]()->int {setBit(1, byteRegs[E], false); return 8; };
	CBopcode[0x93] = [&]()->int {setBit(2, byteRegs[E], false); return 8; };
	CBopcode[0x9B] = [&]()->int {setBit(3, byteRegs[E], false); return 8; };
	CBopcode[0xA3] = [&]()->int {setBit(4, byteRegs[E], false); return 8; };
	CBopcode[0xAB] = [&]()->int {setBit(5, byteRegs[E], false); return 8; };
	CBopcode[0xB3] = [&]()->int {setBit(6, byteRegs[E], false); return 8; };
	CBopcode[0xBB] = [&]()->int {setBit(7, byteRegs[E], false); return 8; };

	//RES b,H
	CBopcode[0x84] = [&]()->int {setBit(0, byteRegs[H], false); return 8; };
	CBopcode[0x8C] = [&]()->int {setBit(1, byteRegs[H], false); return 8; };
	CBopcode[0x94] = [&]()->int {setBit(2, byteRegs[H], false); return 8; };
	CBopcode[0x9C] = [&]()->int {setBit(3, byteRegs[H], false); return 8; };
	CBopcode[0xA4] = [&]()->int {setBit(4, byteRegs[H], false); return 8; };
	CBopcode[0xAC] = [&]()->int {setBit(5, byteRegs[H], false); return 8; };
	CBopcode[0xB4] = [&]()->int {setBit(6, byteRegs[H], false); return 8; };
	CBopcode[0xBC] = [&]()->int {setBit(7, byteRegs[H], false); return 8; };

	//RES b,L
	CBopcode[0x85] = [&]()->int {setBit(0, byteRegs[L], false); return 8; };
	CBopcode[0x8D] = [&]()->int {setBit(1, byteRegs[L], false); return 8; };
	CBopcode[0x95] = [&]()->int {setBit(2, byteRegs[L], false); return 8; };
	CBopcode[0x9D] = [&]()->int {setBit(3, byteRegs[L], false); return 8; };
	CBopcode[0xA5] = [&]()->int {setBit(4, byteRegs[L], false); return 8; };
	CBopcode[0xAD] = [&]()->int {setBit(5, byteRegs[L], false); return 8; };
	CBopcode[0xB5] = [&]()->int {setBit(6, byteRegs[L], false); return 8; };
	CBopcode[0xBD] = [&]()->int {setBit(7, byteRegs[L], false); return 8; };

	//RES b,(HL)
	CBopcode[0x86] = [&]()->int {byte tmp = readByte_(doubleReg(H, L)); setBit(0, tmp, false); writeByte_(doubleReg(H, L), tmp); return 16; };
	CBopcode[0x8E] = [&]()->int {byte tmp = readByte_(doubleReg(H, L)); setBit(1, tmp, false); writeByte_(doubleReg(H, L), tmp); return 16; };
	CBopcode[0x96] = [&]()->int {byte tmp = readByte_(doubleReg(H, L)); setBit(2, tmp, false); writeByte_(doubleReg(H, L), tmp); return 16; };
	CBopcode[0x9E] = [&]()->int {byte tmp = readByte_(doubleReg(H, L)); setBit(3, tmp, false); writeByte_(doubleReg(H, L), tmp); return 16; };
	CBopcode[0xA6] = [&]()->int {byte tmp = readByte_(doubleReg(H, L)); setBit(4, tmp, false); writeByte_(doubleReg(H, L), tmp); return 16; };
	CBopcode[0xAE] = [&]()->int {byte tmp = readByte_(doubleReg(H, L)); setBit(5, tmp, false); writeByte_(doubleReg(H, L), tmp); return 16; };
	CBopcode[0xB6] = [&]()->int {byte tmp = readByte_(doubleReg(H, L)); setBit(6, tmp, false); writeByte_(doubleReg(H, L), tmp); return 16; };
	CBopcode[0xBE] = [&]()->int {byte tmp = readByte_(doubleReg(H, L)); setBit(7, tmp, false); writeByte_(doubleReg(H, L), tmp); return 16; };
	
	//jump
	//JP nn
	opcode[0xC3] = [&]()->int {regPC = readDouble_(regPC); return 12; };

	//JP cc,nn
	opcode[0xC2] = [&]()->int {if (!getFlag(FZ))opcode[0xC3](); else regPC += 2; return 12; };
	opcode[0xCA] = [&]()->int {if (getFlag(FZ))opcode[0xC3](); else regPC += 2; return 12; };
	opcode[0xD2] = [&]()->int {if (!getFlag(FC))opcode[0xC3](); else regPC += 2; return 12; };
	opcode[0xDA] = [&]()->int {if (getFlag(FC))opcode[0xC3](); else regPC += 2; return 12; };

	//JP (HL)
	opcode[0xE9] = [&]()->int {regPC = doubleReg(H, L); return 4; };

	//JR n
	opcode[0x18] = [&]()->int {byte&& tmp = readByte_(regPC++); regPC += reinterpret_cast<signedByte&>(tmp); return 8; };

	//JR cc,n
	opcode[0x20] = [&]()->int {if (!getFlag(FZ))opcode[0x18](); else regPC++; return 8; };
	opcode[0x28] = [&]()->int {if (getFlag(FZ))opcode[0x18](); else regPC++; return 8; };
	opcode[0x30] = [&]()->int {if (!getFlag(FC))opcode[0x18](); else regPC++; return 8; };
	opcode[0x38] = [&]()->int {if (getFlag(FC))opcode[0x18](); else regPC++; return 8; };

	//call
	//CALL nn
	opcode[0xCD] = [&]()->int {doubleByte tmp = readDouble_(regPC); regPC += 2; regSP -= 2; writeDouble_(regSP, regPC); regPC = tmp; return 12; };

	//CALL cc,nn
	opcode[0xC4] = [&]()->int {if (!getFlag(FZ))opcode[0xCD](); else regPC += 2; return 12; };
	opcode[0xCC] = [&]()->int {if (getFlag(FZ))opcode[0xCD](); else regPC += 2; return 12; };
	opcode[0xD4] = [&]()->int {if (!getFlag(FC))opcode[0xCD](); else regPC += 2; return 12; };
	opcode[0xDC] = [&]()->int {if (getFlag(FC))opcode[0xCD](); else regPC += 2; return 12; };

	//restart
	//RST n
	opcode[0xC7] = [&]()->int { regSP -= 2; writeDouble_(regSP, regPC); regPC = 0x00; return 32; };
	opcode[0xCF] = [&]()->int { regSP -= 2; writeDouble_(regSP, regPC); regPC = 0x08; return 32; };
	opcode[0xD7] = [&]()->int { regSP -= 2; writeDouble_(regSP, regPC); regPC = 0x10; return 32; };
	opcode[0xDF] = [&]()->int { regSP -= 2; writeDouble_(regSP, regPC); regPC = 0x18; return 32; };
	opcode[0xE7] = [&]()->int { regSP -= 2; writeDouble_(regSP, regPC); regPC = 0x20; return 32; };
	opcode[0xEF] = [&]()->int { regSP -= 2; writeDouble_(regSP, regPC); regPC = 0x28; return 32; };
	opcode[0xF7] = [&]()->int { regSP -= 2; writeDouble_(regSP, regPC); regPC = 0x30; return 32; };
	opcode[0xFF] = [&]()->int { regSP -= 2; writeDouble_(regSP, regPC); regPC = 0x38; return 32; };

	//return
	//RET
	opcode[0xC9] = [&]()->int {regPC = readDouble_(regSP); regSP += 2; return 8; };

	//RET cc
	opcode[0xC0] = [&]()->int {if (!getFlag(FZ))opcode[0xC9](); return 12; };
	opcode[0xC8] = [&]()->int {if (getFlag(FZ))opcode[0xC9](); return 12; };
	opcode[0xD0] = [&]()->int {if (!getFlag(FC))opcode[0xC9](); return 12; };
	opcode[0xD8] = [&]()->int {if (getFlag(FC))opcode[0xC9](); return 12; };

	//RETI
	opcode[0xD9] = [&]()->int {opcode[0xC9](); opcode[0xFB](); return 8; };

	//ext ops
	opcode[0xCB] = [&]()->int {return CBopcode[readByte_(regPC++)](); };
}

inline bool CPU::getBit(int bit,byte n) { 
	return (n >> bit) & 1; 
}

inline void CPU::setBit(int bit,byte& n,bool val) {
	if (val) {
		n |= (1 << bit);
	}
	else {
		n &= 0xFF - (1 << bit);
	}
}

inline void CPU::ADD(byte n) {
	doubleByte re = byteRegs[A] + n;
	setFlag(FZ, (re == 0 || re == 0x100));
	setFlag(FN, false);
	setFlag(FH, (byteRegs[A] ^ n^re) & 0x10);
	setFlag(FC, (byteRegs[A] ^ n^re) & 0x100);
	byteRegs[A] = re & 0xFF;
}

inline void CPU::ADC(byte n){
	doubleByte re = byteRegs[A] + n + getFlag(FC);
	setFlag(FZ, (re == 0 || re == 0x100));
	setFlag(FN, false);
	setFlag(FH, (byteRegs[A] ^ n^re) & 0x10);
	setFlag(FC, (byteRegs[A] ^ n^re) & 0x100);
	byteRegs[A] = re & 0xFF;
}

inline void CPU::SUB(byte n) {
	doubleByte re = static_cast<doubleByte>(byteRegs[A]) - n;
	setFlag(FZ, (re == 0));
	setFlag(FN, true);
	setFlag(FH, (byteRegs[A] ^ n^re) & 0x10);
	setFlag(FC, (byteRegs[A] ^ n^re) & 0x100);
	byteRegs[A] = re & 0xFF;
}

inline void CPU::SBC(byte n) {
	doubleByte re = static_cast<doubleByte>(byteRegs[A]) - n - getFlag(FC);
	setFlag(FZ, (re == 0));
	setFlag(FN, true);
	setFlag(FH, (byteRegs[A] ^ n^re) & 0x10);
	setFlag(FC, (byteRegs[A] ^ n^re) & 0x100);
	byteRegs[A] = re & 0xFF;
}

inline void CPU::AND(byte n) {
	byte re = byteRegs[A] & n;
	setFlag(FZ, (re == 0));
	setFlag(FN, false);
	setFlag(FH, true);
	setFlag(FC, false);
	byteRegs[A] = re & 0xFF;
}

inline void CPU::OR(byte n) {
	byte re = byteRegs[A] | n;
	setFlag(FZ, (re == 0));
	setFlag(FN, false);
	setFlag(FH, false);
	setFlag(FC, false);
	byteRegs[A] = re & 0xFF;
}

inline void CPU::XOR(byte n) {
	byte re = byteRegs[A] ^ n;
	setFlag(FZ, (re == 0));
	setFlag(FN, false);
	setFlag(FH, false);
	setFlag(FC, false);
	byteRegs[A] = re & 0xFF;
}

inline void CPU::CP(byte n) {
	doubleByte re = static_cast<doubleByte>(byteRegs[A]) - n;
	setFlag(FZ, (re == 0));
	setFlag(FN, true);
	setFlag(FH, (byteRegs[A] ^ n^re) & 0x10);
	setFlag(FC, (byteRegs[A] ^ n^re) & 0x100);
}

inline byte CPU::INC(byte n) {
	doubleByte re = n + 1;
	setFlag(FZ, (re == 0x100));
	setFlag(FN, false);
	setFlag(FH, (n ^re) & 0x10);
	return re & 0xFF;
}

inline byte CPU::DEC(byte n) {
	byte re = n - 1;
	setFlag(FZ, (re == 0));
	setFlag(FN, true);
	setFlag(FH, (n ^re) & 0x10);
	return re & 0xFF;
}

inline void CPU::ADD(doubleByte n) {
	doubleByte HL = doubleReg(H, L);
	unsigned int re = HL + n;
	setFlag(FZ, re == 0);
	setFlag(FN, false);
	//setFlag(FH, (HL^n^re) & 0x1000);
	setFlag(FC, (HL^n^re) & 0x10000);
	byteRegs[H] = (re >> 8) & 0xFF;
	byteRegs[L] = re & 0xFF;
}

inline void CPU::ADD(signedByte & n) {
	doubleByte x = static_cast<int16_t>(n);
	unsigned int re = static_cast<unsigned int>(regSP) + x;
	setFlag(FZ, false);
	setFlag(FN, false);
	setFlag(FH, (regSP^re^x) & 0x1000);
	setFlag(FC, re & 0x10000);
	regSP = re & 0xFFFF;
}

inline byte CPU::RLC(byte n) {
	setFlag(FN, false);
	setFlag(FH, false);
	setFlag(FC, n >> 7);
	n = (n << 1) + getFlag(FC);
	setFlag(FZ, !n);
	return n;
}

inline byte CPU::RL(byte n) {
	setFlag(FN, false);
	setFlag(FH, false);
	bool tmp = getFlag(FC);
	setFlag(FC, n >> 7);
	n = (n << 1) + tmp;
	setFlag(FZ, !n);
	return n;
}

inline byte CPU::RRC(byte n) {
	setFlag(FN, false);
	setFlag(FH, false);
	setFlag(FC, n & 1);
	n = (n >> 1) + (static_cast<byte>(getFlag(FC)) << 7);
	setFlag(FZ, !n);
	return n;
}

inline byte CPU::RR(byte n) {
	setFlag(FN, false);
	setFlag(FH, false);
	bool tmp = getFlag(FC);
	setFlag(FC, n & 1);
	n = (n >> 1) + (static_cast<byte>(tmp) << 7);
	setFlag(FZ, !n);
	return n;
}

inline byte CPU::SLA(byte n) {
	setFlag(FN, false);
	setFlag(FH, false);
	setFlag(FC, n >> 7);
	n <<= 1;
	setFlag(FZ, !n);
	return n;
}

inline byte CPU::SRA(byte n) {
	setFlag(FN, false);
	setFlag(FH, false);
	setFlag(FC, n & 1);
	bool tmp = n >> 7;
	n = (n >> 1) + (static_cast<byte>(tmp) << 7);
	setFlag(FZ, !n);
	return n;
}

inline byte CPU::SRL(byte n) {
	setFlag(FN, false);
	setFlag(FH, false);
	setFlag(FC, n & 1);
	n >>= 1;
	setFlag(FZ, !n);
	return n;
}
