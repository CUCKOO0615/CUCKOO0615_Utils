#pragma once

#ifndef CUCKOO0615_UTILS_MACRO
#define CUCKOO0615_UTILS_MACRO

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

#ifndef ERRMSG_LENGTH
#define ERRMSG_LENGTH 256
#endif

#endif

#include "sqlite3.h"
#include <vector>

#pragma warning(disable:4996)

#define SQLITE3EX_COLNAME_LEN        100
#define SQLITE3EX_COLSTRFORPARAM_LEN 115


/*
** sqlite3_exec()�ص���������,ÿ��ѯ��һ�м�¼,��ָ��ָ��ĺ����ᱻ����һ��
** @param pFirstParam: ��sqlite3_exec()���ĸ���������
** @param iColNum: ��ѯ��������
** @param pRecordArr: ��ѯ���ļ�¼����, argv[0]/argv[1]/argv[2],�ֱ�����е�1/2/3�е��ϵļ�¼
** @param pColNameArr: ��ѯ�����еı�ͷ��������
** @return:
** - 0 �����оٲ��ҵ�������
** - 1 �жϲ���, ��������sqlite3_exec()���᷵�� SQLITE_ABORT
*/
typedef int(*LPEXEC_CALLBACK)(void* pFirstParam, int iColNum, char** pRecordArr, char** pColNameArr);

#define SQLITE3EX_DECLARE_CALLBACK(funcName) \
	int funcName(void* pFirstParam, int iColNum, char** pRecordArr, char** pColNameArr)

enum Sqlite3ExColumnType
{
    SQLITE3EX_NULL,    // ������
    SQLITE3EX_TEXT,    // �ı�����,ʹ�����ݿ������
    SQLITE3EX_INTEGER, // �з�������,����ֵ�Ĵ�С��1/2/4/6��8�ֽڴ��
    SQLITE3EX_REAL,    // ������,��8�ֽ�IEEE���������
    SQLITE3EX_BLOB     // ���������ݿ�
};

class Sqlite3Ex_Reader;
class Sqlite3ExColumnAttrib;
class Sqlite3Ex
{
private:
    sqlite3 * m_pSqlite;
    char m_szErrMsg[ERRMSG_LENGTH];

private:
    Sqlite3Ex(const Sqlite3Ex& sql3){};
    static int ExecuteReader_CallBack(void* pFirstParam, int iColNum, char** pRecordArr, char** pColNameArr);
    
public:
    Sqlite3Ex(void);
    ~Sqlite3Ex(void);

    /*
    ** ����Sqlite3Ex�ڲ����е�sqlite3���ݿ����
    ** ����ֱ��ʹ��sqlite3�������в���
    */
    sqlite3* GetObject() { return m_pSqlite; };

    /*
    ** ͨ������sqlite3_open16�����ݿ�
    ** @param szDbPath: ���ݿ��ļ�·��,UTF16���ַ���,֧�������ַ�
    */
    bool Open(const wchar_t* szDbPath);

    bool IsOpen();

    /*
    ** ����sqlite_close()
    ** @Ret: �����ɹ�����true,ʧ�ܷ���false
    */
    bool Close();

    /*
    ** ������
    ** @param szTableName: ����
    ** @param nColNum: ����
    ** @param tableAttrib: �����Խṹ��, ����Ӷ����, �ṹ���Ա���ColumnAttrib
    */
    bool CreateTable(const char* szTableName, int nColNum, Sqlite3ExColumnAttrib tableAttrib, ...);

    /*
    ** ��ȡ����
    ** @param vecTableNames: �������ݿ������еı���
    ** @return: �����ɹ�����true, ����ʧ�ܷ���false, ��GetLastErrMsg��ȡ������Ϣ
    */
    bool GetTableNames(std::vector<std::string> & vecTableNames);

    /*
    ** ִ��ָ����SQL��ѯ���
    ** @param szQuery: SQL��ѯ���
    ** @param LPEXEC_CALLBACK: �ص�������ָ��,ÿ��ѯ��һ�м�¼,�����һ�θ�ָ��
    ** @param pFirstParam: ��ʼ��LPEXEC_CALLBACK�ĵ�һ������
    ** @return: �����ɹ�����true, ����ʧ�ܷ���false, ��GetLastErrMsg��ȡ������Ϣ
    */
    bool Sqlite3Ex_Exec(const char* szQuery, LPEXEC_CALLBACK callback, void* pFirstParam);
        
