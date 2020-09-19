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
	while (ret == false)
	{
		if (!GetCommState(COm, &dcb))
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
	HANDLE H_Com = 0;
	string send_buf;//承载请求报文数据
	int close;
	while (1)
	{   //打开串口
		bool OPEN = OPEN_COM(H_Com);
		if (OPEN == false)
		{
			return 0;
		}
		else
		{
			open_com = 1;
		}
		//开启线程，同步检测端口是否存在
		thread task01(Serial_port_open, H_Com);
		task01.detach();
		//char write_buf[MAX_NUMBER];
		//memset(write_buf, 0, MAX_NUMBER);
		DWORD dwRead;
		while (1)
		{
			//构建并发送请求报文
			bool send_success = build_send_buf(open_com, H_Com, send_buf);
			if (send_success == false)
			{
				break;
			}
			//开始接收报文
			UINT8 read_buf_16[MAX_NUMBER];
			memset(read_buf_16, 0, sizeof(read_buf_16));
			BOOL bReadOK = ReadFile(H_Com, (char*)read_buf_16, 300, &dwRead, NULL);
			int frequency = 2;//没收到响应报文时，循环次数
			while (frequency > 0)
			{
				if (bReadOK && (dwRead > 0))
				{   //将hex码转换成字符串
					if (read_buf_16[0] >= 0x00 || read_buf_16[0] <= 0x255)
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
					respond_massage(read_str, send_buf);//处理响应报文
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
			//判断超时
			if (bReadOK == false || (dwRead <= 0))
			{
				cout << "超时，从机断开" << endl;
			}
			//判断串口是否关闭
			if (open_com == 0)
			{
				break;
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
		//检测到端口断开，则重头开始运行
		if (open_com == 0)
		{
			continue;
		}
		getchar();
		return 0;
	}
}