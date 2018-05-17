#include "Storage.h"



Storage::Storage()
{
	storage.fill(0);
}

void Storage::getRom(QString filePath) {
	std::ifstream inFile;
	try {
		inFile.open(filePath.toStdWString(),std::ios_base::in | std::ios_base::binary);
		if (!inFile.is_open()) {
			throw fileOpenException();
		}
	}
	catch (fileOpenException) {
		//将来添加报错
		return;
	}
	for (int i = 0x0000; i < 0x8000; i++) {
		inFile >> storage[i];
	}
}
