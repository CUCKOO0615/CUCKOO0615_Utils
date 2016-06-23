#include "stdafx.h"
#include "CkAdoSQL.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//#pragma warning(disable:4101)

/*####################################################################
------------------------------------------------
CAdoConnection class
------------------------------------------------
####################################################################*/
CkAdoSQL::CkAdoSQL()
{
    ///���� Connection ����---------------------------    
    ::CoInitialize(NULL);
    m_pConnection.CreateInstance("ADODB.Connection");
    ASSERT(m_pConnection != NULL);
}

CkAdoSQL::~CkAdoSQL()
{
    if (m_pConnection != NULL)
    {
        Release();
    }
    ::CoUninitialize();
}

/*===================================================================
Name:		���ӵ�����Դ.
-----------------------------------------------------
Params:		strConnect: �����ַ���,����������Ϣ.
lOptions:	��ѡ�������÷����������ӽ���֮��(�첽)��������
����֮ǰ(ͬ��)���ء�����������ĳ��������
����					˵��
adConnectUnspecified	(Ĭ��)ͬ����ʽ�����ӡ�
adAsyncConnect			�첽��ʽ�����ӡ�Ado�� ConnectComplete ��
����֪ͨ��ʱ������ӡ�
=====================================================================*/
BOOL CkAdoSQL::Connect(LPCTSTR strConnect, long lOptions)
{
    ASSERT(m_pConnection != NULL);
    m_strConnect = strConnect;
    try
    {
        if (m_pConnection == NULL)
        {
            //Afx_Log().Error("�����ѱ�ɾ��");
            return FALSE;
        }
        Disconnect();
        // �������ݿ�---------------------------------------------
        return SUCCEEDED(m_pConnection->Open(strConnect, "", "", lOptions));
    }
    catch (const _com_error& e)
    {
        //Afx_Log().Error("�������ݿⷢ���쳣: %s", (LPCTSTR)e.Description());
        TRACE(_T(":( �������ݿⷢ���쳣: %s"), (LPCTSTR)e.Description());
        return FALSE;
    }
    catch (...)
    {
        //Afx_Log().Error("�������ݿ�ʱ����δ֪����:");
    }
    return FALSE;
}

/*===================================================================
Name:		���ӵ�SQLServer���ݿ�.
-----------------------------------------------------
Params:		strDbServer	=	���ݿ����������IP��ַ
strDbName	=	���ݿ���
strUserName	=	���ݿ��û���¼��
strUserPwd	=	���ݿ��û���¼����
lOptions:	��ѡ�������÷����������ӽ���֮��(�첽)��������
����֮ǰ(ͬ��)���ء�����������ĳ��������
����					˵��
adConnectUnspecified	(Ĭ��)ͬ����ʽ�����ӡ�
adAsyncConnect			�첽��ʽ�����ӡ�Ado�� ConnectComplete ��
����֪ͨ��ʱ������ӡ�
=====================================================================*/
BOOL CkAdoSQL::ConnectSQLServer(LPCTSTR strDbServer, LPCTSTR strDbName, LPCTSTR strUserName, LPCTSTR strUserPwd, long lOptions/* = adConnectUnspecified*/)
{
    ASSERT(m_pConnection != NULL);
    //���ݿ������ַ���
    CString strCnn;
    strCnn.Format("Provider=SQLOLEDB; Server=%s; Database=%s; uid=%s; pwd=%s",
        strDbServer, strDbName, strUserName, strUserPwd);

    return Connect(strCnn, lOptions);
}

