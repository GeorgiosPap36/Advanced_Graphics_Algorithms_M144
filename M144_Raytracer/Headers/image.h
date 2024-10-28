#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <iostream>
#include <fstream>

struct Color
{
	float r, g, b;

	Color(): r(0), g(0), b(0)
	{
	}

	Color(float r, float g, float b): r(r), g(g), b(b)
	{
	}

	~Color()
	{
	}

};

class image
{
public:

	image(int width, int height) : m_width(width), m_height(height)
	{
	}

	void Export(const char* path, int*** ar) const
	{
		std::ofstream f;
		f.open(path, std::ios::out | std::ios::binary);

		if (!f.is_open())
		{
			std::cout << "File could not be opened\n";
			return;
		}

		unsigned char bmpPad[3] = { 0, 0, 0 };
		const int paddingAmount = ((4 - (m_width * 3) % 4) % 4);

		const int fileHeaderSize = 14;
		const int informationHeaderSize = 40;
		const int fileSize = fileHeaderSize + informationHeaderSize + m_width * m_height * 3 + paddingAmount * m_height;

		unsigned char fileHeader[fileHeaderSize];

		//File type
		fileHeader[0] = 'B';
		fileHeader[1] = 'M';
		//File size
		fileHeader[2] = fileSize;
		fileHeader[3] = fileSize >> 8;
		fileHeader[4] = fileSize >> 16;
		fileHeader[5] = fileSize >> 24;
		//Reserved 1 (Not used)
		fileHeader[6] = 0;
		fileHeader[7] = 0;
		////Reserved 2 (Not used)
		fileHeader[8] = 0;
		fileHeader[9] = 0;
		//Pixel data offset
		fileHeader[10] = fileHeaderSize + informationHeaderSize;
		fileHeader[11] = 0;
		fileHeader[12] = 0;
		fileHeader[13] = 0;

		unsigned char informationHeader[informationHeaderSize];

		//Header size
		informationHeader[0] = informationHeaderSize;
		informationHeader[1] = 0;
		informationHeader[2] = 0;
		informationHeader[3] = 0;
		//Image width
		informationHeader[4] = m_width;
		informationHeader[5] = m_width >> 8;
		informationHeader[6] = m_width >> 16;
		informationHeader[7] = m_width >> 24;
		//Image height
		informationHeader[8] = m_height;
		informationHeader[9] = m_height >> 8;
		informationHeader[10] = m_height >> 16;
		informationHeader[11] = m_height >> 24;
		//Planes
		informationHeader[12] = 1;
		informationHeader[13] = 0;
		//Bits per pixel (RGB)
		informationHeader[14] = 24;
		informationHeader[15] = 0;
		//Compression (No compression)
		informationHeader[16] = 0;
		informationHeader[17] = 0;
		informationHeader[18] = 0;
		informationHeader[19] = 0;
		//Image size (No compression)
		informationHeader[20] = 0;
		informationHeader[21] = 0;
		informationHeader[22] = 0;
		informationHeader[23] = 0;
		//X pixels per meter (Not specified)
		informationHeader[24] = 0;
		informationHeader[25] = 0;
		informationHeader[26] = 0;
		informationHeader[27] = 0;
		//Y pixels per meter (Not specified)
		informationHeader[28] = 0;
		informationHeader[29] = 0;
		informationHeader[30] = 0;
		informationHeader[31] = 0;
		//Total colors (color palette not used)
		informationHeader[32] = 0;
		informationHeader[33] = 0;
		informationHeader[34] = 0;
		informationHeader[35] = 0;
		//Important colors (Generally ignored)
		informationHeader[36] = 0;
		informationHeader[37] = 0;
		informationHeader[38] = 0;
		informationHeader[39] = 0;

		f.write(reinterpret_cast<char*>(fileHeader), fileHeaderSize);
		f.write(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

		for (int y = 0; y < m_height; y++)
		{
			for (int x = 0; x < m_width; x++)
			{
				unsigned char r = static_cast<unsigned char>(ar[y][x][0]);
				unsigned char g = static_cast<unsigned char>(ar[y][x][1]);
				unsigned char b = static_cast<unsigned char>(ar[y][x][2]);

				unsigned char color[] = { b , g, r };

				f.write(reinterpret_cast<char*>(color), 3);
			}
			f.write(reinterpret_cast<char*>(bmpPad), paddingAmount);
		}

		f.close();

		std::cout << "File created\n";
	}

	public:
		int m_width, m_height;
};

#endif