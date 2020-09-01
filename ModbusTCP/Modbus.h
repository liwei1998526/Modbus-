#include<vector>
#include<string>
using namespace std;
void WriteCoilCommand(int &m, int &n, vector<int>&val);
void WriteRigisterCommand(int &m, int &n, vector<int>&val);
string DEtoHEX(int n);
string DEtoBIN(int n);
char *FUNCTION01(string recv_str, char* send_buf, int k, vector<int>val);
char *FUNCTION03(string recv_str, char* send_buf, int k, vector<int>val);
char *FUNCTION10(string recv_str, char* send_buf, int k, vector<int>&val);
char *FUNCTION0F(string recv_str, char* send_buf, int k, vector<int>&val);
