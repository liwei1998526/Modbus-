#include"main.h"
bool len_effective(string read_str,string send_buf)
{
	string recv_id = read_str.substr(0, 2);//���ص��豸��
	string send_id = send_buf.substr(0, 2);//�������豸��
	string send_code = send_buf.substr(2, 2);//�����Ĺ�����
	int send_code_int = stoi(send_code, 0, 16);
	string respond_code = read_str.substr(2, 2);//���ع�����
	int respond_code_int = stoi(respond_code, 0, 16);
	string recv_ret = read_str.substr(4, 2);//��Ӧ���ķ����ֽ���
	int recv_ret_int = stoi(recv_ret, 0, 16);
	string recv_data = read_str.substr(6, read_str.size() - 10);//��Ӧ��������λ�ַ���
	string send_math = send_buf.substr(8, 4);//�����Ķ�ȡ����
	int send_digit = stoi(send_math, 0, 16);
	string ret_str = read_str.substr(0, read_str.size() - 4);//��Ӧ��������λ
	string CRC_str = read_str.substr(read_str.size() - 4, 4);//��Ӧ����CRCУ��
	char*ret = (char*)ret_str.c_str();
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
	if (CRC != CRC_str)
	{
		cout << "CRCУ�����" << endl;//CRCУ�鲻ͨ�����������ݳ����رմ��ڡ�
		return false;
	}
	//�ж���Ӧ�����Ƿ��Ƿ��ʵĴ�վ����
	if (recv_id != send_id)
	{
		cout << "�豸�ų���" << endl;
		return false;
	}
	//�жϹ������Ƿ���ȷ
	if ((respond_code_int != send_code_int) && (respond_code_int != send_code_int + 128))
	{
		cout << "���������" << endl;
		return false;
	}
	if (respond_code_int == 01)
	{
		//�жϷ����ֽ����Ƿ���ȡ�
		if (recv_ret_int != recv_data.size() / 2)
		{
			cout << "�����ֽ�������" << endl;
			return false;
		}
		//�ж����ݳ���
		int v;
		if (send_digit > 8 && send_digit % 8 != 0)
		{
			v = send_digit / 8 + 1;
		}
		else if (send_digit % 8 == 0)
		{
			v = send_digit / 8;
		}
		else if (send_digit < 8)
		{
			v = 1;
		}
		if (read_str.size() != (v * 2 + 10))
		{
			cout << "���ݳ��ȳ���" << endl;
			return false;
		}
	}
	else if (respond_code_int == 3)
	{
		//�жϷ����ֽ����Ƿ���ȡ�
		if (recv_ret_int != recv_data.size() / 2)
		{
			cout << "�����ֽ�������" << endl;
			return false;
		}
		//�ж����ݳ����Ƿ����
		if (read_str.size() != (send_digit * 4 + 10))
		{
			cout << "���ݳ��ȳ���" << endl;
			return false;
		}
	}
	else if (respond_code_int == 15 || respond_code_int == 16)
	{	//�ж����ݳ����Ƿ����
		if (read_str.size() != 16)
		{
			cout << "���ݳ��ȳ���" << endl;
			return false;
		}
	}
	else
	{
		if (read_str.size() != 10)
		{
			cout << "�쳣��Ӧ���ģ����ݳ��ȳ���" << endl;
			return false;
		}
	}
	return true;
}