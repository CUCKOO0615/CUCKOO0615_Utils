#include "stdafx.h"
#include "CkPipeMsgUtils.h"


HANDLE CkPipeMsgUtils::Create(const char* szPipeName)
{
	m_strPipeName = std::string("\\\\.\\pipe\\") + std::string(szPipeName);
	
	HANDLE hPipe = ::CreateNamedPipeA(
		m_strPipeName.c_str(),
		PIPE_ACCESS_INBOUND,
		PIPE_READMODE_BYTE,
		PIPE_UNLIMITED_INSTANCES,
		m_nPipeBuffSize,
		m_nPipeBuffSize,
		0,
		NULL);

	return hPipe;
}

bool CkPipeMsgUtils::WaitForClient(HANDLE hPipe)
{
	BOOL bRet = ::ConnectNamedPipe(hPipe, NULL);
	return (TRUE == bRet);
}


CkPipeMsgUtils::CkPipeMsgUtils()
{
	memset(m_szErrMsg, 0x00, m_nErrMsgLen);
}


CkPipeMsgUtils::~CkPipeMsgUtils()
{
}

