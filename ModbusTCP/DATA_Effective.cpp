#include"Modbus.h"
#include<iostream>
using namespace std;
bool DATA_effective(string recv_str)
{
	//判断通信问题。
	string signal_str = recv_str.substr(4, 4);//请求报文通信协议
	int signal_int = stoi(signal_str, 0, 16);
	if (signal_int != 0)
	{
		cout << "通信协议错误" << endl;
		return false;
	}
	//判断返回长度是否正确
	string recv_len = recv_str.substr(8, 4);//请求报文报文头数据长度
	int expect_len = stoi(recv_len, 0, 16);
	string recv_exprct_data = recv_str.substr(12, recv_str.size() - 12);//请求报文数据位字符串
	if (expect_len != (recv_exprct_data.size() / 2))
	{
		cout << "报文头数据长度错误" << endl;
		return false;
	}
	return true;
}