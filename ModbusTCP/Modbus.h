#include<vector>
#include<string>
using namespace std;
void WriteCoilCommand(int &m, int &n, vector<int>&val);
void WriteRigisterCommand(int &m, int &n, vector<int>&val);
string DEtoHEX(int n);
char *FUNCTION01(char *s, char* send_buf, int k, vector<int>val);
char *FUNCTION03(char *s, char* send_buf, int k, vector<int>val);