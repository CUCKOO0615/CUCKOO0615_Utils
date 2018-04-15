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

#define SQLITE3EX_DECLARE_CALLBACK(funcName) \
	int funcName(void* pFirstParam, int iColNum, char** pRecordArr, char** pColNameArr)

enum Sqlite3ExColumnType
{
    SQLITE3EX_NULL,    // 空类型
    SQLITE3EX_TEXT,    // 文本类型,使用数据库编码存放
    SQLITE3EX_INTEGER, // 有符号整型,根据值的大小以1/2/4/6或8字节存放
    SQLITE3EX_REAL,    // 浮点型,以8字节IEEE浮点数存放
    SQLITE3EX_BLOB     // 二进制数据块
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
    ** 返回Sqlite3Ex内部持有的sqlite3数据库对象
    ** 用于直接使用sqlite3函数进行操作
    */
    sqlite3* GetObject() { return m_pSqlite; };

    /*
    ** 通过调用sqlite3_open16打开数据库
    ** @param szDbPath: 数据库文件路径,UTF16宽字符串,支持中文字符
    */
    bool Open(const wchar_t* szDbPath);

    bool IsOpen();

    /*
    ** 调用sqlite_close()
    ** @Ret: 操作成功返回true,失败返回false
    */
    bool Close();

    /*
    ** 创建表
    ** @param szTableName: 表名
    ** @param nColNum: 列数
    ** @param tableAttrib: 列属性结构体, 可添加多个列, 结构体成员详见ColumnAttrib
    */
    bool CreateTable(const char* szTableName, int nColNum, Sqlite3ExColumnAttrib tableAttrib, ...);

    /*
    ** 获取表名
    ** @param vecTableNames: 包含数据库中所有的表名
    ** @return: 操作成功返回true, 操作失败返回false, 由GetLastErrMsg获取错误信息
    */
    bool GetTableNames(std::vector<std::string> & vecTableNames);

    /*
    ** 执行指定的SQL查询语句
    ** @param szQuery: SQL查询语句
    ** @param LPEXEC_CALLBACK: 回调函数的指针,每查询到一行记录,会调用一次该指针
    ** @param pFirstParam: 初始化LPEXEC_CALLBACK的第一个参数
    ** @return: 操作成功返回true, 操作失败返回false, 由GetLastErrMsg获取错误信息
    */
    bool Sqlite3Ex_Exec(const char* szQuery, LPEXEC_CALLBACK callback, void* pFirstParam);
        
    /*
    ** 执行指定的无返回值的SQL语句
    ** @Param szQuery: SQL查询语句
    ** @Ret: 操作成功返回true, 操作失败返回false, 由GetLastErrMsg获取错误信息
    */
    bool Sqlite3Ex_ExecuteNonQuery(const char* szQuery);

    /*
    ** 执行指定的SQL查询语句
    ** @Param szQuery: SQL查询语句
    ** @Param pReader: 用于存储查询结果的Sqlite3Ex_Reader类
    ** @Ret: 操作成功返回true, 操作失败返回false, 由GetLastErrMsg获取错误信息
    */
    bool Sqlite3Ex_ExecuteReader(const char* szQuery, Sqlite3Ex_Reader* pReader);

    //开始事务操作
    bool BegTransAction();

    //终止事务操作
    bool EndTransAction();

    //获取错误信息
    const char* GetLastErrMsg(){ return m_szErrMsg; }

    //设置错误信息
    void SetErrMsg(const char* szErrMsg)
    {
        if (NULL == szErrMsg) return;
        ::strcpy(m_szErrMsg, szErrMsg);
    };
};


class Sqlite3Ex_Reader
{
public:
    // 重置
    void Reset()
    {
        for (int i = 0; i != m_vecResult.size(); ++i)
            delete m_vecResult[i];
        m_vecResult.clear();
        m_nCurIndex = -1;
    }
    // 尝试获取记录,获取成功返回true
    bool Read()
    {
        return ((++m_nCurIndex) < m_vecResult.size());
    }
    // 获取当前记录中指定列中的数据的字符串
    const char* GetString(int nCol)
    {
        return (*m_vecResult[m_nCurIndex])[nCol].c_str();
    }
    // 获取当前记录中指定列中的数据的int
    int GetInt32(int nCol)
    {
        return ::atoi((*m_vecResult[m_nCurIndex])[nCol].c_str());
    }
    // 获取当前记录中指定列中的数据的bool
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
** 列属性
** 构造函数:
** @param szCN: 列名
** @param emCt: 列类型,默认为文本类型,详见 ColumnType
** @param bPrimaryKey: 是否设为主键,默认 false
** @param bUnique: 是否唯一,默认 false
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
    ** 获取用于拼接SQLite语句的字符串, 格式: "列名 类型名"
    ** @param szStr长度大于SQLITE3EX_COLSTRFORPARAM_LEN可确保有足够空间
    */
    void GetStringForSQL(char* szStr)
    {
        const char* arrText[] = {    //参见 Sqlite3ExColumnType
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
