#pragma once
#include <ObjBase.h>
#include <string>

static const char
*GUID_FORMAT_01 = "{%08lX-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X}",
*GUID_FORMAT_02 = "%08lX-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X",
*GUID_FORMAT_03 = "%08lX%04X%04x%02X%02X%02X%02X%02X%02X%02X%02X";

/*
** 创建GUID字符串
** @Param szFormat: 按照以下几种方式格式化GUID字符串
** GUID_FORMAT_01 : {xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxxx}
** GUID_FORMAT_02 : xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxxx
** GUID_FORMAT_03 : xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
** @Ret :
*/
std::string CkCreateGUID(const char* szFormat = GUID_FORMAT_01)
{
    GUID guid;
    ::CoInitialize(NULL);
    ::CoCreateGuid(&guid);
    ::CoUninitialize();

    char szGuid[39] = { 0 };
    ::sprintf(szGuid, szFormat,
        guid.Data1, guid.Data2, guid.Data3,
        guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
        guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);

    return std::string(szGuid);
}
