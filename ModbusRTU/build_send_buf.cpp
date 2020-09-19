#include"main.h"
using namespace std;
bool build_send_buf(int &open_com, HANDLE &H_Com,string &send_buf)
{
	while (1)
	{
		//RTU主站，生成并发送请求，计时，读取响应报文
		PurgeComm(H_Com, PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR | PURGE_TXABORT);//清除缓存
		char data[256];
		memset(data, 0, sizeof(data));
		string ad;
		cout << "设备号：";
		cin >> ad;
		//检查串口是否断开
		if (open_com == 0)
		{
			break;
		}
		int add = stoi(ad, 0, 16);
		string address = DEtoHEX(add);
		while (1)
		{
			if (address.size() < 2)
			{
				address = "0" + address;
				continue;
			}
			break;
		}
		/*按功能码输出请求报文*/
		string co;
		cout << "请输入功能码：";
		//检测串口是否断开
		if (open_com == 0)
		{
			return false;
		}
		cin >> co;
		int cod = stoi(co, 0, 16);
		string code = DEtoHEX(cod);
		while (1)
		{
			if (code.size() < 2)
			{
				code = "0" + code;
				continue;
			}
			break;
		}
		if (cod == 1 || cod == 3 || cod == 2)
		{//01或03发送的报文,添加02功能码，以验证从机能否正确返回异常功能码。
			strcpy(data, function01(address, code));
		}
		else if (cod == 15)
		{//0F发送的报文
			strcpy(data, function0F(address, code));
		}
		else if (cod == 16)
		{//10发送的报文
			strcpy(data, function10(address, code));
		}
		send_buf = data;
		if (true == SendData(H_Com, data, strlen(data)))
		{
			cout << "发送请求报文成功" << endl;
			return true;
		}
		else
		{
			cout << "发送请求报文失败" << endl;
			if (open_com == 0)
			{
				return false;
			}
			PurgeComm(H_Com, PURGE_RXCLEAR | PURGE_TXCLEAR);
			continue;
		}
	}
	return true;
}
