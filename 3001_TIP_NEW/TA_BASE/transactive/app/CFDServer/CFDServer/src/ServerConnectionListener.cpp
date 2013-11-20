#include "ServerConnectionListener.h"
#include "UtilityFun.h"
#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/DebugUtil.h"

using namespace TA_Base_Core;
NS_BEGIN(TA_Base_App)




CServerConnectionListener* CServerConnectionListener::m_pInstance = 0;
TA_Base_Core::NonReEntrantThreadLockable CServerConnectionListener::m_instanceLock;


CServerConnectionListener& CServerConnectionListener::getInstance()
{
	FUNCTION_ENTRY("getInstance");
	TA_THREADGUARD(m_instanceLock);
	if (NULL == m_pInstance)
	{
		m_pInstance = new CServerConnectionListener();
	}

	FUNCTION_EXIT;
	return (*m_pInstance);
}

void CServerConnectionListener::removeInstance()
{
	FUNCTION_ENTRY("getInstance");

	DEF_DELETE(m_pInstance);
	FUNCTION_EXIT;

}



CServerConnectionListener::CServerConnectionListener(void)
{
	FUNCTION_ENTRY("CServerConnectionListener");

	m_strListenSocketPort = "8000";
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
	   //Thread::sleep(DEF_INT_NormalThreadSleep);	//m_SetTCPServerSocket.waitForIO(1000)	
   }

   _ProcessUserTerminate();

   FUNCTION_EXIT;
}	
void CServerConnectionListener::terminate()
{
	FUNCTION_ENTRY("terminate");

	m_toTerminate = true; 
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
// 		if (NULL == m_pTCPServerRequestMonitor)
// 		{
// 			m_pTCPServerRequestMonitor = new CTCPServerRequestMonitor();	
// 			m_pTCPServerRequestMonitor->start();
// 		}
		m_nThreadJobState = Job_State_CheckParam;
		break;
	case Job_State_CheckParam:
		nFunRes = _Process_CheckParam();
		if (0 == nFunRes)
		{
		   m_nThreadJobState = Job_State_CreateListenSocket;
		}
		else
		{
			m_nThreadJobState = Job_State_End; 
		}
		break;
	case Job_State_CreateListenSocket:
		nFunRes = _Process_CreateListenSocket();
		if (0 == nFunRes)
		{
			m_nThreadJobState = Job_State_WaitForNewClient;
		}
		else
		{
			m_nThreadJobState = Job_State_End; 
		}
		break;
	case Job_State_WaitForNewClient:
		nFunRes = _Process_WaitForNewClient();
		if (0 == nFunRes)
		{
			m_nThreadJobState = Job_State_WaitForNewClient;
		}
		else
		{
			m_nThreadJobState = Job_State_End; 
		}
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

int CServerConnectionListener::_ProcessUserTerminate()
{
	FUNCTION_ENTRY("_ProcessUserTerminate");

	int nFunRes = 0;
	//user termin
	m_nThreadJobState = Job_State_End;
	
// 	if (NULL != m_pTCPServerRequestMonitor)
// 	{
// 		m_pTCPServerRequestMonitor->terminateAndWait();
// 		DEF_DELETE(m_pTCPServerRequestMonitor);
// 	}

	if (NULL != m_pTCPServerSocket)
	{
		m_SetTCPServerSocket.removeSocket(m_pTCPServerSocket);
		DEF_DELETE(m_pTCPServerSocket);		
	}
	  
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
	int nFunRes = 0;
	m_strListenSocketPort = strListenPort;
	FUNCTION_EXIT;
	return nFunRes;
}



int CServerConnectionListener::_Process_CheckParam()
{
	FUNCTION_ENTRY("_Process_CheckParam");

	int nFunRes = 0;

	if (m_strListenSocketPort.empty())
	{
		nFunRes = -1;
		return nFunRes;
	}
	m_nListenSocketPort = CUtilityFun::getInstance().mysys_str2Int32(m_strListenSocketPort);

	if (0 == m_nListenSocketPort || m_nListenSocketPort > INT_MAX/2)
	{
		nFunRes = -1;
		return nFunRes;
	}

	FUNCTION_EXIT;

	return nFunRes;

}

int CServerConnectionListener::_Process_CreateListenSocket()
{
	FUNCTION_ENTRY("_Process_CreateListenSocket");

	int nFunRes = 0;
	try
	{
		DEF_DELETE(m_pTCPServerSocket);
		m_pTCPServerSocket = new  TcpServerSocket<TcpSocket>(BIND_TO_ALL_INTERFACES, m_strListenSocketPort);	
		m_pTCPServerSocket->bind();
		m_pTCPServerSocket->listen(); 
		m_SetTCPServerSocket.addSocket(m_pTCPServerSocket);
		nFunRes = 0;
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

	if (m_SetTCPServerSocket.getSize() <= 0)
	{
		Thread::sleep(DEF_INT_NormalThreadSleep);
	}

	FUNCTION_EXIT;
	return nFunRes;
}



int CServerConnectionListener::_AcceptClient(TcpServerSocket<TcpSocket>* pTCPServerSocket)
{
	FUNCTION_ENTRY("_AcceptClient");

	int nFunRes = 0;
	TcpSocket*	pNewClientSocket = NULL;
	//CTCPConnectionActor* pActor = NULL;

	try
	{
		pNewClientSocket = pTCPServerSocket->accept(SERVER_NON_BLOCKING_ACCEPT, false); //SERVER_BLOCKING_ACCEPT
		nFunRes = 0;		
	}
	catch(...)
	{
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "ServerSocket accept new Tcp Client error");

		DEF_DELETE(pNewClientSocket);
		nFunRes = -1;
		FUNCTION_EXIT;
		return nFunRes;
	}


// 	try
// 	{
// 		pActor = new CTCPConnectionActor(pNewClientSocket);
// 		pNewClientSocket = NULL;
// 
// 	}
// 	catch (...)
// 	{
// 		DEF_DELETE(pActor);
// 	}

	
	FUNCTION_EXIT;
	return nFunRes;
}


NS_END(TA_Base_App)




