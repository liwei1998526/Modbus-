#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\ModbusRTU\main.h"
#include<stdio.h>
#include<windows.h>
#include<iostream>
#include <string>
#include<tchar.h>
#define CRC_PATH "../CRCTest.ini"
using namespace std;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace ModbusRTUTest
{
	TEST_CLASS(CRCTest)
	{
	public:
		
		TEST_METHOD(CRCTest1)
		{
			int j = GetPrivateProfileInt(_T("test"), _T("j"), 1, _T(CRC_PATH));
			for (int i = 1; i <= j; i++)
			{
				CString a, b;
				char*send_buf = new char(600);
				memset(send_buf, 0, sizeof(send_buf));
				CString str = _T("");
				str.Format(_T("%d"), i);
				USES_CONVERSION;
				const char*st = T2A(str);
				char path[10] = "test";
				strcat_s(path, 8, st);
				CString add(path);
				GetPrivateProfileString(add, _T("input"), _T("error"), a.GetBuffer(MAX_PATH), MAX_PATH, _T(CRC_PATH));
				a.ReleaseBuffer();
				string ret(W2A(a));
				send_buf = (char*)ret.c_str();
				unsigned char crc[255] = { 0 };
				HexstrtoByte(send_buf, crc, strlen(send_buf));
				int CR = CRC16((unsigned char*)crc, strlen(send_buf) / 2);
				string actual = DEtoHEX(CR);
				GetPrivateProfileString(add, _T("output"), _T("error"), b.GetBuffer(MAX_PATH), MAX_PATH, _T(CRC_PATH));
				b.ReleaseBuffer();
				string expect(W2A(b));
				Assert::AreEqual(expect, actual);
			}
		}
	};
}