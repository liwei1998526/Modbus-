#include<string>
#include"Modbus.h"
#include<vector>
#include<iostream>
char *FUNCTION0F(string recv_str, char* send_buf, int k, vector<int>&val)
{
	char send_buf_ret[600];
	memset(send_buf_ret, 0, sizeof(send_buf_ret));
	vector<int>ALL_DATA;
	string recv_start_add = recv_str.substr(16, 4);
	int start_add = stoi(recv_start_add, 0, 16);
	string recv_digit = recv_str.substr(20, 4);
	int digit = stoi(recv_digit, 0, 16);
	string data_recv = recv_str.substr(26, recv_str.size() - 26);
	string data_len = recv_str.substr(24, 2);
	int data_len_int = stoi(data_len, 0, 16);
	int digitlength;
	if (digit % 8 == 0 && digit >= 8)
	{
		digitlength = digit / 8;
	}
	else
	{
		digitlength = (digit / 8) + 1;
	}
	//判断请求报文数据范围是否存在
	if (((start_add + digit) > (k + val.size()) || start_add < k) || digit>2040)
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
		send[17] = '2';
		send_buf = (char*)send.c_str();
		strcpy(send_buf_ret, send_buf);
		cout << "寄存器超限" << endl;
		return send_buf_ret;
	}
	//判断数据值是否符合正常。
	if (data_len_int * 2 != data_recv.size() || digitlength > data_recv.size() / 2 || digit == 0)
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
	else//正常则写入
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
				DATA_N.clear();
				data_Bin.clear();
			}
			else
			{
				int surplus;
				if (digit % 8 == 0)
				{
					surplus = 8;
				}
				else
				{
					surplus = digit % 8;
				}
				while (1)
				{
					if (data_Bin.size() < surplus)
					{
						data_Bin = "0" + data_Bin;
						continue;
					}
					break;
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
			cout << "register" << " " << i << "：" << val[i - k] << endl;
			h++;
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
		return send_buf_ret;
	}
}