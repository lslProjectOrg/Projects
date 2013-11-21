
#include "DataBuffer.h"
#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/DebugUtil.h"

NS_BEGIN(TA_Base_App)



CDataBuffer::CDataBuffer(int nBufferSize)
{
	FUNCTION_ENTRY("CDataBuffer");

	//init value
	m_pBuffer = NULL;			
	m_pWritePos = NULL;		
	m_pReadPos = NULL;	
	m_nBufferSize = 0;			
	m_nBufferSizeLimit = DEF_INT_BUFFER_LEN_MAX;//268M
	m_nRemainDataLen = 0;
	
	//set value
	if (nBufferSize > m_nBufferSizeLimit) 
	{
		nBufferSize = DEF_INT_BUFFER_LEN_MAX;
	}
	m_pBuffer = new char[nBufferSize];
	memset(m_pBuffer, 0, nBufferSize);

	m_pWritePos = m_pBuffer;
	m_pReadPos = m_pBuffer;
	m_nBufferSize = nBufferSize;
	m_nRemainDataLen = m_pWritePos - m_pReadPos;
	FUNCTION_EXIT;
}

CDataBuffer::~CDataBuffer()
{
	FUNCTION_ENTRY("~CDataBuffer");

	if (m_pBuffer) 
	{
		delete[] m_pBuffer;
		m_pBuffer = NULL;
	}
		
	m_pBuffer = NULL;			
	m_pWritePos = NULL;		
	m_pReadPos = NULL;	
	m_nBufferSize = 0;			
	m_nBufferSizeLimit = 0;
	m_nRemainDataLen = 0;
	FUNCTION_EXIT;

}


char* CDataBuffer::getWritePos()  
{
	FUNCTION_ENTRY("getWritePos");
	FUNCTION_EXIT;
	return m_pWritePos; 
	
}

char* CDataBuffer::getReadPos()  
{
	FUNCTION_ENTRY("getReadPos");
	FUNCTION_EXIT;
	return m_pReadPos; 
}

int CDataBuffer::getRemainDataLength()  
{
	FUNCTION_ENTRY("getRemainDataLength");
	m_nRemainDataLen = (int)(m_pWritePos - m_pReadPos);
	FUNCTION_EXIT;

	return m_nRemainDataLen; 
}
int CDataBuffer::getBlankSize()  
{
	FUNCTION_ENTRY("getBlankSize");
	FUNCTION_EXIT;
	return m_pBuffer + m_nBufferSize - m_pWritePos; 
}
int CDataBuffer::getLimit()  
{
	FUNCTION_ENTRY("getLimit");
	FUNCTION_EXIT;
	return m_nBufferSizeLimit; 
}
int CDataBuffer::getBufferSize()  
{ 
	FUNCTION_ENTRY("getBufferSize");
	FUNCTION_EXIT;
	return m_nBufferSize; 
}
//////////////////////////////////////////////////////////////////////////

bool CDataBuffer::write(const void* src, int nLen)
{
	FUNCTION_ENTRY("write");
	char* pPosTmp = NULL;
	const char* byteaddr = (const char*)src;
	if (nLen <= 0)	
	{
		FUNCTION_EXIT;
		return false;
	}
	
	pPosTmp = m_pWritePos + nLen;
	if (pPosTmp > (m_pBuffer + m_nBufferSize))
	{
		FUNCTION_EXIT;
		return false;
	}

	memcpy(m_pWritePos, byteaddr, nLen);
	m_pWritePos = pPosTmp;
	pPosTmp = NULL;

	getRemainDataLength();
	
	FUNCTION_EXIT;
	return true;
}

bool CDataBuffer::read(void* dest, int nLen)
{
	FUNCTION_ENTRY("read");
	char* pPosTmp = NULL;

	if (nLen <= 0)
	{
		FUNCTION_EXIT;
		return false;
	}
	
	pPosTmp = m_pReadPos + nLen;
	if (pPosTmp > m_pWritePos)
	{
		FUNCTION_EXIT;
		return false;
	}

	memcpy(dest, m_pReadPos, nLen);
	m_pReadPos = pPosTmp;
	pPosTmp = NULL;

	getRemainDataLength();
	
	FUNCTION_EXIT;
	return true;
}

bool CDataBuffer::moveWritePos(int nLen)
{
	FUNCTION_ENTRY("moveWritePos");
	char* pPosTmp = NULL;
	
	pPosTmp = m_pWritePos + nLen;
	
	if (pPosTmp < m_pReadPos)
	{
		FUNCTION_EXIT;
		return false;
	}
	
	if (pPosTmp > (m_pBuffer + m_nBufferSize))	
	{
		FUNCTION_EXIT;
		return false;
	}
	m_pWritePos = pPosTmp;
	pPosTmp = NULL;

	getRemainDataLength();

	FUNCTION_EXIT;
	return true;
}

