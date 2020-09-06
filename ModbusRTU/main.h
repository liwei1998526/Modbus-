#include<iostream>
#include<vector>
#include<string>
#include<windows.h>
#include<ctype.h>
using namespace std;
string DEtoHEX(int n);
string DEtoBIN(int n);
unsigned short CRC16(unsigned char* puchMsg, unsigned short usDataLen);
void HexstrtoByte(const char* source, unsigned char* dest, int sourcelen);
char* hex2str(UINT8 *hex, int len);
//UINT16 CRC_16(UINT8 *Data, unsigned int CRC_Len);
char* function01(string add, string code);
char* function0F(string add, string code);
char* function10(string add, string code);
void respond_massage(string read_str, string send_buf);
void respond_01(string Read_buf, string send_buf);
void respond_03(string Read_buf, string send_buf);