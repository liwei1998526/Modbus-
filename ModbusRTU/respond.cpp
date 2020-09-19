#include"main.h"
void respond_massage(string read_str,string send_buf)
{
	string respond_code = read_str.substr(2, 2);//返回功能码
	int respond_code_int = stoi(respond_code, 0, 16);
	string send_math = send_buf.substr(8, 4);//请求报文读取数量
	int send_digit = stoi(send_math, 0, 16);
	//判断响应报文是否是访问的从站传回
	bool effect = len_effective(read_str, send_buf);
	if (effect == false)
	{
		cout << "数据无效" << endl;
		return;
	}
	else
	{
		if (respond_code_int >= 128)
		{
			string Exception_code = read_str.substr(4, 2);//响应报文异常码
			if (Exception_code == "01")
			{
				cout << "功能码异常，从站不接受所发功能码" << endl;
			}
			else if ((Exception_code == "02"))
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
			else
			{
				cout << "无效异常码" << endl;
			}
		}
		else
		{
			//按照功能码开始处理
			if (respond_code_int == 1)
			{	
				respond_01(read_str, send_buf);
			}
			else if (respond_code_int == 3)
			{	
				respond_03(read_str, send_buf);
			}
			else if (respond_code_int == 15 || respond_code_int == 16)
			{	
				//判断返回起始位和读取数量与发送的是否相同
				string send_start_add = send_buf.substr(4, 4);//请求报文的起始地址
				string recv_start_add = read_str.substr(4, 4);//响应报文的起始地址
				string recv_digit = read_str.substr(8, 4);//响应报文读取数量
				int recv_digit_int = stoi(recv_digit, 0, 16);
				if (send_start_add == recv_start_add&&send_digit == recv_digit_int)
				{
					cout << "修改完成" << endl;
				}
				else
				{
					cout << "返回起始地址或读取数量错误" << endl;
				}
			}
		}
	}
}