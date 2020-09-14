#include<iostream>
#include<winsock2.h>
#include<string>
#include<windows.h>
#include"Modbus.h"
#pragma comment(lib,"ws2_32.lib")
using namespace std;
static int open_com;
int main()
{
	struct timeval timeout = { 3, 0 };//select等待3秒，3秒轮询.
	fd_set fds;
	string co;
	int code, m = 0, n = 0;
	vector<int>val;
	cout << "01-读线圈" << " " << "03-读寄存器" << " " << "0F-写线圈" << " " << "10-写寄存器" << endl;
	WriteCoilCommand(m, n, val);
	/*cout << "请输入功能码：";
	while (1)
	{
		cin >> co;
		code = stoi(co, 0, 16);
		if (code != 1 && code != 3 && code != 15 && code != 16)
		{
			cout << "不支持此功能码，请重新输入：";
			continue;
		}
		break;
	}*/
	/*if (code == 1 || code == 15)
	{*/
	/*}
	else if (code == 3 || code == 16)
	{
		WriteRigisterCommand(m, n, val);
	}*/
	//定义长度变量
	int send_len = 0;
	int recv_len = 0;
	int len = 0;
	int socket_close = 0;//判断网口断开。
	/*char send_buf_code[100];
	memset(send_buf_code, 0, sizeof(send_buf_code));*/
	//定义发送缓冲区和接受缓冲区
	char send_buf[600];
	memset(send_buf, 0, sizeof(send_buf));//初始化内存，避免烫烫烫
	char recv_buf[600];
	memset(recv_buf, 0, sizeof(recv_buf));
	UINT8 recv_buf_16[600];
	memset(recv_buf_16, 0, sizeof(recv_buf_16));
	//定义服务端套接字，接受请求套接字
	SOCKET s_server;
	SOCKET s_accept;
	//服务端地址客户端地址
	SOCKADDR_IN server_addr;
	SOCKADDR_IN accept_addr;
	initialization();
	//填充服务端信息
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(526);
	//创建套接字
	s_server = socket(AF_INET, SOCK_STREAM, 0);
	if (bind(s_server, (SOCKADDR *)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		cout << "套接字绑定失败！" << endl;
		WSACleanup();
	}
	else
	{
		cout << "套接字绑定成功！" << endl;
	}
	//设置套接字为监听状态
	if (listen(s_server, SOMAXCONN) < 0)
	{
		cout << "设置监听状态失败！" << endl;
		WSACleanup();
	}
	else
	{
		cout << "设置监听状态成功！" << endl;
	}
	cout << "服务端正在监听连接，请稍候...." << endl;

	//接受连接请求
	len = sizeof(SOCKADDR);
	s_accept = accept(s_server, (SOCKADDR *)&accept_addr, &len);
	if (s_accept == SOCKET_ERROR)
	{
		cout << "连接失败！" << endl;
		WSACleanup();
		return 0;
	}
	cout << "连接建立，准备接受数据" << endl;
	//接收数据
	while (1)
	{
		//int ret;
		//FD_ZERO(&fds); //每次循环都要清空集合，否则不能检测描述符变化
		//FD_SET(s_accept, &fds); //添加描述符
		socket_close = 1;
		memset(recv_buf_16, 0, 600); 
		memset(send_buf, 0, strlen(send_buf));
		recv_len = recv(s_accept, (char*)recv_buf_16, sizeof(recv_buf_16), 0);
		if (recv_len == 0 || recv_len == SOCKET_ERROR)
		{
			int close;
			cout << "网络或主机断开，是否继续监听（0关闭，1继续）：";
			while (1)
			{
				cin >> close;
				if (close == 1 || close == 0)
				{
					break;
				}
				cout << "输入不规范，请重新输入：";
				continue;
			}
			if (close == 0)
			{
				break;
			}
			else if (close == 1)
			{
				if (listen(s_server, SOMAXCONN) < 0)
				{
					cout << "设置监听状态失败！" << endl;
					WSACleanup();
				}
				else
				{
					cout << "设置监听状态成功！" << endl;
				}
				cout << "服务端正在监听连接，请稍候...." << endl;
			}
			s_accept = accept(s_server, (SOCKADDR *)&accept_addr, &len);
			if (s_accept == SOCKET_ERROR)
			{
				cout << "连接失败！" << endl;
				WSACleanup();
				return 0;
			}
			else
			{
				cout << "连接建立，准备接受数据" << endl;
				continue;
			}
		}
		else if (recv_len > 0)
		{
			cout << "接受数据成功" << endl;
		}
		if (recv_buf_16[0] == '0')
		{
			for (int p = 0; p < recv_len; p++)
			{
				recv_buf[p] = recv_buf_16[p];
			}
		}
		else
		{
			strcpy(recv_buf, hex2str(recv_buf_16, recv_len));
		}
		cout << "客户端信息:" << recv_buf << endl;
		string recv_str = recv_buf;
		for (int index = 0; index < recv_str.size(); index++)
		{
			if (recv_str[index] == 0x20)
			{
				recv_str.erase(index, 1);
				index--;
			}
		}
		string recv_len = recv_str.substr(8, 4);
		int expect_len = stoi(recv_len, 0, 16);
		string recv_exprct_data = recv_str.substr(12, recv_str.size() - 12);
		if (expect_len != (recv_exprct_data.size() / 2))
		{
			cout << "数据长度错误" << endl;
			continue;
		}
		string recv_device = recv_str.substr(12, 2);
		int device = stoi(recv_device, 0, 16);
		if (device != 9 && device != 0)
		{
			cout << "设备号出错" << endl;
			continue;
		}
		if (recv_str.size() <= 14)
		{
			return 0;
		}
		string recv_code = recv_str.substr(14, 2);
		int function = stoi(recv_code, 0, 16);
		char *send_buf_code = new char(600);
		memset(send_buf_code, 0, sizeof(send_buf_code));
		if ((function == 01 || function == 03) && recv_str.size() != 24)
		{
			cout << "数据长度错误" << endl;
			continue;
		}
		else if ((function == 15 || function == 16) && recv_str.size() <= 24)
		{
			cout << "数据长度错误" << endl;
			continue;
		}
		/*if (((code == 1 || code == 15) && (function != 1 && function != 15)) || ((code == 3 || code == 16) && (function != 3 && function != 16)))*/
		if (function != 1 && function != 3 && function != 15 && function != 16)
		{
			string send;
			for (int i = 0; i < 18; i++)
			{
				send += recv_str[i];
			}
			send[11] = '3';
			send[14] += 8;
			send[16] = '0';
			send[17] = '1';
			int b;
			memset(send_buf, 0, sizeof(send_buf));
			for (b = 0; b < send.size(); b++)
			{
				send_buf[b] = send[b];
			}
			cout << "无可用功能码" << endl;

		}
		else if (function == 1 )
		{
			strcpy(send_buf, FUNCTION01(recv_str, send_buf_code, m, val));
		}
		else if (function == 3 )
		{
			strcpy(send_buf, FUNCTION03(recv_str, send_buf_code, m, val));
		}
		else if (function == 15 )
		{
			strcpy(send_buf, FUNCTION0F(recv_str, send_buf_code, m, val));
		}
		else if (function == 16 )
		{
			strcpy(send_buf, FUNCTION10(recv_str, send_buf_code, m, val));
		}
		//cout << "请输入回复信息:";
		//cin >> send_buf;
		send_buf[13] = '9';
		cout << "响应报文为：" << send_buf << endl;
		unsigned char end_data[600] = { 0 };
		HexstrtoByte(send_buf, end_data, strlen(send_buf));
		send_len = send(s_accept,(char*)end_data, strlen(send_buf)/2, 0);
		if (send_len < 0)
		{
			cout << "发送失败！" << endl;
			break;
		}
	}
	if (socket_close == 0)
	{
		cout << "网口断开" << endl;
	}
	//关闭套接字
	closesocket(s_server);
	closesocket(s_accept);
	//释放DLL资源
	WSACleanup();
	return 0;
}