    /*
    ** ִ��ָ�����޷���ֵ��SQL���
    ** @Param szQuery: SQL��ѯ���
    ** @Ret: �����ɹ�����true, ����ʧ�ܷ���false, ��GetLastErrMsg��ȡ������Ϣ
    */
    bool Sqlite3Ex_ExecuteNonQuery(const char* szQuery);

    /*
    ** ִ��ָ����SQL��ѯ���
    ** @Param szQuery: SQL��ѯ���
    ** @Param pReader: ���ڴ洢��ѯ�����Sqlite3Ex_Reader��
    ** @Ret: �����ɹ�����true, ����ʧ�ܷ���false, ��GetLastErrMsg��ȡ������Ϣ
    */
    bool Sqlite3Ex_ExecuteReader(const char* szQuery, Sqlite3Ex_Reader* pReader);

    //��ʼ�������
    bool BegTransAction();

    //��ֹ�������
    bool EndTransAction();

    //��ȡ������Ϣ
    const char* GetLastErrMsg(){ return m_szErrMsg; }

    //���ô�����Ϣ
    void SetErrMsg(const char* szErrMsg)
    {
        if (NULL == szErrMsg) return;
        ::strcpy(m_szErrMsg, szErrMsg);
    };
};


class Sqlite3Ex_Reader
{
public:
    // ����
    void Reset()
    {
        for (int i = 0; i != m_vecResult.size(); ++i)
            delete m_vecResult[i];
        m_vecResult.clear();
        m_nCurIndex = -1;
    }
    // ���Ի�ȡ��¼,��ȡ�ɹ�����true
    bool Read()
    {
        return ((++m_nCurIndex) < m_vecResult.size());
    }
    // ��ȡ��ǰ��¼��ָ�����е����ݵ��ַ���
    const char* GetString(int nCol)
    {
        return (*m_vecResult[m_nCurIndex])[nCol].c_str();
    }
    // ��ȡ��ǰ��¼��ָ�����е����ݵ�int
    int GetInt32(int nCol)
    {
        return ::atoi((*m_vecResult[m_nCurIndex])[nCol].c_str());
    }
    // ��ȡ��ǰ��¼��ָ�����е����ݵ�bool
    bool GetBool(int nCol)
    {
        return (0 != GetInt32(nCol));
    }

private:
    std::vector<std::vector<std::string>* > m_vecResult;
    friend class Sqlite3Ex;
    int m_nCurIndex;
public:
    Sqlite3Ex_Reader() : m_nCurIndex(-1) { }
    ~Sqlite3Ex_Reader() { Reset(); }
};


/*
** ������
** ���캯��:
** @param szCN: ����
** @param emCt: ������,Ĭ��Ϊ�ı�����,��� ColumnType
** @param bPrimaryKey: �Ƿ���Ϊ����,Ĭ�� false
** @param bUnique: �Ƿ�Ψһ,Ĭ�� false
*/
class Sqlite3ExColumnAttrib
{
    char szColName[SQLITE3EX_COLNAME_LEN];
    Sqlite3ExColumnType emColumnType;
    bool bIsPrimaryKey;
    bool bIsUnique;

public:
    Sqlite3ExColumnAttrib(const char* szCN, Sqlite3ExColumnType emCT = SQLITE3EX_TEXT, bool bPrimaryKey = false, bool bUnique = false)
    {
        ::memset(szColName, 0x00, SQLITE3EX_COLNAME_LEN);
        ::strcpy(szColName, szCN);
        emColumnType = emCT;
        bIsPrimaryKey = bPrimaryKey;
        bIsUnique = bUnique;
    }

    /*
    ** ��ȡ����ƴ��SQLite�����ַ���, ��ʽ: "���� ������"
    ** @param szStr���ȴ���SQLITE3EX_COLSTRFORPARAM_LEN��ȷ�����㹻�ռ�
    */
    void GetStringForSQL(char* szStr)
    {
        const char* arrText[] = {    //�μ� Sqlite3ExColumnType
            " NULL",
            " TEXT",
            " INTEGER",
            " REAL",
            " BLOB"
        };
        ::strcat(szStr, szColName);
        ::strcat(szStr, arrText[emColumnType]);
        if (bIsPrimaryKey)   ::strcat(szStr, " PRIMARY KEY");
        if (bIsUnique)       ::strcat(szStr, " UNIQUE");
    }
};
