#include "Storage.h"



Storage::Storage()
{
	storage.fill(0);
}


Storage::~Storage()
{
}

void Storage::getRom(QString filePath) {
	std::ifstream inFile { filePath.toStdWString(),std::ios_base::in | std::ios_base::binary };
	assert(inFile.is_open());
	for (int i = 0x0000; i < 0x8000; i++) {
		inFile >> storage[i];
	}
}
