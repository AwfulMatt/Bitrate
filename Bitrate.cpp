#include "pch.h"
#include <iostream>

int getBitrate(std::string filePath);

int main()
{
	std::cout << getBitrate("test.mp3") << std::endl;
}

int getBitrate(std::string filePath) {

	//http://mpgedit.org/mpgedit/mpeg_format/MP3Format.html

	int bitrateTable[16][5] = {
	{0, 0, 0, 0, 0},
	{32, 32, 32, 32, 8},
	{64, 48, 40, 48, 16},
	{96,56,48,56,24},
	{128, 64, 56, 64, 32},
	{160, 80, 64, 80, 40},
	{192, 96, 80, 96, 48},
	{224, 112, 96, 112,56},
	{256, 128, 112, 128, 64},
	{288, 160, 128, 144, 80},
	{320, 192, 160, 160, 96},
	{352, 224, 192, 176, 112},
	{384, 256, 224, 192, 128},
	{416, 320, 256, 224, 144},
	{448, 384, 320, 256, 160},
	{-1, -1, -1, -1, -1}
	};

	FILE * file = fopen(filePath.c_str(), "r");

	int bytesToRead = 1024 * 12;
	char * buffer = new char[bytesToRead];

	fseek(file, -1 * bytesToRead, SEEK_END);
	fread(buffer, bytesToRead, 1, file);

	int frames[10][4];
	int hits[10] = { 0 };

	int frame = 0;

	for (int i = 0; i < bytesToRead - 4; i++)
	{
		if ((buffer[i] & 0xFF) == 255 && (buffer[i + 1] & 0xE0) == 224) {

			int version = ((buffer[i + 1] & 0x18) >> 3);
			int layer = ((buffer[i + 1] & 0x6) >> 1);
			int bitrate = ((buffer[i + 2] & 0xF0) >> 4);

			//Bad frame!
			if (bitrate == 15) continue;

			frames[frame][0] = version;
			frames[frame][1] = layer;
			frames[frame][2] = bitrate;
			if (frame == 9) break;
			frame++;
		}
	}

	for (int i = 0; i < 10; i++)
	{
		if (frames[i] == '\0') break;

		for (int x = 0; x < 10; x++)
		{
			if (i == x) continue;

			if (frames[i][0] == frames[x][0] && frames[i][1] == frames[x][1] && frames[i][2] == frames[x][2]) {
				hits[i] = hits[i] + 1;
			}
		}
	}

	int highestHit = 0;
	int currentHit = 0;

	for (int i = 0; i < 10; i++)
	{
		if (i == 0)
		{
			highestHit = hits[i];
		}
		else {
			if (hits[i] > highestHit) {
				highestHit = hits[i];
				currentHit = i;
			}
		}
	}

	int * validFrame = frames[currentHit];

	int version = validFrame[0];
	int layer = validFrame[1];
	int bitrateIndex = validFrame[2];

	int column = 0;

	if (version == 3 && layer == 3)
		column = 0;

	if (version == 3 && layer == 2)
		column = 1;

	if (version == 3 && layer == 1)
		column = 2;

	if (version == 2 && layer == 3)
		column = 3;

	if (version == 2 && layer == 2)
		column = 4;

	return bitrateTable[bitrateIndex][column];
}