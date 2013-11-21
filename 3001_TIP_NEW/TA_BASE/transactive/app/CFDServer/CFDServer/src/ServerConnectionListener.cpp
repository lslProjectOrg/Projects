#include "ServerConnectionListener.h"
#include "UtilityFun.h"
#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/DebugUtil.h"
#include "ConnectionActor.h"
#include "ServerConnectionManager.h"

using namespace TA_Base_Core;
NS_BEGIN(TA_Base_App)

const std::string CServerConnectionListener::STRDEFAULTSOCKETPORT = "2005";

CServerConnectionListener* CServerConnectionListener::m_pInstance = 0;
TA_Base_Core::NonReEntrantThreadLockable CServerConnectionListener::m_instanceLock;


CServerConnectionListener& CServerConnectionListener::getInstance()
{
	FUNCTION_ENTRY("getInstance");
	TA_THREADGUARD(m_instanceLock);
	if (NULL == m_pInstance)
	{
		m_pInstance = new CServerConnectionListener();
		m_pInstance->start();
	}

	FUNCTION_EXIT;
	return (*m_pInstance);
}

void CServerConnectionListener::removeInstance()
{
	FUNCTION_ENTRY("removeInstance");

	if (NULL != m_pInstance)
	{
		m_pInstance->terminateAndWait();
		DEF_DELETE(m_pInstance);
	}

	FUNCTION_EXIT;

}



CServerConnectionListener::CServerConnectionListener(void)
	:m_semaphore(0)
{
	FUNCTION_ENTRY("CServerConnectionListener");
	
	{
		TA_THREADGUARD(m_LockSocketPort);
		m_strListenSocketPort.clear();
		m_nListenSocketPort = 0;
	}

	m_nThreadJobState = Job_State_Begin;
	m_toTerminate = false;
	m_pTCPServerSocket = NULL;//server listen socket
	m_bTCPRead = true;
	m_bTCPWrite = true; 
	m_nTCPSelectRes = 0; 
	m_timeTCPWait = DEF_INT_MonitorThreadSleep;

	FUNCTION_EXIT;
}

CServerConnectionListener::~CServerConnectionListener(void)
{	
	FUNCTION_ENTRY("~CServerConnectionListener");
	FUNCTION_EXIT;
}

void CServerConnectionListener::run()
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
void CServerConnectionListener::terminate()
{
	FUNCTION_ENTRY("terminate");

	m_toTerminate = true; 
	m_semaphore.post();
	//LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "end CTCPServer::terminate()");
	FUNCTION_EXIT;
}

int CServerConnectionListener::_ThreadJob()
{
	FUNCTION_ENTRY("_ThreadJob");
	int nFunRes = 0;
	
	switch (m_nThreadJobState)
	{
	case Job_State_Begin:
		CServerConnectionManager::getInstance();
		m_nThreadJobState = Job_State_WaitListenPort;
		break;
	case Job_State_WaitListenPort:
		nFunRes = _Process_WaitListenPort();
		break;
	case Job_State_GetListenPort:
		nFunRes = _Process_GetListenPort();
		break;
	case Job_State_CreateListenSocket:
		nFunRes = _Process_CreateListenSocket();
		break;
	case Job_State_WaitForNewClient:
		nFunRes = _Process_WaitForNewClient();
		break;
	case Job_State_End:
		CServerConnectionManager::removeInstance();
		m_semaphore.wait();
		break;		
	default:
		TA_Base_Core::Thread::sleep(DEF_INT_MonitorThreadSleep);
		break;		
	}  //switch

	FUNCTION_EXIT;
	return nFunRes;
}

int CServerConnectionListener::_ProcessUserTerminate()
{
	FUNCTION_ENTRY("_ProcessUserTerminate");

	int nFunRes = 0;


	if (NULL != m_pTCPServerSocket)
	{
		m_SetTCPServerSocket.removeSocket(m_pTCPServerSocket);
		DEF_DELETE(m_pTCPServerSocket);		
	}

	CServerConnectionManager::removeInstance();

	//user termin
	m_nThreadJobState = Job_State_End;

	  
	FUNCTION_EXIT;
	return nFunRes;
}

bool CServerConnectionListener::isFinishWork()
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


int CServerConnectionListener::setListenPort(const std::string& strListenPort)
{
	FUNCTION_ENTRY("setListenPort");

	TA_THREADGUARD(m_LockSocketPort);
	int nFunRes = 0;
	m_strListenSocketPort = strListenPort;
	m_semaphore.post();
	FUNCTION_EXIT;
	return nFunRes;
}


