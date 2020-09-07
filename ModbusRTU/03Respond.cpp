#include"main.h"
void respond_03(string Read_buf, string send_buf)
{
	vector<int>ALL_DATA;
	string start_add_str = send_buf.substr(4, 4);
	int start_add_int = stoi(start_add_str, 0, 16);
	string digit_str = send_buf.substr(8, 4);
	int digit_int = stoi(digit_str, 0, 16);
	int n = 6;
	while (n < Read_buf.size() - 4)
	{
		string data = Read_buf.substr(n, 4);
		int data_int = stoi(data, 0, 16);
		ALL_DATA.push_back(data_int);
		n += 4;
	}
	int j = 0;
	for (int i = start_add_int; i < start_add_int + digit_int; i++)
	{
		cout << "第" << i << "个寄存器的值为：" << ALL_DATA[j] << endl;
		j++;
	}
}