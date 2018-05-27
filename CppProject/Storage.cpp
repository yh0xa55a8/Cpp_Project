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
	doubleByte headAdd = 0xFE00 + 4 * spriteNum;
	SpriteSet[spriteNum].enable = true;
	SpriteSet[spriteNum].posY = readByte(headAdd) - 0x10;
	SpriteSet[spriteNum].posX = readByte(headAdd + 1) - 0x08;
	SpriteSet[spriteNum].dataIndex = readByte(headAdd + 2);
	SpriteSet[spriteNum].underBG = getBit(readByte(headAdd + 3), 7);
	SpriteSet[spriteNum].flipY = getBit(readByte(headAdd + 3), 6);
	SpriteSet[spriteNum].flipX = getBit(readByte(headAdd + 3), 5);
	SpriteSet[spriteNum].palette = getBit(readByte(headAdd + 3), 4);
}

void Storage::writeKey(byte r1keys,byte r2keys) {
	keys[0] = (~r1keys);
	keys[1] = (~r2keys);
}