int CServerConnectionListener::_Process_WaitListenPort()
{
	FUNCTION_ENTRY("_Process_WaitListenPort");
	int nFunRes = 0;
	std::string strPort;

	{
		TA_THREADGUARD(m_LockSocketPort);
		strPort = m_strListenSocketPort;
	}

	if (strPort.empty())
	{
		m_semaphore.wait();
	}

	m_nThreadJobState = Job_State_GetListenPort;
	return nFunRes;
}


int CServerConnectionListener::_Process_GetListenPort()
{
	FUNCTION_ENTRY("_Process_CheckParam");
	int nFunRes = 0;
	std::string strPort;

	{
		TA_THREADGUARD(m_LockSocketPort);
		strPort = m_strListenSocketPort;
	}

	if (strPort.empty())
	{
		m_nThreadJobState = Job_State_WaitForNewClient;
		return nFunRes;
	}

	m_nListenSocketPort = 0;
	m_nListenSocketPort = CUtilityFun::getInstance().mysys_str2Int32(strPort);
	if (0 == m_nListenSocketPort || m_nListenSocketPort > INT_MAX/2)
	{
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError,
			"ListenSocketPort error! strListenSocketPort=%s", strPort.c_str());
		m_nThreadJobState = Job_State_End; 
		nFunRes = -1;
	}
	else
	{
		m_nThreadJobState = Job_State_CreateListenSocket;
	}


	FUNCTION_EXIT;
	return nFunRes;
}

int CServerConnectionListener::_Process_CreateListenSocket()
{
	FUNCTION_ENTRY("_Process_CreateListenSocket");
	int nFunRes = 0;
	TA_THREADGUARD(m_LockSocketPort);

	try
	{
		DEF_DELETE(m_pTCPServerSocket);
		m_pTCPServerSocket = new  TcpServerSocket<TcpSocket>(BIND_TO_ALL_INTERFACES, m_strListenSocketPort);	
		m_pTCPServerSocket->bind();
		m_pTCPServerSocket->listen(); 
		m_SetTCPServerSocket.addSocket(m_pTCPServerSocket);
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, 
			"server port = %s listening", m_strListenSocketPort.c_str());			
	}
	catch (...)
	{
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError,
			"bind server port = %s error!", m_strListenSocketPort.c_str());
		DEF_DELETE(m_pTCPServerSocket);
		nFunRes = -1;
	}


	if (0 == nFunRes)
	{
		m_nThreadJobState = Job_State_WaitForNewClient;
	}
	else
	{
		m_nThreadJobState = Job_State_End; 
	}
	
	FUNCTION_EXIT;
	return nFunRes;
}


int CServerConnectionListener::_Process_WaitForNewClient()
{
	FUNCTION_ENTRY("_Process_WaitForNewClient");

	int nFunRes = 0;
	TcpServerSocket<TcpSocket>* pTCPServerSocket = NULL;

	m_bTCPRead = true;
	m_bTCPWrite = true; 

	m_nTCPSelectRes = -1; 
	m_nTCPSelectRes = m_SetTCPServerSocket.waitForIO( m_bTCPRead, m_bTCPWrite, m_timeTCPWait );//put into io

	m_bTCPRead = false;
	m_bTCPWrite = false; 
	pTCPServerSocket = NULL;
	pTCPServerSocket = m_SetTCPServerSocket.getSocket(0, m_bTCPRead, m_bTCPWrite );

	if ((m_bTCPRead || m_bTCPWrite)&& NULL != pTCPServerSocket)
	{
		nFunRes = _AcceptClient(pTCPServerSocket);
	}
	
	FUNCTION_EXIT;
	return nFunRes;
}



int CServerConnectionListener::_AcceptClient(TcpServerSocket<TcpSocket>* pTCPServerSocket)
{
	FUNCTION_ENTRY("_AcceptClient");

	int nFunRes = 0;
	TcpSocket*	pNewClientSocket = NULL;
	CConnectionActor* pConActor = NULL;

	try
	{
		pNewClientSocket = pTCPServerSocket->accept(SERVER_NON_BLOCKING_ACCEPT, false); //SERVER_BLOCKING_ACCEPT
	}
	catch(...)
	{
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "ServerSocket accept new Tcp Client error");
		DEF_DELETE(pNewClientSocket);
		nFunRes = -1;
		FUNCTION_EXIT;
		return nFunRes;
	}

	pConActor = new CConnectionActor();
	pConActor->setSocketHandle(pNewClientSocket);
	pNewClientSocket = NULL;
	CServerConnectionManager::getInstance().addActor(pConActor);
	pConActor = NULL;
	FUNCTION_EXIT;
	return nFunRes;
}


NS_END(TA_Base_App)




