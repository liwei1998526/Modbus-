#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\ModbusTCP\Modbus.h"
#include<stdio.h>
#include<windows.h>
#include<iostream>
#include <string>
#include<tchar.h>
#define FUN01_PATH "../Function01Test.ini"
using namespace std;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ModbusTCPTest
{
	TEST_CLASS(Function01Test)
	{
	public:

		TEST_METHOD(Function01Test1)
		{
			int j = GetPrivateProfileInt(_T("test"), _T("j"), 1, _T(FUN01_PATH));
			for (int i = 1; i <= j; i++)
			{
				CString a, b, c;
				char*send_buf=new char(600);
				char actual[600];
				memset(actual, 0, sizeof(actual));
				memset(send_buf, 0, sizeof(send_buf));
				CString str = _T("");
				str.Format(_T("%d"), i);
				USES_CONVERSION;
				const char*st = T2A(str);
				char path[10] = "test";
				strcat_s(path, 8, st);
				CString add(path);
				GetPrivateProfileString(add, _T("input1"), _T("error"), a.GetBuffer(MAX_PATH), MAX_PATH, _T(FUN01_PATH));
				a.ReleaseBuffer();
				string recv_buf(W2A(a));
				int k = GetPrivateProfileInt(add, _T("input2"), -1, _T(FUN01_PATH));
				GetPrivateProfileString(add, _T("input3"), _T("error"), b.GetBuffer(MAX_PATH), MAX_PATH, _T(FUN01_PATH));
				b.ReleaseBuffer();
				char*ch = NULL;
				ch = T2A(b);
				char* sh2 = strtok(ch, ",");
				vector<int>val;
				while (sh2 != NULL)
				{
					val.push_back(atoi(sh2));
					sh2 = strtok(NULL, ",");
				}
				strcpy(actual, FUNCTION01(recv_buf, send_buf, k, val));
				GetPrivateProfileString(add, _T("output"), _T("error"), c.GetBuffer(MAX_PATH), MAX_PATH, _T(FUN01_PATH));
				c.ReleaseBuffer();
				char*expect_1 = T2A(c);
				char expect[600];
				memset(expect, 0, 600);
				strcpy(expect,expect_1);
				Assert::AreEqual(expect, actual);
			}
		}
	};
}