#include"Modbus.h"
#include<iostream>
using namespace std;
bool DATA_effective(string recv_str)
{
	//�ж�ͨ�����⡣
	string signal_str = recv_str.substr(4, 4);//������ͨ��Э��
	int signal_int = stoi(signal_str, 0, 16);
	if (signal_int != 0)
	{
		cout << "ͨ��Э�����" << endl;
		return false;
	}
	//�жϷ��س����Ƿ���ȷ
	string recv_len = recv_str.substr(8, 4);//�����ı���ͷ���ݳ���
	int expect_len = stoi(recv_len, 0, 16);
	string recv_exprct_data = recv_str.substr(12, recv_str.size() - 12);//����������λ�ַ���
	if (expect_len != (recv_exprct_data.size() / 2))
	{
		cout << "����ͷ���ݳ��ȴ���" << endl;
		return false;
	}
	return true;
}