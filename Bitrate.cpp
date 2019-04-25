#include "pch.h"
#include <iostream>

int main()
{
	//Use http://mpgedit.org/mpgedit/mpeg_format/MP3Format.html to undertstand the frame results!

	FILE * file = fopen("test.mp3", "r");
	
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

	//0: Version 1: Layer 2: Bitrate
	for (int i = 0; i < 3; i++)
	{
		std::cout << validFrame[i] << std::endl;
	}

}