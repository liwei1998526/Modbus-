#include"main.h"
void respond_massage(string read_str,string send_buf)
{
	string respond_code = read_str.substr(2, 2);//���ع�����
	int respond_code_int = stoi(respond_code, 0, 16);
	string send_math = send_buf.substr(8, 4);//�����Ķ�ȡ����
	int send_digit = stoi(send_math, 0, 16);
	//�ж���Ӧ�����Ƿ��Ƿ��ʵĴ�վ����
	bool effect = len_effective(read_str, send_buf);
	if (effect == false)
	{
		cout << "������Ч" << endl;
		return;
	}
	else
	{
		if (respond_code_int >= 128)
		{
			string Exception_code = read_str.substr(4, 2);//��Ӧ�����쳣��
			if (Exception_code == "01")
			{
				cout << "�������쳣����վ����������������" << endl;
			}
			else if ((Exception_code == "02"))
			{
				cout << "�Ĵ���������վ��Χ" << endl;
			}
			else if (Exception_code == "03")
			{
				cout << "�Ĵ������ݳ��ޣ���Ĵ�������" << endl;
			}
			else if (Exception_code == "04")
			{
				cout << "��վ�豸����" << endl;
			}
			else
			{
				cout << "��Ч�쳣��" << endl;
			}
		}
		else
		{
			//���չ����뿪ʼ����
			if (respond_code_int == 1)
			{	
				respond_01(read_str, send_buf);
			}
			else if (respond_code_int == 3)
			{	
				respond_03(read_str, send_buf);
			}
			else if (respond_code_int == 15 || respond_code_int == 16)
			{	
				//�жϷ�����ʼλ�Ͷ�ȡ�����뷢�͵��Ƿ���ͬ
				string send_start_add = send_buf.substr(4, 4);//�����ĵ���ʼ��ַ
				string recv_start_add = read_str.substr(4, 4);//��Ӧ���ĵ���ʼ��ַ
				string recv_digit = read_str.substr(8, 4);//��Ӧ���Ķ�ȡ����
				int recv_digit_int = stoi(recv_digit, 0, 16);
				if (send_start_add == recv_start_add&&send_digit == recv_digit_int)
				{
					cout << "�޸����" << endl;
				}
				else
				{
					cout << "������ʼ��ַ���ȡ��������" << endl;
				}
			}
		}
	}
}