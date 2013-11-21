#include "RequestManager.h"
#include "UtilityFun.h"
#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/threads/src/LFThreadPoolManager.h"
#include "ConnectionActor.h"
#include "ServerConnectionManager.h"

using namespace TA_Base_Core;
NS_BEGIN(TA_Base_App)




CRequestManager* CRequestManager::m_pInstance = 0;
TA_Base_Core::NonReEntrantThreadLockable CRequestManager::m_instanceLock;


CRequestManager& CRequestManager::getInstance()
{
	FUNCTION_ENTRY("getInstance");
	TA_THREADGUARD(m_instanceLock);
	if (NULL == m_pInstance)
	{
		m_pInstance = new CRequestManager();
		m_pInstance->start();

	}

	FUNCTION_EXIT;
	return (*m_pInstance);
}

void CRequestManager::removeInstance()
{
	FUNCTION_ENTRY("removeInstance");
	if (NULL != m_pInstance)
	{
		m_pInstance->terminateAndWait();
		DEF_DELETE(m_pInstance);
	}
	FUNCTION_EXIT;
}



CRequestManager::CRequestManager(void)
	: m_semaphore(0)
{
	FUNCTION_ENTRY("CRequestManager");

	m_nThreadJobState = Job_State_Begin;
	m_toTerminate = false;
	m_pListProcessConActor = NULL;
	m_pListMonitorConActor = NULL;
	m_nListConActorSize = 0;

	m_pThreadPoolManager = NULL;
	m_nNumberOfThreads = 2;
	m_bIsAutoRun = true;
	_InitListActor();

	FUNCTION_EXIT;
}

CRequestManager::~CRequestManager(void)
{	
	FUNCTION_ENTRY("~CRequestManager");
	_UnInitListActor();

	FUNCTION_EXIT;
}

void CRequestManager::run()
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
void CRequestManager::terminate()
{
	FUNCTION_ENTRY("terminate");

	m_toTerminate = true; 
	m_semaphore.post();
	
	FUNCTION_EXIT;
}

int CRequestManager::_ThreadJob()
{
	FUNCTION_ENTRY("_ThreadJob");
	int nFunRes = 0;
	
	switch (m_nThreadJobState)
	{
	case Job_State_Begin:
		DEF_DELETE(m_pThreadPoolManager);
		m_pThreadPoolManager = new LFThreadPoolManager(m_nNumberOfThreads, m_bIsAutoRun);
		m_nThreadJobState = Job_State_WaitRequest;
		break;
	case Job_State_WaitRequest:
		_Process_WaitRequest();
		break;
	case Job_State_ProcessRequest:
		_Process_ProcessRequest();
		break;
	case Job_State_MonitorRequest:
		_Process_MonitorRequest();
		break;
	case Job_State_End:
		DEF_DELETE(m_pThreadPoolManager);
		m_semaphore.wait();
		break;		
	default:
		TA_Base_Core::Thread::sleep(DEF_INT_MonitorThreadSleep);
		break;		
	}  //switch

	FUNCTION_EXIT;
	return nFunRes;
}

int CRequestManager::_ProcessUserTerminate()
{
	FUNCTION_ENTRY("_ProcessUserTerminate");
	int nFunRes = 0;
	//user termin
	m_nThreadJobState = Job_State_End;
	DEF_DELETE(m_pThreadPoolManager);

	  
	FUNCTION_EXIT;
	return nFunRes;
}

bool CRequestManager::isFinishWork()
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



void CRequestManager::addActor( CConnectionActor* pActor )
{
	FUNCTION_ENTRY("addActor");
	TA_THREADGUARD(m_LockListConActor);
	m_pListProcessConActor->push_back(pActor);
	m_nListConActorSize = m_pListProcessConActor->size();
	m_semaphore.post();
	FUNCTION_EXIT;
}


void CRequestManager::removeActor( CConnectionActor* pActor )
{
	FUNCTION_ENTRY("addActor");
	TA_THREADGUARD(m_LockListConActor);
	m_semaphore.post();
	FUNCTION_EXIT;
}



