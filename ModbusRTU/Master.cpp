#include "main.h"
#include <Windows.h>
#include <string>
#include<iostream>
#define MAX_NUMBER 300
char read_buf[MAX_NUMBER];
int main()
{
	/*按端口号链接*/
	string COMM;
	string send_buf;//承载输出报文数据
	cout << "输入端口号为：";
	string com = "com";
	string how;
	cin >> how;
	COMM = com + how;
	int Bund_rate = 0;
	int bund;
	cout << "1--9600" << " " << "2--14400" << " " << "3--19200" <<" "<<"4--115200"<< endl;
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
	int check;
	cout << "0--无校验" << " " << "1--奇校验" << " " << "2--偶校验" << endl;
	cout << "输入校验位：";
	cin >> check;
	HANDLE H_Com = InitCOM((LPCTSTR)COMM.c_str(), Bund_rate, 8, 0, check);
	if (H_Com == INVALID_HANDLE_VALUE)
	{
		cout << "初始化串口失败" << endl;
		getchar();
		return 0;
	}

	char write_buf[MAX_NUMBER];
	memset(write_buf, 0, MAX_NUMBER);
	DWORD dwRead;
	while (true)
	{
		//RTU主站，生成并发送请求，计时，读取响应报文,先发送后接收
		PurgeComm(H_Com, PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR | PURGE_TXABORT);//清除缓存
		while (1)
		{
			char data[256];
			memset(data, 0, sizeof(data));
			string ad;
			cout << "设备号：";
			cin >> ad;
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
				strcpy(data,function01(address, code));
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
				break;
			}
		}
		UINT8 read_buf_16[MAX_NUMBER];
		memset(read_buf_16, 0, sizeof(read_buf_16));
		BOOL bReadOK = ReadFile(H_Com,(char*)read_buf_16, 256, &dwRead, NULL);
		int frequency = 2;
		while (frequency > 0)
		{
			if (read_buf_16[0] == '0')
			{
				for (int p = 0; p < 256; p++)
				{
					read_buf[p] = read_buf_16[p];
				}
			}
			else if (read_buf_16[0] == 9)
			{
				strcpy(read_buf, hex2str(read_buf_16, dwRead));
			}
			if (bReadOK && (dwRead > 0))
			{
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
				if (CRC != CRC_str)
				{
					cout << "数据出错" << endl;//CRC校验不通过，返回数据出错，关闭串口。
					CloseHandle(H_Com);
					getchar();
				}
				respond_massage(read_str, send_buf);
				break;
			}
			else
			{
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
		int close;
		cout << "是否关闭串口（0关闭，1继续）：";
		while (1)
		{
			cin >> close;
			if (close == 1)
			{
				break;
			}
			else if (close == 0)
			{
				CloseHandle(H_Com);
				break;
			}
			else
			{
				cout << "输入不规范，请在次输入" << endl;
				continue;
			}
		}
		if (close == 0)
		{
			break;
		}
		else
			continue;
		
	}
	getchar();
	return 0;
}