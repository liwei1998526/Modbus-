#include"main.h"
char* function0F(string add, string code)
{
	char* ret;
	vector<string>ALL_DATA;
	int i;
	cout << "��ȡ��Ȧ��ʼ��ַ(��0��ʼ)��";
	cin >> i;
	string start = DEtoHEX(i);
	while (1)
	{
		if (start.size() < 4)
		{
			start = "0" + start;
			continue;
		}
		break;
	}
	int digit;
	cout << "��ȡ��Ȧ������";
	cin >> digit;
	string sum = DEtoHEX(digit);
	while (1)
	{
		if (sum.size() < 4)
		{
			sum = "0" + sum;
			continue;
		}
		break;
	}
	int init = i;
	while (init<i+digit)
	{
		if (init + 8 <= i+digit)
		{
			string data;
			while (1)
			{
				cout << "��" << init << "��" << init + 7 << " ��Ȧ��ֵ(16������)Ϊ��";
				cin >> data;
				int data_max = stoi(data, 0, 16);
				if (data_max > 65535 || data.size() > 2)
				{
					cout << "���벻�淶������������" << endl;
					continue;
				}
				break;
			}
			while (1)
			{
				if (data.size() < 2)
				{
					data = "0" + data;
					continue;
				}
				break;
			}
			ALL_DATA.push_back(data);
		}
		else if (init + 8 >i+digit)
		{
			string data;
			while (1)
			{
				cout << "��" << init << "��" << i + digit-1 << " ��Ȧ��ֵ(16������)Ϊ��";
				cin >> data;
				if (data.size() > 2)
				{
					cout << "�������ݲ��淶������������" << endl;
					continue;
				}
				int data_max = stoi(data, 0, 16);
				if (data_max > 65535)
				{
					cout << "���벻�淶������������" << endl;
					continue;
				}
				break;
			}
			while (1)
			{
				if (data.size() < 2)
				{
					data = "0" + data;
					continue;
				}
				break;
			}
			ALL_DATA.push_back(data);
		}
		init += 8;
	}
	string all_data="";
	for (int index = 0; index < ALL_DATA.size(); index++)
	{
		all_data += ALL_DATA[index];
	}
	int data_ret = all_data.size() / 2;
	string DATA_RET = DEtoHEX(data_ret);
	while (1)
	{
		if (DATA_RET.size() < 2)
		{
			DATA_RET = "0" + DATA_RET;
			continue;
		}
		break;
	}
	string ADU = add + code + start + sum + DATA_RET + all_data;
	ret = (char*)ADU.c_str();
	unsigned char crc[255] = { 0 };
	HexstrtoByte(ret, crc, strlen(ret));
	int CR = CRC16((unsigned char*)crc, strlen(ret)/2);
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
	string dat = ADU + CRC;
	char*data;
	data = (char*)dat.c_str();
	char message[600];
	memset(message, 0, sizeof(message));
	strcpy(message, data);
	return message;
}