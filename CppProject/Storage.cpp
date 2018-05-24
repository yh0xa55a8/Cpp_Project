#include "Storage.h"

void Storage::init() {
	storage.fill(0);
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
		inFile >> storage[i];
	}
	emit getRomComp();
}
