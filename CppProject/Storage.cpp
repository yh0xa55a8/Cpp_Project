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
	return storage[address]; }

void Storage::writeByte(doubleByte address, byte info) {
	if (address == 0xFF00) {
		emit requestKey(info == 0x20 ? true : false);
	}
	if (address > 0xDFFF && address < 0xFE00)
		address -= 0x1000;
	storage[address] = info;
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

void Storage::writeKey(byte keys) {
	storage[0xFF00] |= keys;
}