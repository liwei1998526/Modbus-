#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\ModbusTCP\Modbus.h"
#include<stdio.h>
#include<windows.h>
#include<iostream>
#include <string>
#include<tchar.h>
#define HEX_PATH "../HextostringTest.ini"
using namespace std;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ModbusTCPTest
{
	TEST_CLASS(HextostringTest)
	{
	public:
		
		TEST_METHOD(HextostringTest1)
		{
			int j = GetPrivateProfileInt(_T("test"), _T("j"), 1, _T(HEX_PATH));
			for (int i = 1; i <= j; i++)
			{
				CString a, b;
				char actual[600];
				memset(actual, 0, sizeof(actual));
				CString str = _T("");
				str.Format(_T("%d"), i);
				USES_CONVERSION;
				const char*st = T2A(str);
				char path[10] = "test";
				strcat_s(path, 8, st);
				CString add(path);
				GetPrivateProfileString(add, _T("input"), _T("error"), a.GetBuffer(MAX_PATH), MAX_PATH, _T(HEX_PATH));
				a.ReleaseBuffer();
				char*ch = NULL;
				ch = T2A(a);
				unsigned char crc[256] = { 0 };
				HexstrtoByte(ch, crc, strlen(ch));
				strcpy(actual, hex2str(crc, strlen(ch) / 2));
				GetPrivateProfileString(add, _T("output"), _T("error"), b.GetBuffer(MAX_PATH), MAX_PATH, _T(HEX_PATH));
				b.ReleaseBuffer();
				char*expect_1 = T2A(b);
				char expect[600];
				memset(expect, 0, 600);
				strcpy(expect, expect_1);
				Assert::AreEqual(expect, actual);
			}
		}
	};
}