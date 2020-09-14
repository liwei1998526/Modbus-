#include "main.h"
#include <Windows.h>
#include <string>
#include<iostream>
#include<thread>
#define MAX_NUMBER 600
char read_buf[MAX_NUMBER];
using namespace std;
static int open_com;
void* Serial_port_open(HANDLE COm)
{
	DCB dcb;
	bool ret = false;
	while (ret==false)
	{
		if(!GetCommState(COm, &dcb))
		{
			cout << "串口断开连接" << endl;
			CloseHandle(COm);
			open_com = 0;
			ret = true;
		}
		Sleep(1);
	}
	return 0;
}
int main()
{
	/*按端口号链接*/
	string COMM;
	string COM;
	string send_buf;//承载输出报文数据
	int Bund_rate = 0;
	int bund;
	int check;
	int close;
	HANDLE H_Com;
	HANDLE test_com;
	string com = "com";
	while (1)
	{   //开始打开串口。
		while (1)
		{    //打印所有课使用的串口。
			int COM_open = 0;
			cout << "可使用串口：" << endl;
			for (int port = 1; port < 100; port++)
			{
				COM = com + to_string(port);
				test_com = InitCOM((LPCTSTR)COM.c_str(), 9600, 8, 0, 1);
				if (test_com == INVALID_HANDLE_VALUE)
				{
					continue;
				}
				else
				{
					cout << COM << endl;
					COM_open++;
					CloseHandle(test_com);
					continue;
				}
			}
			if (COM_open == 0)
			{
				cout << "无可用串口，是否再次检测（0结束进程，1重新检测，2继续运行）：";
				int yes;
				cin >> yes;
				if (yes == 1)
				{
					continue;
				}
				else if (yes == 0)
				{
					return 0;
				}
			}
			cout << "输入端口号为：";
			string com = "com";
			string how;
			cin >> how;
			COMM = com + how;
			cout << "1--9600" << " " << "2--14400" << " " << "3--19200" << " " << "4--115200" << endl;
			cout << "请选择波特率：";
			cin >> bund;
			if (bund == 1)
			{
				Bund_rate = 9600;
			}
			else if (bund == 2)
			{
				Bund_rate = 14400;
			}
			else if (bund == 3)
			{
				Bund_rate = 19200;
			}
			else if (bund == 4)
			{
				Bund_rate = 115200;
			}
			cout << "0--无校验" << " " << "1--奇校验" << " " << "2--偶校验" << endl;
			cout << "输入校验位：";
			cin >> check;
			H_Com = InitCOM((LPCTSTR)COMM.c_str(), Bund_rate, 8, 0, check);
			if (H_Com == INVALID_HANDLE_VALUE)
			{
				int or;
				cout << "串口打开失败，是否结束界面(0结束，1继续)：";
				while (1)
				{
					cin >> or;
					if (or == 0 || or == 1)
					{
						break;
					}
					cout << "输入值不规范，请重新输入（0结束，1继续）：";
					continue;
				}
				if (or == 1)
				{
					continue;
				}
				else if (or == 0)
				{
					return 0;
				}
			}
			else
			{
				cout << "串口打开成功" << endl;
			}
			open_com = 1;
			break;
		}
		//开启线程，同步检测端口是否存在
		thread task01(Serial_port_open, H_Com);
		task01.detach();
		char write_buf[MAX_NUMBER];
		memset(write_buf, 0, MAX_NUMBER);
		DWORD dwRead;
		while (true)
		{
			//RTU主站，生成并发送请求，计时，读取响应报文
			PurgeComm(H_Com, PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR | PURGE_TXABORT);//清除缓存
			char data[256];
			memset(data, 0, sizeof(data));
			string ad;
			cout << "设备号：";
			cin >> ad;
			Sleep(1);
			//检查串口是否断开
			if (open_com == 0)
			{
				break;
			}
			int add = stoi(ad, 0, 16);
			string address = DEtoHEX(add);
			while (1)
			{
				if (address.size() < 2)
				{
					address = "0" + address;
					continue;
				}
				break;
			}
			/*按功能码输出请求报文*/
			string co;
			cout << "请输入功能码：";
			//检测串口是否断开
			if (open_com == 0)
			{
				break;
			}
			cin >> co;
			int cod = stoi(co, 0, 16);
			string code = DEtoHEX(cod);
			while (1)
			{
				if (code.size() < 2)
				{
					code = "0" + code;
					continue;
				}
				break;
			}
			if (cod == 1 || cod == 3 || cod == 2)
			{//01或03发送的报文,添加02功能码，以验证从机能否正确返回异常功能码。
				strcpy(data, function01(address, code));
			}
			else if (cod == 15)
			{//0F发送的报文
				strcpy(data, function0F(address, code));
			}
			else if (cod == 16)
			{//10发送的报文
				strcpy(data, function10(address, code));
			}
			send_buf = data;
			if (true == SendData(H_Com, data, strlen(data)))
			{
				cout << "发送请求报文成功" << endl;
			}
			else
			{
				cout << "发送请求报文失败" << endl;
				if (open_com == 0)
				{
					break;
				}
				PurgeComm(H_Com, PURGE_RXCLEAR | PURGE_TXCLEAR);
				continue;
			}
			//开始接收报文
			UINT8 read_buf_16[MAX_NUMBER];
			memset(read_buf_16, 0, sizeof(read_buf_16));
			BOOL bReadOK = ReadFile(H_Com, (char*)read_buf_16, 300, &dwRead, NULL);
			int frequency = 2;
			while (frequency > 0)
			{
				if (bReadOK && (dwRead > 0))
				{
					if (read_buf_16[0] == '0')
					{
						for (int p = 0; p < 256; p++)
						{
							read_buf[p] = read_buf_16[p];
						}
					}
					else if (read_buf_16[0] == 0x09 || read_buf_16[0] == 0)
					{
						strcpy(read_buf, hex2str(read_buf_16, dwRead));
					}
					else
					{
						cout << "数据出错" << endl;
						break;
					}
					cout << "收到响应：" << read_buf << endl;
					/*处理响应报文*/
					string read_str = read_buf;
					string ret_str = read_str.substr(0, read_str.size() - 4);
					string CRC_str = read_str.substr(read_str.size() - 4, 4);
					char*ret = (char*)ret_str.c_str();
					unsigned char crc[255] = { 0 };
					HexstrtoByte(ret, crc, strlen(ret));
					int CR = CRC16((unsigned char*)crc, strlen(ret) / 2);
					string CRC = DEtoHEX(CR);
					while (1)
					{
						if (CRC.size() < 4)
						{
							CRC = "0" + CRC;
							continue;
						}
						break;
					}
					if (CRC != CRC_str)
					{
						cout << "CRC校验出错" << endl;//CRC校验不通过，返回数据出错，关闭串口。
						PurgeComm(H_Com, PURGE_RXCLEAR | PURGE_TXCLEAR);
						break;
					}
					respond_massage(read_str, send_buf);
					PurgeComm(H_Com, PURGE_RXCLEAR | PURGE_TXCLEAR);
					break;
				}
				else
				{
					//没有接受到数据，则再发两次请求报文。
					char*data_again;
					data_again = (char*)send_buf.c_str();
					SendData(H_Com, data_again, strlen(data_again));
					BOOL bReadOK = ReadFile(H_Com, (char*)read_buf_16, 256, &dwRead, NULL);
				}
				frequency--;
			}
			if (bReadOK == false || (dwRead <= 0))
			{
				cout << "超时，从机断开" << endl;
			}
			/*关闭串口*/
			cout << "是否关闭串口（0关闭，1继续）：";
			while (1)
			{
				cin >> close;
				if (close == 0 || close == 1)
				{
					/*CloseHandle(H_Com);*/
					break;
				}
				else
				{
					cout << "输入不规范，请在次输入" << endl;
					continue;
				}
			}
			if (close == 1)
			{
				continue;
			}
			else if (close == 0)
			{
				break;
			}
		}
		//若串口断开则从新从串口打开重新开始
		if (open_com == 0)
		{
			continue;
		}
		getchar();
		return 0;
	}
}