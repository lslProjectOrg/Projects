
#include "ConnectionActor.h"
#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/synchronisation/src/ThreadGuard.h"
#include "RequestManager.h"
#include "ComWorker.h"

NS_BEGIN(TA_Base_App)



CConnectionActor::CConnectionActor()
{
	FUNCTION_ENTRY("CConnectionActor");
	m_nActorState = Actor_State_Begin;
	m_pSocket = NULL;
	m_pComWorker = NULL;
	m_bIsHealth = true;
	m_nActorState = Actor_State_WaitTCPHandle;
	FUNCTION_EXIT;
}

CConnectionActor::~CConnectionActor()
{
	FUNCTION_ENTRY("~CConnectionActor");
	DEF_DELETE(m_pSocket);
	DEF_DELETE(m_pComWorker);

	FUNCTION_EXIT;

}

void CConnectionActor::executeWorkItem()
{
	FUNCTION_ENTRY("executeWorkItem");
	setRequestStatusProcessing();

	//processing request
	CComWorker::lstFrameBufferConT ListFrameBuffer;
	m_pComWorker->getListRecvFrame(ListFrameBuffer);

	CDataBuffer* pFrameBuffer = NULL;
	CComWorker::lstFrameBufferConIterT iterLst;
	iterLst = ListFrameBuffer.begin();
	while (iterLst != ListFrameBuffer.end())
	{
		pFrameBuffer = (*iterLst);
		pFrameBuffer->print();
		DEF_DELETE(pFrameBuffer);
		iterLst++;
	}
	ListFrameBuffer.clear();

	setRequestStatusProcessed();
	CRequestManager::getInstance().removeActor(this);
	FUNCTION_EXIT;
}

void CConnectionActor::setSocketHandle( TA_Base_Core::TcpSocket* pSocket )
{
	FUNCTION_ENTRY("setSocketHandle");
	TA_THREADGUARD(m_lockActorState);
	m_pSocket = pSocket;
	m_pComWorker = new CComWorker(m_pSocket);
	m_nActorState = Actor_State_GetTCPHandle;
	m_nActorState = Actor_State_WaitData;
	m_bIsHealth =  m_pComWorker->isStillConnected();
	FUNCTION_EXIT;
}

TA_Base_Core::TcpSocket* CConnectionActor::getSocketHandle()
{
	FUNCTION_ENTRY("getSocketHandle");

	FUNCTION_EXIT;
	return m_pSocket;
}


int CConnectionActor::recvData(bool bCanRead)
{
	FUNCTION_ENTRY("recvData");
	int nFunRes = 0;
	TA_THREADGUARD(m_lockActorState);
	m_nActorState = Actor_State_RecvingData;
	nFunRes = m_pComWorker->readData();
	if (0 != nFunRes)
	{
		m_bIsHealth = false;
	}
	FUNCTION_EXIT;
	return nFunRes;
}

int CConnectionActor::getRequestCount()
{
	FUNCTION_ENTRY("getRequestCount");
	int nFunRes = 0;
	{
		TA_THREADGUARD(m_lockActorState);
		m_nActorState = Actor_State_ProcessDataToRequest;
	}

	//check frame
	m_pComWorker->analysisData();

	nFunRes = m_pComWorker->getRecvFrameCount();
	if (nFunRes > 0)
	{
		setRequestStatusNew();
	}

	FUNCTION_EXIT;
	return nFunRes;
}



bool CConnectionActor::isFinishProcessedRequest()
{
	FUNCTION_ENTRY("isFinishProcessedRequest");
	bool nFunRes = false;

	TA_THREADGUARD(m_lockActorState);
	if (Actor_State_FinishProcessRequest == m_nActorState)
	{
		nFunRes = true;
	}

	FUNCTION_EXIT;
	return nFunRes;
}
int CConnectionActor::setRequestStatusNew()
{
	FUNCTION_ENTRY("resetJobStatus");
	int nFunRes = 0;

	TA_THREADGUARD(m_lockActorState);
	m_nActorState = Actor_State_NewRequest;


	FUNCTION_EXIT;
	return nFunRes;
}

int CConnectionActor::setRequestStatusProcessing()
{
	FUNCTION_ENTRY("setRequestStatusProcessing");
	int nFunRes = 0;

	TA_THREADGUARD(m_lockActorState);
	m_nActorState = Actor_State_ProcessingRequest;


	FUNCTION_EXIT;
	return nFunRes;
}

int CConnectionActor::setRequestStatusProcessed()
{
	FUNCTION_ENTRY("setRequestStatusProcessed");
	int nFunRes = 0;

	TA_THREADGUARD(m_lockActorState);
	m_nActorState = Actor_State_FinishProcessRequest;


	FUNCTION_EXIT;
	return nFunRes;
}

bool CConnectionActor::isHealth()
{
	FUNCTION_ENTRY("isHealth");
	if (m_bIsHealth)
	{
		m_bIsHealth = m_pComWorker->isStillConnected();
	}
	FUNCTION_EXIT;
	return m_bIsHealth;
}






NS_END(TA_Base_App)





