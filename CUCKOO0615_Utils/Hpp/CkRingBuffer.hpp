//************************************************
//◇作者：CUCKOO0615
//◇日期：2015/06/27
//◇说明：RingBuffer 环形缓冲区
//*************************************************
#pragma once
#include <assert.h>
#include <string.h>

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif
#ifndef NULL
#define NULL 0
#endif

class CkRingBuffer
{
	//////////////////////////////////////////////////////////////////////////
	//Interface
public:
	/*
	** 写入指定的字节数到RingBuffer中
	** @Param pSrcBuff: 源缓冲区
	** @Param nNumOfBytes: 指定的字节数
	** @Ret: 成功写入的字节数
	*/
	size_t PushBytes(char* pSrcBuff, size_t nNumOfBytes);

	/*
	** 读出指定数量的字节到pDescBuff中
	** @Param pDescBuff: 目标缓冲区
	** @Param nNumOfBytes: 指定的字节数
	** @Ret: 实际读出的字节数
	*/
	size_t PopBytes(char* pDescBuff, size_t nNumOfBytes);

	/*
	** 拷贝指定数量的字节到pDescBuff中
	** @Param pDescBuff: 目标缓冲区
	** @Param nNumOfBytes: 指定的字节数
	** @Ret: 实际拷贝的字节数
	*/
	size_t CopyBytes(char* pDescBuff, size_t nNumOfBytes);

	/*
	** 跳过指定数量的字节
	** @Param nNumOfBytes: 指定的字节数
	** @Ret: 实际跳过的字节数
	*/
	size_t SkipBytes(size_t nNumOfBytes);

	// 清空缓冲区
	void ClearBuffer();

	// 获取已使用的空间大小
	size_t GetUsageSize()   const{ return m_nTotalSize - m_nRemainSize; }

	// 获取剩余空间大小
	size_t GetRemainSize()  const{ return m_nRemainSize; }

	// 获取总空间大小
	size_t GetTotalSize()   const{ return m_nTotalSize; }


	//////////////////////////////////////////////////////////////////////////
public:
	CkRingBuffer(size_t nBuffSize) :
		m_nWritePos(0),	m_nReadPos(0),
		m_nRemainSize(nBuffSize),
		m_nTotalSize(nBuffSize),
		m_pRingBuffer(NULL)
	{
		m_pRingBuffer = new char[nBuffSize];
		assert(m_pRingBuffer);
		::memset(m_pRingBuffer, 0x00, nBuffSize);
	}
	~CkRingBuffer()
	{
		if (m_pRingBuffer)
			delete[] m_pRingBuffer;
	}

private:
	char * m_pRingBuffer;
	size_t m_nWritePos;
	size_t m_nReadPos;
	size_t m_nRemainSize;
	size_t m_nTotalSize;
};

//////////////////////////////////////////////////////////////////////////
// inline 

inline size_t CkRingBuffer::PushBytes(char* pSrcBuff, size_t nNumOfBytes)
{
	if (0 == m_nRemainSize || 0 == nNumOfBytes)
		return 0;

	int nQuantityForPush = min(m_nRemainSize, nNumOfBytes);

	if (m_nWritePos + nQuantityForPush <= m_nTotalSize)
	{
		::memcpy(m_pRingBuffer + m_nWritePos, pSrcBuff, nQuantityForPush);
		m_nWritePos += nQuantityForPush;
		if (m_nTotalSize == m_nWritePos)
			m_nWritePos = 0;
	}
	else
	{
		size_t nCopyBytesR = m_nTotalSize - m_nWritePos;
		::memcpy(m_pRingBuffer + m_nWritePos, pSrcBuff, nCopyBytesR);
		size_t nCopyBytesL = m_nWritePos + nQuantityForPush - m_nTotalSize;
		::memcpy(m_pRingBuffer, pSrcBuff + nCopyBytesR, nCopyBytesL);
		m_nWritePos = nCopyBytesL;
	}

	m_nRemainSize -= nQuantityForPush;
	return nQuantityForPush;
}

inline size_t CkRingBuffer::PopBytes(char* pDescBuff, size_t nNumOfBytes)
{
	size_t nNumOfBytesStored = m_nTotalSize - m_nRemainSize;
	if (0 == nNumOfBytesStored || 0 == nNumOfBytes)
		return 0;

	int nQuantityForPop = min(nNumOfBytesStored, nNumOfBytes);

	if (m_nReadPos + nQuantityForPop <= m_nTotalSize)
	{
		::memcpy(pDescBuff, m_pRingBuffer + m_nReadPos, nQuantityForPop);
		m_nReadPos += nQuantityForPop;
		if (m_nTotalSize == m_nReadPos)
			m_nReadPos = 0;
	}
	else
	{
		size_t nCopyBytesR = m_nTotalSize - m_nReadPos;
		::memcpy(pDescBuff, m_pRingBuffer + m_nReadPos, nCopyBytesR);
		size_t nCopyBytesL = m_nReadPos + nQuantityForPop - m_nTotalSize;
		::memcpy(pDescBuff + nCopyBytesR, m_pRingBuffer, nCopyBytesL);
		m_nReadPos = nCopyBytesL;
	}

	m_nRemainSize += nQuantityForPop;
	return nQuantityForPop;
}

inline size_t CkRingBuffer::CopyBytes(char* pDescBuff, size_t nNumOfBytes)
{
	size_t nNumOfBytesStored = m_nTotalSize - m_nRemainSize;
	if (0 == nNumOfBytesStored || 0 == nNumOfBytes)
		return 0;

	int nQuantityForCopy = min(nNumOfBytesStored, nNumOfBytes);

	if (m_nReadPos + nQuantityForCopy <= m_nTotalSize)
		::memcpy(pDescBuff, m_pRingBuffer + m_nReadPos, nQuantityForCopy);
	else
	{
		size_t nCopyBytesR = m_nTotalSize - m_nReadPos;
		::memcpy(pDescBuff, m_pRingBuffer + m_nReadPos, nCopyBytesR);
		size_t nCopyBytesL = m_nReadPos + nQuantityForCopy - m_nTotalSize;
		::memcpy(pDescBuff + nCopyBytesR, m_pRingBuffer, nCopyBytesL);
	}
	return nQuantityForCopy;
}

inline size_t CkRingBuffer::SkipBytes(size_t nNumOfBytes)
{
	size_t nNumOfBytesStored = m_nTotalSize - m_nRemainSize;
	if (0 == nNumOfBytesStored || 0 == nNumOfBytes)
		return 0;

	int nQuantityForSkip = min(nNumOfBytesStored, nNumOfBytes);

	if (m_nReadPos + nQuantityForSkip == m_nTotalSize)
		m_nReadPos = 0;
	else if(m_nReadPos + nQuantityForSkip < m_nTotalSize)
		m_nReadPos += nQuantityForSkip;
	else
		m_nReadPos = m_nReadPos + nQuantityForSkip - m_nTotalSize;

	m_nRemainSize += nQuantityForSkip;
	return nQuantityForSkip;
}

inline void CkRingBuffer::ClearBuffer()
{
	::memset(m_pRingBuffer, 0x00, m_nTotalSize);
	m_nWritePos = 0;
	m_nReadPos = 0;
	m_nRemainSize = m_nTotalSize;
}

