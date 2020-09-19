#include<iostream>
#include<vector>
#include<string>
#include<windows.h>
#include<ctype.h>
using namespace std;
HANDLE InitCOM(LPCTSTR Port, int baud_rate, BYTE date_bits, BYTE stop_bit, BYTE parity);//打开串口
bool SendData(HANDLE m_hComm, char* data, int len);//发送请求报文
bool OPEN_COM(HANDLE &H_Com);//打开串口
bool build_send_buf(int &open_com, HANDLE &H_Com, string &send_buf);//创建请求报文
string DEtoHEX(int n);
string DEtoBIN(int n);
unsigned short CRC16(unsigned char* puchMsg, unsigned short usDataLen);
void HexstrtoByte(const char* source, unsigned char* dest, int sourcelen);
char* hex2str(UINT8 *hex, int len);
char* function01(string add, string code);//根据01、03功能码生成请求报文
char* function0F(string add, string code);//根据0F功能码生成请求报文
char* function10(string add, string code);//根据10功能码生成请求报文
void respond_massage(string read_str, string send_buf);//对响应报文解析
void respond_01(string Read_buf, string send_buf);//对01响应报文解析
void respond_03(string Read_buf, string send_buf);//对03响应报文解析
bool len_effective(string read_str, string send_buf);//判断数据是否有效