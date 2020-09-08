#include<string>
#include"Modbus.h"
#include<vector>
#include<iostream>
using namespace std;
char *FUNCTION10(string recv_str, char* send_buf, int k, vector<int>&val)
{
	char send_buf_ret[600];
	memset(send_buf_ret, 0, sizeof(send_buf_ret));
	string recv_code = recv_str.substr(14, 2);
	int function = stoi(recv_code, 0, 16);
	if (function != 16)
	{
		string send;
		for (int i = 0; i < 18; i++)
		{
			send += recv_str[i];
		}
		send[11] = '3';
		send[14] += 8;
		send[16] = '0';
		send[17] = '1';
		int b;
		memset(send_buf, 0, sizeof(send_buf));
		for (b = 0; b < send.size(); b++)
		{
			send_buf[b] = send[b];
		}
		cout << "无可用功能码" << endl;
		cout << "响应报文为：" << send_buf << endl;
		strcpy(send_buf_ret, send_buf);
		return send_buf_ret;
	}
	vector<int>ALL_DATA;
	string recv_start_add = recv_str.substr(16, 4);
	int start_add = stoi(recv_start_add, 0, 16);
	string recv_digit = recv_str.substr(20, 4);
	int digit = stoi(recv_digit, 0, 16);
	string data_recv = recv_str.substr(26, recv_str.size() - 26);
	string data_len = recv_str.substr(24, 2);
	int data_len_int = stoi(data_len, 0, 16);
	if ((start_add + digit) > (k + val.size()) || start_add < k)
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
		cout << "响应报文为：" << send_buf_ret << endl;
		return send_buf_ret;
	}
	else if (data_len_int * 2 != data_recv.size() || digit * 2 != data_recv.size())
	{
		string send;
		for (int i = 0; i < 18; i++)
		{
			send += recv_str[i];
		}
		send[11] = '3';
		send[14] += 8;
		send[16] = '0';
		send[17] = '3';
		send_buf = (char*)send.c_str();
		strcpy(send_buf_ret, send_buf);
		cout << "非法数据值" << endl;
		cout << "响应报文为：" << send_buf_ret << endl;
		return send_buf_ret;
	}
	else
	{
		int n = 26;
		while(n < recv_str.size())
		{
			string DATA_N = recv_str.substr(n, 4);
			int data = stoi(DATA_N, 0, 16);
			ALL_DATA.push_back(data);
			n += 4;
		}
		int h = 0;
		for (int i = start_add; i < start_add + digit; i++)
		{
			val[i - k] = ALL_DATA[h];
			h++;
		}
		for (int math = k; math < k + val.size(); math++)
		{
			cout << "register" << " " << math << "：" << val[math-k] << endl;
		}
		int length = 6;
		string len = DEtoHEX(length);
		while (1)
		{
			if (len.size() < 4)
			{
				len = "0" + len;
				continue;
			}
			break;
		}
		string MATA = recv_str.substr(0, 8);
		string ADDRESS = recv_str.substr(12, 12);
		string send = MATA + len + ADDRESS;
		send_buf = (char*)send.c_str();
		strcpy(send_buf_ret, send_buf);
		cout << "响应报文为：" << send_buf_ret << endl;
		return send_buf_ret;
	}
}