#include "StdAfx.h"
#include "ProfileUtils.h"
#include <Windows.h>


ProfileUtils::ProfileUtils(const char* szFilePath)
{
    ::memset(m_szFilePath, 0x00, MAX_PATH);
    ::strcpy(m_szFilePath, szFilePath);
}

ProfileUtils::~ProfileUtils(void)
{
    ::memset(m_szFilePath, 0x00, MAX_PATH);
}

bool ProfileUtils::Write2Profile( const char* szSectionName, const char* szKey, const char* szValue )
{
    return (FALSE != WritePrivateProfileStringA(szSectionName, szKey, szValue, m_szFilePath));
}

bool ProfileUtils::GetProfile( 
    const char* szSectionName, 
    const char* szKey, 
    const char* szDefault, 
    char* pValueBuff, 
    size_t nBufSize )
{
    return (nBufSize >= GetPrivateProfileStringA(szSectionName, szKey, szDefault, pValueBuff, nBufSize, m_szFilePath));
}