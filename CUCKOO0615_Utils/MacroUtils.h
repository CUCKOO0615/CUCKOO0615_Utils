
#pragma once

#ifndef NULL
#define NULL 0
#endif

#if _MSC_VER<=1200
#define  _WIN32_WINNT 0x0500
#endif

//BOOL-bool 互转
#define B2b(B)  (0!=B)
#define b2B(b)  (b?1:0)

/************************************************************************/
// __STR_LINE__:  __LINE__转字符串形式
#define _s_l_(x) #x
#define _str_line_(x) _s_l_(x)
#define __STR_LINE__ _str_line_(__LINE__)
/************************************************************************/

/*
#ifdef EXPORT_STDC
#define CK_API extern "C" __declspec(dllexport)
#else
#define CK_API extern "C" __declspec(dllimport)
#endif

#ifdef EXPORT_CPP 
#define CK_API __declspec(dllexport)  
#else 
#define CK_API __declspec(dllimport) 
#endif
*/

