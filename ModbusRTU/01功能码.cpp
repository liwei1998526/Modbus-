#include"main.h"
char* function01(string add,string code)
{
	char* ret;
	int start_add;
	cout << "起始寄存器地址：";
	cin >> start_add;
	string start = DEtoHEX(start_add);
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
	string ADU = add + code + start + sum;
	ret = (char*)ADU.c_str();
	int CR = CRC16((unsigned char*)ret, strlen(ret));
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
	char ALL_DATA[100];
	memset(ALL_DATA, 0, sizeof(ALL_DATA));
	strcpy(ALL_DATA, data);
	return ALL_DATA;
}