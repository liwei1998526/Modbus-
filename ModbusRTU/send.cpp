#include"main.h"
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
	DWORD dwRet = WriteFile(m_hComm, end_data, len / 2, &dwWrite, NULL);
	//清空串口
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR);
	if (!dwRet)
		return FALSE;
	return TRUE;
}