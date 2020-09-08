#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\ModbusTCP\Modbus.h"
#include<stdio.h>
#include<windows.h>
#include<iostream>
#include <string>
#include<tchar.h>
#define atoi_PATH "../atoi.ini"
using namespace std;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ModbusTCPTest
{
	TEST_CLASS(WriteCoilCommandTest)
	{
	public:
		
		TEST_METHOD(WriteCoilCommandTest1)
		{
			int actual, expect;
			int j = GetPrivateProfileInt(_T("test"), _T("j"), 1, _T(atoi_PATH));
			for (int i = 1; i <= j; i++)
			{
				CString a, b, ch;
				CString str = _T("");
				str.Format(_T("%d"), i);
				USES_CONVERSION;
				const char*st = T2A(str);
				char path[10] = "test";
				strcat_s(path, 8, st);
				CString add(path);
		}

	};
}