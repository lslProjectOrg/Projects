#include "ServerConnectionManager.h"
#include "UtilityFun.h"
#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/synchronisation/src/ThreadGuard.h"
#include "ServerRequestMonitor.h"
#include "ConnectionActor.h"

using namespace TA_Base_Core;
NS_BEGIN(TA_Base_App)




CServerConnectionManager* CServerConnectionManager::m_pInstance = 0;
TA_Base_Core::NonReEntrantThreadLockable CServerConnectionManager::m_instanceLock;


CServerConnectionManager& CServerConnectionManager::getInstance()
{
	FUNCTION_ENTRY("getInstance");
	TA_THREADGUARD(m_instanceLock);
	if (NULL == m_pInstance)
	{
		m_pInstance = new CServerConnectionManager();
		m_pInstance->start();
	}

	FUNCTION_EXIT;
	return (*m_pInstance);
}

void CServerConnectionManager::removeInstance()
{
	FUNCTION_ENTRY("removeInstance");

	if (NULL != m_pInstance)
	{
		m_pInstance->terminateAndWait();
		DEF_DELETE(m_pInstance);
	}
	FUNCTION_EXIT;
}



CServerConnectionManager::CServerConnectionManager(void)
	: m_semaphore(0)
{
	FUNCTION_ENTRY("CServerConnectionListener");

	m_nThreadJobState = Job_State_Begin;
	m_toTerminate = false;
	m_pListConActor = NULL;
	m_nListConActorSize = 0;
	_InitListActor();

	FUNCTION_EXIT;
}

CServerConnectionManager::~CServerConnectionManager(void)
{	
	FUNCTION_ENTRY("~CServerConnectionListener");

	_UnInitListActor();

	FUNCTION_EXIT;
}

void CServerConnectionManager::run()
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
void CServerConnectionManager::terminate()
{
	FUNCTION_ENTRY("terminate");

	m_toTerminate = true; 
	m_semaphore.post();
	//LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "end CTCPServer::terminate()");
	FUNCTION_EXIT;
}

int CServerConnectionManager::_ThreadJob()
{
	FUNCTION_ENTRY("_ThreadJob");
	int nFunRes = 0;
	
	switch (m_nThreadJobState)
	{
	case Job_State_Begin:
		CServerRequestMonitor::getInstance();
		m_nThreadJobState = Job_State_WaitConActor;
		break;
	case Job_State_WaitConActor:
		_Process_WaitConActor();
		break;
	case Job_State_ProcessConActor:		
		_Process_ProcessConActor();
		break;
	case Job_State_End:
		CServerRequestMonitor::removeInstance();
		m_semaphore.wait();
		break;		
	default:
		TA_Base_Core::Thread::sleep(DEF_INT_LongMonitorThreadSleep);
		break;		
	}  //switch

	FUNCTION_EXIT;
	return nFunRes;
}

int CServerConnectionManager::_ProcessUserTerminate()
{
	FUNCTION_ENTRY("_ProcessUserTerminate");
	int nFunRes = 0;
	//user termin
	m_nThreadJobState = Job_State_End;
	CServerRequestMonitor::removeInstance();

	  
	FUNCTION_EXIT;
	return nFunRes;
}

bool CServerConnectionManager::isFinishWork()
{
	FUNCTION_ENTRY("isFinishWork");

	bool bIsFinishWork = false;
	if (Job_State_End == m_nThreadJobState)
	{
		bIsFinishWork = true;
	}
	FUNCTION_EXIT;
	return bIsFinishWork;
	

}

void  CServerConnectionManager::addActor(CConnectionActor* pActor)
{
	FUNCTION_ENTRY("addActor");

	TA_THREADGUARD(m_LockListConActor);
	m_pListConActor->push_back(pActor);
	m_nListConActorSize = m_pListConActor->size();
	m_semaphore.post();
	FUNCTION_EXIT;
}

void CServerConnectionManager::_InitListActor()
{	
	FUNCTION_ENTRY("_InitListActor");

	TA_THREADGUARD(m_LockListConActor);
	m_pListConActor = new std::list<CConnectionActor*>();
	m_pListConActor->clear();
	m_nListConActorSize = 0;
	FUNCTION_EXIT;

}

void CServerConnectionManager::_UnInitListActor()
{	
	FUNCTION_ENTRY("_UnInitListActor");

	TA_THREADGUARD(m_LockListConActor);
	CConnectionActor* pActor = NULL;
	std::list<CConnectionActor*>::iterator iterList;
	iterList = m_pListConActor->begin();
	while (iterList != m_pListConActor->end())
	{
		pActor = (*iterList);
		DEF_DELETE(pActor);
		iterList++;
	}
	m_pListConActor->clear();
	DEF_DELETE(m_pListConActor);
	m_nListConActorSize = 0;
	FUNCTION_EXIT;

}


int	CServerConnectionManager::_Process_WaitConActor()
{
	FUNCTION_ENTRY("_Process_WaitRequest");
	int nFunRes =0;

	{
		TA_THREADGUARD(m_LockListConActor);
		m_nListConActorSize = 0;
		m_nListConActorSize = m_pListConActor->size();
	}

	if (m_nListConActorSize == 0)
	{
		m_semaphore.wait();//wait  addActor post
	}
	m_nThreadJobState = Job_State_ProcessConActor;

	FUNCTION_EXIT;
	return nFunRes;
}

int CServerConnectionManager::_Process_ProcessConActor()
{
	FUNCTION_ENTRY("_Process_ProcessConActor");
	int nFunRes = 0;
	TA_THREADGUARD(m_LockListConActor);
	std::list<CConnectionActor*>::iterator iterList =  m_pListConActor->begin();
	CConnectionActor* pActor = NULL;
	while (iterList != m_pListConActor->end())
	{
		pActor = NULL;
		pActor = (*iterList);
		
		//check TCP status
		if (false == pActor->isHealth())
		{
			DEF_DELETE(pActor);
		}

		//add to Request monitor
		CServerRequestMonitor::getInstance().addActor(pActor);
		
		iterList++;
	}
	m_pListConActor->clear();

	m_nThreadJobState = Job_State_WaitConActor;

	FUNCTION_EXIT;
	return nFunRes;
}



NS_END(TA_Base_App)




