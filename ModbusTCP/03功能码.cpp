#include<string>
#include"Modbus.h"
#include<vector>
#include<iostream>
using namespace std;
char *FUNCTION03(string recv_str, char* send_buf, int k, vector<int>val)
{
	char send_buf_ret[600];
	memset(send_buf_ret, 0, sizeof(send_buf_ret));
	string recv_code = recv_str.substr(14, 2);
	int function = stoi(recv_code, 0, 16);
	if (function != 3 && function != 16)
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
		send[17] = '1';
		memset(send_buf, 0, sizeof(send_buf));
		send_buf = (char*)send.c_str();
		cout << "无可用功能码" << endl;
		cout << "响应报文为：" << send_buf << endl;
		strcpy(send_buf_ret, send_buf);
		return send_buf_ret;
	}
	vector<string>ALL_DATA;
	string recv_start_add = recv_str.substr(16, 4);
	int start_add = stoi(recv_start_add, 0, 16);
	string recv_digit = recv_str.substr(20, 4);
	int digit = stoi(recv_digit, 0, 16);
	if ((start_add + digit) > (k + val.size()) || start_add < k )
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