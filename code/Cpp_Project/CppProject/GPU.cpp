#include "GPU.h"

void GPU::init(Storage& _sto)
{
	simSto = &_sto;
	TileSet = &(_sto.VRamTileSet);
	SpriteSet = &(_sto.SpriteSet);
	imageBuffer.fill(Qt::white);
}

void GPU::freshLine()
{
	auto getBit = [](byte val, int bit)->bool {return (val >> bit) & 1; };
	enum color { black = 3, dark = 2, light = 1, white = 0 };

	//for set particular palette from storage data
	auto setPalette = [getBit](byte PLT)->std::array<color, 4>
	{
		std::array<color, 4> re;
		for (int i = 0; i < 4; i++) {
			bool bitA = getBit(PLT, 2 * i);
			bool bitB = getBit(PLT, 2 * i + 1);
			re[i] = static_cast<color>((bitB ? 2 : 0) + (bitA ? 1 : 0));
		}
		return re;
	};
	//convert grayscale color to RGB
	auto toRGB = [](color n)->const QColor 
	{
		switch (n) {
		case white:
			return QColor(255, 255, 255);
		case light:
			return QColor(192, 192, 192);
		case dark:
			return QColor(96, 96, 96);
		case black:
			return QColor(0, 0, 0);
		}
		return QColor(0, 0, 0);
	};

	byte LCDC = readByte_(addLCDC);
	byte LY = readByte_(addLY);
	byte SCX = readByte_(addSCX);
	byte SCY = readByte_(addSCY);
	byte BGP = readByte_(addBGP);
	std::array<color, 4> palette = setPalette(BGP);
	std::array<std::array<color, 4>, 2> spritePalette;
	spritePalette[0] = setPalette(readByte_(addOBP0));
	spritePalette[1] = setPalette(readByte_(addOBP1));
	int inBgX, inBgY, LX, tileX, tileY, inTileX, inTileY;
	bool bgMapLoc = getBit(LCDC, 3);
	bool bgSetLoc = getBit(LCDC, 4);
	for (LX = 0; LX < 160; LX++) {
		//background render
		if (getBit(LCDC, 0)) {
			inBgX = SCX + LX;
			inBgY = SCY + LY;
			tileX = inBgX / 8;
			tileY = inBgY / 8;
			if (tileX > 32)tileX -= 32;
			if (tileY > 32)tileY -= 32;
			inTileX = (SCX % 8 + LX) % 8;
			inTileY = (SCY % 8 + LY) % 8;
			byte tileInSet = readByte_(bgMapLoc ? (0x9C00 + 32 * tileY + tileX) : (0x9800 + 32 * tileY + tileX));
			tileInSet += bgSetLoc ? 0 : 256;
			int tmpColor = TileSet->at(tileInSet)[inTileY][inTileX];
			imageBuffer.setPixelColor(LX, LY, toRGB(palette[tmpColor]));
		}
		//sprite render
		if (getBit(LCDC, 1)) {
			auto putSprite = [&](Sprite& tmp,int tmpColor) {
				if (!tmp.underBG) {
				if (tmpColor != 0) {
					imageBuffer.setPixelColor(LX, LY, toRGB(spritePalette[tmp.palette][tmpColor]));
				}
			}
				else {
					if (imageBuffer.pixelColor(LX, static_cast<int>(LY)) == Qt::white) {
						imageBuffer.setPixelColor(LX, LY, toRGB(spritePalette[tmp.palette][tmpColor]));
					}
				}
			};
			if (getBit(LCDC, 2)) {																			//false for 8*8,true for 8*16
				for (int i = 0; i < 40; i += 2) {
					if (SpriteSet->at(i).enable) {
						Sprite& tmp = SpriteSet->at(i);
						if (LY >= tmp.posY&&LY < tmp.posY + 16 && LX >= tmp.posX&&LX < tmp.posX + 8) {
							inTileX = LX - tmp.posX;
							inTileY = LY - tmp.posY;
							byte tileIndex = (inTileY >= 8 ^ tmp.flipY) ? (tmp.dataIndex % 2 == 0 ? tmp.dataIndex : tmp.dataIndex + 1) : (tmp.dataIndex % 2 == 1 ? tmp.dataIndex : tmp.dataIndex - 1);
							inTileY = inTileY >= 8 ? inTileY - 8 : inTileY;
							int tmpColor = TileSet->at(tileIndex)[tmp.flipY ? 7 - inTileY : inTileY][tmp.flipX ? 7 - inTileX : inTileX];
							if (tmpColor != 0) {
								putSprite(tmp, tmpColor);
							}
						}
					}
				}
			}//8*16部分可能有未展示出的问题
			else {
				for (int i = 0; i < 40; i++) {
					if (SpriteSet->at(i).enable) {
						Sprite& tmp = SpriteSet->at(i);
						if (LY >= tmp.posY&&LY < tmp.posY + 8 && LX >= tmp.posX&&LX < tmp.posX + 8) {
							inTileX = LX - tmp.posX;
							inTileY = LY - tmp.posY;
							int tmpColor = TileSet->at(tmp.dataIndex)[tmp.flipY ? 7 - inTileY : inTileY][tmp.flipX ? 7 - inTileX : inTileX];
							if (tmpColor != 0) {
								putSprite(tmp, tmpColor);
							}
						}
					}
				}
			}
		}
		writeByte_(addLY, LY+1);
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
			byte line = readByte_(addLY);
			if (line > 143) {
				writeByte_(0xFF0F, readByte_(0xFF0F) | 1);
				mode = Vb;
				if (!(readByte_(addLCDC) & 0x80)) {
					imageBuffer.fill(Qt::white);
				}
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
			byte line = readByte_(addLY) + 1;
			writeByte_(addLY, line);
			if (line > 153) {
				mode = OAM;
				writeByte_(addLY, 0);
			}
		}
	}
	byte STAT = 0, LY = readByte_(addLY),LYC=readByte_(addSTAT);
	if (LY == LYC) {
		STAT |= 0b01000100;
	}
	switch (mode) {
	case OAM:
		STAT |= 0b00100010; break;
	case Hb:
		STAT |= 0b00001000; break;
	case Vb:
		STAT |= 0b00010001; break;
	case VRAM:
		STAT |= 0b00000011; break;
	}
	writeByte_(addSTAT, STAT);
}

