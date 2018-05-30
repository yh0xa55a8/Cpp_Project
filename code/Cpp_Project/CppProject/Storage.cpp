#include "Storage.h"

void Storage::init() {
	storage.fill(0);
	storage[0xFF40] = 0x91;
	storage[0xFF47] = 0xFC;
	storage[0xFF48] = 0xFF;
	storage[0xFF49] = 0xFF;
	std::array<int, 8> tmp1;
	tmp1.fill(0);
	std::array<std::array<int, 8>,8> tmp2;
	tmp2.fill(tmp1);
	VRamTileSet.fill(tmp2);
}

void Storage::getRom(QString filePath) {
	init();
	std::ifstream inFile;
	try {
		inFile.open(filePath.toStdWString(),std::ios_base::in | std::ios_base::binary);
		if (!inFile.is_open()) {
			throw fileOpenException();
		}
	}
	catch (fileOpenException) {
		QMessageBox::information(nullptr, "GameBoy Simulator", "ROM opening failed!\nPlease try again!");
		return;
	}
	for (int i = 0x0000; i < 0x8000; i++) {
		char tmp;
		inFile.get(tmp);
		storage[i] = reinterpret_cast<byte&>(tmp);
	}
	storage[0xFF40] = 0x91;
	storage[0xFF47] = 0xFC;
	storage[0xFF48] = 0xFF;
	storage[0xFF49] = 0xFF;
	emit getRomComp();
}

inline const byte & Storage::readByte(doubleByte address) { 
	if (address > 0xDFFF && address < 0xFE00)
		address -= 0x1000; 
	if (address == 0xFF00) {
		if (storage[0xFF00]==0x10) {
			return keys[0];
		}
		else return keys[1];
	}
	return storage[address]; }

void Storage::writeByte(doubleByte address, byte info) {
	if (address > 0xDFFF && address < 0xFE00)
		address -= 0x1000;
	storage[address] = info;
	if (address > 0xFDFF && address < 0xFEA0) {
		updataSpriteSet(address);
	}
	if (address > 0x7FFF && address < 0x9800)
		updateTileSet(address);
}

void Storage::updateTileSet(doubleByte add) {
	add = add % 2 == 0 ? add : add - 1;
	auto getBit = [](byte val, int bit)->bool {return (val >> bit) & 1; };
	int tileNum = (add - 0x8000) / 16;
	int rowNum = (add - 0x8000) % 16 / 2;
	byte dataA = readByte(add);
	byte dataB = readByte(add + 1);
	for (int j = 0; j < 8; j++) {
		int tmpColor = (getBit(dataA, 7 - j) ? 1 : 0) + (getBit(dataB, 7 - j) ? 2 : 0);
		VRamTileSet[tileNum][rowNum][j] = tmpColor;
	}
}

void Storage::updataSpriteSet(doubleByte add)
{
	auto getBit = [](byte val, int bit)->bool {return (val >> bit) & 1; };
	auto spriteNum = (add - 0xFE00) / 4;
	auto byteNum = (add - 0xFE00) % 4;
	SpriteSet[spriteNum].enable = true;
	switch(byteNum) {
	case 0:
		SpriteSet[spriteNum].posY = readByte(add) - 0x10; break;
	case 1:
		SpriteSet[spriteNum].posX = readByte(add) - 0x08; break;
	case 2:
		SpriteSet[spriteNum].dataIndex = readByte(add); break;
	case 3:
		SpriteSet[spriteNum].underBG = getBit(readByte(add), 7);
		SpriteSet[spriteNum].flipY = getBit(readByte(add), 6);
		SpriteSet[spriteNum].flipX = getBit(readByte(add), 5);
		SpriteSet[spriteNum].palette = getBit(readByte(add), 4);
	}
}

void Storage::writeKey(byte r1keys,byte r2keys) {
	keys[0] = (~r1keys);
	keys[1] = (~r2keys);
	storage[0xFF0F] |= 0x10;
}