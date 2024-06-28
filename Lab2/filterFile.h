#pragma once
#include <math.h>

unsigned char raw(unsigned char* scanline, int index, int i, int j)
{
	if (j < 1)
	{
		return 0;
	}
	return scanline[index * i + j];
}

unsigned char prior(unsigned char* scanline, int index, int i, int j)
{
	if (i == 0 || j < 1)
	{
		return 0;
	}
	return scanline[index * (i - 1) + j];
}

unsigned char paethPredictor(unsigned char* scanline, int index, int i, int j, int bpp)
{
	unsigned char a = raw(scanline, index, i, j - bpp);		 // 0
	unsigned char b = prior(scanline, index, i, j);			 // 0
	unsigned char c = prior(scanline, index, i, j - bpp);	 // 0
	int p = a + b - c;
	int pa = abs(p - a);
	int pb = abs(p - b);
	int pc = abs(p - c);
	if (pa <= pb && pa <= pc)
	{
		return a;
	}
	else if (pb <= pc)
	{
		return b;
	}
	else
	{
		return c;
	}
}

void filterFunction(unsigned char* scanline, int height, int width, int colorType)
{
	int bpp = colorType + 1;
	int width_of_scanline = bpp * width + 1;
	for (int i = 0; i < height; i++)
	{
		int filter_type = scanline[width_of_scanline * i];
		if (filter_type == 1)
		{
			for (int j = bpp + 1; j < width_of_scanline; j++)
			{
				scanline[width_of_scanline * i + j] += raw(scanline, width_of_scanline, i, j - bpp);
			}
		}
		else if (filter_type == 2 && i != 0)
		{
			for (int j = 1; j < width_of_scanline; j++)
			{
				scanline[i * width_of_scanline + j] += prior(scanline, width_of_scanline, i, j);
			}
		}
		else if (filter_type == 3)
		{
			for (int j = 1; j < width_of_scanline; j++)
			{
				unsigned char a = prior(scanline, width_of_scanline, i, j);
				unsigned char b = raw(scanline, width_of_scanline, i, j - bpp);
				scanline[i * width_of_scanline + j] += (unsigned char)floor((a + b) / 2);
			}
		}
		else if (filter_type == 4)
		{
			for (int j = 1; j < width_of_scanline; j++)
			{
				scanline[i * width_of_scanline + j] += paethPredictor(scanline, width_of_scanline, i, j, bpp);
			}
		}
	}
}
