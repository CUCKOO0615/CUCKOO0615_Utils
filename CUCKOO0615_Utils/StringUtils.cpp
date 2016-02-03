
#include "StringUtils.h"

#ifdef CUCKOO0615_USE_STL

void StringUtils::ReplaceSpecifiedString(string& str, string oldStr, string newStr)
{
    size_t nOldStrLength = oldStr.length();
    size_t nPos = str.find(oldStr);
    while (string::npos != nPos)
    {
        str.replace(nPos, nOldStrLength, newStr);
        nPos = str.find(oldStr);
    }
}

void StringUtils::SplitStr2Array(vector<string>& vecOut, const string& str, char chSpliter, bool bSkipContinuous)
{	
	const char* pB = str.c_str();
	if (!*pB) return;

	if (bSkipContinuous)
	{
		while ((*pB) == chSpliter) ++pB;
		if (!*pB) return;
	}
	const char* pF = pB;

	do
	{
		if ((*pB) != chSpliter)
			++pB;
		else
		{
			vecOut.push_back(string(pF, pB));
			if (bSkipContinuous)
			{
				while ((*pB) == chSpliter) ++pB;
				if (!*pB) return;
			}
			else
				++pB;
			pF = pB;
		}
	} while (*pB);
    vecOut.push_back(string(pF, pB));
}

bool StringUtils::StrConv_A2W(const char* szMultibyteStr, wstring& strWide, string& strErrMsg)
{
    int nLength = MultiByteToWideChar(CP_ACP, 0, szMultibyteStr, -1, NULL, 0);
    wchar_t* szWideStr = new wchar_t[nLength];
    if (!szWideStr)
    {
        strErrMsg = "Allocate buffer failed";
        return false;
    }
    memset(szWideStr, 0x00, nLength * sizeof(wchar_t));
    int nRet = MultiByteToWideChar(CP_ACP, 0, szMultibyteStr, -1, szWideStr, nLength);

    bool bFuncRet = true;
    if (0 == nRet)
    {
        bFuncRet = false;
        switch (GetLastError())
        {
        case ERROR_INSUFFICIENT_BUFFER:
            strErrMsg = "Buffer too small";
            break;
        case ERROR_INVALID_FLAGS:
            strErrMsg = "Invalid flags";
            break;
        case ERROR_INVALID_PARAMETER:
            strErrMsg = "Parameters is incorrect";
            break;
        case ERROR_NO_UNICODE_TRANSLATION:
            strErrMsg = "No mapping for the Unicode character exists in the target multi-byte code page";
            break;
        default:
            strErrMsg = "Unknow error";
        }
    }
    strWide = szWideStr;
    delete[] szWideStr;
    return bFuncRet;
}

bool StringUtils::StrConv_W2A(const wchar_t* szWidestr, string& strMultibyte, string& strErrMsg)
{
    int nLength = WideCharToMultiByte(CP_ACP, 0, szWidestr, -1, NULL, 0, NULL, NULL);

    char* szChar = new char[nLength];
    if (!szChar)
    {
        strErrMsg = "Allocate buffer failed";
        return false;
    }
    memset(szChar, 0x00, nLength);

    BOOL bUseHolderCheck = FALSE;
    char chHolder = 'c';
    int nRet = WideCharToMultiByte(CP_ACP, 0, szWidestr, -1, szChar, nLength, &chHolder, &bUseHolderCheck);

    bool bFuncRet = true;
    if (0 == nRet)
    {
        bFuncRet = false;
        switch (GetLastError())
        {
        case ERROR_INSUFFICIENT_BUFFER:
            strErrMsg = "Buffer too small";
            break;
        case ERROR_INVALID_FLAGS:
            strErrMsg = "Invalid flags";
            break;
        case ERROR_INVALID_PARAMETER:
            strErrMsg = "Parameters is incorrect";
            break;
        case ERROR_NO_UNICODE_TRANSLATION:
            strErrMsg = "No mapping for the Unicode character exists in the target multi-byte code page";
            break;
        default:
            strErrMsg = "Unknow error";
        }
    }

    strMultibyte = szChar;
    delete[] szChar;
    return bFuncRet;
}

#endif

void StringUtils::TrimLeft(char * szStr, size_t nBufSize, char chSpilter /*= ' ' */)
{
    if (NULL == szStr)
        return;

    char* szTmp = new char[nBufSize];
    memcpy(szTmp, szStr, nBufSize);

    size_t nStrLength = strlen(szTmp);
    size_t nCheck = min(nStrLength, nBufSize);
    size_t nPos = 0;
    for (; nPos != nCheck; ++nPos)
    {
        if (chSpilter != szTmp[nPos])
            break;
    }
    memset(szStr, 0x00, nBufSize);
    memcpy(szStr, szTmp + nPos, nCheck - nPos);
    delete[] szTmp;
}

