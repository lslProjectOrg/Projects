#include "ComWorker.h"

#include "CommonData.h"

#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/synchronisation/src/ThreadGuard.h"


NS_BEGIN(TA_Base_App)

   

CComWorker::CComWorker(TA_Base_Core::TcpSocket* pSocketRef)
{
	FUNCTION_ENTRY("CComWorker");

	m_pSocketRef = NULL;
	m_pSocketRef = pSocketRef;
	m_pRecvBufferOnce = new CDataBuffer(DEF_INT_BUFFER_LEN_READ);
	m_pRecvBufferTotal = new CDataBuffer(DEF_INT_BUFFER_LEN_READ);
	m_pBufferDataTmp = new CDataBuffer(DEF_INT_BUFFER_LEN_READ);
    m_pRecvBufferOnce->clear();
    m_pRecvBufferTotal->clear();
	m_pBufferDataTmp->clear();

	{
		TA_THREADGUARD(m_lockSocketRead);
		m_bIsStillConnected = m_pSocketRef->stillConnected();
	}

    FUNCTION_EXIT;
}

CComWorker::~CComWorker()
{
	FUNCTION_ENTRY("~CComWorker");

	TA_THREADGUARD(m_lockSocketRead);
	TA_THREADGUARD(m_lockSocketWrite);
	m_pSocketRef = NULL;
	DEF_DELETE(m_pRecvBufferOnce);
	DEF_DELETE(m_pRecvBufferTotal);
	DEF_DELETE(m_pBufferDataTmp);
	FUNCTION_EXIT;
}

int CComWorker::readData()
{
	FUNCTION_ENTRY("readData");
	TA_THREADGUARD(m_lockSocketRead);

    int					nFunRes = 0;     
	unsigned int		nGetLength = 0;
	int					nRcvedRes = -1;		

	nGetLength = 0;
	nRcvedRes = -1;

	try
	{  
		m_pRecvBufferOnce->prepareSize(DEF_INT_BUFFER_LEN_READ);
		nRcvedRes = m_pSocketRef->read(m_pRecvBufferOnce->getWritePos(), &nGetLength, m_pRecvBufferOnce->getBlankSize()-1);
		m_pRecvBufferOnce->moveWritePos(nGetLength);
		nFunRes = 0;
	}
	catch (...)
	{
		m_bIsStillConnected = false;
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Socket read Data Error");
		m_pRecvBufferOnce->clear();
		nFunRes = -1;
		FUNCTION_EXIT;
		return nFunRes;	
	}

	if (nGetLength <= 0 || nRcvedRes != 0)
	{
		m_bIsStillConnected = false;
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, 
			"Read Data Error! GetDatlength = %d, RecvRes = %d", nGetLength, nRcvedRes);
		m_pRecvBufferOnce->clear();
		nFunRes = -1;
		FUNCTION_EXIT;
		return nFunRes;	
	}

	//copy data
	{
		TA_THREADGUARD(m_lockBufferDataTmp);
		m_pBufferDataTmp->prepareSize(m_pRecvBufferOnce->getRemainDataLength());
		m_pBufferDataTmp->write(m_pRecvBufferOnce->getReadPos(), m_pRecvBufferOnce->getRemainDataLength());
		//m_pRecvBufferOnce->moveReadPos(m_pRecvBufferOnce->getRemainDataLength());
		m_pRecvBufferOnce->clear();		
	}
	nFunRes = 0;
	FUNCTION_EXIT;	  
	return nFunRes;
}

bool CComWorker::_HaveFrameInBuffer(CDataBuffer* pRecvBufferTotal)
{
	FUNCTION_ENTRY("_HaveFrameInBuffer");

	bool bHaveFrame = false;

	char* pszFrameEnd = "\r\n";
	char* pPosFrameEnd = NULL;
	int nFrameSize = 0;

	pPosFrameEnd = NULL;
	nFrameSize = 0;
	pPosFrameEnd = strstr(pRecvBufferTotal->getReadPos(), pszFrameEnd);//find frame end pos
	if (NULL != pPosFrameEnd)
	{
		pPosFrameEnd += strlen(pszFrameEnd);
		nFrameSize = (pPosFrameEnd - pRecvBufferTotal->getReadPos());//frame end pos is in data area
	}

	bHaveFrame = false;
	if (NULL != pPosFrameEnd && nFrameSize > 0 && nFrameSize <= pRecvBufferTotal->getRemainDataLength())
	{
		bHaveFrame = true;
	}
	FUNCTION_EXIT;
	return bHaveFrame;
}

