#include<vector>
#include<string>
#include<windows.h>
using namespace std;
bool IsSocketClosed(int clientSocket);
void initialization();//��ʼ��
void WriteCoilCommand(int &m, int &n, vector<int>&val);
void WriteRigisterCommand(int &m, int &n, vector<int>&val);
bool DATA_effective(string recv_str);//�ж������Ƿ���Ч
string DEtoHEX(int n);
string DEtoBIN(int n);
bool respond(string recv_str, char* &send_buf, int m1, int m2, vector<int>val1, vector<int>val2);
char *FUNCTION01(string recv_str, char* send_buf, int k, vector<int>val);//��01�����봦��
char *FUNCTION03(string recv_str, char* send_buf, int k, vector<int>val);//��03�����봦��
char *FUNCTION10(string recv_str, char* send_buf, int k, vector<int>&val);//��0F�����봦��
char *FUNCTION0F(string recv_str, char* send_buf, int k, vector<int>&val);//��10�����봦��
char *hex2str(UINT8 *hex,int len);//hexת���ַ���
void HexstrtoByte(const char* source, unsigned char* dest, int sourcelen);