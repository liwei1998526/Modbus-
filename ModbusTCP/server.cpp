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
	string co;
	int m1 = 0, n1 = 0;
	int m2 = 0, n2 = 0;
	vector<int>val1;//存储线圈的值
	vector<int>val2;//存储寄存器的值
	cout << "01-读线圈" << " " << "03-读寄存器" << " " << "0F-写线圈" << " " << "10-写寄存器" << endl;
	WriteCoilCommand(m1, n1, val1);
	cout << "线圈创建成功" << endl;
	WriteRigisterCommand(m2, n2, val2);
	cout << "寄存器创建成功" << endl;
	//定义长度变量
	int send_len = 0;//发送数据长度
	int recv_len = 0;//接受数据长度
	int len = 0;
	int socket_close = 0;//判断网口断开。
	//定义发送缓冲区和接受缓冲区
	char send_buf[600];
	memset(send_buf, 0, sizeof(send_buf));//初始化内存，避免烫烫烫
	char recv_buf[600];
	memset(recv_buf, 0, sizeof(recv_buf));
	UINT8 recv_buf_16[600];//接受缓存区
	memset(recv_buf_16, 0, sizeof(recv_buf_16));
	//定义服务端套接字，接受请求套接字
	SOCKET s_server;
	SOCKET s_accept;
	//服务端地址客户端地址
	SOCKADDR_IN server_addr;
	SOCKADDR_IN accept_addr;
	initialization();//初始化套接字库
	//填充服务端信息
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(526);//端口号
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
		memset(recv_buf_16, 0, 600); //初始化接受缓冲区
		memset(send_buf, 0, strlen(send_buf));
		recv_len = recv(s_accept, (char*)recv_buf_16, sizeof(recv_buf_16), 0);
		//判断网络是否断开以及检测主机断开信号
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
			//当网络或主机断开后，选择关闭0,则关闭套接字，并释放dll资源。
			if (close == 0)
			{
				break;
			}
			else if (close == 1)
			{
				//当网络或者主机断开后，选择继续则重新开始监听状态。
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
		//判断接受数据最大值
		if (recv_len > 300)
		{
			cout << "数据出错" << endl;
			continue;
		}
		strcpy(recv_buf, hex2str(recv_buf_16, recv_len));//将数据从hex转为char*操作
		cout << "客户端信息:" << recv_buf << endl;
		string recv_str = recv_buf;
		for (int index = 0; index < recv_str.size(); index++)
		{//去空格
			if (recv_str[index] == 0x20)
			{
				recv_str.erase(index, 1);
				index--;
			}
		}
		string recv_code = recv_str.substr(14, 2);
		int function = stoi(recv_code, 0, 16);
		bool effective = DATA_effective(recv_str);//判断数据是否有效
		if (effective == false)
		{
			cout << "数据无效,不做处理" << endl;
			continue;
		}
		char *send_buf_code = new char[600];
		memset(send_buf_code, 0, sizeof(send_buf_code));
		memset(send_buf, 0, sizeof(send_buf));
		bool send_suc = respond(recv_str, send_buf_code, m1, m2, val1, val2);//处理请求报文
		if (send_suc == true)
		{
			strcpy(send_buf, send_buf_code);
		}
		else
		{
			cout << "数据无效" << endl;
			continue;
		}
		send_buf[13] = '9';//回复广播报文
		cout << "响应报文为：" << send_buf << endl;
		unsigned char end_data[600] = { 0 };
		HexstrtoByte(send_buf, end_data, strlen(send_buf));//将string转换位hex
		send_len = send(s_accept,(char*)end_data, strlen(send_buf)/2, 0);
		if (send_len < 0)
		{
			cout << "发送失败！" << endl;
			continue;
		}
	}
	//关闭套接字
	closesocket(s_server);
	closesocket(s_accept);
	//释放DLL资源
	WSACleanup();
	return 0;
}