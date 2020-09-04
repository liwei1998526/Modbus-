#include"main.h"
void HexstrtoByte(const char* source,unsigned char* dest,int sourcelen)
{
	short i;
	unsigned char highByte, lowbyte;
	for (i = 0; i < sourcelen; i += 2)
	{
		highByte = toupper(source[i]);
		lowbyte = toupper(source[i + 1]);
		if (highByte > 0x39)
		{
			highByte -= 0x37;
		}
		else
		{
			highByte -= 0x30;
		}
		if (lowbyte > 0x39)
		{
			lowbyte -= 0x37;
		}
		else
		{
			lowbyte -= 0x30;
		}
		dest[i / 2] = (highByte << 4) | lowbyte;
	}
	return;
}