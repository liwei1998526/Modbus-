#include<iomanip>
#include<string>
#include<windows.h>
#include"Modbus.h"
using namespace std;
//unsigned int hex2int(unsigned char ucTemp)
//{
//	unsigned char ucResult = 0;
//	if (ucTemp >= 0&&ucTemp <= 9)
//	{
//		ucResult = ucTemp - 0x30;
//	}
//	else if (ucTemp >= 'a'&&ucTemp <= 'f')
//	{
//		ucResult = ucTemp - 'a' + 10;
//	}
//	else if (ucTemp >= 'A'&&ucTemp <= 'F')
//	{
//		ucResult = ucTemp - 'A' + 10;
//	}
//	return ucResult;
//}
char* hex2str(UINT8 *hex,int len)
{
	char str_ret[300];
	char* str;
	vector<string>Hex;
	string ALL_data;
	for (int j = 0; j < len;j++)
	{
		string data = DEtoHEX(hex[j]);
		while (1)
		{
			if (data.size() < 2)
			{
				data = "0" + data;
				continue;
			}
			break;
		}
		Hex.push_back(data);
		data.clear();
		//unsigned int a = hex2int(hex[j++]);
		//unsigned int b = hex2int(hex[j++]);
		//str[i++] = char(a * 16 + b);
	}
	for (int i = 0; i < Hex.size(); i++)
	{
		ALL_data += Hex[i];
	}
	str = (char*)ALL_data.c_str();
	strcpy(str_ret, str);
	return str_ret;
	//str[i] = '\0';
}