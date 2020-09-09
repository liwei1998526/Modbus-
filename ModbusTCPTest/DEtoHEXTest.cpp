#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\ModbusTCP\Modbus.h"
#include<stdio.h>
#include<windows.h>
#include<iostream>
#include <string>
#include<tchar.h>
#define DEtoHEX_PATH "../DEtoHEXTest.ini"
using namespace std;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ModbusTCPTest
{
	TEST_CLASS(DEtoHEXTest)
	{
	public:
		
		TEST_METHOD(DEtoHEX1)
		{
			int j = GetPrivateProfileInt(_T("test"), _T("j"), 1, _T(DEtoHEX_PATH));
			for (int i = 1; i <= j; i++)
			{
				CString a;
				CString str = _T("");
				str.Format(_T("%d"), i);
				USES_CONVERSION;
				const char*st = T2A(str);
				char path[10] = "test";
				strcat_s(path, 8, st);
				CString add(path);
				int k = GetPrivateProfileInt(add, _T("input"), -1, _T(DEtoHEX_PATH));
				string actual = DEtoHEX(k);
				GetPrivateProfileString(add, _T("output"), _T("error"), a.GetBuffer(MAX_PATH), MAX_PATH, _T(DEtoHEX_PATH));
				a.ReleaseBuffer();
				string expect(W2A(a));
				Assert::AreEqual(expect, actual);
			}
		}
	};
}