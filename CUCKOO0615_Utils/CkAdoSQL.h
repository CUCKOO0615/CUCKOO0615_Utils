#pragma once

/*########################################################################
------------------------------------------------
CAdoConnection class
------------------------------------------------
########################################################################*/
//ADO连接对象类封装
//放到公共头文件stdafx.h
#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","adoEOF"),rename("BOF","adoBOF")

class CkAdoSQL
{
    //构建-----------------------------------------------
public:
    CkAdoSQL();
    virtual ~CkAdoSQL();

    //属性-----------------------------------------------
public:
    BOOL SetConnectTimeOut(long lTime);
    BOOL SetCommandTimeOut(long lTime);
    BOOL IsOpen();
    _ConnectionPtr GetConnection();

    //操作-----------------------------------------------
public:
    ErrorsPtr GetErrors();
    //连接数据库，传入连接字符串，包含连接信息
    BOOL Connect(LPCTSTR strConnect, long lOptions = adConnectUnspecified);
    //连接SQLServer数据库，传入服务器、数据库、用户名、密码
    BOOL ConnectSQLServer(LPCTSTR strDbServer, LPCTSTR strDbName, LPCTSTR strUserName, LPCTSTR strUserPwd, long lOptions = adConnectUnspecified);
    BOOL Disconnect();

    BOOL Cancel();

    long BeginTrans();
    BOOL RollbackTrans();
    BOOL CommitTrans();

    _RecordsetPtr OpenSchema(SchemaEnum QueryType);
    CString GetLastError();

    //执行无返回值的SQL语句
    BOOL ExecuteSQL(const CString& strSQL);
    //数据-----------------------------------------------
protected:
    BOOL Release();
    CString			m_strConnect;
    _ConnectionPtr	m_pConnection;
};

/*########################################################################
------------------------------------------------
CAdoRecordSet class
------------------------------------------------
########################################################################*/
//ADO记录集对象类封装
class  CAdoRecordSet
{
    //构建 ------------------------------------------------
public:
    CAdoRecordSet();
    CAdoRecordSet(CkAdoSQL *pConnection);
    virtual ~CAdoRecordSet();
    HRESULT Open(LPCTSTR strSQL, long lOption = adCmdText, CursorTypeEnum CursorType = adOpenStatic, LockTypeEnum LockType = adLockOptimistic);
    void SetAdoConnection(CkAdoSQL *pConnection);
    BOOL Close();

    //属性 ------------------------------------------------	
public:
    BOOL IsEOF();
    BOOL IsBOF();

    long GetState();
    long GetStatus();

    long GetRecordCount();
    long GetFieldsCount();

    CString GetLastError();

    CString GetFieldName(long lIndex);
    DWORD GetFieldType(long lIndex);
    DWORD GetFieldType(LPCTSTR strfield);

    long GetFieldAttributes(long lIndex);
    long GetFieldAttributes(LPCTSTR lpszFieldName);
    long GetFieldDefineSize(long lIndex);
    long GetFieldDefineSize(LPCTSTR lpszFieldName);
    long GetFieldActualSize(long lIndex);
    long GetFieldActualSize(LPCTSTR lpszFieldName);

    Fields* GetFields();
    FieldPtr GetField(long lIndex);
    FieldPtr GetField(LPCTSTR lpszFieldName);
    const _RecordsetPtr& GetRecordset();

    //记录集分页操作 --------------------------------------
public:
    BOOL SetPageSize(long nPageSize);
    long GetPageSize() const;
    BOOL SetAbsolutePage(PositionEnum posEnum);
    PositionEnum GetAbsolutePage() const;
    long GetPageCount() const;

    //记录集更新操作 --------------------------------------
public:
    BOOL AddNew();
    BOOL Update();
    BOOL UpdateBatch(AffectEnum AffectRecords = adAffectAll);
    BOOL CancelUpdate();
    BOOL CancelBatch(AffectEnum AffectRecords = adAffectAll);
    BOOL Delete(AffectEnum AffectRecords = adAffectCurrent);

    //记录集导航操作 --------------------------------------
public:
    BOOL MoveFirst();
    BOOL MovePrevious();
    BOOL MoveNext();
    BOOL MoveLast();
    BOOL Move(long lRecords, long Start = adBookmarkCurrent);

    //其他方法 --------------------------------------------
public:
    BOOL IsOpen();
    BOOL PutCollect(long index, const _variant_t &value);
    BOOL PutCollect(long index, const LPCSTR &value);
    BOOL PutCollect(long index, const double &value);
    BOOL PutCollect(long index, const float &value);
    BOOL PutCollect(long index, const long &value);
    BOOL PutCollect(long index, const int &value);
    BOOL PutCollect(long index, const short &value);
    BOOL PutCollect(long index, const BYTE &value);

    BOOL PutCollect(LPCSTR strFieldName, const _variant_t &value);
    BOOL PutCollect(LPCSTR strFieldName, const LPCSTR &value);
    BOOL PutCollect(LPCSTR strFieldName, const double &value);
    BOOL PutCollect(LPCSTR strFieldName, const float &value);
    BOOL PutCollect(LPCSTR strFieldName, const long &value);
    BOOL PutCollect(LPCSTR strFieldName, const int &value);
    BOOL PutCollect(LPCSTR strFieldName, const short &value);
    BOOL PutCollect(LPCSTR strFieldName, const BYTE &value);

