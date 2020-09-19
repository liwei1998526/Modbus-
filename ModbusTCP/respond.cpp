#include"Modbus.h"
#include<iostream>
bool respond(string recv_str, char* &send_buf,int m1,int m2,vector<int>val1,vector<int>val2)
{
	char *send_buf_code = new char[600];
	memset(send_buf_code, 0, sizeof(send_buf_code));
	//判断设备号是否正确
	string recv_device = recv_str.substr(12, 2);//请求报文设备号
	int device = stoi(recv_device, 0, 16);
	if (device != 9 && device != 0)
	{
		cout << "设备号出错" << endl;
		return false;
	}
	//先大致判断数据长度是否正确
	if (recv_str.size() <= 14)
	{
		return false;
	}
	//根据功能码判断数据长度是否正确
	string recv_code = recv_str.substr(14, 2);
	int function = stoi(recv_code, 0, 16);
	if ((function == 01 || function == 03) && recv_str.size() != 24)
	{
		cout << "数据长度错误" << endl;
		return false;
	}
	else if ((function == 15 || function == 16) && recv_str.size() <= 24)
	{
		cout << "数据长度错误" << endl;
		return false;
	}
	//判断功能码是否存在
	if (function != 1 && function != 3 && function != 15 && function != 16)
	{
		string send;
		for (int i = 0; i < 18; i++)
		{
			send += recv_str[i];
		}
		int abn = stoi(send.substr(14, 1), 0, 16) + 8;//计算异常功能码
		string abn_str = DEtoHEX(abn);
		send[8] = '0';
		send[9] = '0';
		send[10] = '0';
		send[11] = '3';
		send[14] = abn_str[0];
		send[16] = '0';
		send[17] = '1';
		int b;
		send_buf = new char(0);
		memset(send_buf, 0, sizeof(send_buf));
		for (b = 0; b < send.size(); b++)
		{
			send_buf[b] = send[b];
		}
		cout << "无可用功能码" << endl;
		return true;
	}
	//根据功能码进行处理
	else if (function == 1)
	{
		strcpy(send_buf, FUNCTION01(recv_str, send_buf_code, m1, val1));
	}
	else if (function == 3)
	{
		strcpy(send_buf, FUNCTION03(recv_str, send_buf_code, m2, val2));
	}
	else if (function == 15)
	{
		strcpy(send_buf, FUNCTION0F(recv_str, send_buf_code, m1, val1));
	}
	else if (function == 16)
	{
		strcpy(send_buf, FUNCTION10(recv_str, send_buf_code, m2, val2));
	}
	return true;
}