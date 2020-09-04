#include<iostream>
#include<winsock.h>
#include<string>
#include<windows.h>
#include"Modbus.h"
#pragma comment(lib,"ws2_32.lib")
using namespace std;
void initialization();
int main()
{
	string co;
	int code, m = 0, n = 0;
	vector<int>val;
	cout << "请输入功能码：";
	cin >> co;
	code = stoi(co, 0, 16);
	if (code == 1 || code == 15)
	{
		WriteCoilCommand(m, n, val);
	}
	if (code == 3 || code == 16)
	{
		WriteRigisterCommand(m, n, val);
	}
	//定义长度变量
	int send_len = 0;
	int recv_len = 0;
	int len = 0;
	/*char send_buf_code[100];
	memset(send_buf_code, 0, sizeof(send_buf_code));*/
	//定义发送缓冲区和接受缓冲区
	char send_buf[100];
	//初始化内存，避免烫烫烫
	memset(send_buf, 0, sizeof(send_buf));
	char recv_buf[100];
	memset(recv_buf, 0, sizeof(recv_buf));
	UINT8 recv_buf_16[100];
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
		recv_len = recv(s_accept,(char*)recv_buf_16, 100, 0);
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

		if (recv_len < 0)
		{
			cout << "接受失败！" << endl;
			break;
		}
		else
		{
			cout << "客户端信息:" << recv_buf << endl;
		}
		string recv_str = recv_buf;
		for (int index = 0; index < recv_str.size(); index++)
		{
			if (recv_str[index] == 0x20)
			{
				recv_str.erase(index, 1);
				index--;
			}
		}
		string recv_device = recv_str.substr(12, 2);
		int device = stoi(recv_device, 0, 16);
		if (device != 9)
		{
			getchar();
			return -1;
		}
		if (recv_str.size() <= 14)
		{
			return 0;
		}
		string recv_code = recv_str.substr(14, 2);
		int function = stoi(recv_code, 0, 16);
		if (function == 1)
		{
			char *send_buf_code = new char(100);
			memset(send_buf_code, 0, sizeof(send_buf_code));
			//send_buf_code1= FUNCTION01(recv_buf, send_buf_code, m, val);
			//FUNCTION01(recv_buf, send_buf_code, m, val);
			strcpy(send_buf, FUNCTION01(recv_str, send_buf_code, m, val));
		}
		if (function == 3)
		{
			char *send_buf_code = new char(100);
			memset(send_buf_code, 0, sizeof(send_buf_code));
			strcpy(send_buf, FUNCTION03(recv_str, send_buf_code, m, val));
		}
		if (function == 15)
		{
			char *send_buf_code = new char(100);
			memset(send_buf_code, 0, sizeof(send_buf_code));
			strcpy(send_buf, FUNCTION0F(recv_str, send_buf_code, m, val));
		}
		if (function == 16)
		{
			char *send_buf_code = new char(100);
			memset(send_buf_code, 0, sizeof(send_buf_code));
			strcpy(send_buf, FUNCTION10(recv_str, send_buf_code, m, val));
		}
		else if (function != 1 && function != 3 && function != 15 && function != 16)
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
			int space = 2;
			while (space <= send.size())
			{
				send.insert(space, " ");
				space += 3;
			}
			int b;
			memset(send_buf, 0, sizeof(send_buf));
			for (b = 0; b < send.size(); b++)
			{
				send_buf[b] = send[b];
			}
			cout << "无可用功能码" << endl;
			cout << "响应报文为：" << send_buf << endl;

		}
		//cout << "请输入回复信息:";
		//cin >> send_buf;
		unsigned char end_data[255] = { 0 };
		HexstrtoByte(send_buf, end_data, strlen(send_buf));
		send_len = send(s_accept,(char*)end_data, strlen(send_buf)/2, 0);
		if (send_len < 0)
		{
			cout << "发送失败！" << endl;
			break;
		}
	}
	//关闭套接字
	closesocket(s_server);
	closesocket(s_accept);
	//释放DLL资源
	WSACleanup();
	return 0;
}
void initialization()
{
	//初始化套接字库
	WORD w_req = MAKEWORD(2, 2);//版本号
	WSADATA wsadata;
	int err;
	err = WSAStartup(w_req, &wsadata);
	if (err != 0)
	{
		cout << "初始化套接字库失败！" << endl;
	}
	else
	{
		cout << "初始化套接字库成功！" << endl;
	}
	//检测版本号
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2)
	{
		cout << "套接字库版本号不符！" << endl;
		WSACleanup();
	}
	else
	{
		cout << "套接字库版本正确！" << endl;
	}
	//填充服务端地址信息
}