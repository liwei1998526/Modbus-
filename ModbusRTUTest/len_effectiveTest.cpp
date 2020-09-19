#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\ModbusRTU\main.h"
#include<stdio.h>
#include<windows.h>
#include<iostream>
#include <string>
#include<tchar.h>
#define eff_PATH "../len_effectiveTest.ini"
using namespace std;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace ModbusRTUTest
{
	TEST_CLASS(len_effectiveTest)
	{
	public:
		
		TEST_METHOD(len_effectiveTest1)
		{
			int j = GetPrivateProfileInt(_T("test"), _T("j"), 1, _T(eff_PATH));
			for (int i = 1; i <= j; i++)
			{
				CString a, b, Con;
				CString str = _T("");
				str.Format(_T("%d"), i);
				USES_CONVERSION;
				const char*st = T2A(str);
				char path[10] = "test";
				strcat_s(path, 8, st);
				CString add(path);
				GetPrivateProfileString(add, _T("input1"), _T("error"), a.GetBuffer(MAX_PATH), MAX_PATH, _T(eff_PATH));
				a.ReleaseBuffer();
				string recv(W2A(a));
				GetPrivateProfileString(add, _T("input2"), _T("error"), b.GetBuffer(MAX_PATH), MAX_PATH, _T(eff_PATH));
				b.ReleaseBuffer();
				string send(W2A(b));
				bool actual = len_effective(recv, send);
				bool expect;
				GetPrivateProfileString(add, _T("output"), _T("no test"), Con.GetBuffer(MAX_PATH), MAX_PATH, _T(eff_PATH));
				Con.ReleaseBuffer();
				if (Con == "true")
				{
					expect = true;
				}
				else if (Con == "false")
				{
					expect = false;
				}
				Assert::AreEqual(expect, actual);
			}
		}
	};
}