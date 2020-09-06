#include "main.h"
#include <Windows.h>
#include <string>
#include<iostream>
#define MAX_NUMBER 300
char read_buf[MAX_NUMBER];


/*******************************************************************************************
 * 功能     ：	打开串口
 * port     :	串口号, 如("COM1")
 * baud_rate:	波特率
 * date_bits:	数据位（有效范围4~8）
 * stop_bit :	停止位
 * parity   :	奇偶校验。默认为无校验。NOPARITY 0； ODDPARITY 1；EVENPARITY 2

 * dcb.StopBits= 0,1,2对应的是1bit,1.5bits,2bits.
 * dcb.ByteSize=6,7,8时   dcb.StopBits不能为1
 * dcb.ByteSize=5时   dcb.StopBits不能为2
 ********************************************************************************************/
HANDLE InitCOM(LPCTSTR Port, int baud_rate, BYTE date_bits, BYTE stop_bit, BYTE parity)
{
	HANDLE hCom = CreateFile(Port, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);//同步方式打开串口
	if (INVALID_HANDLE_VALUE == hCom)
	{
		return INVALID_HANDLE_VALUE;
	}
	SetupComm(hCom, 4096, 4096);//设置缓存
	DCB dcb;
	if (!GetCommState(hCom, &dcb))
	{
		cout << "获取串口配置失败" << endl;
	}
	dcb.BaudRate = baud_rate;	//波特率
	dcb.fBinary = TRUE;			//二进制模式。必须为TRUE
	dcb.ByteSize = date_bits;	//数据位。范围4-8
	if (stop_bit == 0)
		dcb.StopBits = ONESTOPBIT;	//停止位
	if (stop_bit == 1)
		dcb.StopBits = ONE5STOPBITS;	//停止位
	if (stop_bit == 2)
		dcb.StopBits = TWOSTOPBITS;	//停止位
	switch (parity) //校验位
	{
	case 0:
		dcb.Parity = NOPARITY; //无校验
		break;
	case 1:
		dcb.Parity = ODDPARITY; //奇校验
		break;
	case 2:
		dcb.Parity = EVENPARITY; //偶校验
		break;
	case 3:
		dcb.Parity = MARKPARITY; //标记校验
		break;
	}
	if (parity == NOPARITY)
	{
		dcb.fParity = FALSE;	//奇偶校验关闭
		dcb.Parity = parity;	//校验模式
	}
	else
	{
		dcb.fParity = TRUE;		//奇偶校验开启
		dcb.Parity = parity;	//校验模式
	}
	cout <<"端口为：com"<<SetCommState(hCom, &dcb) << endl;
	PurgeComm(hCom, PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR | PURGE_TXABORT);//清除缓存
	//设置串口读写时间
	COMMTIMEOUTS CommTimeOuts;
	GetCommTimeouts(hCom, &CommTimeOuts);
	CommTimeOuts.ReadIntervalTimeout = 5;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 1;
	CommTimeOuts.ReadTotalTimeoutConstant = 0;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 10;
	CommTimeOuts.WriteTotalTimeoutConstant = 1000;
	if (!SetCommTimeouts(hCom, &CommTimeOuts)) {
		return INVALID_HANDLE_VALUE;
	}
	//创建线程，读取数据
	//HANDLE hReadCommThread = (HANDLE)CreateThread(NULL, 0, CommProc, &hCom, 0, NULL);
	return hCom;
}

/*********************************************************************************************
* 功能     ：   发送响应数据
* 描述	   ：	向串口写入数据
* 返回值   ：   true  发送成功   false  发送失败
* m_hComm  ：   串口句柄
* data     ：   要写入的数据
* len      ：   写入数据的长度
********************************************************************************************/
bool SendData(HANDLE m_hComm, char* data, int len)
{
	if (m_hComm == INVALID_HANDLE_VALUE)
		return FALSE;

	//清空串口
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR);
	//写串口
	DWORD dwWrite = 0;
	cout << "请求报文：" << data << endl;
	unsigned char end_data[255] = { 0 };
	HexstrtoByte(data, end_data, strlen(data));
	DWORD dwRet = WriteFile(m_hComm, end_data,len/2, &dwWrite, NULL);
	//清空串口
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR);
	if (!dwRet)
		return FALSE;
	return TRUE;
}

/*********************************************************************************************
* 功能     ：   获取串口读取缓存区中数据的字节数
* 描述	   ：	向串口写入数据
* 返回值   ：   缓存区中数据的字节数
* m_hComm  ：   串口句柄
********************************************************************************************/
unsigned int GetBytesInCOM(HANDLE m_hComm)
{
	DWORD dwError = 0;  /** 错误码 */
	COMSTAT  comstat;   /** COMSTAT结构体,记录通信设备的状态信息 */
	memset(&comstat, 0, sizeof(COMSTAT));

	unsigned int BytesInQue = 0;
	/** 在调用ReadFile和WriteFile之前,通过本函数清除以前遗留的错误标志 */
	if (ClearCommError(m_hComm, &dwError, &comstat))
	{
		BytesInQue = comstat.cbInQue; /** 获取在输入缓冲区中的字节数 */
	}
	return BytesInQue;
}

void MSleep(long lTime)
{
	LARGE_INTEGER litmp;
	LONGLONG QPart1, QPart2;
	double dfMinus, dfFreq, dfTim, dfSpec;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;
	dfSpec = 0.000001*lTime;

	do
	{
		QueryPerformanceCounter(&litmp);
		QPart2 = litmp.QuadPart;
		dfMinus = (double)(QPart2 - QPart1);
		dfTim = dfMinus / dfFreq;
	} while (dfTim<dfSpec);
}

int main()
{
	/*按端口号链接*/
	string COMM;
	cout << "输入端口号为：";
	string com = "com";
	string how;
	cin >> how;
	COMM = com + how;
	HANDLE H_Com = InitCOM((LPCTSTR)COMM.c_str(), 4800, 8, 0, 1);
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
			if (cod == 1 || cod == 3)
			{
				strcpy(data,function01(address, code));//01或03发送的报文
			}
			else if (cod == 15)
			{
				strcpy(data, function0F(address, code));//0F发送的报文
			}
			else if (cod == 16)
			{
				strcpy(data, function10(address, code));//10发送的报文
			}
			if (true == SendData(H_Com, data, strlen(data)))
			{
				cout << "发送请求报文成功" << endl;
				break;
			}
		}
		UINT8 read_buf_16[MAX_NUMBER];
		memset(read_buf_16, 0, sizeof(read_buf_16));
		BOOL bReadOK = ReadFile(H_Com,(char*)read_buf_16, 256, &dwRead, NULL);
		if (read_buf_16[0] == '0')
		{
			for (int p = 0; p < 256; p++)
			{
				read_buf[p] = read_buf_16[p];
			}
		}
		else
		{
			strcpy(read_buf, hex2str(read_buf_16,256));
		}
		if (bReadOK && (dwRead > 0))
		{
			read_buf[dwRead] = '\0';
			cout << "收到响应：" << read_buf << endl;
			/*处理响应报文*/
		}
		else
			cout << "loss" << endl;
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