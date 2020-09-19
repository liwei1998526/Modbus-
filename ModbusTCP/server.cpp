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
	vector<int>val1;//�洢��Ȧ��ֵ
	vector<int>val2;//�洢�Ĵ�����ֵ
	cout << "01-����Ȧ" << " " << "03-���Ĵ���" << " " << "0F-д��Ȧ" << " " << "10-д�Ĵ���" << endl;
	WriteCoilCommand(m1, n1, val1);
	cout << "��Ȧ�����ɹ�" << endl;
	WriteRigisterCommand(m2, n2, val2);
	cout << "�Ĵ��������ɹ�" << endl;
	//���峤�ȱ���
	int send_len = 0;//�������ݳ���
	int recv_len = 0;//�������ݳ���
	int len = 0;
	int socket_close = 0;//�ж����ڶϿ���
	//���巢�ͻ������ͽ��ܻ�����
	char send_buf[600];
	memset(send_buf, 0, sizeof(send_buf));//��ʼ���ڴ棬����������
	char recv_buf[600];
	memset(recv_buf, 0, sizeof(recv_buf));
	UINT8 recv_buf_16[600];//���ܻ�����
	memset(recv_buf_16, 0, sizeof(recv_buf_16));
	//���������׽��֣����������׽���
	SOCKET s_server;
	SOCKET s_accept;
	//����˵�ַ�ͻ��˵�ַ
	SOCKADDR_IN server_addr;
	SOCKADDR_IN accept_addr;
	initialization();//��ʼ���׽��ֿ�
	//���������Ϣ
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(526);//�˿ں�
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
		memset(recv_buf_16, 0, 600); //��ʼ�����ܻ�����
		memset(send_buf, 0, strlen(send_buf));
		recv_len = recv(s_accept, (char*)recv_buf_16, sizeof(recv_buf_16), 0);
		//�ж������Ƿ�Ͽ��Լ���������Ͽ��ź�
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
			//������������Ͽ���ѡ��ر�0,��ر��׽��֣����ͷ�dll��Դ��
			if (close == 0)
			{
				break;
			}
			else if (close == 1)
			{
				//��������������Ͽ���ѡ����������¿�ʼ����״̬��
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
		//�жϽ����������ֵ
		if (recv_len > 300)
		{
			cout << "���ݳ���" << endl;
			continue;
		}
		strcpy(recv_buf, hex2str(recv_buf_16, recv_len));//�����ݴ�hexתΪchar*����
		cout << "�ͻ�����Ϣ:" << recv_buf << endl;
		string recv_str = recv_buf;
		for (int index = 0; index < recv_str.size(); index++)
		{//ȥ�ո�
			if (recv_str[index] == 0x20)
			{
				recv_str.erase(index, 1);
				index--;
			}
		}
		string recv_code = recv_str.substr(14, 2);
		int function = stoi(recv_code, 0, 16);
		bool effective = DATA_effective(recv_str);//�ж������Ƿ���Ч
		if (effective == false)
		{
			cout << "������Ч,��������" << endl;
			continue;
		}
		char *send_buf_code = new char[600];
		memset(send_buf_code, 0, sizeof(send_buf_code));
		memset(send_buf, 0, sizeof(send_buf));
		bool send_suc = respond(recv_str, send_buf_code, m1, m2, val1, val2);//����������
		if (send_suc == true)
		{
			strcpy(send_buf, send_buf_code);
		}
		else
		{
			cout << "������Ч" << endl;
			continue;
		}
		send_buf[13] = '9';//�ظ��㲥����
		cout << "��Ӧ����Ϊ��" << send_buf << endl;
		unsigned char end_data[600] = { 0 };
		HexstrtoByte(send_buf, end_data, strlen(send_buf));//��stringת��λhex
		send_len = send(s_accept,(char*)end_data, strlen(send_buf)/2, 0);
		if (send_len < 0)
		{
			cout << "����ʧ�ܣ�" << endl;
			continue;
		}
	}
	//�ر��׽���
	closesocket(s_server);
	closesocket(s_accept);
	//�ͷ�DLL��Դ
	WSACleanup();
	return 0;
}