/*===================================================================
Name:	�ر�������Դ������.
-----------------------------------------------------
Remarks: ʹ�� Close �����ɹر� Connection �����Ա��ͷ����й�����ϵͳ��
Դ���رն��󲢷ǽ������ڴ���ɾ�������Ը��������������ò����ڴ˺���
�δ򿪡�Ҫ��������ڴ�����ȫɾ���������Release������
=====================================================================*/
BOOL CkAdoSQL::Disconnect()
{
    try
    {
        if (m_pConnection != NULL && m_pConnection->State == adStateOpen)
        {
            if (SUCCEEDED(m_pConnection->Close()))
            {
                return TRUE;
            }
        }
    }
    catch (const _com_error& e)
    {
        //		Afx_Log().Error(_T(":( �ر����ݿⷢ���쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return FALSE;
}

/*===================================================================
Name:	�ر����Ӳ��ͷŶ���.
-----------------------------------------------------
Remarks: �ر����Ӳ��ͷ�connection�������������ϴ���������ȫ���
��connection����
=====================================================================*/
BOOL CkAdoSQL::Release()
{
    try
    {
        Disconnect();
        m_pConnection.Release();
        return TRUE;
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( Release���������쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return TRUE;
}

/*========================================================================
Remarks:	�뿼�� CAdoRecordSet �� Cancel ����.
==========================================================================*/
BOOL CkAdoSQL::Cancel()
{
    ASSERT(m_pConnection != NULL);
    try
    {
        if (m_pConnection == NULL)
        {
            return FALSE;
        }

        return m_pConnection->Cancel();
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( Cancel�����쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return FALSE;
}

_ConnectionPtr CkAdoSQL::GetConnection()
{
    return m_pConnection;
}

/*===================================================================
Name:	ȡ��������Ĵ�����Ϣ.
-----------------------------------------------------
Remarks: �κ��漰 ADO ����Ĳ��������Բ���һ�������ṩ�ߴ��󡣲�����
��ʱ,���Խ�һ������ Error �������� Connection ����� Errors ����
�С����� ADO ������������ʱ������� Errors ���ϣ����ҽ��µ� Error
�������� Errors �����С�
ÿ�� Error ��������ض����ṩ�ߴ��󣬶����� ADO ����ADO ����
������������ʱ���쳣��������С�
û�в�������� ADO ������ Errors ����û��Ӱ�졣ʹ�� Clear ��ʽ
���ֹ���� Errors ���ϡ�
Errors �����е� Error ���󼯺�������Ӧ�������ʱ���������д���
�о� Errors ������ָ�������ʹ���������̸���ȷ��ȷ�����������ԭ
�򼰴�����Դ����ִ���ʵ���ԭ���衣
ĳЩ���Ժͷ�����������Ϊ Errors �����е� Error ������ʾ�ľ��棬
��������ֹ�����ִ�С��ڵ��� Recordset �����ϵ� Resync��UpdateBatch
�� CancelBatch ������Connection �� Open ������������ Recordset ����
�ϵ� Filter ����ǰ������� Errors �����ϵ� Clear �������������Ϳ���
�Ķ� Errors ���ϵ� Count �����Բ��Է��صľ��档
=====================================================================*/
CString CkAdoSQL::GetLastError()
{
    ASSERT(m_pConnection != NULL);
    CString strErrors = "";
    try
    {
        if (m_pConnection != NULL)
        {
            ErrorsPtr pErrors = m_pConnection->Errors;
            CString strError;
            for (long n = 0; n < pErrors->Count; n++)
            {
                ErrorPtr pError = pErrors->GetItem(n);
                strError.Format(_T("Description: %s\r\n\r\nState: %s, Native: %d, Source: %s\r\n"),
                    (LPCTSTR)pError->Description,
                    (LPCTSTR)pError->SQLState,
                    pError->NativeError,
                    (LPCTSTR)pError->Source);
                strErrors += strError;
            }
        }
        return strErrors;
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( GetLastError�����쳣: %s\n"), (LPCTSTR)e.Description());
        return strErrors;
    }
    return strErrors;
}

ErrorsPtr CkAdoSQL::GetErrors()
{
    ASSERT(m_pConnection != NULL);
    try
    {
        if (m_pConnection != NULL)
        {
            return m_pConnection->Errors;
        }
        return NULL;
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( GetLastError�����쳣: %s\n"), (LPCTSTR)e.Description());
        return NULL;
    }
    return NULL;
}

BOOL CkAdoSQL::IsOpen()
{
    try
    {
        if (m_pConnection == NULL)
        {
            return FALSE;
        }

        long dwState = m_pConnection->State;
        return (m_pConnection != NULL && (dwState & adStateOpen));
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( IsOpen���������쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return FALSE;
}

BOOL CkAdoSQL::SetConnectTimeOut(long lTime)
{
    ASSERT(m_pConnection != NULL);
    try
    {
        if (m_pConnection == NULL)
            return FALSE;
        return SUCCEEDED(m_pConnection->put_ConnectionTimeout(lTime));
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( SetConnectTimeOut���������쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return FALSE;
}

BOOL CkAdoSQL::SetCommandTimeOut(long lTime)
{
    ASSERT(m_pConnection != NULL);
    try
    {
        if (m_pConnection == NULL)
        {
            return FALSE;
        }

        return SUCCEEDED(m_pConnection->put_CommandTimeout(lTime));
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( SetCommandTimeOut���������쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return FALSE;
}

/*========================================================================
Name:		������Դ��ȡ���ݿ���Ϣ.
-----------------------------------------------------
Params:		QueryType  ��Ҫ���е�ģʽ��ѯ����,����Ϊ�������ⳣ��.
adSchemaAsserts				CONSTRAINT_CATALOG
CONSTRAINT_SCHEMA
CONSTRAINT_NAME
adSchemaCatalogs			CATALOG_NAME
adSchemaCharacterSets		CHARACTER_SET_CATALOG
CHARACTER_SET_SCHEMA
CHARACTER_SET_NAME
adSchemaCheckConstraints	CONSTRAINT_CATALOG
CONSTRAINT_SCHEMA
CONSTRAINT_NAME
adSchemaCollations			 COLLATION_CATALOG
COLLATION_SCHEMA
COLLATION_NAME
adSchemaColumnDomainUsage	DOMAIN_CATALOG
DOMAIN_SCHEMA
DOMAIN_NAME
COLUMN_NAME
adSchemaColumnPrivileges	TABLE_CATALOG
TABLE_SCHEMA
TABLE_NAME
COLUMN_NAME
GRANTOR
GRANTEE
adSchemaColumns				TABLE_CATALOG
TABLE_SCHEMA
TABLE_NAME
COLUMN_NAME
adSchemaConstraintColumnUsage TABLE_CATALOG
TABLE_SCHEMA
TABLE_NAME
COLUMN_NAME
adSchemaConstraintTableUsage TABLE_CATALOG
TABLE_SCHEMA
TABLE_NAME
adSchemaForeignKeys			PK_TABLE_CATALOG
PK_TABLE_SCHEMA
PK_TABLE_NAME
FK_TABLE_CATALOG
FK_TABLE_SCHEMA
FK_TABLE_NAME
adSchemaIndexes				TABLE_CATALOG
TABLE_SCHEMA
INDEX_NAME
TYPE
TABLE_NAME
adSchemaKeyColumnUsage		CONSTRAINT_CATALOG
CONSTRAINT_SCHEMA
CONSTRAINT_NAME
TABLE_CATALOG
TABLE_SCHEMA
TABLE_NAME
COLUMN_NAME
adSchemaPrimaryKeys			PK_TABLE_CATALOG
PK_TABLE_SCHEMA
PK_TABLE_NAME
adSchemaProcedureColumns	PROCEDURE_CATALOG
PROCEDURE_SCHEMA
PROCEDURE_NAME
COLUMN_NAME
adSchemaProcedureParameters PROCEDURE_CATALOG
PROCEDURE_SCHEMA
PROCEDURE_NAME
PARAMTER_NAME
adSchemaProcedures			PROCEDURE_CATALOG
PROCEDURE_SCHEMA
PROCEDURE_NAME
PROCEDURE_TYPE
adSchemaProviderSpecific	 �μ�˵��
adSchemaProviderTypes		DATA_TYPE
BEST_MATCH
adSchemaReferentialConstraints CONSTRAINT_CATALOG
CONSTRAINT_SCHEMA
CONSTRAINT_NAME
adSchemaSchemata			CATALOG_NAME
SCHEMA_NAME
SCHEMA_OWNER
adSchemaSQLLanguages		<��>
adSchemaStatistics			TABLE_CATALOG
TABLE_SCHEMA
TABLE_NAME
adSchemaTableConstraints	CONSTRAINT_CATALOG
CONSTRAINT_SCHEMA
CONSTRAINT_NAME
TABLE_CATALOG
TABLE_SCHEMA
TABLE_NAME
CONSTRAINT_TYPE
adSchemaTablePrivileges		TABLE_CATALOG
TABLE_SCHEMA
TABLE_NAME
GRANTOR
GRANTEE
adSchemaTables				TABLE_CATALOG
TABLE_SCHEMA
TABLE_NAME
TABLE_TYPE
adSchemaTranslations		TRANSLATION_CATALOG
TRANSLATION_SCHEMA
TRANSLATION_NAME
adSchemaUsagePrivileges		OBJECT_CATALOG
OBJECT_SCHEMA
OBJECT_NAME
OBJECT_TYPE
GRANTOR
GRANTEE
adSchemaViewColumnUsage		VIEW_CATALOG
VIEW_SCHEMA
VIEW_NAME
adSchemaViewTableUsage		VIEW_CATALOG
VIEW_SCHEMA
VIEW_NAME
adSchemaViews				TABLE_CATALOG
TABLE_SCHEMA
TABLE_NAME
-----------------------------------------------------
returns:	���ذ������ݿ���Ϣ�� Recordset ����. Recordset ����ֻ������̬
�α��.
-----------------------------------------------------
Remarks:	OpenSchema��������������Դ�йص���Ϣ��������ڷ������ϵı�
�Լ����е��е���Ϣ, �������ݽ����ο�, �Ӿ��������Դ���ܻ��в�ͬ��
==========================================================================*/
_RecordsetPtr CkAdoSQL::OpenSchema(SchemaEnum QueryType)
{
    ASSERT(m_pConnection != NULL);
    try
    {
        if (m_pConnection == NULL)
        {
            return NULL;
        }

        return m_pConnection->OpenSchema(QueryType, vtMissing, vtMissing);
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( OpenSchema���������쳣: %s\n"), (LPCTSTR)e.Description());
        return NULL;
    }
    return NULL;
}

/*########################################################################
------------------------------------------------
������
------------------------------------------------
BeginTrans		- ��ʼ������
CommitTrans		- �����κθ��Ĳ�������ǰ������Ҳ��������������
RollbackTrans	- ȡ����ǰ�������������κθ��Ĳ�����������Ҳ��������
������

һ�������� BeginTrans �������ڵ��� CommitTrans �� RollbackTrans ��
������֮ǰ�����ݿ⽫���������ύ�������κθ��ġ�
����֧��Ƕ����������ݿ���˵�����Ѵ򿪵������е��� BeginTrans ����
����ʼ�µ�Ƕ�����񡣷���ֵ��ָʾǶ�ײ�Σ�����ֵΪ 1 ��ʾ�Ѵ򿪶�������
(�����񲻱���һ��������Ƕ��)������ֵΪ 2 ��ʾ�Ѵ򿪵ڶ�������(Ƕ��
�ڶ��������е�����)���������ơ����� CommitTrans �� RollbackTrans ֻӰ
�����´򿪵������ڴ����κθ��߲�����֮ǰ����رջ�ؾ�ǰ����
���� CommitTrans ���������������ϴ򿪵������������ĸ��Ĳ���������
���� RollbackTrans������ԭ�������������ĸ��Ĳ�����������δ������
ʱ���������κ�һ�ַ���������������
ȡ���� Connection ����� Attributes ���ԣ����� CommitTrans ��
RollbackTrans �����������Զ�������������� Attributes ��������Ϊ
adXactCommitRetaining�����ݿ��� CommitTrans ���ú���Զ�����������
��� Attributes ��������Ϊ adXactAbortRetaining�����ݿ��ڵ���
RollbackTrans ֮���Զ�����������
########################################################################*/
long CkAdoSQL::BeginTrans()
{
    ASSERT(m_pConnection != NULL);
    try
    {
        if (m_pConnection == NULL)
        {
            return -1;
        }

        return m_pConnection->BeginTrans();
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( BeginTrans�����쳣: %s\n"), (LPCTSTR)e.Description());
        return -1;
    }
    return -1;
}

BOOL CkAdoSQL::CommitTrans()
{
    ASSERT(m_pConnection != NULL);
    try
    {
        if (m_pConnection == NULL)
        {
            return FALSE;
        }

        return SUCCEEDED(m_pConnection->CommitTrans());
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( CommitTrans�����쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return FALSE;
}

BOOL CkAdoSQL::RollbackTrans()
{
    ASSERT(m_pConnection != NULL);
    try
    {
        if (m_pConnection == NULL)
        {
            return FALSE;
        }

        return SUCCEEDED(m_pConnection->RollbackTrans());
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( RollbackTrans�����쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return FALSE;
}

//ִ���޷���ֵ��SQL���
BOOL CkAdoSQL::ExecuteSQL(const CString& strSQL)
{
    try
    {
        if (m_pConnection == NULL)
        {
            return FALSE;
        }

        m_pConnection->Execute(_bstr_t(strSQL), NULL, adCmdText | adExecuteNoRecords);
        return TRUE;
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( ִ��SQL���������ݿⷢ���쳣: %s\n"), (LPCTSTR)e.Description());
        //        Afx_Log().Error("%s",(LPCTSTR)e.Description());
        return FALSE;
    }
    return FALSE;
}

/*########################################################################
------------------------------------------------
CAdoRecordSet class ���캯��
------------------------------------------------
########################################################################*/

CAdoRecordSet::CAdoRecordSet()
{
    m_pConnection = NULL;
    m_pRecordset.CreateInstance("ADODB.Recordset");
    ASSERT(m_pRecordset != NULL);
}

CAdoRecordSet::CAdoRecordSet(CkAdoSQL *pConnection)
{
    m_pConnection = pConnection;
    ASSERT(m_pConnection != NULL);
    m_pRecordset.CreateInstance("ADODB.Recordset");
    ASSERT(m_pRecordset != NULL);
}

CAdoRecordSet::~CAdoRecordSet()
{
    Release();
}

/*========================================================================
Params:
- strSQL:		���� Command ����ı�����,SQL ���,����,�洢���̵�
�û�־� Recordset �ļ���.
- CursorType:   ��ѡ.CursorTypeEnum ֵ, ȷ����RecordsetʱӦ
��ʹ�õ��α�����.��Ϊ���г���֮һ.
����				˵��
-----------------------------------------------
adOpenForwardOnly	�򿪽���ǰ�����α�.
adOpenKeyset		�򿪼��������α�.
adOpenDynamic		�򿪶�̬�����α�.
adOpenStatic		�򿪾�̬�����α�.
-----------------------------------------------
- LockType:     ��ѡ,ȷ����RecordsetʱӦ��ʹ�õ���������(����)��
LockTypeEnumֵ,��Ϊ���г���֮һ.
����				˵��
-----------------------------------------------
adLockReadOnly		ֻ�� �� ���ܸı�����.
adLockPessimistic	����ʽ���� - ͨ��ͨ���ڱ༭ʱ������������Դ�ļ�¼.
adLockOptimistic	����ʽ���� - ֻ�ڵ��� Update ����ʱ��������¼.
adLockBatchOptimistic ����ʽ������ - ����������ģʽ(����������ģʽ
���).
-----------------------------------------------
- lOption		��ѡ. ������ֵ, ����ָʾ strSQL ���������Ϳ�Ϊ����
����֮һ.
����				˵��
-------------------------------------------------
adCmdText			ָʾstrSQLΪ�����ı�, ����ͨ��SQL���.
adCmdTable			ָʾADO����SQL��ѯ�Ա����strSQL�������ı��з�
��������.
adCmdTableDirect	ָʾ�����ĸ�����strSQL�������ı��з���������.
adCmdStoredProc		ָʾstrSQLΪ�洢����.
adCmdUnknown		ָʾstrSQL�����е���������Ϊδ֪.
adCmdFile			ָʾӦ����strSQL���������ļ��лָ�����(�����)
Recordset.
adAsyncExecute		ָʾӦ�첽ִ��strSQL.
adAsyncFetch		ָʾ����ȡ Initial Fetch Size ������ָ���ĳ�ʼ
������,Ӧ���첽��ȡ����ʣ�����.������������δ��
ȡ,��Ҫ���߳̽�������ֱ�������¿���.
adAsyncFetchNonBlocking ָʾ��Ҫ�߳�����ȡ�ڼ��δ����. ���������
������δ��ȡ,��ǰ���Զ��Ƶ��ļ�ĩβ.
==========================================================================*/
HRESULT CAdoRecordSet::Open(LPCTSTR strSQL, long lOption, CursorTypeEnum CursorType, LockTypeEnum LockType)
{
    ASSERT(m_pConnection != NULL);
    ASSERT(m_pRecordset != NULL);

    try
    {
        if (m_pConnection == NULL || m_pConnection->GetConnection() == NULL)
        {
            return -1;
        }

        Close();

        m_pRecordset->Open(_bstr_t(strSQL), _variant_t((IDispatch*)m_pConnection->GetConnection(), true),
            CursorType, LockType, lOption);
        if (m_pRecordset == NULL)
        {
            return -1;
        }
        return (m_pRecordset->adoEOF) ? 0 : 1;
    }
    catch (const _com_error& e)
    {
        //		Afx_Log().Info(_T(":( �򿪼�¼�������쳣: %s  sql:%s\n"), (LPCTSTR)e.Description(), strSQL);
        return -1;
    }
}

/*===================================================================
Name:		�رմ򿪵Ķ����κ���ض���.
-----------------------------------------------------
Remarks:	ʹ��Close�����ɹر�Recordset�����Ա��ͷ����й�����ϵͳ��Դ.
�رն��󲢷ǽ������ڴ���ɾ��, ���Ը��������������ò����ڴ˺��ٴδ�
��.Ҫ��������ڴ�����ȫɾ��, �ɽ������������Ϊ Nothing.
ʹ��Close�����ر�Recordset�����ͬʱ, ���ͷŹ��������ݺͿ�����
��ͨ�����ض�Recordset��������ݽ��еĶ�������.���ɵ���Open������
�´򿪾�����ͬ���Ի����޸����Ե�Recordset.��Recordset����رպ�,��
���κ���Ҫ��α�ķ�������������.
���������������ģʽ�½��б༭,����Close��������������, Ӧ����
����Update��CancelUpdat ����.������������ڼ�ر�Recordset����, ��
���ϴ�UpdateBatch���������������޸Ľ�ȫ����ʧ.
���ʹ��Clone���������Ѵ򿪵�Recordset����ĸ���,�ر�ԭʼRecordset
���丱������Ӱ���κ���������.
Ҫ��������ڴ�����ȫɾ���������Release������
=====================================================================*/
BOOL CAdoRecordSet::Close()
{
    try
    {
        if (m_pRecordset != NULL && m_pRecordset->State != adStateClosed)
        {
            return SUCCEEDED(m_pRecordset->Close());
        }
        return TRUE;
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( �رռ�¼�������쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }

    return TRUE;
}

/*===================================================================
Name:	�ر����Ӳ��ͷŶ���.
-----------------------------------------------------
Remarks: �ر����Ӳ��ͷ�CAdoRecordSet�������������ϴ���������ȫ���
��CAdoRecordSet����
=====================================================================*/
BOOL CAdoRecordSet::Release()
{
    try
    {
        if (m_pRecordset != NULL)
        {
            Close();
            m_pRecordset.Release();
        }
        return TRUE;
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( Release���������쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return TRUE;
}

/*########################################################################
------------------------------------------------
��¼����ҳ����
------------------------------------------------
########################################################################*/
BOOL CAdoRecordSet::SetPageSize(long nPageSize)
{
    ASSERT(m_pRecordset != NULL);
    try
    {
        if (m_pRecordset != NULL)
        {
            return SUCCEEDED(m_pRecordset->put_PageSize(nPageSize));
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( SetPageSize���������쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }

    return FALSE;
}

long CAdoRecordSet::GetPageSize() const
{
    ASSERT(m_pRecordset != NULL);
    long lSize = 0;
    try
    {
        if (m_pRecordset != NULL && SUCCEEDED(m_pRecordset->get_PageSize(&lSize)))
        {
            return lSize;
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( GetPageSize�����쳣: %s\n"), (LPCTSTR)e.Description());
        return -1;
    }

    return -1;
}

BOOL CAdoRecordSet::SetAbsolutePage(PositionEnum posEnum)
{
    ASSERT(m_pRecordset != NULL);
    try
    {
        if (m_pRecordset != NULL)
        {
            return SUCCEEDED(m_pRecordset->put_AbsolutePage(posEnum));
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( SetAbsolutePage���������쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }

    return FALSE;
}

PositionEnum CAdoRecordSet::GetAbsolutePage() const
{
    ASSERT(m_pRecordset != NULL);
    PositionEnum posEnum = (PositionEnum)0;
    try
    {
        if (m_pRecordset != NULL && SUCCEEDED(m_pRecordset->get_AbsolutePage(&posEnum)))
        {
            return posEnum;
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( GetAbsolutePage�����쳣: %s\n"), (LPCTSTR)e.Description());
        return posEnum;
    }

    return posEnum;
}

long CAdoRecordSet::GetPageCount() const
{
    ASSERT(m_pRecordset != NULL);
    long lCount = 0;
    try
    {
        if (m_pRecordset != NULL && SUCCEEDED(m_pRecordset->get_PageCount(&lCount)))
        {
            return lCount;
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( GetPageCount�����쳣: %s\n"), (LPCTSTR)e.Description());
        return -1;
    }

    return -1;
}

/*########################################################################
------------------------------------------------
��¼�����²���
------------------------------------------------
########################################################################*/

/*===================================================================
Remarks:	����µļ�¼.��AddNew()����, Ȼ����PutCollect()����ÿ
�е�ֵ, �����Update()�������ݿ�.
=====================================================================*/
BOOL CAdoRecordSet::AddNew()
{
    ASSERT(m_pRecordset != NULL);
    try
    {
        if (m_pRecordset != NULL)
        {
            return SUCCEEDED(m_pRecordset->AddNew());
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( AddNew�����쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return	FALSE;
}

/*===================================================================
Remarks:	�ڵ���AddNew()�ȷ����󣬵��ô˷�����ɸ��»��޸�.
=====================================================================*/
BOOL CAdoRecordSet::Update()
{
    ASSERT(m_pRecordset != NULL);
    try
    {
        if (m_pRecordset != NULL)
        {
            return SUCCEEDED(m_pRecordset->Update());
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( Update�����쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return	FALSE;
}

/*===================================================================
Remarks:	�ڵ���AddNew()�ȷ����󣬵��ô˷�����ɸ��»��޸�.
=====================================================================*/
BOOL CAdoRecordSet::UpdateBatch(AffectEnum AffectRecords/* = adAffectAll*/)
{
    ASSERT(m_pRecordset != NULL);
    try
    {
        if (m_pRecordset != NULL)
        {
            return SUCCEEDED(m_pRecordset->UpdateBatch(AffectRecords));
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( UpdateBatch�����쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return	FALSE;
}

/*========================================================================
Name:		ȡ���ڵ��� Update ����ǰ�Ե�ǰ��¼���¼�¼�������κθ���.
-----------------------------------------------------
Remarks:	ʹ�� CancelUpdate ������ȡ���Ե�ǰ��¼�������κθ��Ļ�
��������ӵļ�¼.�ڵ��� Update �������޷������Ե�ǰ��¼���¼�¼
�����ĸ���,���Ǹ����ǿ����� RollbackTrans �����ؾ�������һ����,
�����ǿ����� CancelBatch ����ȡ���������µ�һ����.
����ڵ��� CancelUpdate ����ʱ����¼�¼,����� AddNew ֮ǰ�ĵ�ǰ
��¼���ٴγ�Ϊ��ǰ��¼.
�����δ���ĵ�ǰ��¼������¼�¼,���� CancelUpdate ��������������.
==========================================================================*/
BOOL CAdoRecordSet::CancelUpdate()
{
    ASSERT(m_pRecordset != NULL);
    try
    {
        if (m_pRecordset != NULL)
        {
            return SUCCEEDED(m_pRecordset->CancelUpdate());
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( CancelUpdate�����쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return FALSE;
}

/*========================================================================
Name:		ȡ�������������.
-----------------------------------------------------
Params:		AffectRecords   ��ѡ�� AffectEnum ֵ,����CancelBatch
������Ӱ���¼����Ŀ,��Ϊ���г���֮һ��
����			˵��
-------------------------------------------------
AdAffectCurrent ��ȡ����ǰ��¼�Ĺ������.
AdAffectGroup	�����㵱ǰ Filter �������õļ�¼ȡ���������.
ʹ�ø�ѡ��ʱ,���뽫 Filter ��������Ϊ�Ϸ���Ԥ
���峣��֮һ.
AdAffectAll		Ĭ��ֵ.ȡ�� Recordset ���������м�¼�Ĺ����
��,�����ɵ�ǰ Filter �������������ص��κμ�¼.
==========================================================================*/
BOOL CAdoRecordSet::CancelBatch(AffectEnum AffectRecords)
{
    ASSERT(m_pRecordset != NULL);
    try
    {
        if (m_pRecordset != NULL)
        {
            return SUCCEEDED(m_pRecordset->CancelBatch(AffectRecords));
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( CancelBatch�����쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return FALSE;
}

/*========================================================================
Params:		AffectRecords:  AffectEnumֵ, ȷ��Delete������Ӱ��ļ�¼��
Ŀ,��ֵ���������г���֮һ.
����				˵��
-------------------------------------------------
AdAffectCurrent		Ĭ��.��ɾ����ǰ��¼.
AdAffectGroup		ɾ�����㵱ǰFilter�������õļ�¼.Ҫʹ�ø�ѡ��,
���뽫 Filter ��������Ϊ��Ч��Ԥ���峣��֮һ.
adAffectAll			ɾ�����м�¼.
adAffectAllChapters ɾ�������Ӽ���¼.
==========================================================================*/
BOOL CAdoRecordSet::Delete(AffectEnum AffectRecords)
{
    ASSERT(m_pRecordset != NULL);
    try
    {
        if (m_pRecordset != NULL)
        {
            return SUCCEEDED(m_pRecordset->Delete(AffectRecords));
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( Delete�����쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return	FALSE;
}

/*########################################################################
------------------------------------------------
��¼����������
------------------------------------------------
Remarks:
ʹ��MoveFirst��������ǰ��¼λ���ƶ���Recordse�еĵ�һ����¼.
ʹ��MoveLast��������ǰ��¼λ���ƶ���Recordset�е����һ����¼.
Recordset�������֧����ǩ��������ƶ�;������ø÷�������������.

ʹ��MoveNext��������ǰ��¼��ǰ�ƶ�һ����¼(��Recordset�ĵײ�).���
���һ����¼�ǵ�ǰ��¼���ҵ���MoveNext����,��ADO����ǰ��¼���õ�
Recordset(EOFΪTrue)��β��¼֮��.��EOF�����Ѿ�ΪTrueʱ��ͼ��ǰ�ƶ�
����������.
ʹ��MovePrevious��������ǰ��¼λ������ƶ�һ����¼(���¼���Ķ���).
Recordset �������֧����ǩ������α��ƶ�;���򷽷����ý���������.
����׼�¼�ǵ�ǰ��¼���ҵ���MovePrevious����,��ADO����ǰ��¼������
Recordset(BOFΪTrue)���׼�¼֮ǰ.��BOF����ΪTrueʱ����ƶ���������
��. ���Recordset����֧����ǩ������α��ƶ�,��MovePrevious������
��������.
�����¼���ǽ���ǰ��,�����û�ϣ��֧����ǰ��������,�����ʹ��
CacheSize ���Դ�����¼����,ͨ�� Move����֧������α��ƶ�.����
�����¼�Ǽ��ص��ڴ��е�,����Ӧ���ⲻ��Ҫ�ػ���̫���¼.���Ե���
����ǰ Recordset ����� MoveFirst ����;��������ʹ�ṩ������ִ����
�� Recordset ���������.
########################################################################*/

BOOL CAdoRecordSet::MoveFirst()
{
    ASSERT(m_pRecordset != NULL);
    try
    {
        if (m_pRecordset != NULL)
        {
            return SUCCEEDED(m_pRecordset->MoveFirst());
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( MoveFirst�����쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return	FALSE;
}

BOOL CAdoRecordSet::MoveLast()
{
    ASSERT(m_pRecordset != NULL);
    try
    {
        if (m_pRecordset != NULL)
        {
            return SUCCEEDED(m_pRecordset->MoveLast());
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( MoveLast�����쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return	FALSE;
}

BOOL CAdoRecordSet::MovePrevious()
{
    ASSERT(m_pRecordset != NULL);
    try
    {
        if (m_pRecordset != NULL)
        {
            return SUCCEEDED(m_pRecordset->MovePrevious());
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( MovePrevious�����쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return FALSE;
}

BOOL CAdoRecordSet::MoveNext()
{
    ASSERT(m_pRecordset != NULL);
    try
    {
        if (m_pRecordset != NULL)
        {
            return SUCCEEDED(m_pRecordset->MoveNext());
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( MoveNext�����쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return FALSE;
}

/*========================================================================
Name:		�ƶ� Recordset �����е�ǰ��¼��λ�á�
----------------------------------------------------------
Params:
- lRecords    �����ų����ͱ��ʽ��ָ����ǰ��¼λ���ƶ��ļ�¼����
- Start    ��ѡ���ַ���������ͣ����ڼ�����ǩ��Ҳ��Ϊ���� BookmarkEnum ֵ֮һ��
����				˵��
adBookmarkCurrent	Ĭ�ϡ��ӵ�ǰ��¼��ʼ��
adBookmarkFirst		���׼�¼��ʼ��
adBookmarkLast		��β��¼��ʼ��
==========================================================================*/
BOOL CAdoRecordSet::Move(long lRecords, long Start)
{
    ASSERT(m_pRecordset != NULL);
    try
    {
        if (m_pRecordset != NULL)
        {
            return SUCCEEDED(m_pRecordset->Move(lRecords, _variant_t(Start)));
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( Move�����쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return	FALSE;
}

/*########################################################################
------------------------------------------------
��¼������
------------------------------------------------
########################################################################*/

/*========================================================================
Name:		�����п�Ӧ�ö���,˵�������״̬�Ǵ򿪻��ǹر�.��ִ��
�첽������ Recordset ����,˵����ǰ�Ķ���״̬�����ӡ�ִ�л��ǻ�ȡ.
-----------------------------------------------------
returns:	�������г���֮һ�ĳ�����ֵ.
����				˵��
----------------------------------
adStateClosed		ָʾ�����ǹرյ�.
adStateOpen			ָʾ�����Ǵ򿪵�.
adStateConnecting	ָʾ Recordset ������������.
adStateExecuting	ָʾ Recordset ��������ִ������.
adStateFetching		ָʾ Recordset ����������ڱ���ȡ.
-----------------------------------------------------
Remarks:		������ʱʹ�� State ����ȷ��ָ������ĵ�ǰ״̬.��
������ֻ����.Recordset ����� State ���Կ��������ֵ.����,���
����ִ�����,�����Խ��� adStateOpen �� adStateExecuting �����ֵ.
==========================================================================*/
long CAdoRecordSet::GetState()
{
    ASSERT(m_pRecordset != NULL);
    long lState = 0;
    try
    {
        if (SUCCEEDED(m_pRecordset->get_State(&lState)))
        {
            return lState;
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( GetState�����쳣: %s\n"), (LPCTSTR)e.Description());
        return NULL;
    }
    return NULL;
}

/*========================================================================
Name:		ָʾ�й������»��������������ĵ�ǰ��¼��״̬.
-----------------------------------------------------
returns:	��������һ������ RecordStatusEnum ֵ֮��.
����						˵��
-------------------------------------------------
adRecOK						�ɹ��ظ��¼�¼.
adRecNew					��¼���½���.
adRecModified				��¼���޸�.
adRecDeleted				��¼��ɾ��.
adRecUnmodified				��¼û���޸�.
adRecInvalid				������ǩ��Ч,��¼û�б���.
adRecMultipleChanges		����Ӱ������¼,��˼�¼δ������.
adRecPendingChanges			���ڼ�¼���ù���Ĳ���,���δ������.
adRecCanceled				���ڲ�����ȡ��,δ�����¼.
adRecCantRelease			�������м�¼����,û�б����¼�¼.
adRecConcurrencyViolation	���ڿ���ʽ������ʹ����,��¼δ������.
adRecIntegrityViolation		�����û�Υ��������Լ��,��¼δ������.
adRecMaxChangesExceeded		���ڴ��ڹ���������,��¼δ������.
adRecObjectOpen				������򿪵Ĵ�������ͻ,��¼δ������.
adRecOutOfMemory			���ڼ�����ڴ治��,��¼δ������.
adRecPermissionDenied		�����û�û���㹻��Ȩ��,��¼δ������.
adRecSchemaViolation		���ڼ�¼Υ���������ݿ�Ľṹ,���δ������.
adRecDBDeleted				��¼�Ѿ�������Դ��ɾ��.
-----------------------------------------------------
Remarks:	ʹ�� Status ���Բ鿴���������б��޸ĵļ�¼����Щ���ı�
����.Ҳ��ʹ�� Status ���Բ鿴��������ʱʧ�ܼ�¼��״̬.����,����
Recordset ����� Resync��UpdateBatch �� CancelBatch ����,��������
Recordset ����� Filter ����Ϊ��ǩ����.ʹ�ø�����,�ɼ��ָ����¼
Ϊ��ʧ�ܲ���������.
==========================================================================*/
long CAdoRecordSet::GetStatus()
{
    ASSERT(m_pRecordset != NULL);
    long lStatus = 0;
    try
    {
        if (SUCCEEDED(m_pRecordset->get_Status(&lStatus)))
        {
            return lStatus;
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( GetStatus�����쳣: %s\n"), (LPCTSTR)e.Description());
        return NULL;
    }
    return NULL;
}

/*========================================================================
Name:		��ȡ��ǰ��¼���м�¼��Ŀ
==========================================================================*/
long CAdoRecordSet::GetRecordCount()
{
    ASSERT(m_pRecordset != NULL);
    long lcount;
    try
    {
        m_pRecordset->get_RecordCount(&lcount);
        return lcount;
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( GetRecordCount�����쳣: %s\n"), (LPCTSTR)e.Description());
        return NULL;
    }
}

/*========================================================================
Name:		��ȡ��ǰ��¼�����ֶ���Ŀ
==========================================================================*/
long CAdoRecordSet::GetFieldsCount()
{
    ASSERT(m_pRecordset != NULL);
    try
    {
        return GetFields()->Count;
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( GetFieldsCount�����쳣: %s\n"), (LPCTSTR)e.Description());
        return -1;
    }
}

/*========================================================================
Name:		ָ���Ƿ����ڼ�¼��ͷ
==========================================================================*/
BOOL CAdoRecordSet::IsBOF()
{
    ASSERT(m_pRecordset != NULL);
    try
    {
        return m_pRecordset->adoBOF;
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( IsBOF�����쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return FALSE;
}

/*========================================================================
Name:		ָ���Ƿ����ڼ�¼��β
==========================================================================*/
BOOL CAdoRecordSet::IsEOF()
{
    ASSERT(m_pRecordset != NULL);
    try
    {
        return m_pRecordset->adoEOF;
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( IsEOF�����쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
}

void CAdoRecordSet::SetAdoConnection(CkAdoSQL *pConnection)
{
    m_pConnection = pConnection;
}

/*===================================================================
Remarks:	Recordset ������� Field ������ɵ� Fields ����.ÿ��
Field �����Ӧ Recordset ���е�һ��.
=====================================================================*/
Fields* CAdoRecordSet::GetFields()
{
    ASSERT(m_pRecordset != NULL);
    FieldsPtr pFields;
    try
    {
        if (SUCCEEDED(m_pRecordset->get_Fields(&pFields)))
        {
            return pFields;
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( GetFields�����쳣: %s\n"), (LPCTSTR)e.Description());
        return NULL;
    }
    return NULL;
}

/*========================================================================
Remarks:	ȡ��ָ�����ֶε��ֶ���.
==========================================================================*/
CString CAdoRecordSet::GetFieldName(long lIndex)
{
    ASSERT(m_pRecordset != NULL);
    CString strFieldName;
    try
    {
        FieldPtr field = GetFields()->GetItem(_variant_t(lIndex));
        strFieldName = LPCTSTR(_bstr_t(field->GetName()));
        return strFieldName;
        field->GetType();
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( GetFieldName�����쳣: %s\n"), (LPCTSTR)e.Description());
        return strFieldName;
    }
}

/*========================================================================
returns:	���� Field ����Attributes ����Ϊֻ������ֵ����Ϊ��������һ������ FieldAttributeEnum ֵ�ĺ͡�
����				˵��
adFldMayDefer			ָʾ�ֶα��ӳ٣�������ӵ��������¼������Դ����
�ֶ�ֵ��������ʽ������Щ�ֶ�ʱ�Ž��м�����
adFldUpdatable		ָʾ����д����ֶΡ�
adFldUnknownUpdatable ָʾ�ṩ���޷�ȷ���Ƿ����д����ֶΡ�
adFldFixed			ָʾ���ֶΰ����������ݡ�
adFldIsNullable		ָʾ���ֶν��� Null ֵ��
adFldMayBeNull		ָʾ���ԴӸ��ֶζ�ȡ Null ֵ��
adFldLong				ָʾ���ֶ�Ϊ���������ֶΡ���ָʾ����ʹ�� AppendChunk �� GetChunk ������
adFldRowID			ָʾ�ֶΰ����־õ��б�ʶ�����ñ�ʶ���޷���д��
���ҳ��˶��н��б�ʶ(���¼�š�Ψһ��ʶ����)�ⲻ
�����������ֵ��
adFldRowVersion		ָʾ���ֶΰ����������ٸ��µ�ĳ��ʱ������ڱ�ǡ�
adFldCacheDeferred	ָʾ�ṩ�߻������ֶ�ֵ������������Ի���Ķ�ȡ��
----------------------------------------------------------
Remarks:	ȡ�� Field ����һ���������ԡ�
==========================================================================*/
long CAdoRecordSet::GetFieldAttributes(long lIndex)
{
    ASSERT(m_pRecordset != NULL);
    long lAttributes;
    try
    {
        FieldPtr field = GetFields()->GetItem(_variant_t(lIndex));
        field->get_Attributes(&lAttributes);
        return lAttributes;
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( GetFieldAttributes�����쳣: %s\n"), (LPCTSTR)e.Description());
        return -1;
    }
}

long CAdoRecordSet::GetFieldAttributes(LPCTSTR lpszFieldName)
{
    ASSERT(m_pRecordset != NULL);
    try
    {
        return m_pRecordset->Fields->GetItem(_variant_t(lpszFieldName))->GetAttributes();
    }
    catch (_com_error e)
    {
        TRACE(_T("Warning: GetFieldAttributes ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
        return -1;
    }
}

/*========================================================================
Name:		ָʾ Field ����������ĳ��ȡ�
----------------------------------------------------------
returns:	����ĳ���ֶζ���ĳ���(���ֽ���)�ĳ�����ֵ��
----------------------------------------------------------
Remarks:	ʹ�� DefinedSize ���Կ�ȷ�� Field ���������������
==========================================================================*/

long CAdoRecordSet::GetFieldDefineSize(long lIndex)
{
    ASSERT(m_pRecordset != NULL);
    try
    {
        FieldPtr field = GetFields()->GetItem(_variant_t(lIndex));
        return field->GetDefinedSize();
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( GetDefineSize�����쳣: %s\n"), (LPCTSTR)e.Description());
        return -1;
    }
}

long CAdoRecordSet::GetFieldDefineSize(LPCTSTR lpszFieldName)
{
    ASSERT(m_pRecordset != NULL);
    try
    {
        return m_pRecordset->Fields->GetItem(_variant_t(lpszFieldName))->DefinedSize;
    }
    catch (_com_error e)
    {
        TRACE(_T("Warning: GetDefineSize ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
        return -1;
    }
}

/*========================================================================
Name:	ȡ���ֶε�ֵ��ʵ�ʳ��ȡ�
----------------------------------------------------------
returns:	���س�����ֵ��ĳЩ�ṩ���������ø������Ա�Ϊ BLOB ����Ԥ��
�ռ䣬�ڴ������Ĭ��ֵΪ 0��
----------------------------------------------------------
Remarks:	ʹ�� ActualSize ���Կɷ��� Field ����ֵ��ʵ�ʳ���.��������
�ֶ�,ActualSize ����Ϊֻ������� ADO �޷�ȷ�� Field ����ֵ��ʵ
�ʳ��ȣ�ActualSize ���Խ����� adUnknown��
�����·�����ʾ��ActualSize ��  DefinedSize ����������ͬ��
adVarChar ������������󳤶�Ϊ 50 ���ַ��� Field ���󽫷���Ϊ
50 �� DefinedSize ����ֵ�����Ƿ��ص� ActualSize ����ֵ�ǵ�ǰ��
¼���ֶ��д洢�����ݵĳ��ȡ�
==========================================================================*/
long CAdoRecordSet::GetFieldActualSize(long lIndex)
{
    ASSERT(m_pRecordset != NULL);
    try
    {
        FieldPtr field = GetFields()->GetItem(_variant_t(lIndex));
        return field->GetActualSize();
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( GetFieldActualSize�����쳣: %s\n"), (LPCTSTR)e.Description());
        return -1;
    }
}

long CAdoRecordSet::GetFieldActualSize(LPCTSTR lpszFieldName)
{
    ASSERT(m_pRecordset != NULL);
    try
    {
        return m_pRecordset->Fields->GetItem(_variant_t(lpszFieldName))->ActualSize;
    }
    catch (_com_error e)
    {
        TRACE(_T("Warning: GetFieldActualSize ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
        return -1;
    }
}

/*========================================================================
returns:	��������ֵ֮һ����Ӧ�� OLE DB ���ͱ�ʶ�����±��˵�����������и�����
����				˵��
---------------------------------------------------------
adArray			����������һ������߼� OR ��ָʾ���������������͵�
��ȫ���� (DBTYPE_ARRAY)��
adBigInt			8 �ֽڴ����ŵ����� (DBTYPE_I8)��
adBinary			������ֵ (DBTYPE_BYTES)��
adBoolean			������ֵ (DBTYPE_BOOL)��
adByRef			����������һ������߼� OR ��ָʾ������������������
�ݵ�ָ�� (DBTYPE_BYREF)��
adBSTR			�Կս�β���ַ��� (Unicode) (DBTYPE_BSTR)��
adChar			�ַ���ֵ (DBTYPE_STR)��
adCurrency		����ֵ (DBTYPE_CY)���������ֵ�С����λ�ù̶���С��
���Ҳ�����λ���֡���ֵ����Ϊ 8 �ֽڷ�ΧΪ10,000 �Ĵ���
������ֵ��
adDate			����ֵ (DBTYPE_DATE)�����ڰ�˫��������ֵ�����棬��
��ȫ����ʾ�� 1899 �� 12 �� 30 ��ʼ����������С��������
һ�쵱�е�Ƭ��ʱ�䡣
adDBDate			����ֵ (yyyymmdd) (DBTYPE_DBDATE)��
adDBTime			ʱ��ֵ (hhmmss) (DBTYPE_DBTIME)��
adDBTimeStamp		ʱ��� (yyyymmddhhmmss �� 10 �ڷ�֮һ��С��)(DBTYPE_DBTIMESTAMP).
adDecimal			���й̶����Ⱥͷ�Χ�ľ�ȷ����ֵ (DBTYPE_DECIMAL)��
adDouble			˫���ȸ���ֵ (DBTYPE_R8)��
adEmpty			δָ��ֵ (DBTYPE_EMPTY)��
adError			32 - λ������� (DBTYPE_ERROR)��
adGUID			ȫ��Ψһ�ı�ʶ�� (GUID) (DBTYPE_GUID)��
adIDispatch		OLE ������ Idispatch �ӿڵ�ָ�� (DBTYPE_IDISPATCH)��
adInteger			4 �ֽڵĴ��������� (DBTYPE_I4)��
adIUnknown		OLE ������ IUnknown �ӿڵ�ָ�� (DBTYPE_IUNKNOWN)��
adLongVarBinary	��������ֵ��
adLongVarChar		���ַ���ֵ��
adLongVarWChar	�Կս�β�ĳ��ַ���ֵ��
adNumeric			���й̶����Ⱥͷ�Χ�ľ�ȷ����ֵ (DBTYPE_NUMERIC)��
adSingle			�����ȸ���ֵ (DBTYPE_R4)��
adSmallInt		2 �ֽڴ��������� (DBTYPE_I2)��
adTinyInt			1 �ֽڴ��������� (DBTYPE_I1)��
adUnsignedBigInt	8 �ֽڲ����������� (DBTYPE_UI8)��
adUnsignedInt		4 �ֽڲ����������� (DBTYPE_UI4)��
adUnsignedSmallInt 2 �ֽڲ����������� (DBTYPE_UI2)��
adUnsignedTinyInt 1 �ֽڲ����������� (DBTYPE_UI1)��
adUserDefined		�û�����ı��� (DBTYPE_UDT)��
adVarBinary		������ֵ��
adVarChar			�ַ���ֵ��
adVariant			�Զ������� (DBTYPE_VARIANT)��
adVector			����������һ������߼� OR �У�ָʾ������ DBVECTOR
�ṹ(�� OLE DB ����)���ýṹ����Ԫ�صļ�������������
(DBTYPE_VECTOR) ���ݵ�ָ�롣
adVarWChar		�Կս�β�� Unicode �ַ�����
adWChar			�Կս�β�� Unicode �ַ��� (DBTYPE_WSTR)��
----------------------------------------------------------
Remarks:	����ָ���е��������͡�
==========================================================================*/
DWORD CAdoRecordSet::GetFieldType(long lIndex)
{
    ASSERT(m_pRecordset != NULL);
    try
    {
        FieldPtr field = GetFields()->GetItem(_variant_t(lIndex));
        return field->GetType();
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( GetField�����쳣: %s\n"), (LPCTSTR)e.Description());
        return NULL;
    }
}

DWORD CAdoRecordSet::GetFieldType(LPCTSTR strfield)
{
    ASSERT(m_pRecordset != NULL);
    try
    {
        FieldPtr field = GetFields()->GetItem(strfield);
        return field->GetType();
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( GetField�����쳣: %s\n"), (LPCTSTR)e.Description());
        return NULL;
    }
}
/*========================================================================
Name:	ȡ��ָ���е��ֶζ����ָ��.
==========================================================================*/
FieldPtr CAdoRecordSet::GetField(long lIndex)
{
    try
    {
        return GetFields()->GetItem(_variant_t(lIndex));
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( GetField�����쳣: %s\n"), (LPCTSTR)e.Description());
        return NULL;
    }
}

FieldPtr CAdoRecordSet::GetField(LPCTSTR lpszFieldName)
{
    try
    {
        return GetFields()->GetItem(_variant_t(lpszFieldName));
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( GetField�����쳣: %s\n"), (LPCTSTR)e.Description());
        return NULL;
    }
}

const _RecordsetPtr& CAdoRecordSet::GetRecordset()
{
    return m_pRecordset;
}

CString CAdoRecordSet::GetLastError()
{
    ASSERT(m_pConnection != NULL);

    try
    {
        if (m_pConnection != NULL && m_pConnection->GetConnection() != NULL)
        {

            return m_pConnection->GetLastError();
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( GetLastError�����쳣: %s\n"), (LPCTSTR)e.Description());
    }

    return "";
}

/*========================================================================
Name:		ȡ��ִ�й�����첽 Execute �� Open �����ĵ���.
-----------------------------------------------------
Remarks:	ʹ�� Cancel ������ִֹ���첽 Execute �� Open ��������(��ͨ
�� adAsyncConnect��adAsyncExecute �� adAsyncFetch �������õķ���).
�������ͼ��ֹ�ķ�����û��ʹ�� adAsyncExecute,�� Cancel ����������
ʱ����.
�±���ʾ���ض����Ͷ�����ʹ�� Cancel ����ʱ����ֹ������.
�������Ϊ		����ֹ�Ը÷�������һ���첽����
-------------------------------------------------
Command			Execute
Connection		Execute �� Open
Recordset		Open
==========================================================================*/
BOOL CAdoRecordSet::Cancel()
{
    ASSERT(m_pRecordset != NULL);
    try
    {
        return m_pRecordset->Cancel();
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( Cancel�����쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return FALSE;
}

//��ֵ����
const _RecordsetPtr& CAdoRecordSet::operator =(const _RecordsetPtr &pRecordSet)
{
    try
    {
        if (m_pRecordset != pRecordSet)
        {
            Release();
            m_pRecordset = pRecordSet->Clone(adLockUnspecified);
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( ��¼����ֵ�����쳣: %s\n"), (LPCTSTR)e.Description());
    }

    return m_pRecordset;
}

const CAdoRecordSet& CAdoRecordSet::operator =(const CAdoRecordSet &adoRecordSet)
{
    try
    {
        if (this != &adoRecordSet)
        {
            Release();
            m_pRecordset = adoRecordSet.m_pRecordset->Clone(adLockUnspecified);
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( ��¼����ֵ�����쳣: %s\n"), (LPCTSTR)e.Description());
    }

    return (*this);
}

/*########################################################################
------------------------------------------------
��������
------------------------------------------------
########################################################################*/

/*========================================================================
Remarks:	ȡ�õ�ǰ��¼��ָ���ֶε�ֵ��ת�����ַ���
==========================================================================*/
BOOL CAdoRecordSet::GetValueString(long lFieldIndex, CString &strValue)
{
    ASSERT(m_pRecordset != NULL);
    if (lFieldIndex < 0 || lFieldIndex >= GetFieldsCount())
    {
        return FALSE;
    }
    try
    {
        if (m_pRecordset == NULL || m_pRecordset->State == adStateClosed)
        {
#ifdef _DEBUG
            MessageBox(NULL, "���ݿ�����Ѿ��Ͽ�,\r\n���������ӡ�Ȼ������.", "��ʾ", MB_ICONINFORMATION);
#endif
            return FALSE;
        }
        if (m_pRecordset->adoEOF)
        {
            return FALSE;
        }
        _variant_t value = m_pRecordset->GetCollect(_variant_t(lFieldIndex));
        strValue = Format(value);
        return TRUE;
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( �ֶη���ʧ��: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }

    return FALSE;
}

BOOL CAdoRecordSet::GetValueString(LPCTSTR strFieldName, CString &strValue)
{
    ASSERT(m_pRecordset != NULL);
    try
    {
        if (m_pRecordset == NULL || m_pRecordset->State == adStateClosed)
        {
#ifdef _DEBUG
            MessageBox(NULL, "���ݿ�����Ѿ��Ͽ�,\r\n���������ӡ�Ȼ������.", "��ʾ", MB_ICONINFORMATION);
#endif
            return FALSE;
        }
        if (m_pRecordset->adoEOF)
        {
            return FALSE;
        }
        _variant_t value = m_pRecordset->GetCollect(_variant_t(strFieldName));
        strValue = Format(value);
        return TRUE;
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( �ֶη���ʧ��: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }

    return FALSE;
}

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
BOOL CAdoRecordSet::GetFieldValueByCondition(LPCTSTR strTableName, LPCTSTR strFieldName, LPCTSTR strCondition, CString &strValue)
{
    try
    {
        //����շ��ض�������
        strValue.Empty();

        if (m_pConnection == NULL || m_pConnection->GetConnection() == NULL)
        {
            return FALSE;
        }

        if (strTableName == NULL || strFieldName == NULL ||
            lstrlen(strTableName) == 0 || lstrlen(strFieldName) == 0)
        {
            return FALSE;
        }

        //�����ѯ����
        CString strSQL;
        if (strCondition != NULL && lstrlen(strCondition) > 0)
        {
            strSQL.Format(_T("SELECT %s FROM %s WHERE %s"), strFieldName, strTableName, strCondition);
        }
        else
        {
            strSQL.Format(_T("SELECT %s FROM %s"), strFieldName, strTableName);
        }

        //������ʱ��¼��
        _RecordsetPtr rsTt;
        HRESULT hr = rsTt.CreateInstance(__uuidof(Recordset));
        if (FAILED(hr))
        {
            return FALSE;
        }

        rsTt->CursorLocation = adUseClient;
        hr = rsTt->Open(_bstr_t(strSQL), _variant_t((IDispatch *)m_pConnection->GetConnection(), true), adOpenDynamic, adLockUnspecified, adCmdText);
        if (FAILED(hr))
        {
            return FALSE;
        }

        VARIANT vtValue;
        long lIndex = 0L;

        //ȡֵ
        if (!rsTt->adoEOF)
        {
            vtValue = rsTt->Fields->Item[lIndex]->Value;
            strValue = Format(vtValue);
        }
        else
        {
            rsTt->Close();
            rsTt.Release();
            return FALSE;
        }

        rsTt->Close();
        rsTt.Release();
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( GetFieldValueByCondition�����쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }

    return TRUE;
}

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
BOOL CAdoRecordSet::GetFieldValuesArrayByCondition(LPCTSTR strTableName, LPCTSTR strFieldName, LPCTSTR strCondition, LPCTSTR strOrderBy, CStringArray& strValuesArray)
{
    try
    {
        //����շ��ض�������
        strValuesArray.RemoveAll();

        if (m_pConnection == NULL || m_pConnection->GetConnection() == NULL)
        {
            return FALSE;
        }

        if (strTableName == NULL || strFieldName == NULL ||
            lstrlen(strTableName) == 0 || lstrlen(strFieldName) == 0)
        {
            return FALSE;
        }

        //�����ѯ����
        CString strSQL;
        if (strCondition != NULL && lstrlen(strCondition) > 0)
        {
            if (strOrderBy != NULL)
            {
                strSQL.Format(_T("SELECT %s FROM %s WHERE %s %s"), strFieldName, strTableName, strCondition, strOrderBy);
            }
            else
            {
                strSQL.Format(_T("SELECT %s FROM %s WHERE %s"), strFieldName, strTableName, strCondition);
            }
        }
        else
        {
            if (strOrderBy != NULL)
            {
                strSQL.Format(_T("SELECT %s FROM %s %s"), strFieldName, strTableName, strOrderBy);
            }
            else
            {
                strSQL.Format(_T("SELECT %s FROM %s"), strFieldName, strTableName);
            }
        }

        //������ʱ��¼��
        _RecordsetPtr rsTt;
        HRESULT hr = rsTt.CreateInstance(__uuidof(Recordset));
        if (FAILED(hr))
        {
            return FALSE;
        }

        rsTt->CursorLocation = adUseClient;
        hr = rsTt->Open(_bstr_t(strSQL), _variant_t((IDispatch *)m_pConnection->GetConnection(), true), adOpenDynamic, adLockUnspecified, adCmdText);
        if (FAILED(hr))
        {
            return FALSE;
        }

        if (rsTt->adoEOF)
        {
            rsTt->Close();
            rsTt.Release();
            return FALSE;
        }

        VARIANT vtValue;
        long lIndex = 0L;
        CString strValue;

        //ȡֵ
        while (!rsTt->adoEOF)
        {
            strValue.Empty();

            vtValue = rsTt->Fields->Item[lIndex]->Value;
            strValue = Format(vtValue);
            strValuesArray.Add(strValue);

            rsTt->MoveNext();
        }

        rsTt->Close();
        rsTt.Release();
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( GetFieldValuesArrayByCondition�����쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }

    return TRUE;
}

/*========================================================================
Remarks:	��_variant_t�ͱ�����ʽ����CString���ַ���.
==========================================================================*/
CString CAdoRecordSet::Format(_variant_t var)
{
    DATE dt;
    COleDateTime da;
    CString strValue;

    switch (var.vt)
    {
    case VT_BSTR://�ַ���
    case VT_LPSTR://�ַ���
    case VT_LPWSTR://�ַ���
        strValue = (LPCTSTR)(_bstr_t)var;
        break;
    case VT_I2://������
        strValue.Format("%d", var.iVal);
        break;
    case VT_UI2://�޷��Ŷ�����
        strValue.Format("%d", var.uiVal);
        break;
    case VT_VOID://
    case VT_INT://����
    case VT_I4://����
        strValue.Format("%d", var.intVal);
        break;
    case VT_I8://������
        strValue.Format("%d", var.lVal);
        break;
    case VT_UINT://�޷�������
    case VT_UI4://�޷�������
        strValue.Format("%d", var.uintVal);
        break;
    case VT_UI8://�޷��ų�����
        strValue.Format("%d", var.ulVal);
        break;
    case VT_R4://������
        strValue.Format("%.4f", var.fltVal);
        break;
    case VT_R8://˫������
        strValue.Format("%.8f", var.dblVal);
        break;
    case VT_DECIMAL: //С��
    {
                         if (var.decVal.scale == 0)
                         {
                             long val = var.decVal.Lo32;
                             strValue.Format("%ld", val);
                         }
                         else
                         {
                             double val = var.decVal.Lo32;
                             val *= (var.decVal.sign == 128) ? -1 : 1;
                             val /= pow(10.0, var.decVal.scale);
                             strValue.Format("%.4f", val);
                         }
    }
        break;
    case VT_BOOL://������
        strValue = var.lVal ? "TRUE" : "FALSE";
        break;
    case VT_DATE: //������
        dt = var.date;
        da = COleDateTime(dt);
        strValue = da.Format("%Y-%m-%d %H:%M:%S");
        break;
    case VT_NULL://NULLֵ
        //	strValue = "NULL";
        break;
    case VT_EMPTY://��
        //	strValue = "EMPTY";
        break;
    case VT_UNKNOWN://δ֪����
    default:
        //	strValue = "UN_KNOW";
        break;
    }
    return strValue;
}


BOOL CAdoRecordSet::SetCacheSize(const long &lCacheSize)
{
    ASSERT(m_pRecordset != NULL);
    try
    {
        if (m_pRecordset != NULL && !(GetState() & adStateExecuting))
        {
            return SUCCEEDED(m_pRecordset->put_CacheSize(lCacheSize));
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( SetCacheSize���������쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return FALSE;
}

/*========================================================================
Name:	ȷ��ָ���� Recordset �����Ƿ�֧���ض����͵Ĺ��ܡ�
----------------------------------------------------------
Params:	CursorOptions   �����ͣ�����һ���������� CursorOptionEnum ֵ��
����				˵��
adAddNew			��ʹ�� AddNew ��������¼�¼��
adApproxPosition	�ɶ�ȡ������ AbsolutePosition �� AbsolutePage �����ԡ�
adBookmark			��ʹ�� Bookmark ���Ի�ö��ض���¼�ķ��ʡ�
adDelete			����ʹ�� Delete ����ɾ����¼��
AdHoldRecords		���Լ��������¼���߸�����һ������λ�ö������ύ���й���ĸ��ġ�
AdMovePrevious		��ʹ�� MoveFirst �� MovePrevious �������Լ� Move ��
GetRows ��������ǰ��¼λ������ƶ�������ʹ����ǩ��
AdResync			ͨ�� Resync ������ʹ���ڻ��������ݿ��пɼ������ݸ����αꡣ
AdUpdate			��ʹ�� Update �����޸����е����ݡ�
AdUpdateBatch		����ʹ��������(UpdateBatch �� CancelBatch ����)�������鴫����ṩ�ߡ�
AdIndex				����ʹ�� Index ��������������
AdSeek				����ʹ�� Seek ������λ Recordset �е��С�
----------------------------------------------------------
returns:	���ز�����ֵ��ָʾ�Ƿ�֧�� CursorOptions ��������ʶ�����й��ܡ�
----------------------------------------------------------
Remarks:	ʹ�� Supports ����ȷ�� Recordset ������֧�ֵĹ������͡����
Recordset ����֧������Ӧ������ CursorOptions �еĹ��ܣ���ô Supports
�������� True�����򷵻� False��
ע��   ���� Supports �����ɶԸ����Ĺ��ܷ��� True���������ܱ�֤�ṩ�߿�
��ʹ���������л����¾���Ч��Supports ����ֻ�����ṩ���Ƿ�֧��ָ���Ĺ���
(�ٶ�����ĳЩ����)�����磬Supports ��������ָʾ Recordset ����֧�ָ���
(��ʹ�α���ڶ����ĺϲ�)��������ĳЩ����Ȼ�޷����¡�
==========================================================================*/
BOOL CAdoRecordSet::Supports(CursorOptionEnum CursorOptions)
{
    ASSERT(m_pRecordset != NULL);
    try
    {
        if (m_pRecordset != NULL)
        {
            BOOL bSupports = m_pRecordset->Supports(CursorOptions);
            return bSupports;
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( Supports���������쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return FALSE;
}

/*########################################################################
------------------------------------------------
���ݴ�ȡ
------------------------------------------------
########################################################################*/

BOOL CAdoRecordSet::PutCollect(long index, const _variant_t &value)
{
    ASSERT(m_pRecordset != NULL);
    ASSERT(index < GetFieldsCount());
    try
    {
        if (m_pRecordset != NULL && index < GetFieldsCount())
        {
            m_pRecordset->PutCollect(_variant_t(index), value);
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( PutCollect���������쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return	FALSE;
}

BOOL CAdoRecordSet::PutCollect(LPCSTR strFieldName, const _variant_t &value)
{
    ASSERT(m_pRecordset != NULL);
    try
    {
        if (m_pRecordset != NULL)
        {
            m_pRecordset->put_Collect(_variant_t(strFieldName), value);
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( PutCollect���������쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return	FALSE;
}

BOOL CAdoRecordSet::PutCollect(long index, const LPCTSTR &value)
{
    ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
    if (!(GetFieldType(index) == adVarChar
        || GetFieldType(index) == adChar
        || GetFieldType(index) == adLongVarChar))
        AfxMessageBox("��Ҫ�洢���ֶ���������������Ͳ���");
#endif

    ASSERT(GetFieldType(index) == adVarChar
        || GetFieldType(index) == adChar
        || GetFieldType(index) == adLongVarChar);

    return PutCollect(index, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(long index, const BYTE &value)
{
    ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
    if (GetFieldType(index) != adUnsignedTinyInt)
        AfxMessageBox("��Ҫ�洢���ֶ���������������Ͳ���");
#endif
    ASSERT(GetFieldType(index) == adUnsignedTinyInt);

    return PutCollect(index, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(long index, const short &value)
{
    ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
    if (GetFieldType(index) != adSmallInt)
        AfxMessageBox("��Ҫ�洢���ֶ���������������Ͳ���");
#endif
    ASSERT(GetFieldType(index) == adSmallInt);

    return PutCollect(index, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(long index, const int &value)
{
    ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
    if (GetFieldType(index) != adInteger)
        AfxMessageBox("��Ҫ�洢���ֶ���������������Ͳ���");
#endif
    ASSERT(GetFieldType(index) == adInteger);

    return PutCollect(index, _variant_t(long(value)));
}

BOOL CAdoRecordSet::PutCollect(long index, const long &value)
{
    ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
    if (GetFieldType(index) != adBigInt)
        AfxMessageBox("��Ҫ�洢���ֶ���������������Ͳ���");
#endif
    ASSERT(GetFieldType(index) == adBigInt);

    return PutCollect(index, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(long index, const float &value)
{
    ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
    if (GetFieldType(index) != adSingle)
        AfxMessageBox("��Ҫ�洢���ֶ���������������Ͳ���");
#endif
    ASSERT(GetFieldType(index) == adSingle);

    return PutCollect(index, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(long index, const double &value)
{
    ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
    if (GetFieldType(index) != adDouble)
        AfxMessageBox("��Ҫ�洢���ֶ���������������Ͳ���");
#endif
    ASSERT(GetFieldType(index) == adDouble);

    return PutCollect(index, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const LPCTSTR &value)
{
    ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
    if (!(GetFieldType(strFieldName) == adVarChar
        || GetFieldType(strFieldName) == adChar
        || GetFieldType(strFieldName) == adLongVarChar))
        AfxMessageBox("��Ҫ�洢���ֶ���������������Ͳ���");
#endif
    ASSERT(GetFieldType(strFieldName) == adVarChar
        || GetFieldType(strFieldName) == adChar
        || GetFieldType(strFieldName) == adLongVarChar);

    return PutCollect(strFieldName, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const BYTE &value)
{
    ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
    if (GetFieldType(strFieldName) != adUnsignedTinyInt)
        AfxMessageBox("��Ҫ�洢���ֶ���������������Ͳ���");
#endif
    ASSERT(GetFieldType(strFieldName) == adUnsignedTinyInt);

    return PutCollect(strFieldName, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const short &value)
{
    ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
    if (GetFieldType(strFieldName) != adSmallInt)
        AfxMessageBox("��Ҫ�洢���ֶ���������������Ͳ���");
#endif
    ASSERT(GetFieldType(strFieldName) == adSmallInt);
    return PutCollect(strFieldName, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const int &value)
{
    ASSERT(m_pRecordset != NULL);

#ifdef _DEBUG
    if (GetFieldType(strFieldName) != adInteger)
        AfxMessageBox("��Ҫ�洢���ֶ���������������Ͳ���");
#endif
    ASSERT(GetFieldType(strFieldName) == adInteger);

    return PutCollect(strFieldName, _variant_t(long(value)));
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const long &value)
{
    ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
    if (GetFieldType(strFieldName) != adBigInt)
        AfxMessageBox("��Ҫ�洢���ֶ���������������Ͳ���");
#endif
    ASSERT(GetFieldType(strFieldName) == adBigInt);

    return PutCollect(strFieldName, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const float &value)
{
    ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
    if (GetFieldType(strFieldName) != adSingle)
        AfxMessageBox("��Ҫ�洢���ֶ���������������Ͳ���");
#endif
    ASSERT(GetFieldType(strFieldName) == adSingle);

    return PutCollect(strFieldName, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const double &value)
{
    ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
    if (GetFieldType(strFieldName) != adDouble)
        AfxMessageBox("��Ҫ�洢���ֶ���������������Ͳ���");
#endif
    ASSERT(GetFieldType(strFieldName) == adDouble);

    return PutCollect(strFieldName, _variant_t(value));
}

/*########################################################################
------------------------------------------------
���ݴ�ȡ
------------------------------------------------
########################################################################*/

BOOL CAdoRecordSet::GetCollect(long index, _variant_t &value)
{
    ASSERT(m_pRecordset != NULL);

    try
    {
        if (m_pRecordset != NULL)
        {
            value = m_pRecordset->GetCollect(_variant_t(index));
            return TRUE;
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( GetCollect���������쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }

    return FALSE;
}

BOOL CAdoRecordSet::GetCollect(long index, COleDateTime &value)
{
    ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
    if (GetFieldType(index) != adDate && GetFieldType(index) != adDBDate && GetFieldType(index) != adDBTime && GetFieldType(index) != adDBTimeStamp)
        AfxMessageBox("��Ҫ��ȡ���ֶ���������������Ͳ���");
#endif
    ASSERT(GetFieldType(index) == adDate || GetFieldType(index) == adDBDate || GetFieldType(index) == adDBTime || GetFieldType(index) == adDBTimeStamp);

    try
    {
        if (m_pRecordset != NULL)
        {
            _variant_t result = m_pRecordset->GetCollect(_variant_t(index));

            if (VT_NULL == result.vt)
            {
                value = COleDateTime();
                return FALSE;
            }

            value = result.date;
            return TRUE;
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( GetCollect���������쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return FALSE;
}

BOOL CAdoRecordSet::GetCollect(long index, BYTE &value)
{
    ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
    if (GetFieldType(index) != adUnsignedTinyInt)
        AfxMessageBox("��Ҫ��ȡ���ֶ���������������Ͳ���");
#endif
    ASSERT(GetFieldType(index) == adUnsignedTinyInt);

    try
    {
        if (m_pRecordset != NULL)
        {
            _variant_t result = m_pRecordset->GetCollect(_variant_t(index));

            if (VT_NULL == result.vt)
            {
                value = 0;
                return FALSE;
            }

            value = result.bVal;
            return TRUE;
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( GetCollect���������쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return FALSE;
}

BOOL CAdoRecordSet::GetCollect(long index, short &value)
{
    ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
    if (GetFieldType(index) != adSmallInt)
        AfxMessageBox("��Ҫ��ȡ���ֶ���������������Ͳ���");
#endif
    ASSERT(GetFieldType(index) == adSmallInt);

    try
    {
        if (m_pRecordset != NULL)
        {
            _variant_t result = m_pRecordset->GetCollect(_variant_t(index));

            if (VT_NULL == result.vt)
            {
                value = 0;
                return FALSE;
            }

            value = result.iVal;
            return TRUE;
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( GetCollect���������쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return FALSE;
}

BOOL CAdoRecordSet::GetCollect(long index, int &value)
{
    ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
    if (GetFieldType(index) != adInteger)
        AfxMessageBox("��Ҫ��ȡ���ֶ���������������Ͳ���");
#endif
    DWORD type = GetFieldType(index);
    ASSERT(GetFieldType(index) == adInteger);

    try
    {
        if (m_pRecordset != NULL)
        {
            _variant_t result = m_pRecordset->GetCollect(_variant_t(index));

            if (VT_NULL == result.vt)
            {
                value = 0;
                return FALSE;
            }

            value = result.intVal;
            return TRUE;
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( GetCollect���������쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return FALSE;
}

BOOL CAdoRecordSet::GetCollect(long index, long &value)
{
    ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
    if (GetFieldType(index) != adBigInt)
        AfxMessageBox("��Ҫ��ȡ���ֶ���������������Ͳ���");
#endif
    ASSERT(GetFieldType(index) == adBigInt);

    try
    {
        if (m_pRecordset != NULL)
        {
            _variant_t result = m_pRecordset->GetCollect(_variant_t(index));

            if (VT_NULL == result.vt)
            {
                value = 0;
                return FALSE;
            }

            value = result.lVal;
            return TRUE;
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( GetCollect���������쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return FALSE;
}

BOOL CAdoRecordSet::GetCollect(long index, float &value)
{
    ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
    if (GetFieldType(index) != adSingle)
        AfxMessageBox("��Ҫ��ȡ���ֶ���������������Ͳ���");
#endif
    ASSERT(GetFieldType(index) == adSingle);

    try
    {
        if (m_pRecordset != NULL)
        {
            _variant_t result = m_pRecordset->GetCollect(_variant_t(index));

            if (VT_NULL == result.vt)
            {
                value = 0;
                return FALSE;
            }

            value = result.fltVal;
            return TRUE;
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( GetCollect���������쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return FALSE;
}

BOOL CAdoRecordSet::GetCollect(long index, double &value)
{
    ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
    if (GetFieldType(index) != adDouble)
        AfxMessageBox("��Ҫ��ȡ���ֶ���������������Ͳ���");
#endif
    ASSERT(GetFieldType(index) == adDouble);

    try
    {
        if (m_pRecordset != NULL)
        {
            _variant_t result = m_pRecordset->GetCollect(_variant_t(index));

            if (VT_NULL == result.vt)
            {
                value = 0;
                return FALSE;
            }

            value = result.dblVal;
            return TRUE;
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( GetCollect���������쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return FALSE;
}

BOOL CAdoRecordSet::GetCollect(LPCSTR strFieldName, _variant_t &value)
{
    ASSERT(m_pRecordset != NULL);

    try
    {
        if (m_pRecordset != NULL)
        {
            value = m_pRecordset->GetCollect(_variant_t(strFieldName));
            return TRUE;
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( GetCollect���������쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }

    return FALSE;
}

BOOL CAdoRecordSet::GetCollect(LPCSTR strFieldName, COleDateTime &value)
{
    ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
    if (GetFieldType(strFieldName) != adDate && GetFieldType(strFieldName) != adDBDate && GetFieldType(strFieldName) != adDBTime && GetFieldType(strFieldName) != adDBTimeStamp)
        AfxMessageBox("��Ҫ��ȡ���ֶ���������������Ͳ���");
#endif
    ASSERT(GetFieldType(strFieldName) == adDate || GetFieldType(strFieldName) == adDBDate || GetFieldType(strFieldName) == adDBTime || GetFieldType(strFieldName) == adDBTimeStamp);

    try
    {
        if (m_pRecordset != NULL)
        {
            _variant_t result = m_pRecordset->GetCollect(_variant_t(strFieldName));

            if (VT_NULL == result.vt)
            {
                value = COleDateTime();
                return FALSE;
            }

            value = result.date;
            return TRUE;
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( GetCollect���������쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return FALSE;
}

BOOL CAdoRecordSet::GetCollect(LPCSTR strFieldName, BYTE &value)
{
    ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
    if (GetFieldType(strFieldName) != adUnsignedTinyInt)
        AfxMessageBox("��Ҫ��ȡ���ֶ���������������Ͳ���");
#endif
    ASSERT(GetFieldType(strFieldName) == adUnsignedTinyInt);

    try
    {
        if (m_pRecordset != NULL)
        {
            _variant_t result = m_pRecordset->GetCollect(_variant_t(strFieldName));

            if (VT_NULL == result.vt)
            {
                value = 0;
                return FALSE;
            }

            value = result.bVal;
            return TRUE;
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( GetCollect���������쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return FALSE;
}

BOOL CAdoRecordSet::GetCollect(LPCSTR strFieldName, short &value)
{
    ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
    if (GetFieldType(strFieldName) != adSmallInt)
        AfxMessageBox("��Ҫ��ȡ���ֶ���������������Ͳ���");
#endif
    ASSERT(GetFieldType(strFieldName) == adSmallInt);

    try
    {
        if (m_pRecordset != NULL)
        {
            _variant_t result = m_pRecordset->GetCollect(_variant_t(strFieldName));

            if (VT_NULL == result.vt)
            {
                value = 0;
                return FALSE;
            }

            value = result.iVal;
            return TRUE;
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( GetCollect���������쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return FALSE;
}

BOOL CAdoRecordSet::GetCollect(LPCSTR strFieldName, int &value)
{
    ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
    if (GetFieldType(strFieldName) != adInteger)
        AfxMessageBox("��Ҫ��ȡ���ֶ���������������Ͳ���");
#endif
    ASSERT(GetFieldType(strFieldName) == adInteger);

    try
    {
        if (m_pRecordset != NULL)
        {
            _variant_t result = m_pRecordset->GetCollect(_variant_t(strFieldName));

            if (VT_NULL == result.vt)
            {
                value = 0;
                return FALSE;
            }

            value = result.intVal;
            return TRUE;
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( GetCollect���������쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return FALSE;
}

BOOL CAdoRecordSet::GetCollect(LPCSTR strFieldName, long &value)
{
    ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
    if (GetFieldType(strFieldName) != adBigInt)
        AfxMessageBox("��Ҫ��ȡ���ֶ���������������Ͳ���");
#endif
    ASSERT(GetFieldType(strFieldName) == adBigInt);

    try
    {
        if (m_pRecordset != NULL)
        {
            _variant_t result = m_pRecordset->GetCollect(_variant_t(strFieldName));

            if (VT_NULL == result.vt)
            {
                value = 0;
                return FALSE;
            }

            value = result.lVal;
            return TRUE;
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( GetCollect���������쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return FALSE;
}

BOOL CAdoRecordSet::GetCollect(LPCSTR strFieldName, float &value)
{
    ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
    if (GetFieldType(strFieldName) != adSingle)
        AfxMessageBox("��Ҫ��ȡ���ֶ���������������Ͳ���");
#endif
    ASSERT(GetFieldType(strFieldName) == adSingle);

    try
    {
        if (m_pRecordset != NULL)
        {
            _variant_t result = m_pRecordset->GetCollect(_variant_t(strFieldName));

            if (VT_NULL == result.vt)
            {
                value = 0;
                return FALSE;
            }

            value = result.fltVal;
            return TRUE;
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( GetCollect���������쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return FALSE;
}

BOOL CAdoRecordSet::GetCollect(LPCSTR strFieldName, double &value)
{
    ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
    if (GetFieldType(strFieldName) != adDouble)
        AfxMessageBox("��Ҫ��ȡ���ֶ���������������Ͳ���");
#endif
    ASSERT(GetFieldType(strFieldName) == adDouble);

    try
    {
        if (m_pRecordset != NULL)
        {
            _variant_t result = m_pRecordset->GetCollect(_variant_t(strFieldName));

            if (VT_NULL == result.vt)
            {
                value = 0;
                return FALSE;
            }

            value = result.dblVal;
            return TRUE;
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( GetCollect���������쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return FALSE;
}

BOOL CAdoRecordSet::GetCollect(long index, CString &value)
{
    ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
    if (!(GetFieldType(index) == adVarChar
        || GetFieldType(index) == adChar
        || GetFieldType(index) == adLongVarChar))
        AfxMessageBox("��Ҫ��ȡ���ֶ���������������Ͳ���");
#endif

    ASSERT(GetFieldType(index) == adVarChar
        || GetFieldType(index) == adChar
        || GetFieldType(index) == adLongVarChar);

    try
    {
        if (m_pRecordset != NULL)
        {
            _variant_t result = m_pRecordset->GetCollect(_variant_t(index));
            value = Format(result);//(LPCTSTR)(_bstr_t)result;
            return TRUE;
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( GetCollect���������쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return FALSE;
}

BOOL CAdoRecordSet::GetCollect(LPCSTR strFieldName, CString &value)
{
    ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
    if (!(GetFieldType(strFieldName) == adVarChar
        || GetFieldType(strFieldName) == adChar
        || GetFieldType(strFieldName) == adLongVarChar))
        AfxMessageBox("��Ҫ��ȡ���ֶ���������������Ͳ���");
#endif

    ASSERT(GetFieldType(strFieldName) == adVarChar
        || GetFieldType(strFieldName) == adChar
        || GetFieldType(strFieldName) == adLongVarChar);

    try
    {
        if (m_pRecordset != NULL)
        {
            _variant_t result = m_pRecordset->GetCollect(_variant_t(strFieldName));
            value = Format(result);//(LPCTSTR)(_bstr_t)result;
            return TRUE;
        }
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( %s GetCollect���������쳣: %s\n"), strFieldName, (LPCTSTR)e.Description());
        return FALSE;
    }
    return FALSE;
}

BOOL CAdoRecordSet::IsOpen()
{
    try
    {
        return (m_pRecordset != NULL && (GetState() & adStateOpen));
    }
    catch (const _com_error& e)
    {
        TRACE(_T(":( IsOpen���������쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return FALSE;
}

/*########################################################################
------------------------------------------------
������׷�ӵ������ı������������� Field ����.
------------------------------------------------
########################################################################*/
BOOL CAdoRecordSet::AppendChunk(FieldPtr pField, LPVOID lpData, UINT nBytes)
{
    SAFEARRAY FAR *pSafeArray = NULL;
    SAFEARRAYBOUND rgsabound[1];

    try
    {
        rgsabound[0].lLbound = 0;
        rgsabound[0].cElements = nBytes;
        pSafeArray = SafeArrayCreate(VT_UI1, 1, rgsabound);

        for (long i = 0; i < (long)nBytes; i++)
        {
            UCHAR &chData = ((UCHAR*)lpData)[i];
            HRESULT hr = SafeArrayPutElement(pSafeArray, &i, &chData);
            if (FAILED(hr))	return FALSE;
        }

        _variant_t varChunk;
        varChunk.vt = VT_ARRAY | VT_UI1;
        varChunk.parray = pSafeArray;

        return (pField->AppendChunk(varChunk) == S_OK);
    }
    catch (_com_error &e)
    {
        TRACE(_T(":( AppendChunk���������쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
}

BOOL CAdoRecordSet::AppendChunk(long index, LPVOID lpData, UINT nBytes)
{
    ASSERT(m_pRecordset != NULL);
    ASSERT(lpData != NULL);
    if (adFldLong & GetFieldAttributes(index))
    {
        return AppendChunk(GetField(index), lpData, nBytes);
    }

    return FALSE;
}

BOOL CAdoRecordSet::AppendChunk(LPCSTR strFieldName, LPVOID lpData, UINT nBytes)
{
    ASSERT(m_pRecordset != NULL);
    ASSERT(lpData != NULL);
    if (adFldLong & GetFieldAttributes(strFieldName))
    {
        return AppendChunk(GetField(strFieldName), lpData, nBytes);
    }

    return FALSE;
}

BOOL CAdoRecordSet::AppendChunk(long index, LPCTSTR lpszFileName)
{
    ASSERT(m_pRecordset != NULL);
    ASSERT(lpszFileName != NULL);
    BOOL bret = FALSE;
    if (adFldLong & GetFieldAttributes(index))
    {
        CFile file;
        if (file.Open(lpszFileName, CFile::modeRead))
        {
            long length = (long)file.GetLength();
            char *pbuf = new char[length];
            if (pbuf != NULL && file.Read(pbuf, length) == (DWORD)length)
            {
                bret = AppendChunk(GetField(index), pbuf, length);
            }
            if (pbuf != NULL) delete[] pbuf;
        }
        file.Close();
    }
    return bret;
}

BOOL CAdoRecordSet::AppendChunk(LPCSTR strFieldName, LPCTSTR lpszFileName)
{
    ASSERT(m_pRecordset != NULL);
    ASSERT(lpszFileName != NULL);
    BOOL bret = FALSE;
    if (adFldLong & GetFieldAttributes(strFieldName))
    {
        CFile file;
        if (file.Open(lpszFileName, CFile::modeRead))
        {
            long length = (long)file.GetLength();
            char *pbuf = new char[length];
            if (pbuf != NULL && file.Read(pbuf, length) == (DWORD)length)
            {
                bret = AppendChunk(GetField(strFieldName), pbuf, length);
            }
            if (pbuf != NULL) delete[] pbuf;
        }
        file.Close();
    }
    return bret;
}

BOOL CAdoRecordSet::GetChunk(FieldPtr pField, LPVOID lpData)
{
    ASSERT(pField != NULL);
    ASSERT(lpData != NULL);

    UCHAR chData;
    long index = 0;

    while (index < pField->ActualSize)
    {
        try
        {
            _variant_t varChunk = pField->GetChunk(pField->ActualSize);
            if (varChunk.vt != (VT_ARRAY | VT_UI1))
            {
                return FALSE;
            }

            for (long i = 0; i < pField->ActualSize; i++)
            {
                if (SUCCEEDED(SafeArrayGetElement(varChunk.parray, &i, &chData)))
                {
                    ((UCHAR*)lpData)[index] = chData;
                    index++;
                }
                else
                {
                    break;
                }
            }
        }
        catch (_com_error e)
        {
            TRACE(_T("Warning: GetChunk ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
            return FALSE;
        }
    }

    return TRUE;
}

BOOL CAdoRecordSet::GetChunk(long index, LPVOID lpData)
{
    if (adFldLong & GetFieldAttributes(index))
        return  GetChunk(GetField(index), lpData);
    else return FALSE;
}

BOOL CAdoRecordSet::GetChunk(LPCSTR strFieldName, LPVOID lpData)
{
    if (adFldLong & GetFieldAttributes(strFieldName))
        return  GetChunk(GetField(strFieldName), lpData);
    else return FALSE;
}

BOOL CAdoRecordSet::GetChunk(long index, CBitmap &bitmap)
{
    return GetChunk(GetFieldName(index), bitmap);
}

BOOL CAdoRecordSet::GetChunk(LPCSTR strFieldName, CBitmap &bitmap)
{
    BOOL bret = FALSE;
    long size = GetFieldActualSize(strFieldName);
    if ((adFldLong & GetFieldAttributes(strFieldName)) && size > 0)
    {
        BYTE *lpData = new BYTE[size];

        if (GetChunk(GetField(strFieldName), (LPVOID)lpData))
        {
            BITMAPFILEHEADER bmpHeader;
            DWORD bmfHeaderLen = sizeof(bmpHeader);
            strncpy((LPSTR)&bmpHeader, (LPSTR)lpData, bmfHeaderLen);

            // �Ƿ���λͼ ----------------------------------------
            if (bmpHeader.bfType == (*(WORD*)"BM"))
            {
                BYTE* lpDIBBits = lpData + bmfHeaderLen;
                BITMAPINFOHEADER &bmpiHeader = *(LPBITMAPINFOHEADER)lpDIBBits;
                BITMAPINFO &bmpInfo = *(LPBITMAPINFO)lpDIBBits;
                lpDIBBits = lpData + ((BITMAPFILEHEADER *)lpData)->bfOffBits;

                // ����λͼ --------------------------------------
                CDC dc;
                HDC hdc = GetDC(NULL);
                dc.Attach(hdc);
                HBITMAP hBmp = CreateDIBitmap(dc.m_hDC, &bmpiHeader, CBM_INIT, lpDIBBits, &bmpInfo, DIB_RGB_COLORS);
                if (bitmap.GetSafeHandle() != NULL) bitmap.DeleteObject();
                bitmap.Attach(hBmp);
                dc.Detach();
                ReleaseDC(NULL, hdc);
                bret = TRUE;
            }
        }
        delete[] lpData;
        lpData = NULL;
    }
    return bret;
}

//============================CDBAccess���ݿ���ʲ������װ============================
//���캯������ɳ�ʼ��
CDBAccess::CDBAccess()
{
    //��¼������������
    m_nResultRow = 0;
    m_nResultCol = 0;
    m_pConnection = NULL;
}

//�ر����ݿ���������
CDBAccess::~CDBAccess()
{
    CloseDataBase();
}

//�����ݿ�����
BOOL CDBAccess::OpenDataBase(const CString& strDsnName, const CString& strUserID, const CString& strPassword)
{
    try
    {
        //�������Ӷ���
        HRESULT hr = m_pConnection.CreateInstance("ADODB.Connection");
        if (FAILED(hr))
            return FALSE;
        //�������ݿ�
        hr = m_pConnection->Open((_bstr_t)strDsnName, (_bstr_t)strUserID, (_bstr_t)strPassword, adOpenUnspecified);
        if (FAILED(hr))
            return FALSE;
    }
    catch (const _com_error& e)
    {
        m_strErrorMessage = (LPCTSTR)e.Description();
        TRACE(_T(":( �������ݿⷢ���쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return TRUE;
}

//�����ݿ����� ���������ַ���
BOOL CDBAccess::OpenDataBase(const CString& strConnection, long lOptions)
{
    try
    {
        //�������Ӷ���
        HRESULT hr = m_pConnection.CreateInstance("ADODB.Connection");
        if (FAILED(hr))
            return FALSE;
        // �������ݿ�
        return SUCCEEDED(m_pConnection->Open(_bstr_t(strConnection), "", "", lOptions));
    }
    catch (const _com_error& e)
    {
        m_strErrorMessage = (LPCTSTR)e.Description();
        TRACE(_T(":( �������ݿⷢ���쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    catch (...)
    {
        TRACE(_T(":( �������ݿ�ʱ����δ֪����:"));
    }
    return FALSE;
}

//ִ��SQL��ѯ��䣬�����������pData��
//����ֵΪ���������
long CDBAccess::QueryData(const CString& strSQL)
{
    try
    {
        if (m_pConnection == NULL)
        {
            return -1;
        }

        HRESULT hr = NULL;
        _variant_t vValue, vValue1;
        _bstr_t strVal;
        int i = 0;

        //������¼������ʵ��
        hr = m_pRecordset.CreateInstance("ADODB.Recordset");
        //�򿪼�¼��
        hr = m_pRecordset->Open(_bstr_t(strSQL), _variant_t((IDispatch*)m_pConnection, true), adOpenDynamic, adLockOptimistic, adCmdText);
        //��ü�¼��������
        m_nResultCol = m_pRecordset->Fields->GetCount();
        while (!m_pRecordset->adoEOF)
        {
            for (int j = 0; j < m_nResultCol; j++)//ȡһ������
            {
                //ȡ�õ�ǰ��¼�ֶ�����
                vValue = m_pRecordset->Fields->Item[(long)j]->Value;
                //���ݷǿ�
                if (vValue.vt != VT_NULL)
                {
                    //ת���������ͣ�Ϊ�ַ�����
                    if (VariantChangeType(&vValue1, &vValue, 0, VT_BSTR) == S_OK)
                    {
                        //������ֵ����ʱ����strVal��
                        strVal = vValue1.bstrVal;
                    }
                    else
                    {
                        strVal = vValue;
                    }
                }
                else
                {
                    strVal = "";
                }

                //��̬��������Ĵ�С
                if (m_pData.GetSize() < i*m_nResultCol + j + 1)
                    m_pData.SetSize(i*m_nResultCol + j + 1);

                //��������������
                m_pData[i*m_nResultCol + j] = (char *)strVal;
            }

            //�ƶ���¼��ָ�뵽��һ��
            m_pRecordset->MoveNext();
            i++;
        }

        m_pRecordset->Close();
        m_pRecordset = NULL;
        m_nResultRow = i;
    }
    catch (const _com_error& e)///��׽�쳣
    {
        m_pRecordset->Close();
        m_pRecordset = NULL;
        m_nResultRow = 0;
        m_nResultCol = 0;

        m_strErrorMessage = (LPCTSTR)e.Description();
        TRACE(_T(":( ���ݿ��ѯ�����쳣: %s\n"), (LPCTSTR)e.Description());
        return -1;
    }
    //�ͷŶ�����ڴ�ռ�
    m_pData.FreeExtra();
    return m_nResultRow;
}

//ִ���޷���ֵ��SQL���
BOOL CDBAccess::ExecuteSQL(const CString& strSQL)
{
    try
    {
        if (m_pConnection == NULL)
        {
            return FALSE;
        }

        m_pConnection->Execute(_bstr_t(strSQL), NULL, adCmdText | adExecuteNoRecords);
        return TRUE;
    }
    catch (const _com_error& e)
    {
        m_strErrorMessage = (LPCTSTR)e.Description();
        TRACE(_T(":( ִ��SQL���������ݿⷢ���쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
    return FALSE;
}

//�ر����ݿ�����
BOOL CDBAccess::CloseDataBase()
{
    try
    {
        if (m_pConnection != NULL && m_pConnection->State != adStateClosed)
        {
            m_pConnection->Close();
            m_pConnection = NULL;
        }

        if (m_pRecordset != NULL && m_pRecordset->State != adStateClosed)
        {
            m_pRecordset->Close();
            m_pRecordset = NULL;
        }
    }
    catch (const _com_error& e)
    {
        m_strErrorMessage = (LPCTSTR)e.Description();
        TRACE(_T(":( �ر����ݿⷢ���쳣: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }

    return TRUE;
}
//==========================End CDBAccess���ݿ���ʲ������װ==========================