int CComWorker::_GetFrameFromBuffer(CDataBuffer* pRecvBufferTotal, lstFrameBufferConT& lstRecvFrame)
{
	FUNCTION_ENTRY("_GetFrameFromBuffer");

	int nFunRes = 0;

	char* pszFrameEnd = "\r\n";
	char* pPosFrameEnd = NULL;
	int nFrameSize = 0;
	CDataBuffer* pFrameBuff = NULL;

	pPosFrameEnd = NULL;
	nFrameSize = 0;
	pPosFrameEnd = strstr(pRecvBufferTotal->getReadPos(), pszFrameEnd);//find frame end pos
	if (NULL != pPosFrameEnd)
	{
		pPosFrameEnd += strlen(pszFrameEnd);
		nFrameSize = (pPosFrameEnd - pRecvBufferTotal->getReadPos());//frame end pos is in data area
	}
	else
	{
		nFunRes = -1;
		FUNCTION_EXIT;
		return nFunRes;
	}

	if (nFrameSize > 0 && nFrameSize <= pRecvBufferTotal->getRemainDataLength())
	{
		pFrameBuff = new CDataBuffer(nFrameSize + DEF_INT_BUFFER_LEN_READ);
		pRecvBufferTotal->read(pFrameBuff->getWritePos(), nFrameSize);
		pFrameBuff->moveWritePos(nFrameSize);
		pRecvBufferTotal->trim();
		lstRecvFrame.push_back(pFrameBuff);

		pFrameBuff = NULL;
		nFunRes = 0;
	}
	else
	{
		nFunRes = -1;
	}
	FUNCTION_EXIT;
	return nFunRes;
}


int CComWorker::analysisData()
{
	FUNCTION_ENTRY("analysisAndProcessData");

	int nFunRes = 0;
	bool bConninueProceeData = true;
	bool bHaveFrame = false;


	//copy data
	{
		TA_THREADGUARD(m_lockBufferDataTmp);
		if (m_pBufferDataTmp->getRemainDataLength() > 0)
		{
			m_pRecvBufferTotal->prepareSize(m_pBufferDataTmp->getRemainDataLength());
			m_pRecvBufferTotal->write(m_pBufferDataTmp->getReadPos(), m_pBufferDataTmp->getRemainDataLength());
			//m_pBufferDataTmp->moveReadPos(m_pBufferDataTmp->getRemainDataLength());
			m_pBufferDataTmp->clear();
		}
	}



	if (m_pRecvBufferTotal->getRemainDataLength() <= 0)
	{
		nFunRes = 0;
		bConninueProceeData = false;
		return nFunRes;
	}

	while (true == bConninueProceeData)
	{
		bHaveFrame = false;
		bHaveFrame = _HaveFrameInBuffer(m_pRecvBufferTotal);
		if (true == bHaveFrame)
		{
			nFunRes = _GetFrameFromBuffer(m_pRecvBufferTotal, m_lstRecvFrame);
		}
		else
		{
			bConninueProceeData = false;
		}
	}//while (true == bConninueProceeData)
	
	FUNCTION_EXIT;
	return nFunRes;
	
}



int CComWorker::getRecvFrameCount()
{
	FUNCTION_ENTRY("getRecvFrameCount");

	int nFunRes = 0;
	nFunRes = m_lstRecvFrame.size();

	FUNCTION_EXIT;
	return nFunRes;
}


int CComWorker::getListRecvFrame(lstFrameBufferConT& LstFrameBuffer)
{
	FUNCTION_ENTRY("getRecvFrameCount");

	int nFunRes = 0;
	CDataBuffer* pFrameBuffer = NULL;
	lstFrameBufferConIterT iterLst;
	LstFrameBuffer.clear();
	iterLst = m_lstRecvFrame.begin();
	while (iterLst != m_lstRecvFrame.end())
	{
		pFrameBuffer = (*iterLst);
		LstFrameBuffer.push_back(pFrameBuffer);
		iterLst++;
	}
	m_lstRecvFrame.clear();

	FUNCTION_EXIT;
	return nFunRes;
}



int CComWorker::writeBuffer(CDataBuffer* pSendBuff)
{
	FUNCTION_ENTRY("writeBuffer");
	TA_THREADGUARD(m_lockSocketWrite);

	int		nSendRes = 0;
	char*	pSendPos = NULL;
	int     nSendLen = 0;


	pSendPos = pSendBuff->getReadPos();
	nSendLen = pSendBuff->getRemainDataLength();

	try
	{		
		nSendRes = m_pSocketRef->write(pSendPos, nSendLen);

		if (nSendRes != nSendLen)
		{
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError,
				"Write Data Error, DataLen = %d, SendDataLen = %d", nSendLen, nSendRes);
			m_bIsStillConnected = false;
			nSendRes = -1;
		}
		else
		{
			//send data ok, return value is 0
			nSendRes = 0;
		}
	}					
	catch (...)
	{
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Socket Write Data Error");
		m_bIsStillConnected = false;
		nSendRes = -1;
	}
	FUNCTION_EXIT;
	return nSendRes;
}


bool CComWorker::isStillConnected()
{
	
	if (m_bIsStillConnected)
	{
		TA_THREADGUARD(m_lockSocketRead);
		m_bIsStillConnected = m_pSocketRef->stillConnected();
	}
	return m_bIsStillConnected;
}

NS_END(TA_Base_App)





