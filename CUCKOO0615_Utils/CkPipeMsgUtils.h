#pragma once
#include <string>
#include <windows.h>

typedef enum emCkPipeType
{
	emPtRead,
	emPtWrite
} PIPE_TYPE;

class CkPipeMsgUtils
{

public:
	const char* GetErrMsg() const { return m_szErrMsg; }

	/*
	** ����ָ�����Ƶ������ܵ�����
	** @Param szPipeName: ָ������
	** @Ret: �����ɹ�������Ч�������ܵ����, 
	**       ����ʧ�ܷ��� INVALID_HANDLE_VALUE
	*/
	HANDLE Create(const char* szPipeName);

	bool WaitForClient(HANDLE hPipe);

public:
	CkPipeMsgUtils();
	~CkPipeMsgUtils();
	
private:
	std::string m_strPipeName;

	static const int m_nErrMsgLen = 256;
	char m_szErrMsg[m_nErrMsgLen];

	static const int m_nPipeBuffSize = 512;

private:
	void SetErrMsg(const char* szErrMsg) { ::memcpy(m_szErrMsg, szErrMsg, m_nErrMsgLen); }

};

