#include "ServerRequestMonitor.h"
#include "UtilityFun.h"
#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/synchronisation/src/ThreadGuard.h"

#include "ConnectionActor.h"
#include "RequestManager.h"
#include "ServerConnectionManager.h"

using namespace TA_Base_Core;
NS_BEGIN(TA_Base_App)




CServerRequestMonitor* CServerRequestMonitor::m_pInstance = 0;
TA_Base_Core::NonReEntrantThreadLockable CServerRequestMonitor::m_instanceLock;


CServerRequestMonitor& CServerRequestMonitor::getInstance()
{
	FUNCTION_ENTRY("getInstance");
	TA_THREADGUARD(m_instanceLock);
	if (NULL == m_pInstance)
	{
		m_pInstance = new CServerRequestMonitor();
		m_pInstance->start();
	}

	FUNCTION_EXIT;
	return (*m_pInstance);
}

void CServerRequestMonitor::removeInstance()
{
	FUNCTION_ENTRY("removeInstance");
	if (NULL != m_pInstance)
	{
		m_pInstance->terminateAndWait();
		DEF_DELETE(m_pInstance);
	}
	FUNCTION_EXIT;
}



CServerRequestMonitor::CServerRequestMonitor(void)
	: m_semaphore(0)
{
	FUNCTION_ENTRY("CServerRequestMonitor");

	m_nThreadJobState = Job_State_Begin;
	m_toTerminate = false;
	m_pMapSocketConActor = NULL;
	m_nListConActorSize = 0;

	m_bTCPRead = true;
	m_bTCPWrite = true; 
	m_nTCPSelectRes = 0; 
	m_timeTCPWait = DEF_INT_LongMonitorThreadSleep;

	_IniMap();

	FUNCTION_EXIT;
}

CServerRequestMonitor::~CServerRequestMonitor(void)
{	
	FUNCTION_ENTRY("~CServerRequestMonitor");
	_UnInitMap();
	m_pMapSocketConActor = NULL;
	m_nListConActorSize = 0;
	FUNCTION_EXIT;
}

void CServerRequestMonitor::run()
{
   FUNCTION_ENTRY("run");
   int nFunRes = 0;
   
   m_nThreadJobState = Job_State_Begin;
   LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug,	"m_nThreadJobState = Job_State_Begin");	


   while (false == m_toTerminate)
   {
	   _ThreadJob();
   }

   _ProcessUserTerminate();

   FUNCTION_EXIT;
}	
void CServerRequestMonitor::terminate()
{
	FUNCTION_ENTRY("terminate");

	m_toTerminate = true; 
	m_semaphore.post();

	FUNCTION_EXIT;
}

int CServerRequestMonitor::_ThreadJob()
{
	FUNCTION_ENTRY("_ThreadJob");
	int nFunRes = 0;
	
	switch (m_nThreadJobState)
	{
	case Job_State_Begin:
		CRequestManager::getInstance();
		m_nThreadJobState = Job_State_waitConActor;
		break;
	case Job_State_waitConActor:
		m_semaphore.wait();
		m_nThreadJobState = Job_State_MonitorData;
		break;
	case Job_State_MonitorData:
		_Process_MonitorData();
		break;
	case Job_State_MonitorRequest:
		_Process_MonitorRequest();
		break;
	case Job_State_End:
		CRequestManager::removeInstance();
		m_semaphore.wait();
		break;		
	default:
		TA_Base_Core::Thread::sleep(DEF_INT_LongMonitorThreadSleep);
		break;		
	}  //switch

	FUNCTION_EXIT;
	return nFunRes;
}

int CServerRequestMonitor::_ProcessUserTerminate()
{
	FUNCTION_ENTRY("_ProcessUserTerminate");
	int nFunRes = 0;
	//user termin
	m_nThreadJobState = Job_State_End;
	
	CRequestManager::removeInstance();
	FUNCTION_EXIT;
	return nFunRes;
}

bool CServerRequestMonitor::isFinishWork()
{
	FUNCTION_ENTRY("isFinishWork");

	bool bIsFinishWork = false;
	if (Job_State_End == m_nThreadJobState)
	{
		bIsFinishWork = true;
	}
	return bIsFinishWork;
	FUNCTION_EXIT;

}


