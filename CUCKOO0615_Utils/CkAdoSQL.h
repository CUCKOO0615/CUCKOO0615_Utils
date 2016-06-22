#pragma once

/*########################################################################
------------------------------------------------
CAdoConnection class
------------------------------------------------
########################################################################*/
//ADO���Ӷ������װ
//�ŵ�����ͷ�ļ�stdafx.h
#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","adoEOF"),rename("BOF","adoBOF")

class CkAdoSQL
{
    //����-----------------------------------------------
public:
    CkAdoSQL();
    virtual ~CkAdoSQL();

    //����-----------------------------------------------
public:
    BOOL SetConnectTimeOut(long lTime);
    BOOL SetCommandTimeOut(long lTime);
    BOOL IsOpen();
    _ConnectionPtr GetConnection();

    //����-----------------------------------------------
public:
    ErrorsPtr GetErrors();
    //�������ݿ⣬���������ַ���������������Ϣ
    BOOL Connect(LPCTSTR strConnect, long lOptions = adConnectUnspecified);
    //����SQLServer���ݿ⣬��������������ݿ⡢�û���������
    BOOL ConnectSQLServer(LPCTSTR strDbServer, LPCTSTR strDbName, LPCTSTR strUserName, LPCTSTR strUserPwd, long lOptions = adConnectUnspecified);
    BOOL Disconnect();

    BOOL Cancel();

    long BeginTrans();
    BOOL RollbackTrans();
    BOOL CommitTrans();

    _RecordsetPtr OpenSchema(SchemaEnum QueryType);
    CString GetLastError();

    //ִ���޷���ֵ��SQL���
    BOOL ExecuteSQL(const CString& strSQL);
    //����-----------------------------------------------
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
//ADO��¼���������װ
class  CAdoRecordSet
{
    //���� ------------------------------------------------
public:
    CAdoRecordSet();
    CAdoRecordSet(CkAdoSQL *pConnection);
    virtual ~CAdoRecordSet();
    HRESULT Open(LPCTSTR strSQL, long lOption = adCmdText, CursorTypeEnum CursorType = adOpenStatic, LockTypeEnum LockType = adLockOptimistic);
    void SetAdoConnection(CkAdoSQL *pConnection);
    BOOL Close();

    //���� ------------------------------------------------	
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

    //��¼����ҳ���� --------------------------------------
public:
    BOOL SetPageSize(long nPageSize);
    long GetPageSize() const;
    BOOL SetAbsolutePage(PositionEnum posEnum);
    PositionEnum GetAbsolutePage() const;
    long GetPageCount() const;

    //��¼�����²��� --------------------------------------
public:
    BOOL AddNew();
    BOOL Update();
    BOOL UpdateBatch(AffectEnum AffectRecords = adAffectAll);
    BOOL CancelUpdate();
    BOOL CancelBatch(AffectEnum AffectRecords = adAffectAll);
    BOOL Delete(AffectEnum AffectRecords = adAffectCurrent);

    //��¼���������� --------------------------------------
public:
    BOOL MoveFirst();
    BOOL MovePrevious();
    BOOL MoveNext();
    BOOL MoveLast();
    BOOL Move(long lRecords, long Start = adBookmarkCurrent);

    //�������� --------------------------------------------
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

    // BLOB ���ݴ�ȡ ------------------------------------------
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

    //ȡ�õ�ǰ��¼��ָ���ֶε�ֵ��ת�����ַ���
    BOOL GetValueString(long lFieldIndex, CString &strValue);
    BOOL GetValueString(LPCTSTR strFieldName, CString &strValue);

    /*
    ���ܣ����ݱ������ֶ�������ѯ������ȡ�ֶ�ֵ�����ô˺���ǰ�����ȵ���Open()����

    ������
    ���룺strTableName = Ҫ��ѯ�ı���
    strFieldName = Ҫ��ѯ���ֶ���
    strCondition = �ֶβ�ѯ����
    �����
    strValue	   = ��ѯ�õ����ֶ�ֵ

    ����ֵ��TRUE	=	�ɹ���strValue�ķ���ֵ����
    FALSE	=	ʧ�ܣ�strValue�ķ���ֵ������
    */
    BOOL GetFieldValueByCondition(LPCTSTR strTableName, LPCTSTR strFieldName, LPCTSTR strCondition, CString &strValue);

    /*
    ���ܣ����ݱ������ֶ�������ѯ����������������ȡ�ֶ�ֵ���飬���ô˺���ǰ�����ȵ���Open()����

    ������
    ���룺strTableName = Ҫ��ѯ�ı���
    strFieldName = Ҫ��ѯ���ֶ���
    strCondition = �ֶβ�ѯ����
    strOrderBy   = �ֶ���������
    �����
    strValuesArray = ��ѯ�õ����ֶ�ֵ����

    ����ֵ��TRUE	=	�ɹ���strValuesArray�ķ���ֵ����
    FALSE	=	ʧ�ܣ�strValuesArray�ķ���ֵ������
    */
    BOOL GetFieldValuesArrayByCondition(LPCTSTR strTableName, LPCTSTR strFieldName, LPCTSTR strCondition, LPCTSTR strOrderBy, CStringArray& strValuesArray);

    //��ֵ����
    const _RecordsetPtr& operator =(const _RecordsetPtr &pRecordSet);
    const CAdoRecordSet& operator =(const CAdoRecordSet &adoRecordSet);

    //��_variant_t�ͱ�����ʽ����CString���ַ���
    CString Format(_variant_t var);

    //��Ա���� --------------------------------------------
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
//���ݿ���ʲ����������װ
class  CDBAccess
{
public:
    //���캯��
    CDBAccess();
    //��������
    virtual ~CDBAccess();
public:
    //��ѯ�������
    long m_nResultRow;
    //��ѯ�������
    long m_nResultCol;
    //��Ų�ѯ��������СΪnRow*nCol
    CStringArray m_pData;
    //ADO������Ϣ
    CString m_strErrorMessage;

    //�����ݿ����� ����DSN���ơ��û�ID������
    BOOL OpenDataBase(const CString& strDsnName, const CString& strUserID, const CString& strPassword);
    //�����ݿ����� ���������ַ���
    BOOL OpenDataBase(const CString& strConnection, long lOptions = adConnectUnspecified);
    //ִ��SQL��ѯ��䣬�����������pData��
    //����ֵΪ���������
    long QueryData(const CString& strSQL);
    //ִ���޷���ֵ��SQL���
    BOOL ExecuteSQL(const CString& strSQL);
    //�ر����ݿ�����
    BOOL CloseDataBase();
protected:
    //��������ָ��
    _ConnectionPtr m_pConnection;
    _RecordsetPtr m_pRecordset;
};
