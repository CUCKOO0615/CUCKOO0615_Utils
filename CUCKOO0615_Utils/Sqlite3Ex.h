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
        // 空类型
        SQLITE3EX_NULL,
        // 文本类型,使用数据库编码存放
        SQLITE3EX_TEXT,
        // 有符号整型,根据值的大小以1/2/4/6或8字节存放
        SQLITE3EX_INTEGER,
        // 浮点型,以8字节IEEE浮点数存放
        SQLITE3EX_REAL,
        // 二进制数据块
        SQLITE3EX_BLOB
    };

    /*
    ** 列属性结构体
    ** 构造函数:
    ** @param szCN: 列名
    ** @param emCt: 列类型,默认为文本类型,详见 ColumnType
    ** @param bPrimaryKey: 是否设为主键,默认 false
    ** @param bUnique: 是否唯一,默认 false
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
        ** 获取用于拼接SQLite语句的字符串, 格式: "列名 类型名"
        ** @param szStr长度大于SQLITE3EX_COLSTRFORPARAM_LEN可确保有足够空间
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
    ** 返回Sqlite3Ex内部持有的sqlite3数据库对象
    ** 用于直接使用sqlite3函数进行操作
    */
    sqlite3* GetObject() { return m_pSqlite; };

    /*
    ** 通过调用sqlite3_open16打开数据库
    ** @param szDbPath: 数据库文件路径,UTF16宽字符串,支持中文字符
    */
    bool Open(const wchar_t* szDbPath);

    /*
    ** 创建表
    ** @param szTableName: 表名
    ** @param nColNum: 列数
    ** @param tableAttrib: 列属性结构体, 可添加多个列, 结构体成员详见ColumnAttrib
    */
    bool CreateTable(const char* szTableName, int nColNum, ColumnAttrib tableAttrib, ...);

    /*
    ** 获取表名
    ** @param vecTableNames: 包含数据库中所有的表名
    ** @return: 操作成功返回true, 操作失败返回false, 由GetErrMsg获取错误信息
    */
    bool GetTableNames(vector<string> & vecTableNames);

    /*
    ** sqlite3_exec()回调函数类型,每查询到一行记录,该指针指向的函数会被触发一次
    ** @param pFirstParam: 由sqlite3_exec()第四个参数传入
    ** @param iColNum: 查询到的列数
    ** @param pRecordArr: 查询到的记录数组, argv[0]/argv[1]/argv[2],分别代表本行地1/2/3列的上的记录
    ** @param pColNameArr: 查询到的列的表头名字数组
    ** @return:
    ** - 0 继续列举查找到的数据
    ** - 1 中断查找, 继续调用sqlite3_exec()将会返回 SQLITE_ABORT
    */
    typedef int(*LPEXEC_CALLBACK)(void* pFirstParam, int iColNum, char** pRecordArr, char** pColNameArr);

    /*
    ** 执行指定的SQL查询语句
    ** @param szQuery: SQL查询语句
    ** @param LPEXEC_CALLBACK: 回调函数的指针,每查询到一行记录,会调用一次该指针
    ** @param pFirstParam: 初始化LPEXEC_CALLBACK的第一个参数
    ** @return: 操作成功返回true, 操作失败返回false, 由GetErrMsg获取错误信息
    */
    bool Sqlite3Ex_Exec(const char* szQuery, LPEXEC_CALLBACK callback, void* pFirstParam);


    //开始事务操作
    bool BegTransAction();

    //终止事务操作
    bool EndTransAction();

    //获取错误信息
    const char* GetErrMsg(){ return m_szErrMsg; }

};