void CRequestManager::_InitListActor()
{	
	FUNCTION_ENTRY("_InitListActor");

	TA_THREADGUARD(m_LockListConActor);
	m_pListProcessConActor = new std::list<CConnectionActor*>();
	m_pListProcessConActor->clear();
	m_nListConActorSize = 0;
	m_pListMonitorConActor = new std::list<CConnectionActor*>();
	m_pListMonitorConActor->clear();
	FUNCTION_EXIT;

}

void CRequestManager::_UnInitListActor()
{	
	FUNCTION_ENTRY("_UnInitListActor");

	TA_THREADGUARD(m_LockListConActor);

	CConnectionActor* pActor = NULL;
	std::list<CConnectionActor*>::iterator iterList;
	iterList = m_pListMonitorConActor->begin();
	while (iterList != m_pListMonitorConActor->end())
	{
		pActor = (*iterList);
		DEF_DELETE(pActor);
		iterList++;
	}
	m_pListMonitorConActor->clear();
	DEF_DELETE(m_pListMonitorConActor);
	m_nListConActorSize = 0;


	iterList = m_pListProcessConActor->begin();
	while (iterList != m_pListProcessConActor->end())
	{
		pActor = (*iterList);
		DEF_DELETE(pActor);
		iterList++;
	}
	m_pListProcessConActor->clear();
	DEF_DELETE(m_pListProcessConActor);
	FUNCTION_EXIT;

}


int	CRequestManager::_Process_WaitRequest()
{
	FUNCTION_ENTRY("_Process_WaitRequest");
	int nFunRes =0;	
	
	m_nThreadJobState = Job_State_ProcessRequest;

	FUNCTION_EXIT;
	return nFunRes;
}
int	CRequestManager::_Process_ProcessRequest()
{
	FUNCTION_ENTRY("_Process_ProcessRequest");
	int nFunRes =0;

	{
		TA_THREADGUARD(m_LockListConActor);
		m_nListConActorSize = 0;
		m_nListConActorSize = m_pListProcessConActor->size();
	}
	if (m_nListConActorSize == 0)
	{
		m_semaphore.wait();//wait addActor() post or removeActor  post
	}

	{
		TA_THREADGUARD(m_LockListConActor);
		TA_Base_Core::IThreadPoolExecuteItem* pItem = NULL;
		CConnectionActor* pActor = NULL;
		std::list<CConnectionActor*>::iterator iterList = m_pListProcessConActor->begin();
		while (iterList != m_pListProcessConActor->end())
		{
			pActor = (*iterList);
			pItem = (TA_Base_Core::IThreadPoolExecuteItem*)(*iterList);
			m_pThreadPoolManager->queueWorkItem(pItem);
			m_pListMonitorConActor->push_back(pActor);
			iterList++;
		}//while
	}

	m_nThreadJobState = Job_State_MonitorRequest;

	FUNCTION_EXIT;
	return nFunRes;
}
int	CRequestManager::_Process_MonitorRequest()
{
	FUNCTION_ENTRY("_Process_MonitorRequest");
	int nFunRes =0;

	{
		TA_THREADGUARD(m_LockListConActor);
		TA_Base_Core::IThreadPoolExecuteItem* pItem = NULL;
		CConnectionActor* pActor = NULL;
		std::list<CConnectionActor*>::iterator iterList = m_pListMonitorConActor->begin();

		while (iterList != m_pListMonitorConActor->end())
		{
			pActor = NULL;
			pItem = NULL;
			pActor = (*iterList);
			pItem = (TA_Base_Core::IThreadPoolExecuteItem*)(*iterList);

			if (true == pActor->isFinishProcessedRequest())
			{
				m_pThreadPoolManager->removeWorkItem(pItem);
				CServerConnectionManager::getInstance().addActor(pActor);
				pActor = NULL;
				pItem = NULL;
				m_pListMonitorConActor->erase(iterList);
				iterList = m_pListMonitorConActor->begin();
			}
			else
			{
				iterList++;
			}	
		}//while
	}

	
	
	m_nThreadJobState = Job_State_ProcessRequest;

	FUNCTION_EXIT;
	return nFunRes;
}


NS_END(TA_Base_App)