int CServerRequestMonitor::_Process_MonitorData()
{
	FUNCTION_ENTRY("_Process_MonitorConActor");
	TA_THREADGUARD(m_LockMapSocketConActor);

	int nFunRes = 0;
	TA_Base_Core::TcpSocket* pTcpSocket = NULL;
	MapSocketConActorIterT iterMap;
	CConnectionActor* pActor = NULL;
	//m_nListConActorSize = m_pSetSocket.getSize();
	if (m_nListConActorSize <= 0)
	{
		m_nThreadJobState = Job_State_waitConActor;
		FUNCTION_EXIT;
		return nFunRes;
	}


	m_bTCPRead = true;
	m_bTCPWrite = false; 
	m_nTCPSelectRes = -1; 
	m_nTCPSelectRes = m_pSetSocket.waitForIO( m_bTCPRead, m_bTCPWrite, m_timeTCPWait );//select()
		
	for (int nIndex = 0; nIndex < m_nListConActorSize; nIndex++)
	{
		m_bTCPRead = false;
		m_bTCPWrite = false; 
		pTcpSocket = NULL;
		pTcpSocket = m_pSetSocket.getSocket(0, m_bTCPRead, m_bTCPWrite );

		if ( m_bTCPRead  && NULL != pTcpSocket)
		{
			iterMap = m_pMapSocketConActor->find(pTcpSocket);
			if (iterMap != m_pMapSocketConActor->end())
			{
				pActor = (*iterMap).second;
				pActor->recvData(m_bTCPRead);
			}
		}
	}


	m_nThreadJobState = Job_State_MonitorRequest;

	FUNCTION_EXIT;
	return nFunRes;
}



int CServerRequestMonitor::_Process_MonitorRequest()
{
	FUNCTION_ENTRY("_Process_MonitorConActor");
	TA_THREADGUARD(m_LockMapSocketConActor);

	int nFunRes = 0;
	MapSocketConActorIterT iterMap;
	TA_Base_Core::TcpSocket* pTcpSocket = NULL;
	CConnectionActor* pActor = NULL;
	//m_nListConActorSize = m_pMapSocketConActor->size();
	if (m_nListConActorSize <= 0)
	{
		m_nThreadJobState = Job_State_waitConActor;
		FUNCTION_EXIT;
		return nFunRes;
	}


	iterMap = m_pMapSocketConActor->begin();
	while (iterMap != m_pMapSocketConActor->end())
	{
		pTcpSocket = (*iterMap).first;
		pActor = (*iterMap).second;

		if (false == pActor->isHealth())
		{
			//remove from Monitor SokertSet
			m_pSetSocket.removeSocket(pTcpSocket);
			pTcpSocket = NULL;

			m_pMapSocketConActor->erase(iterMap);
			iterMap = m_pMapSocketConActor->begin();

			m_nListConActorSize = m_pMapSocketConActor->size();

			CServerConnectionManager::getInstance().addActor(pActor);
			pActor = NULL;
		}
		else if (pActor->getRequestCount() > 0 )
		{
			//remove from Monitor SokertSet
			m_pSetSocket.removeSocket(pTcpSocket);
			//
			CRequestManager::getInstance().addActor(pActor);
			pTcpSocket = NULL;
			m_pMapSocketConActor->erase(iterMap);
			iterMap = m_pMapSocketConActor->begin();
			m_nListConActorSize = m_pMapSocketConActor->size();
		}
		else
		{
			iterMap++;
		}
		
	}//while



	m_nThreadJobState = Job_State_MonitorData;

	FUNCTION_EXIT;
	return nFunRes;
}

void CServerRequestMonitor::addActor( CConnectionActor* pActor )
{
	FUNCTION_ENTRY("addActor");
	if (NULL == pActor)
	{
		FUNCTION_EXIT;
		return;
	}
	TA_THREADGUARD(m_LockMapSocketConActor);
	TA_Base_Core::TcpSocket* pTcpSocket = NULL;
	pTcpSocket = pActor->getSocketHandle();
	if (NULL == pTcpSocket)
	{
		FUNCTION_EXIT;
		return;
	}
	m_pMapSocketConActor->insert(MapSocketConActorValueTypeT(pTcpSocket, pActor));
	m_nListConActorSize = m_pMapSocketConActor->size();
	m_pSetSocket.addSocket(pTcpSocket);
	pTcpSocket = NULL;
	m_semaphore.post();
	FUNCTION_EXIT;
}


void CServerRequestMonitor::_IniMap()
{	
	FUNCTION_ENTRY("_InitListActor");

	TA_THREADGUARD(m_LockMapSocketConActor);
	m_pMapSocketConActor = new MapSocketConActorT();
	m_pMapSocketConActor->clear();
	m_nListConActorSize = 0;

	
	FUNCTION_EXIT;

}

void CServerRequestMonitor::_UnInitMap()
{	
	FUNCTION_ENTRY("_UnInitListActor");

	TA_THREADGUARD(m_LockMapSocketConActor);
	CConnectionActor* pActor = NULL;
	MapSocketConActorIterT iterMap;
	iterMap = m_pMapSocketConActor->begin();
	while (iterMap != m_pMapSocketConActor->end())
	{
		pActor = (*iterMap).second;
		DEF_DELETE(pActor);
		iterMap++;
	}
	m_pMapSocketConActor->clear();
	DEF_DELETE(m_pMapSocketConActor);
	m_nListConActorSize = 0;



	FUNCTION_EXIT;

}


NS_END(TA_Base_App)




