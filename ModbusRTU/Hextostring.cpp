#include<iomanip>
#include"main.h"
char* hex2str(UINT8 *hex, int len)
{
	char str_ret[600];
	char* str;
	vector<string>Hex;
	string ALL_data;
	for (int j = 0; j < len; j++)
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
	}
	for (int i = 0; i < Hex.size(); i++)
	{
		ALL_data += Hex[i];
	}
	str = (char*)ALL_data.c_str();
	strcpy(str_ret, str);
	return str_ret;
}