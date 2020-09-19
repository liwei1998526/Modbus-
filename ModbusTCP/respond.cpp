#include"Modbus.h"
#include<iostream>
bool respond(string recv_str, char* &send_buf,int m1,int m2,vector<int>val1,vector<int>val2)
{
	char *send_buf_code = new char[600];
	memset(send_buf_code, 0, sizeof(send_buf_code));
	//�ж��豸���Ƿ���ȷ
	string recv_device = recv_str.substr(12, 2);//�������豸��
	int device = stoi(recv_device, 0, 16);
	if (device != 9 && device != 0)
	{
		cout << "�豸�ų���" << endl;
		return false;
	}
	//�ȴ����ж����ݳ����Ƿ���ȷ
	if (recv_str.size() <= 14)
	{
		return false;
	}
	//���ݹ������ж����ݳ����Ƿ���ȷ
	string recv_code = recv_str.substr(14, 2);
	int function = stoi(recv_code, 0, 16);
	if ((function == 01 || function == 03) && recv_str.size() != 24)
	{
		cout << "���ݳ��ȴ���" << endl;
		return false;
	}
	else if ((function == 15 || function == 16) && recv_str.size() <= 24)
	{
		cout << "���ݳ��ȴ���" << endl;
		return false;
	}
	//�жϹ������Ƿ����
	if (function != 1 && function != 3 && function != 15 && function != 16)
	{
		string send;
		for (int i = 0; i < 18; i++)
		{
			send += recv_str[i];
		}
		int abn = stoi(send.substr(14, 1), 0, 16) + 8;//�����쳣������
		string abn_str = DEtoHEX(abn);
		send[8] = '0';
		send[9] = '0';
		send[10] = '0';
		send[11] = '3';
		send[14] = abn_str[0];
		send[16] = '0';
		send[17] = '1';
		int b;
		send_buf = new char(0);
		memset(send_buf, 0, sizeof(send_buf));
		for (b = 0; b < send.size(); b++)
		{
			send_buf[b] = send[b];
		}
		cout << "�޿��ù�����" << endl;
		return true;
	}
	//���ݹ�������д���
	else if (function == 1)
	{
		strcpy(send_buf, FUNCTION01(recv_str, send_buf_code, m1, val1));
	}
	else if (function == 3)
	{
		strcpy(send_buf, FUNCTION03(recv_str, send_buf_code, m2, val2));
	}
	else if (function == 15)
	{
		strcpy(send_buf, FUNCTION0F(recv_str, send_buf_code, m1, val1));
	}
	else if (function == 16)
	{
		strcpy(send_buf, FUNCTION10(recv_str, send_buf_code, m2, val2));
	}
	return true;
}