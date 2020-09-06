#include"main.h"
void respond_massage(string read_str,string send_buf)
{
	string respond_high_code = read_str.substr(2, 1);
	int high_code = stoi(respond_high_code, 0, 16);
	string respond_code = read_str.substr(2, 2);
	int respond_code_int = stoi(respond_code, 0, 16);
	if (high_code >= 8)
	{
		string Exception_code = read_str.substr(4, 2);
		if (Exception_code == "01")
		{
			cout << "功能码异常，从站不接受所发功能码" << endl;
		}
		else if (Exception_code == "02")
		{
			cout << "寄存器超出从站范围" << endl;
		}
		else if (Exception_code == "03")
		{
			cout << "寄存器数据超限，或寄存器受限" << endl;
		}
		else if (Exception_code == "04")
		{
			cout << "从站设备故障" << endl;
		}
	}
	else
	{
		if (respond_code_int == 1)
		{
			respond_01(read_str, send_buf);
		}
	}
}