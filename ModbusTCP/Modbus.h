#include<vector>
#include<string>
#include<windows.h>
using namespace std;
bool IsSocketClosed(int clientSocket);
void initialization();//初始化
void WriteCoilCommand(int &m, int &n, vector<int>&val);
void WriteRigisterCommand(int &m, int &n, vector<int>&val);
bool DATA_effective(string recv_str);//判断数据是否有效
string DEtoHEX(int n);
string DEtoBIN(int n);
bool respond(string recv_str, char* &send_buf, int m1, int m2, vector<int>val1, vector<int>val2);
char *FUNCTION01(string recv_str, char* send_buf, int k, vector<int>val);//对01功能码处理
char *FUNCTION03(string recv_str, char* send_buf, int k, vector<int>val);//对03功能码处理
char *FUNCTION10(string recv_str, char* send_buf, int k, vector<int>&val);//对0F功能码处理
char *FUNCTION0F(string recv_str, char* send_buf, int k, vector<int>&val);//对10功能码处理
char *hex2str(UINT8 *hex,int len);//hex转成字符串
void HexstrtoByte(const char* source, unsigned char* dest, int sourcelen);