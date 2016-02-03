// CUCKOO0615_Utils.cpp : 定义控制台应用程序的入口点。
//

//test
//#include "ThreadUtils.hpp"
//#include "WinAPI_Utils.h"
//#include "CkRingBuffer.hpp"
//#include "LogUtils.hpp"

//
//std
#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <time.h>
#include <cvt/wstring>
#include <codecvt>
#include <algorithm>
//
//win
#include <shlwapi.h>
//

// LUA TEST
// #pragma comment(lib, "./LuaJIT-2.0.3/src/lua51.lib")
extern "C"
{
	// #include "LuaJIT-2.0.3/src/lua.h"
	// #include "LuaJIT-2.0.3/src/lua.hpp"
	// #include "LuaJIT-2.0.3/src/lualib.h"
#include "ctest.h"
};

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
}

int main()
{

	system("pause");

//  ThreadUtils threadUtils;
//  threadUtils.Run(&ThreadCallBack);

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

    return 0;
}

