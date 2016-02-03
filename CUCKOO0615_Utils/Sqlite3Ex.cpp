#include "StdAfx.h"
#include "Sqlite3Ex.h"


Sqlite3Ex::Sqlite3Ex(void) :m_pSqlite(NULL), m_bIsOpened(false)
{
    memset(m_szErrMsg, 0x00, ERRMSG_LENGTH);
}


Sqlite3Ex::~Sqlite3Ex(void)
{
}

bool Sqlite3Ex::Open(const wchar_t* szDbPath)
{
    int nRet = sqlite3_open16(szDbPath, &m_pSqlite);
    if (SQLITE_OK != nRet)
    {
        SetErrMsg(sqlite3_errmsg(m_pSqlite));
        return false;
    }
    return true;
}

bool Sqlite3Ex::CreateTable(const char* szTableName, int nColNum, ColumnAttrib emColAttrib, ...)
{
    if (NULL == szTableName || 0 == strlen(szTableName) || 0 >= nColNum)
    {
        SetErrMsg("Parameter is invalid.");
        return false;
    }

    int nBufLength = strlen("CREATE TABLE ") + strlen(szTableName) + 1 + nColNum*SQLITE3EX_COLSTRFORPARAM_LEN + 2;
    char* szBuf = new char[nBufLength];
    memset(szBuf, 0x00, nBufLength);
    sprintf(szBuf, "CREATE TABLE %s(", szTableName);

    va_list ap;
    va_start(ap, nColNum);
    char szTmp[SQLITE3EX_COLSTRFORPARAM_LEN];
    for (int i = 0; i != nColNum; ++i)
    {
        memset(szTmp, 0x00, SQLITE3EX_COLSTRFORPARAM_LEN);
        ColumnAttrib & ta = va_arg(ap, ColumnAttrib);
        ta.GetStringForParam(szTmp);
        strcat(szBuf, szTmp);
        if (i != nColNum - 1)
            strcat(szBuf, ",");
        else
            strcat(szBuf, ")");
    }
    va_end(ap);

    char* szErrMsg = NULL;
    int nRet = sqlite3_exec(m_pSqlite, szBuf, 0, NULL, &szErrMsg);
    delete[] szBuf;

    if (SQLITE_OK != nRet)
    {
        SetErrMsg(szErrMsg);
        sqlite3_free(szErrMsg);
        return false;
    }
    return true;
}

bool Sqlite3Ex::BegTransAction()
{
    char* szErrMsg = NULL;
    if (SQLITE_OK != sqlite3_exec(m_pSqlite, "BEGIN TRANSACTION", 0, NULL, &szErrMsg))
    {
        SetErrMsg(szErrMsg);
        sqlite3_free(szErrMsg);
        return false;
    }
    return true;
}

bool Sqlite3Ex::EndTransAction()
{
    char* szErrMsg = NULL;
    if (SQLITE_OK != sqlite3_exec(m_pSqlite, "END TRANSACTION", 0, NULL, &szErrMsg))
    {
        SetErrMsg(szErrMsg);
        sqlite3_free(szErrMsg);
        return false;
    }
    return true;
}

bool Sqlite3Ex::Sqlite3Ex_Exec(const char* szQuery, LPEXEC_CALLBACK callback, void* pFirstParam)
{
    char* szErrMsg = NULL;
    int nRet = sqlite3_exec(m_pSqlite, szQuery, callback, pFirstParam, &szErrMsg);
    if (SQLITE_OK != nRet)
    {
        SetErrMsg(szErrMsg);
        sqlite3_free(szErrMsg);
        return false;
    }
    return true;
}

bool Sqlite3Ex::GetTableNames(vector<string> & vecTableNames)
{
    char* szErrMsg = NULL;
    char** pRetData = NULL;
    int nRow, nCol;
    int nRet = sqlite3_get_table(m_pSqlite, 
        "SELECT name FROM sqlite_master WHERE type='table'", 
        &pRetData, &nRow, &nCol, &szErrMsg);

    if (SQLITE_OK != nRet)
    {
        SetErrMsg(szErrMsg);
        sqlite3_free(szErrMsg);
        return false;
    }

    for (int i = 1; i <= nRow; ++i)
        vecTableNames.push_back(string(pRetData[i]));

    sqlite3_free_table(pRetData);
    return true;
}
