#include "StdAfx.h"
#include "ProfileUtils.h"
#include <Windows.h>


ProfileUtils::ProfileUtils(const char* szFilePath):
m_bInited(false)
{
    memset(m_szFilePath, 0x00, MAX_PATH);
    strcpy(m_szFilePath, szFilePath);
    m_bInited = true;
}

ProfileUtils::~ProfileUtils(void)
{
}

bool ProfileUtils::Write2Profile( const char* szSectionName, const char* szKey, const char* szValue )
{
    if(!m_bInited)
        return false;
    return (FALSE != WritePrivateProfileStringA(szSectionName, szKey, szValue, m_szFilePath));
}

bool ProfileUtils::GetProfile( 
    const char* szSectionName, 
    const char* szKey, 
    const char* szDefault, 
    char* pValueBuff, 
    size_t nBufSize )
{
    if(!m_bInited)
        return false;

#ifdef _UNICODE
    if(nBufSize >= GetPrivateProfileStringA(szSectionName, szKey, szDefault, pValueBuff, nBufSize, m_szFilePath))
        return true;
    else
        return false;
#else
    if(nBufSize >= GetPrivateProfileString(szSectionName, szKey, szDefault, pValueBuff, nBufSize, m_szFilePath))
        return true;
    else
        return false;
#endif
}