#include"main.h"
char* function10(string add, string code)
{
	char* ret;
	vector<string>ALL_DATA;
	int i;
	cout << "读取寄存器起始地址(从0开始)：";
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
	cout << "读取寄存器数量：";
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
	while (init<=i+digit-1)
	{
		string date;
		while (1)
		{
			cout << "第" << init << "寄存器的值(16进制)：";
			cin >> date;
			int date_max = stoi(date, 0, 16);
			if (date_max > 65535)
			{
				cout << "输入不规范，请重新输入" << endl;
				continue;
			}
			break;
		}	

		while (1)
		{
			if (date.size() < 4)
			{
				date = "0" + date;
				continue;
			}
			break;
		}
		ALL_DATA.push_back(date);
		init++;
	}
	string all_data = "";
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
	int CR = CRC16((unsigned char*)crc, strlen(ret) / 2);
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
	char message[100];
	memset(message, 0, sizeof(message));
	strcpy(message, data);
	return message;
}