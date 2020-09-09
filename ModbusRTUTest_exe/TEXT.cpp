#include<stdio.h>
#include<stdlib.h>
#include"main.h"
#include<iostream>
#include<windows.h>
using namespace std;
int main()
{
	char data1[256];
	memset(data1, 0, sizeof(data1));
	string add_01 = "09";
	string code_01 = "01";
	strcpy(data1, function01(add_01, code_01));
	cout <<"01请求报文："<< data1 << endl;
	string send_buf_01= data1;
	string recv_buf_01 = "090105FFFFFFFF00C534";
	respond_massage(recv_buf_01, send_buf_01);
	char data3[256];
	memset(data3, 0, sizeof(data3));
	string add_03 = "09";
	string code_03 = "03";
	strcpy(data3, function01(add_03, code_03));
	cout << "03请求报文：" << data3 << endl;
	string send_buf_03 = data3;
	string recv_buf_03 = "090306FFFFFFFF0000F453";
	respond_massage(recv_buf_03, send_buf_03);
	char data0F[600];
	memset(data0F, 0, sizeof(data0F));
	string add_0F = "09";
	string code_0F = "0F";
	strcpy(data0F, function0F(add_0F, code_0F));
	cout << "0F请求报文：" << data0F << endl;
	string send_buf_0F = data0F;
	string recv_buf_0F = "090F000200093545";
	respond_massage(recv_buf_0F, send_buf_0F);
	char data10[600];
	memset(data10, 0, sizeof(data10));
	string add_10 = "09";
	string code_10 = "10";
	strcpy(data10, function10(add_10, code_0F));
	cout << "10请求报文：" << data0F << endl;
	string send_buf_10 = data10;
	string recv_buf_10 = "090F00020004F80";
	respond_massage(recv_buf_10, send_buf_10);
	system("pause");
	return 0;
}