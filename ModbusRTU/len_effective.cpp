#include"main.h"
bool len_effective(string read_str,string send_buf)
{
	string recv_id = read_str.substr(0, 2);//返回的设备号
	string send_id = send_buf.substr(0, 2);//请求报文设备号
	string send_code = send_buf.substr(2, 2);//请求报文功能码
	int send_code_int = stoi(send_code, 0, 16);
	string respond_code = read_str.substr(2, 2);//返回功能码
	int respond_code_int = stoi(respond_code, 0, 16);
	string recv_ret = read_str.substr(4, 2);//响应报文返回字节数
	int recv_ret_int = stoi(recv_ret, 0, 16);
	string recv_data = read_str.substr(6, read_str.size() - 10);//响应报文数据位字符串
	string send_math = send_buf.substr(8, 4);//请求报文读取数量
	int send_digit = stoi(send_math, 0, 16);
	string ret_str = read_str.substr(0, read_str.size() - 4);//响应报文数据位
	string CRC_str = read_str.substr(read_str.size() - 4, 4);//响应报文CRC校验
	char*ret = (char*)ret_str.c_str();
	unsigned char crc[255] = { 0 };
	HexstrtoByte(ret, crc, strlen(ret));
	int CR = CRC16((unsigned char*)crc, strlen(ret) / 2);
	string CRC = DEtoHEX(CR);
	while (1)
	{
		if (CRC.size() < 4)
		{
			CRC = "0" + CRC;
			continue;
		}
		break;
	}
	if (CRC != CRC_str)
	{
		cout << "CRC校验出错" << endl;//CRC校验不通过，返回数据出错，关闭串口。
		return false;
	}
	//判断响应报文是否是访问的从站传回
	if (recv_id != send_id)
	{
		cout << "设备号出错" << endl;
		return false;
	}
	//判断功能码是否正确
	if ((respond_code_int != send_code_int) && (respond_code_int != send_code_int + 128))
	{
		cout << "功能码错误" << endl;
		return false;
	}
	if (respond_code_int == 01)
	{
		//判断返回字节数是否相等。
		if (recv_ret_int != recv_data.size() / 2)
		{
			cout << "返回字节数出错" << endl;
			return false;
		}
		//判断数据长度
		int v;
		if (send_digit > 8 && send_digit % 8 != 0)
		{
			v = send_digit / 8 + 1;
		}
		else if (send_digit % 8 == 0)
		{
			v = send_digit / 8;
		}
		else if (send_digit < 8)
		{
			v = 1;
		}
		if (read_str.size() != (v * 2 + 10))
		{
			cout << "数据长度出错" << endl;
			return false;
		}
	}
	else if (respond_code_int == 3)
	{
		//判断返回字节数是否相等。
		if (recv_ret_int != recv_data.size() / 2)
		{
			cout << "返回字节数出错" << endl;
			return false;
		}
		//判断数据长度是否符合
		if (read_str.size() != (send_digit * 4 + 10))
		{
			cout << "数据长度出错" << endl;
			return false;
		}
	}
	else if (respond_code_int == 15 || respond_code_int == 16)
	{	//判断数据长度是否符合
		if (read_str.size() != 16)
		{
			cout << "数据长度出错" << endl;
			return false;
		}
	}
	else
	{
		if (read_str.size() != 10)
		{
			cout << "异常响应报文，数据长度出错" << endl;
			return false;
		}
	}
	return true;
}