#include "RequestManager.h"
#include "UtilityFun.h"
#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/DebugUtil.h"

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
	}

	FUNCTION_EXIT;
	return (*m_pInstance);
}

void CRequestManager::removeInstance()
{
	FUNCTION_ENTRY("removeInstance");

	DEF_DELETE(m_pInstance);
	FUNCTION_EXIT;
}



CRequestManager::CRequestManager(void)
{
	FUNCTION_ENTRY("CServerConnectionListener");

	m_nThreadJobState = Job_State_Begin;
	m_toTerminate = false;


	FUNCTION_EXIT;
}

CRequestManager::~CRequestManager(void)
{	
	FUNCTION_ENTRY("~CServerConnectionListener");
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
	   //Thread::sleep(DEF_INT_NormalThreadSleep);	//m_SetTCPServerSocket.waitForIO(1000)	
   }

   _ProcessUserTerminate();

   FUNCTION_EXIT;
}	
void CRequestManager::terminate()
{
	FUNCTION_ENTRY("terminate");

	m_toTerminate = true; 
	//LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "end CTCPServer::terminate()");
	FUNCTION_EXIT;
}

int CRequestManager::_ThreadJob()
{
	FUNCTION_ENTRY("_ThreadJob");
	int nFunRes = 0;
	
	switch (m_nThreadJobState)
	{
	case Job_State_Begin:
		break;

	case Job_State_End:
		Thread::sleep(DEF_INT_NormalThreadSleep);
		break;		
	default:
		Thread::sleep(DEF_INT_NormalThreadSleep);
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


NS_END(TA_Base_App)