void StringUtils::TrimRight(char* szStr, size_t nBufSize, char chSpilter /*= ' ' */)
{
    if (NULL == szStr)
        return;

    char* szTmp = new char[nBufSize];
    memcpy(szTmp, szStr, nBufSize);

    size_t nStrLength = strlen(szTmp);
    size_t nCheck = min(nStrLength, nBufSize);
    size_t nPos = nCheck - 1;
    for (; nPos != -1; --nPos)
    {
        if (chSpilter != szTmp[nPos])
            break;
    }
    memset(szStr, 0x00, nBufSize);
    memcpy(szStr, szTmp, nPos + 1);
    delete[] szTmp;
}

void StringUtils::Trim(char* szStr, size_t nBufSize, char chSpilter /*= ' ' */)
{
    if (NULL == szStr)
        return;

    char* szTmp = new char[nBufSize];
    memcpy(szTmp, szStr, nBufSize);
    memset(szStr, 0x00, nBufSize);

    size_t nStrLength = strlen(szTmp);
    size_t nCheck = min(nStrLength, nBufSize);

    size_t nPosL = 0;
    for (; nPosL != nCheck; ++nPosL)
    {
        if (chSpilter != szTmp[nPosL])
            break;
    }
    if (nCheck == nPosL)
    {
        delete[] szTmp;
        return;
    }

    size_t nPosR = nCheck - 1;
    for (; nPosR != -1; --nPosR)
    {
        if (chSpilter != szTmp[nPosR])
            break;
    }
    if (-1 == nPosR)
    {
        delete[] szTmp;
        return;
    }

    memcpy(szStr, szTmp + nPosL, nPosR - nPosL + 1);
    delete[] szTmp;
}

bool StringUtils::IsDigitStr(const char* szStr)
{
    const char* szTmp = szStr;
    while (' ' == (*szTmp))
        ++szTmp;

    size_t strLength = strlen(szTmp);
    if (0 == strLength)
        return false;

    if ((1 == strLength) && (szTmp[0] >= '0') && (szTmp[0] <= '9'))
        return true;

    //首字符
    if ((szTmp[0] > '9' || szTmp[0] < '0')
        && szTmp[0] != '-'
        && szTmp[0] != '+')
        return false;

    int dotNum = 0;
    size_t nPos = 1;
    for (; nPos != strLength; ++nPos)
    {
        //小数点个数
        if ('.' == szTmp[nPos])
        {
            ++dotNum;
            continue;
        }

        //逐位检测
        if ((szTmp[nPos] > '9' || szTmp[nPos] < '0'))
            break;
    }
    if ((dotNum > 1) || (nPos != strLength))
        return false;
    else
        return true;
}

void StringUtils::ReplaceSpecifiedString(char* szStr, size_t nBufSize, const char* szOldStr, const char* szNewStr)
{
    char* szTmp = new char[nBufSize];
    memcpy(szTmp, szStr, nBufSize);
    memset(szStr, 0x00, nBufSize);
    char* pWriter = szStr;

    size_t nOldStrLength = strlen(szOldStr);
    size_t nNewStrLength = strlen(szNewStr);
    size_t nStrLength = strlen(szTmp);
    size_t nCheck = min(nStrLength, nBufSize);
    for (size_t i = 0; i<nCheck; ++i)
    {
        if (szTmp[i] != szOldStr[0])
        {
            *pWriter++ = szTmp[i];
        }
        else if (0 == memcmp(szTmp + i, szOldStr, nOldStrLength))
        {
            size_t nCopyLength = nNewStrLength;
            if (pWriter + nNewStrLength > szStr + nBufSize)
            {
                nCopyLength = szStr + nBufSize - pWriter;
            }
            memcpy(pWriter, szNewStr, nCopyLength);
            pWriter += nNewStrLength;
        }
        else
        {
            *pWriter++ = szTmp[i];
        }

        //越界检测
        if (pWriter == szStr + nBufSize)
            break;
    }
    delete[] szTmp;
}

static const char map[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
bool StringUtils::Hex2Text(const char* inBuff, size_t nInBuffSize, char* outBuff, size_t nOutBuffSize)
{
    if (nOutBuffSize < nInBuffSize << 1)
        return false;

    const unsigned char* pReader = (const unsigned char*)inBuff;
    char* pWriter = outBuff;

    for (size_t i = 0; i != nInBuffSize; ++i)
    {
        *pWriter++ = map[(*pReader) >> 4];
        *pWriter++ = map[(*pReader) & 0xF];
        ++pReader;
    }
    return true;
}

