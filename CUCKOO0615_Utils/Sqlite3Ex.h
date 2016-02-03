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

#include "Sqlite3/sqlite3.h"
#include <string.h>
#include <vector>

using namespace std;

#define SQLITE3EX_COLNAME_LEN        100
#define SQLITE3EX_COLSTRFORPARAM_LEN 115

class Sqlite3Ex
{
private:
    sqlite3 * m_pSqlite;
    bool m_bIsOpened;
    char m_szErrMsg[ERRMSG_LENGTH];
    Sqlite3Ex(const Sqlite3Ex& sql3){};
    void SetErrMsg(const char* szErrMsg){ strcpy(m_szErrMsg, szErrMsg); };

public:
    Sqlite3Ex(void);
    ~Sqlite3Ex(void);

    enum ColumnType
    {
        // ������
        SQLITE3EX_NULL,
        // �ı�����,ʹ�����ݿ������
        SQLITE3EX_TEXT,
        // �з�������,����ֵ�Ĵ�С��1/2/4/6��8�ֽڴ��
        SQLITE3EX_INTEGER,
        // ������,��8�ֽ�IEEE���������
        SQLITE3EX_REAL,
        // ���������ݿ�
        SQLITE3EX_BLOB
    };

    /*
    ** �����Խṹ��
    ** ���캯��:
    ** @param szCN: ����
    ** @param emCt: ������,Ĭ��Ϊ�ı�����,��� ColumnType
    ** @param bPrimaryKey: �Ƿ���Ϊ����,Ĭ�� false
    ** @param bUnique: �Ƿ�Ψһ,Ĭ�� false
    */
    struct ColumnAttrib
    {
        char szColName[SQLITE3EX_COLNAME_LEN];
        ColumnType emColumnType;
        bool bIsPrimaryKey;
        bool bIsUnique;

        ColumnAttrib(const char* szCN, ColumnType emCT = SQLITE3EX_TEXT, bool bPrimaryKey = false, bool bUnique = false)
        {
            memset(szColName, 0x00, SQLITE3EX_COLNAME_LEN);
            strcpy(szColName, szCN);
            emColumnType = emCT;
            bIsPrimaryKey = bPrimaryKey;
            bIsUnique = bUnique;
        }

        /*
        ** ��ȡ����ƴ��SQLite�����ַ���, ��ʽ: "���� ������"
        ** @param szStr���ȴ���SQLITE3EX_COLSTRFORPARAM_LEN��ȷ�����㹻�ռ�
        */
        void GetStringForParam(char* szStr)
        {
            strcat(szStr, szColName);
            switch (emColumnType)
            {
            case SQLITE3EX_NULL:
                strcat(szStr, " NULL");
                break;
            case SQLITE3EX_TEXT:
                strcat(szStr, " TEXT");
                break;
            case SQLITE3EX_INTEGER:
                strcat(szStr, " INTEGER");
                break;
            case SQLITE3EX_REAL:
                strcat(szStr, " REAL");
                break;
            case SQLITE3EX_BLOB:
                strcat(szStr, " BLOB");
                break;
            default:
                strcat(szStr, " TEXT");
            }

            if (bIsPrimaryKey)
                strcat(szStr, " PRIMARY KEY");
            if (bIsUnique)
                strcat(szStr, " UNIQUE");
        }

    };

public:

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

    /*
    ** ������
    ** @param szTableName: ����
    ** @param nColNum: ����
    ** @param tableAttrib: �����Խṹ��, ����Ӷ����, �ṹ���Ա���ColumnAttrib
    */
    bool CreateTable(const char* szTableName, int nColNum, ColumnAttrib tableAttrib, ...);

    /*
    ** ��ȡ����
    ** @param vecTableNames: �������ݿ������еı���
    ** @return: �����ɹ�����true, ����ʧ�ܷ���false, ��GetErrMsg��ȡ������Ϣ
    */
    bool GetTableNames(vector<string> & vecTableNames);

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

    /*
    ** ִ��ָ����SQL��ѯ���
    ** @param szQuery: SQL��ѯ���
    ** @param LPEXEC_CALLBACK: �ص�������ָ��,ÿ��ѯ��һ�м�¼,�����һ�θ�ָ��
    ** @param pFirstParam: ��ʼ��LPEXEC_CALLBACK�ĵ�һ������
    ** @return: �����ɹ�����true, ����ʧ�ܷ���false, ��GetErrMsg��ȡ������Ϣ
    */
    bool Sqlite3Ex_Exec(const char* szQuery, LPEXEC_CALLBACK callback, void* pFirstParam);


    //��ʼ�������
    bool BegTransAction();

    //��ֹ�������
    bool EndTransAction();

    //��ȡ������Ϣ
    const char* GetErrMsg(){ return m_szErrMsg; }

};