    BOOL GetCollect(long index, _variant_t &value);
    BOOL GetCollect(long index, CString &value);
    BOOL GetCollect(long index, double &value);
    BOOL GetCollect(long index, float &value);
    BOOL GetCollect(long index, long &value);
    BOOL GetCollect(long index, int& value);
    BOOL GetCollect(long index, short &value);
    BOOL GetCollect(long index, BYTE &value);
    BOOL GetCollect(long index, COleDateTime &value);

    BOOL GetCollect(LPCSTR strFieldName, _variant_t &value);
    BOOL GetCollect(LPCSTR strFieldName, CString &value);
    BOOL GetCollect(LPCSTR strFieldName, double &value);
    BOOL GetCollect(LPCSTR strFieldName, float &value);
    BOOL GetCollect(LPCSTR strFieldName, long &value);
    BOOL GetCollect(LPCSTR strFieldName, int& value);
    BOOL GetCollect(LPCSTR strFieldName, short &value);
    BOOL GetCollect(LPCSTR strFieldName, BYTE &value);
    BOOL GetCollect(LPCSTR strFieldName, COleDateTime &value);

    // BLOB 数据存取 ------------------------------------------
    BOOL AppendChunk(FieldPtr pField, LPVOID lpData, UINT nBytes);
    BOOL AppendChunk(long index, LPVOID lpData, UINT nBytes);
    BOOL AppendChunk(LPCSTR strFieldName, LPVOID lpData, UINT nBytes);
    BOOL AppendChunk(long index, LPCTSTR lpszFileName);
    BOOL AppendChunk(LPCSTR strFieldName, LPCTSTR lpszFileName);

    BOOL GetChunk(FieldPtr pField, LPVOID lpData);
    BOOL GetChunk(long index, LPVOID lpData);
    BOOL GetChunk(LPCSTR strFieldName, LPVOID lpData);
    BOOL GetChunk(long index, CBitmap &bitmap);
    BOOL GetChunk(LPCSTR strFieldName, CBitmap &bitmap);

    BOOL Supports(CursorOptionEnum CursorOptions = adAddNew);
    BOOL SetCacheSize(const long& lCacheSize);
    BOOL Cancel();

    //取得当前记录中指定字段的值并转换成字符串
    BOOL GetValueString(long lFieldIndex, CString &strValue);
    BOOL GetValueString(LPCTSTR strFieldName, CString &strValue);

    /*
    功能：根据表名、字段名及查询条件获取字段值，调用此函数前不用先调用Open()函数

    参数：
    输入：strTableName = 要查询的表名
    strFieldName = 要查询的字段名
    strCondition = 字段查询条件
    输出：
    strValue	   = 查询得到的字段值

    返回值：TRUE	=	成功，strValue的返回值可用
    FALSE	=	失败，strValue的返回值不可用
    */
    BOOL GetFieldValueByCondition(LPCTSTR strTableName, LPCTSTR strFieldName, LPCTSTR strCondition, CString &strValue);

    /*
    功能：根据表名、字段名、查询条件及排序条件获取字段值数组，调用此函数前不用先调用Open()函数

    参数：
    输入：strTableName = 要查询的表名
    strFieldName = 要查询的字段名
    strCondition = 字段查询条件
    strOrderBy   = 字段排序条件
    输出：
    strValuesArray = 查询得到的字段值数组

    返回值：TRUE	=	成功，strValuesArray的返回值可用
    FALSE	=	失败，strValuesArray的返回值不可用
    */
    BOOL GetFieldValuesArrayByCondition(LPCTSTR strTableName, LPCTSTR strFieldName, LPCTSTR strCondition, LPCTSTR strOrderBy, CStringArray& strValuesArray);

    //赋值重载
    const _RecordsetPtr& operator =(const _RecordsetPtr &pRecordSet);
    const CAdoRecordSet& operator =(const CAdoRecordSet &adoRecordSet);

    //把_variant_t型变量格式化成CString型字符串
    CString Format(_variant_t var);

    //成员变量 --------------------------------------------
protected:
    BOOL Release();

    CkAdoSQL     *m_pConnection;
    _RecordsetPtr		m_pRecordset;
    CString				m_strSQL;
};

/*########################################################################
------------------------------------------------
CDBAccess class
------------------------------------------------
########################################################################*/
//数据库访问操作对象类封装
class  CDBAccess
{
public:
    //构造函数
    CDBAccess();
    //析构函数
    virtual ~CDBAccess();
public:
    //查询结果行数
    long m_nResultRow;
    //查询结果列数
    long m_nResultCol;
    //存放查询结果数组大小为nRow*nCol
    CStringArray m_pData;
    //ADO错误信息
    CString m_strErrorMessage;

    //打开数据库连接 输入DSN名称、用户ID和密码
    BOOL OpenDataBase(const CString& strDsnName, const CString& strUserID, const CString& strPassword);
    //打开数据库连接 传入连接字符串
    BOOL OpenDataBase(const CString& strConnection, long lOptions = adConnectUnspecified);
    //执行SQL查询语句，结果放在数组pData中
    //返回值为结果的行数
    long QueryData(const CString& strSQL);
    //执行无返回值的SQL语句
    BOOL ExecuteSQL(const CString& strSQL);
    //关闭数据库连接
    BOOL CloseDataBase();
protected:
    //定义连接指针
    _ConnectionPtr m_pConnection;
    _RecordsetPtr m_pRecordset;
};
