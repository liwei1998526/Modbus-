#include<string>
#include"Modbus.h"
#include<vector>
#include<iostream>
using namespace std;
char *FUNCTION0F(string recv_str, char* send_buf, int k, vector<int>&val)
{
	char send_buf_ret[100];
	memset(send_buf_ret, 0, sizeof(send_buf_ret));
	vector<int>ALL_DATA;
	string recv_start_add = recv_str.substr(16, 4);
	int start_add = stoi(recv_start_add, 0, 16);
	string recv_digit = recv_str.substr(20, 4);
	int digit = stoi(recv_digit, 0, 16);
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
		int space = 2;
		while (space <= send.size())
		{
			send.insert(space, " ");
			space += 3;
		}
		send_buf = (char*)send.c_str();
		strcpy(send_buf_ret, send_buf);
		cout << "寄存器超限" << endl;
		cout << "响应报文为：" << send_buf_ret << endl;
		return send_buf_ret;
	}
	else
	{
		int n = 26;
		while (n < recv_str.size())
		{
			int p = 1;
			string DATA_N = recv_str.substr(n, 2);
			int data_s = stoi(DATA_N, 0, 16);
			string data_Bin = DEtoBIN(data_s);
			if (n + 2 < recv_str.size())
			{
				while (1)
				{
					if (data_Bin.size() < 8)
					{
						data_Bin = "0" + data_Bin;
						continue;
					}
					break;
				}
				while (p <= 8)
				{
					int data = stoi(data_Bin.substr(data_Bin.size() - p, 1), 0, 10);
					ALL_DATA.push_back(data);
					p++;
				}
				p = 1;
			}
			else
			{
				int surplus = digit % 8;
				while (1)
				{
					if (data_Bin.size() < surplus)
					{
						data_Bin = "0" + data_Bin;
						continue;
					}
					break;
				}
				if (data_Bin.size() > surplus)
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
				while (p <= surplus)
				{
					int data = stoi(data_Bin.substr(data_Bin.size() - p, 1), 0, 10);
					ALL_DATA.push_back(data);
					p++;
				}
				p = 1;
			}
			n += 2;
		}
		int h = 0;
		for (int i = start_add; i < start_add + digit; i++)
		{
			val[i - k] = ALL_DATA[h];
			h++;
		}
		for (int math = k; math < k + val.size(); math++)
		{
			cout << "register" << " " << math << "：" << val[math - k] << endl;
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
		/*int space = 2;
		while (space <= send.size())
		{
			send.insert(space, " ");
			space += 3;
		}*/
		send_buf = (char*)send.c_str();
		strcpy(send_buf_ret, send_buf);
		cout << "响应报文为：" << send_buf_ret << endl;
		return send_buf_ret;
	}
}