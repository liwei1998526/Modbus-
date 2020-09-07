#include"main.h"
void respond_01(string Read_buf, string send_buf)
{
	vector<string>ALL_DATA;
	string start_add_str = send_buf.substr(4, 4);
	int start_add_int = stoi(start_add_str, 0, 16);
	string digit_str = send_buf.substr(8, 4);
	int digit_int = stoi(digit_str, 0, 16);
	int n = 6;
	while (n < Read_buf.size() - 4)
	{
		string data = Read_buf.substr(n, 2);
		int data_int = stoi(data, 0, 16);
		string data_bin = DEtoBIN(data_int);
		if (n + 2 < Read_buf.size() - 4)
		{
			while (1)
			{
				if (data_bin.size() < 8)
				{
					data_bin = "0" + data_bin;
					continue;
				}
				break;
			}
		}
		else
		{
			int surplus = digit_int - ((n - 6) * 4);
			while (1)
			{
				if (data_bin.size() < surplus)
				{
					data_bin = "0" + data_bin;
					continue;
				}
				break;
			}
		}
		ALL_DATA.push_back(data_bin);
		n += 2;
	}
	int j = 0;
	for (int i = start_add_int; i < start_add_int + digit_int; i += 8)
	{
		if (i + 7 <= start_add_int + digit_int)
		{
			cout << "第" << i + 7 << "--" << i << "个线圈的值为：" << ALL_DATA[j] << endl;
			j++;
		}
		else if (i + 7>start_add_int + digit_int)
		{
			cout << "第" << start_add_int+digit_int-1 << "--" << i << "个线圈的值为：" << ALL_DATA[j] << endl;
			break;
		}
	}
}