bool CDataBuffer::moveReadPos(int nLen)
{
	FUNCTION_ENTRY("moveWritePos");

	char* pPosTmp = NULL;

	pPosTmp = m_pReadPos + nLen;
	if (pPosTmp < m_pBuffer)
	{
		FUNCTION_EXIT;
		return false;
	}
	
	if (pPosTmp > m_pWritePos)	
	{
		FUNCTION_EXIT;
		return false;
	}
	m_pReadPos = pPosTmp;
	pPosTmp = NULL;

	getRemainDataLength();

	FUNCTION_EXIT;
	return true;
}

void CDataBuffer::clear()		
{
	FUNCTION_ENTRY("moveWritePos");

	m_pWritePos = m_pBuffer;
	m_pReadPos = m_pBuffer;
	getRemainDataLength();
	FUNCTION_EXIT;
}

void CDataBuffer::trim()		
{
	FUNCTION_ENTRY("moveWritePos");

	int nDataLen = 0;
	
	nDataLen = m_pWritePos - m_pReadPos;
	if (nDataLen > 0)
	{
		memcpy(m_pBuffer, m_pReadPos, nDataLen);
	}
	m_pReadPos = m_pBuffer;
	m_pWritePos = m_pBuffer + nDataLen;
	memset(m_pWritePos, 0, m_nBufferSize - nDataLen);
	
	getRemainDataLength();
	FUNCTION_EXIT;
}


//////////////////////////////////////////////////////////////////////////

bool CDataBuffer::resize(int nsize)
{
	FUNCTION_ENTRY("resize");

	int nDataLen = 0;
	char* pszTmp = NULL;

	if (nsize > m_nBufferSizeLimit)
	{
		nsize = m_nBufferSizeLimit;
	}
	
	nDataLen = m_pWritePos - m_pReadPos;
	if (nDataLen >= nsize)
	{
		FUNCTION_EXIT;
		return false;
	}
	try
	{
		pszTmp = new char[nsize];
		memset(pszTmp, 0, nsize);
		memcpy(pszTmp, m_pReadPos, nDataLen);
		delete[] m_pBuffer;
		m_pBuffer = NULL;

		m_pBuffer = pszTmp;
		pszTmp = NULL;
		m_pReadPos = m_pBuffer;
		m_pWritePos = m_pBuffer + nDataLen;
		m_nBufferSize = nsize;
	}
	catch (...)
	{
		FUNCTION_EXIT;
		return false;
	}
	
	getRemainDataLength();
	FUNCTION_EXIT;
	return true;
}

bool CDataBuffer::resizeFitData()
{
	FUNCTION_ENTRY("resizeFitData");

	char* pszTmp = NULL;

	int nDataLen = m_pWritePos - m_pReadPos;
	int nBufferSize = nDataLen + DEF_INT_BUFFER_LEN_ADDEX;
	try
	{
		pszTmp = new char[nBufferSize];
		memset(pszTmp, 0, nBufferSize);
		if (nDataLen < nBufferSize)
		{
			memcpy(pszTmp, m_pReadPos, nDataLen);
		}
		delete[] m_pBuffer;
		m_pBuffer = NULL;

		m_pBuffer = pszTmp;
		pszTmp = NULL;
		m_pReadPos = m_pBuffer;
		m_pWritePos = m_pBuffer + nDataLen;
		m_nBufferSize = nBufferSize;
	}
	catch (...)
	{
		FUNCTION_EXIT;
		return false;
	}
	
	getRemainDataLength();
	FUNCTION_EXIT;
	return true;
}

void CDataBuffer::prepareSize( int nSize )
{
	FUNCTION_ENTRY("prepareSize");

	if (this->getBlankSize() < nSize)
	{
		this->resize(this->getRemainDataLength() + nSize + DEF_INT_BUFFER_LEN_ADDEX);
	}
	FUNCTION_EXIT;
}
void CDataBuffer::print()
{
	FUNCTION_ENTRY("print");
	char* pPrintPos = NULL;
	pPrintPos = m_pReadPos;
	if (getRemainDataLength() <= 0)
	{
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "[BufferData=%s]", "");
		FUNCTION_EXIT;
		return;
	}
	
	LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "[BufferData=");
	for (pPrintPos = m_pReadPos; pPrintPos < m_pWritePos; pPrintPos++)
	{
		char chPos = *pPrintPos;
		if (0x20 <= chPos && chPos <= 0x7E)	  //32 <=chpso <= 126
		{
			LOGMORE(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "%c", chPos);
		}
		else
		{
			LOGMORE(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "%02X", chPos);
		}
		
	}
	LOGMORE(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "]");
	FUNCTION_EXIT;
}


NS_END(TA_Base_App)





