#include<vector>
#include<string>
#include<windows.h>
using namespace std;
string DEtoBIN(int n);
string DEtoHEX(int n);
char* hex2str(UINT8 *hex, int len);
void HexstrtoByte(const char* source, unsigned char* dest, int sourcelen);