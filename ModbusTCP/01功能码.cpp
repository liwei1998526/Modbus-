#include<string>
#include"Modbus.h"
#include<vector>
#include<iostream>
using namespace std;
char *FUNCTION01(string recv_str, char* send_buf, int k, vector<int>val)
{
	char send_buf_ret[600];
	memset(send_buf_ret, 0, sizeof(send_buf_ret));
	vector<string>ALL_DATA;
	string recv_start_add = recv_str.substr(16, 4);
	int start_add = stoi(recv_start_add, 0, 16);
	string recv_digit = recv_str.substr(20, 4);
	int digit = stoi(recv_digit, 0, 16);
	//判断字节长度
	if (((start_add + digit) > (k + val.size()) || start_add < k)||digit>2040)
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
	else if (digit == 0)
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
		int p = 0;
		int DIFF = 8;
		int v;
		if (digit % 8 == 0 && digit >= 8)
		{
			v = digit / 8;
		}
		else
		{
			v = (digit / 8)+1;
		}
		while (v > 0)
		{
			if (start_add + DIFF <= digit + start_add)
			{
				for (int h = start_add + DIFF - k - 1; h > start_add + DIFF - 8 - k - 1; h--)
				{
					if (h >= val.size())
					{
						break;
					}
					reg = reg + to_string(val[h]);
				}
				int DE = stoi(reg, 0, 2);
				string DATA = DEtoHEX(DE);
				while (1)
				{
					if (DATA.size() != 2)
					{
						DATA = "0" + DATA;
						continue;
					}
					break;
				}
				ALL_DATA.push_back(DATA);
				p = 0;
				reg.clear();
				DATA.clear();
			}
			else if (start_add + DIFF > digit + start_add)
			{
				for (int n = start_add + digit - k - 1; n > start_add + DIFF - 8 - k - 1; n--)
				{
					if (n >= val.size())
					{
						break;
					}
					reg = reg + to_string(val[n]);
				}
				int DE = stoi(reg, 0, 2);
				string DATA = DEtoHEX(DE);
				while (1)
				{
					if (DATA.size() != 2)
					{
						DATA = "0" + DATA;
						continue;
					}
					break;
				}
				ALL_DATA.push_back(DATA);
			}
			DIFF += 8;
			v--;
		}
		string str_data = "";
		for (int s = 0; s < ALL_DATA.size(); s++)
		{
			str_data = str_data + ALL_DATA[s];
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
		int digitlength;
		if (digit % 8 == 0 && digit >= 8)
		{
			digitlength = digit / 8;
		}
		else
		{
			digitlength = (digit / 8) + 1;
		}
		string digitlen = DEtoHEX(digitlength);
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