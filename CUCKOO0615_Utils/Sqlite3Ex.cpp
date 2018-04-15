/*#include "StdAfx.h"*/
#include "Sqlite3Ex.h"
#include <string.h>

Sqlite3Ex::Sqlite3Ex(void) :m_pSqlite(NULL)
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

bool Sqlite3Ex::Close()
{
    bool bRet = false;
    if (SQLITE_OK == sqlite3_close(m_pSqlite))
    {
        bRet = true;
        m_pSqlite = NULL;
    }
    return bRet;
}

bool Sqlite3Ex::CreateTable(const char* szTableName, int nColNum, Sqlite3ExColumnAttrib emColAttrib, ...)
{
    if (NULL == szTableName || 0 == strlen(szTableName) || 0 >= nColNum)
    {
        SetErrMsg("Parameter is invalid.");
        return false;
    }

    std::string strSql("CREATE TABLE ");
    strSql += szTableName;
    strSql += "(";

    va_list ap;
    va_start(ap, nColNum);
    char szTmp[SQLITE3EX_COLSTRFORPARAM_LEN];
    for (int i = 0; i != nColNum; ++i)
    {
        ::memset(szTmp, 0x00, SQLITE3EX_COLSTRFORPARAM_LEN);
        Sqlite3ExColumnAttrib & ta = va_arg(ap, Sqlite3ExColumnAttrib);
        ta.GetStringForSQL(szTmp);
        strSql += szTmp;
        strSql += ",";
    }
    va_end(ap);
    *(strSql.end() - 1) = ')';

    return Sqlite3Ex_ExecuteNonQuery(strSql.c_str());
}

bool Sqlite3Ex::BegTransAction()
{
    return Sqlite3Ex_ExecuteNonQuery("BEGIN TRANSACTION");   
}

bool Sqlite3Ex::EndTransAction()
{
    return Sqlite3Ex_ExecuteNonQuery("END TRANSACTION");
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

bool Sqlite3Ex::GetTableNames(std::vector<std::string> & vecTableNames)
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

    vecTableNames.resize(nRow);
    for (int i = 1; i <= nRow; ++i)   
        //i从1开始,以为索引0处的固定值是"name"
        vecTableNames[i - 1] = pRetData[i];

    sqlite3_free_table(pRetData);
    return true;
}

//////////////////////////////////////////////////////////////////////////

int Sqlite3Ex::ExecuteReader_CallBack(void* pFirstParam, int iColNum, char** pRecordArr, char** pColNameArr)
{
    if (iColNum <= 0)
        return 0;
    Sqlite3Ex_Reader* pReader = (Sqlite3Ex_Reader*)pFirstParam;
    if (NULL == pReader)
        return 0;

    std::vector<std::string>* pVecLine = new std::vector<std::string>;
    pVecLine->resize(iColNum);
    for (int i = 0; i != iColNum; ++i)
        (*pVecLine)[i] = pRecordArr[i];
    pReader->m_vecResult.push_back(pVecLine);
    return 0;
}

bool Sqlite3Ex::Sqlite3Ex_ExecuteReader(const char* szQuery, Sqlite3Ex_Reader* pReader)
{
    char* szErrMsg = NULL;
    if (SQLITE_OK == sqlite3_exec(m_pSqlite, szQuery, ExecuteReader_CallBack, (void*)pReader, &szErrMsg))
        return true;
    SetErrMsg(szErrMsg);
    sqlite3_free(szErrMsg);
    return false;
}

bool Sqlite3Ex::Sqlite3Ex_ExecuteNonQuery(const char* szQuery)
{
    char* szErrMsg = NULL;
    if (SQLITE_OK == sqlite3_exec(m_pSqlite, szQuery, NULL, NULL, &szErrMsg))
        return true;
    SetErrMsg(szErrMsg);
    sqlite3_free(szErrMsg);
    return false;
}

bool Sqlite3Ex::IsOpen()
{
    if (!m_pSqlite)
        return false;
    return true;
}

//////////////////////////////////////////////////////////////////////////

