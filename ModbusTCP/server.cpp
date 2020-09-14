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
	struct timeval timeout = { 3, 0 };//select�ȴ�3�룬3����ѯ.
	fd_set fds;
	string co;
	int code, m = 0, n = 0;
	vector<int>val;
	cout << "01-����Ȧ" << " " << "03-���Ĵ���" << " " << "0F-д��Ȧ" << " " << "10-д�Ĵ���" << endl;
	WriteCoilCommand(m, n, val);
	/*cout << "�����빦���룺";
	while (1)
	{
		cin >> co;
		code = stoi(co, 0, 16);
		if (code != 1 && code != 3 && code != 15 && code != 16)
		{
			cout << "��֧�ִ˹����룬���������룺";
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
	//���峤�ȱ���
	int send_len = 0;
	int recv_len = 0;
	int len = 0;
	int socket_close = 0;//�ж����ڶϿ���
	/*char send_buf_code[100];
	memset(send_buf_code, 0, sizeof(send_buf_code));*/
	//���巢�ͻ������ͽ��ܻ�����
	char send_buf[600];
	memset(send_buf, 0, sizeof(send_buf));//��ʼ���ڴ棬����������
	char recv_buf[600];
	memset(recv_buf, 0, sizeof(recv_buf));
	UINT8 recv_buf_16[600];
	memset(recv_buf_16, 0, sizeof(recv_buf_16));
	//���������׽��֣����������׽���
	SOCKET s_server;
	SOCKET s_accept;
	//����˵�ַ�ͻ��˵�ַ
	SOCKADDR_IN server_addr;
	SOCKADDR_IN accept_addr;
	initialization();
	//���������Ϣ
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(526);
	//�����׽���
	s_server = socket(AF_INET, SOCK_STREAM, 0);
	if (bind(s_server, (SOCKADDR *)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		cout << "�׽��ְ�ʧ�ܣ�" << endl;
		WSACleanup();
	}
	else
	{
		cout << "�׽��ְ󶨳ɹ���" << endl;
	}
	//�����׽���Ϊ����״̬
	if (listen(s_server, SOMAXCONN) < 0)
	{
		cout << "���ü���״̬ʧ�ܣ�" << endl;
		WSACleanup();
	}
	else
	{
		cout << "���ü���״̬�ɹ���" << endl;
	}
	cout << "��������ڼ������ӣ����Ժ�...." << endl;

	//������������
	len = sizeof(SOCKADDR);
	s_accept = accept(s_server, (SOCKADDR *)&accept_addr, &len);
	if (s_accept == SOCKET_ERROR)
	{
		cout << "����ʧ�ܣ�" << endl;
		WSACleanup();
		return 0;
	}
	cout << "���ӽ�����׼����������" << endl;
	//��������
	while (1)
	{
		//int ret;
		//FD_ZERO(&fds); //ÿ��ѭ����Ҫ��ռ��ϣ������ܼ���������仯
		//FD_SET(s_accept, &fds); //���������
		socket_close = 1;
		memset(recv_buf_16, 0, 600); 
		memset(send_buf, 0, strlen(send_buf));
		recv_len = recv(s_accept, (char*)recv_buf_16, sizeof(recv_buf_16), 0);
		if (recv_len == 0 || recv_len == SOCKET_ERROR)
		{
			int close;
			cout << "����������Ͽ����Ƿ����������0�رգ�1��������";
			while (1)
			{
				cin >> close;
				if (close == 1 || close == 0)
				{
					break;
				}
				cout << "���벻�淶�����������룺";
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
					cout << "���ü���״̬ʧ�ܣ�" << endl;
					WSACleanup();
				}
				else
				{
					cout << "���ü���״̬�ɹ���" << endl;
				}
				cout << "��������ڼ������ӣ����Ժ�...." << endl;
			}
			s_accept = accept(s_server, (SOCKADDR *)&accept_addr, &len);
			if (s_accept == SOCKET_ERROR)
			{
				cout << "����ʧ�ܣ�" << endl;
				WSACleanup();
				return 0;
			}
			else
			{
				cout << "���ӽ�����׼����������" << endl;
				continue;
			}
		}
		else if (recv_len > 0)
		{
			cout << "�������ݳɹ�" << endl;
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
		cout << "�ͻ�����Ϣ:" << recv_buf << endl;
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
			cout << "���ݳ��ȴ���" << endl;
			continue;
		}
		string recv_device = recv_str.substr(12, 2);
		int device = stoi(recv_device, 0, 16);
		if (device != 9 && device != 0)
		{
			cout << "�豸�ų���" << endl;
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
			cout << "���ݳ��ȴ���" << endl;
			continue;
		}
		else if ((function == 15 || function == 16) && recv_str.size() <= 24)
		{
			cout << "���ݳ��ȴ���" << endl;
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
			cout << "�޿��ù�����" << endl;

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
		//cout << "������ظ���Ϣ:";
		//cin >> send_buf;
		send_buf[13] = '9';
		cout << "��Ӧ����Ϊ��" << send_buf << endl;
		unsigned char end_data[600] = { 0 };
		HexstrtoByte(send_buf, end_data, strlen(send_buf));
		send_len = send(s_accept,(char*)end_data, strlen(send_buf)/2, 0);
		if (send_len < 0)
		{
			cout << "����ʧ�ܣ�" << endl;
			break;
		}
	}
	if (socket_close == 0)
	{
		cout << "���ڶϿ�" << endl;
	}
	//�ر��׽���
	closesocket(s_server);
	closesocket(s_accept);
	//�ͷ�DLL��Դ
	WSACleanup();
	return 0;
}