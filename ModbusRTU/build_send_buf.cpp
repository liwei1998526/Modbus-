#include"main.h"
using namespace std;
bool build_send_buf(int &open_com, HANDLE &H_Com,string &send_buf)
{
	while (1)
	{
		//RTU��վ�����ɲ��������󣬼�ʱ����ȡ��Ӧ����
		PurgeComm(H_Com, PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR | PURGE_TXABORT);//�������
		char data[256];
		memset(data, 0, sizeof(data));
		string ad;
		cout << "�豸�ţ�";
		cin >> ad;
		//��鴮���Ƿ�Ͽ�
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
		/*�����������������*/
		string co;
		cout << "�����빦���룺";
		//��⴮���Ƿ�Ͽ�
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
		{//01��03���͵ı���,���02�����룬����֤�ӻ��ܷ���ȷ�����쳣�����롣
			strcpy(data, function01(address, code));
		}
		else if (cod == 15)
		{//0F���͵ı���
			strcpy(data, function0F(address, code));
		}
		else if (cod == 16)
		{//10���͵ı���
			strcpy(data, function10(address, code));
		}
		send_buf = data;
		if (true == SendData(H_Com, data, strlen(data)))
		{
			cout << "���������ĳɹ�" << endl;
			return true;
		}
		else
		{
			cout << "����������ʧ��" << endl;
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
