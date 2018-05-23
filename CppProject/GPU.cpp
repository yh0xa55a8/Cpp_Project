#include "GPU.h"

void GPU::init(Storage& _sto,std::array<std::array<std::array<int, 8>, 8>, 384>& Vram)
{
	simSto = &_sto;
	TileSet = &Vram;
	imageBuffer.fill(Qt::white);

}

void GPU::freshLine()
{
	auto getBit = [](byte val, int bit)->bool {return (val >> bit) & 1; };
	enum color { black = 0, dark = 1, light = 2, white = 3 };
	auto setPalette = [getBit](byte BGP)->std::array<color, 4>{
		std::array<color, 4> re;
		for (int i = 0; i < 5; i++) {
			bool bitA = getBit(BGP, 2 * i);
			bool bitB = getBit(BGP, 2 * i + 1);
			re[i] = static_cast<color>(bitB ? 2 : 0 + bitA ? 1 : 0);
		}
		return re;
	};
	auto toRGB = [](color n)->const QColor {
		switch (n) {
		case black:
			return QColor(255, 255, 255);
		case dark:
			return QColor(192, 192, 192);
		case light:
			return QColor(96, 96, 96);
		case white:
			return QColor(0, 0, 0);
		}
	};
	byte LCDC = readByte_(addLCDC);
	byte LY = readByte_(addLY);
	byte SCX = readByte_(addSCX);
	byte SCY = readByte_(addSCY);
	byte BGP = readByte_(addBGP);
	std::array<color, 4> palette = setPalette(BGP);
	int inBgX, inBgY, LX, tileX, tileY, inTileX, inTileY;
	bool bgMapLoc = getBit(LCDC, 3);
	bool bgSetLoc = getBit(LCDC, 4);
	for (LX = 0; LX < 144; LX++) {
		inBgX = SCX + LX;
		inBgX = SCY + LY;
		tileX = inBgX / 8;
		tileY = inBgX / 8;
		if (tileX > 32)tileX -= 32;
		if (tileY > 32)tileY -= 32;
		inTileX = SCX % 8 + LX;
		inTileY = SCY % 8 + LY;
		byte tileInSet = readByte_(bgMapLoc ? (0x9C00 + 32 * tileY + tileX) : (0x9800 + 32 * tileY + tileX));
		tileInSet += bgSetLoc ? 0 : 256;
		imageBuffer.setPixelColor(LX, LY, toRGB(palette[TileSet->operator[](tileInSet)[tileX][tileY]]));
	}
}

void GPU::step(int deltaTime)
{
	GPUclock += deltaTime;
	switch (mode) {
	case OAM:
		if (GPUclock >= 80) {
			GPUclock -= 80;
			mode = VRAM;
		}
		break;
	case VRAM:
		if (GPUclock >= 172) {
			GPUclock -= 172;
			mode = Hb;
			freshLine();
		}
		break;
	case Hb:
		if (GPUclock >= 204) {
			GPUclock -=204;
			line++;
			if (line == 143) {
				mode = Vb;
				emit freshImage(imageBuffer);
				imageBuffer.fill(Qt::white);
			}
			else {
				mode = OAM;
			}
		}
		break;
	case Vb:
		if (GPUclock >= 456) {
			GPUclock -= 456;
			line++;
			if (line > 153) {
				mode = OAM;
				line = 0;
			}
		}
	}
}

