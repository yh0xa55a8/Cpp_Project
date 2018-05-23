#include "GPU.h"C00

void GPU::init(Storage& _sto,std::array<std::array<std::array<int, 8>, 8>, 384>& Vram)
{
	simSto = &_sto;
	TileSet = &Vram;
	imageBuffer.fill(Qt::white);

}

void GPU::freshLine()
{
	auto getBit = [](byte val, int bit)->bool {return (val >> bit) & 1; };
	byte LCDC = readByte_(addLCDC);
	byte LY = readByte_(addLY);
	byte SCX = readByte_(addSCX);
	byte SCY = readByte_(addSCY);
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
			//向缓存更新一行
		}
		break;
	case Hb:
		if (GPUclock >= 204) {
			GPUclock -=204;
			line++;
			if (line == 143) {
				mode = Vb;
				//发送数据到window
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

