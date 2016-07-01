// CUCKOO0615_Utils.cpp : 定义控制台应用程序的入口点。
//

//test
//#include "ThreadUtils.hpp"
#include "PathUtils.h"
#include "WinAPI_Utils.h"
//#include "CkRingBuffer.hpp"
//#include "Hpp/LogUtils.hpp"
// #include "Hpp/PtrUtils.hpp"
// #include "Hpp/CkRunnable.hpp"
// #include "Hpp/DigitUtils.hpp"
#include "MultiIncludeTest.h"
#include "TimeUtils.h"
//#include <thread>
#include <iostream>
#include "MacroUtils.h"
#include "GuidUtils.hpp"
#include <windows.h>

using namespace std;


// LUA TEST
// #pragma comment(lib, "./LuaJIT-2.0.3/src/lua51.lib")
extern "C"
{
	// #include "LuaJIT-2.0.3/src/lua.h"
	// #include "LuaJIT-2.0.3/src/lua.hpp"
	// #include "LuaJIT-2.0.3/src/lualib.h"
#include "ctest.h"
};

//////////////////////////////////////////////////////////////////////////
#include "CUCKOO0615_Utils.h"
/*

void SetConsoleUtf8()
{
	SetConsoleOutputCP(CP_UTF8);
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_FONT_INFOEX info;
	memset(&info, 0, sizeof(CONSOLE_FONT_INFOEX));
	info.cbSize = sizeof(CONSOLE_FONT_INFOEX);

	GetCurrentConsoleFontEx(h, FALSE, &info);
	info.FontFamily = FF_DONTCARE;
	info.dwFontSize.X = 0;
	info.dwFontSize.Y = 16;
	info.FontWeight = FW_NORMAL;

	wcscpy_s(info.FaceName, L"Lucida Console");
	SetCurrentConsoleFontEx(h, FALSE, &info);
	
	HWND hwnd = GetConsoleWindow();
	SetWindowPos(hwnd, HWND_TOP, 100, 80, 1000, 700, 0);
}*/

#include ".\\zlib128-dll\\include\\zconf.h"
#include ".\\zlib128-dll\\include\\zlib.h"
#pragma comment(lib, ".\\zlib128-dll\\lib\\zdll.lib")


#include<iostream>
#include<cmath>
#include<iomanip>
using namespace std;

#include "iostream"
using namespace std;
int fact(int n)//求阶乘
{
	int sum = 1;
	int i;
	if (n == 0)
		return 1;
	else
	{
		for (i = 1; i <= n; i++)
			sum *= i;
		return sum;
	}
}

int main()
{
	int i = 1, j = 1, n;
	double e, t = 0;


	cin >> n;
	e = 0;
	for (i = 0; i <= n; i++)
		t += 1.0 / fact(i);

	e = e + t;



	cout << "e约等于" <<fixed<< setprecision(1000) << e << '\n';
	return 0;

    //     const char* d = "";
    // 
    //     char* b = (char*)d;
    //     pu.FixBackSlashInDirPath(a);


    //     HZIP hz = CreateZip(L"x:\\simple1.zip", 0);
    //     ZipAdd(hz, L"doc\\1.docx", L"x:\\1.docx");
    //     ZipAdd(hz, L"pdf\\2.pdf", L"x:\\2.pdf");
    //     ZipAdd(hz, L"doc\\3.doc", L"x:\\3.doc");
    //     CloseZip(hz);
}

/*
	char sz[] = "CUCKOO0615 TEST";
	int nLen = strlen(sz) + 1;

	int nBuffLen = compressBound(nLen);
	char* pBuff = new char[nBuffLen];
	memset(pBuff, 0x00, nBuffLen);

	compress((Bytef*)pBuff, (uLongf*)&nBuffLen, (Bytef*)sz, nLen);

	fstream fs;
	fs.open("D:/123.txt", ios::out|ios::binary);
	fs.write(pBuff, nBuffLen);
	fs.close();*/
// 
// 	char tmpBuff[30] = { 0 };
// 	int n = 30;
// 	uncompress((Bytef*)tmpBuff, (uLongf*)&n, (Bytef*)pBuff, nBuffLen);

	//////////////////////////////////////////////////////////////////////////
// 	std::wcout.imbue(std::locale("chs"));
// 	std::cout << "最大值: " << std::endl;
// 
// 	unsigned __int64 llNum = ULLONG_MAX;
// 	std::wstring wstr;
// 	while (true)
// 	{
// 		DigitUtils::Num2Chinese(llNum, wstr);
// 		std::wcout << wstr.c_str() << std::endl;
// 		std::cout << "输入: ";
// 		std::cin >> llNum;
// 		std::cout << ">>> ";
// 	}
// 
// 
// 	system("pause");

    /*
    lua_State * L = luaL_newstate();
    luaL_openlibs(L);

    luaL_dofile(L, "E:\\test.lua");
    lua_getglobal(L, "add");
    lua_pushnumber(L, x);
    lua_pushnumber(L, y);
 
    int nRet = lua_pcall(L, 2, 1, NULL);
    if (nRet != 0)
    {
       cout << lua_tostring(L, -1) << endl;
    }
 
    z = lua_tonumber(L, -1);
    lua_pop(L, 1);
 
    cout << z << endl;
    lua_close(L);
    */
//    system("pause");

    /*
    ProcessUtils pu;
    bool bRet = pu.CheckProcessExist(TEXT("dwm.EXE"));
    */

    /*
    vector<PROCESSENTRY32> vec;
    ProcessUtils pu;
    pu.GetProcessEntryList(vec);
    */

    /*
    char a[] = "1";
    StringUtils::Trim(a, strlen(a)+1);
    */

    /*
    time_t a;
    time(&a);
    char szBuf[TIME_STR_LENGTH] = {0};
    TimeUtils::Time_t2Str(a, szBuf, TIME_STR_LENGTH);
    */

    /*
    Sqlite3Ex sqlite3Ex;
    sqlite3Ex.Open(L"E:\\1.DB");

    Sqlite3Ex::ColumnAttrib ta1("TestCol1");
    Sqlite3Ex::ColumnAttrib ta2("TestCol2", Sqlite3Ex::SQLITE3EX_INTEGER);
    Sqlite3Ex::ColumnAttrib ta3("TestCol3");

    sqlite3Ex.CreateTable("TestTable1", 3, ta1, ta2, ta3);
    sqlite3Ex.CreateTable("TestTable2", 2, ta1, ta2, ta3);
    */

