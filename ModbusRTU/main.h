#include<iostream>
#include<vector>
#include<string>
#include<windows.h>
#include<ctype.h>
using namespace std;
string DEtoHEX(int n);
unsigned short CRC16(unsigned char* puchMsg, unsigned short usDataLen);
void HexstrtoByte(const char* source, unsigned char* dest, int sourcelen);
//UINT16 CRC_16(UINT8 *Data, unsigned int CRC_Len);
char* function01(string add, string code);
char* function0F(string add, string code);
char* function10(string add, string code);