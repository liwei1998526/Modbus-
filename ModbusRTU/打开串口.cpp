#include "main.h"
#include <Windows.h>
#include <string>
#include<iostream>
#define MAX_NUMBER 600


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
	SetCommState(hCom, &dcb);
	if (SetCommState(hCom, &dcb) == 1)
	{
		cout << "串口打开成功" << endl;
	}
	else
	{
		cout << "串口打开失败" << endl;
	}
	PurgeComm(hCom, PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR | PURGE_TXABORT);//清除缓存
	//设置串口读写时间
	COMMTIMEOUTS CommTimeOuts;
	GetCommTimeouts(hCom, &CommTimeOuts);
	CommTimeOuts.ReadIntervalTimeout = 5;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 1;
	CommTimeOuts.ReadTotalTimeoutConstant =  0;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 10;
	CommTimeOuts.WriteTotalTimeoutConstant = 1000;
	if (!SetCommTimeouts(hCom, &CommTimeOuts))
	{
		return INVALID_HANDLE_VALUE;
	}
	//创建线程，读取数据
	//HANDLE hReadCommThread = (HANDLE)CreateThread(NULL, 0, CommProc, &hCom, 0, NULL);
	return hCom;
}