#include<string>
#include"Modbus.h"
#include<vector>
#include<iostream>
using namespace std;
char *FUNCTION03(string recv_str, char* send_buf, int k, vector<int>val)
{
	char send_buf_ret[600];
	memset(send_buf_ret, 0, sizeof(send_buf_ret));
	vector<string>ALL_DATA;//用以存储输出的16进制数据位数据
	string recv_start_add = recv_str.substr(16, 4);//请求报文起始地址
	int start_add = stoi(recv_start_add, 0, 16);
	string recv_digit = recv_str.substr(20, 4);//请求报文修改寄存器数量
	int digit = stoi(recv_digit, 0, 16);
	//检测请求报文读取范围是否在设定范围之内
	if (((start_add + digit) > (k + val.size()) || start_add < k )||digit>123)
	{
		string send;
		for (int i = 0; i < 18; i++)
		{
			send += recv_str[i];
		}
		send[11] = '3';
		send[14] += 8;
		send[16] = '0';
		send[17] = '2';
		send_buf = (char*)send.c_str();
		strcpy(send_buf_ret, send_buf);
		cout << "寄存器超限" << endl;
		return send_buf_ret;
	}
	//检测读取个数是否为0
	if (digit == 0)
	{
		string send;
		for (int i = 0; i < 18; i++)
		{
			send += recv_str[i];
		}
		send[8] = '0';
		send[9] = '0';
		send[10] = '0';
		send[11] = '3';
		send[14] += 8;
		send[16] = '0';
		send[17] = '3';
		send_buf = (char*)send.c_str();
		strcpy(send_buf_ret, send_buf);
		cout << "非法数据值" << endl;
		return send_buf_ret;
	}
	else
	{
		string reg;
		string DATA_n;
		for (int n = start_add + digit - k-1; n > start_add - k-1; n--)
		{
			if (n >= val.size())
			{
				break;
			}
			DATA_n = DEtoHEX(val[n]);
			while (1)
			{
				if (DATA_n.size() != 4)
				{
					DATA_n = "0" + DATA_n;
					continue;
				}
				break;
			}
			ALL_DATA.push_back(DATA_n);
			DATA_n.clear();
		}
		string str_data = "";
		for (int s = 0; s < ALL_DATA.size(); s++)
		{
			str_data = ALL_DATA[s] + str_data;
		}
		int length = (str_data.size() + 6) / 2;
		string len = DEtoHEX(length);
		while (1)
		{
			if (len.size() != 4)
			{
				len = "0" + len;
				continue;
			}
			break;
		}
		string digitlen = DEtoHEX(str_data.size() / 2);
		while (1)
		{
			if (digitlen.size() != 2)
			{
				digitlen = "0" + digitlen;
				continue;
			}
			break;
		}
		string MATA = recv_str.substr(0, 8);
		string ADDRESS = recv_str.substr(12, 4);
		string send = MATA + len + ADDRESS + digitlen + str_data;
		send_buf = (char*)send.c_str();
		strcpy(send_buf_ret, send_buf);
		return send_buf_ret;
	}
}