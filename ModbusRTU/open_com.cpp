#include"main.h"
bool OPEN_COM(HANDLE &H_Com)
{
	string COMM;
	string COM;
	int Bund_rate = 0;
	int bund;//选择波特率
	int check;//选择校验位
	HANDLE test_com;
	string com = "com";
	while (1)
	{    //打印所有课使用的串口。
		int COM_open = 0;
		cout << "可使用串口：" << endl;
		for (int port = 1; port < 100; port++)
		{
			COM = com + to_string(port);
			test_com = InitCOM((LPCTSTR)COM.c_str(), 9600, 8, 0, 1);
			if (test_com == INVALID_HANDLE_VALUE)
			{
				continue;
			}
			else
			{
				cout << COM << endl;
				COM_open++;
				CloseHandle(test_com);
				continue;
			}
		}
		if (COM_open == 0)
		{
			cout << "无可用串口，是否再次检测（0结束进程，1重新检测，2继续运行）：";
			int yes;
			cin >> yes;
			if (yes == 1)
			{
				continue;
			}
			else if (yes == 0)
			{
				return false;
			}
		}
		//开始打开串口
		cout << "输入端口号为：";
		string com = "com";
		string how;
		cin >> how;
		COMM = com + how;
		cout << "1--9600" << " " << "2--14400" << " " << "3--19200" << " " << "4--115200" << endl;
		cout << "请选择波特率：";
		cin >> bund;
		if (bund == 1)
		{
			Bund_rate = 9600;
		}
		else if (bund == 2)
		{
			Bund_rate = 14400;
		}
		else if (bund == 3)
		{
			Bund_rate = 19200;
		}
		else if (bund == 4)
		{
			Bund_rate = 115200;
		}
		cout << "0--无校验" << " " << "1--奇校验" << " " << "2--偶校验" << endl;
		cout << "输入校验位：";
		cin >> check;
		H_Com = InitCOM((LPCTSTR)COMM.c_str(), Bund_rate, 8, 0, check);
		if (H_Com == INVALID_HANDLE_VALUE)
		{
			int or;
			cout << "串口打开失败，是否结束界面(0结束，1继续)：";
			while (1)
			{
				cin >> or;
				if (or == 0 || or == 1)
				{
					break;
				}
				cout << "输入值不规范，请重新输入（0结束，1继续）：";
				continue;
			}
			if (or == 1)
			{
				continue;
			}
			else if (or == 0)
			{
				return false;
			}
		}
		else
		{
			cout << "串口打开成功" << endl;
			return true;
		}
		break;
	}
	return true;
}
