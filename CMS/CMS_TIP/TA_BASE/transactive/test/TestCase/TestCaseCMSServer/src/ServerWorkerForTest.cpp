#include "ServerWorkerForTest.h"
#include "HelpClass.h"
#include "WorkTime.h"
#include "ClientSessionActor.h"
#include "ProcessClientReqHelperForTest.h"

#include "BoostLogger.h"
USING_BOOST_LOG;


using namespace TA_Base_Test;

NS_BEGIN(TA_Base_Test) 

extern std::string g_string_strServerAddress;



//////////////////////////////////////////////////////////////////////////
CServerWorkerForTest::CServerWorkerForTest(void)
{	
	BOOST_LOG_FUNCTION();	

	{
		boost::mutex::scoped_lock lock(m_mutexMapClientsSessionIDActor);
		m_mapClientsSessionIDActor.clear();
		m_nClientsCount = 0;
	}

	{
		boost::mutex::scoped_lock lock(m_mutexLstWaitProcessMessages);
		m_lstWaitProcessMessages.clear();
		m_nWaitProcessMsgCount = 0;
		m_nTotalRecvMsgCount = 0;
	}


	{
		boost::mutex::scoped_lock lock(m_mutexLstPorcessingMessages);
		m_lstPorcessingMessages.clear();
		m_nPorcessingMsgCount = 0;
	}

	m_toTerminate = false;
	m_nThreadJobState = JobState_Begin;
	
	//
	m_pRecvWorkTime = new CWorkTimeLock();
	m_pHelpClass = new CHelpClass();

	m_pProcessReqHelper = new CProcessClientReqHelperForTest();

}

CServerWorkerForTest::~CServerWorkerForTest(void)
{
	BOOST_LOG_FUNCTION();

	DEF_DELETE(m_pRecvWorkTime);

	DEF_DELETE(m_pHelpClass);

	DEF_DELETE(m_pProcessReqHelper);
}


void CServerWorkerForTest::run()
{
	BOOST_LOG_FUNCTION();

	m_nThreadJobState = JobState_Begin;


	while (false == m_toTerminate)
	{
		_ThreadJob();
		//TA_Base_Test::CBoostThread::sleep(DEF_INT_MonitorThreadSleep);

	}

	_ProcessUserTerminate();
}

void CServerWorkerForTest::terminate()
{
	BOOST_LOG_FUNCTION();

	m_toTerminate = true;
}


int CServerWorkerForTest::_ProcessUserTerminate()
{
	BOOST_LOG_FUNCTION();

	int nFunRes = 0;
	_Process_UnInitServer();
	m_nThreadJobState = JobState_End;
	return nFunRes;
}

bool CServerWorkerForTest::isFinishWork()
{
	BOOST_LOG_FUNCTION();

	bool bFinishWork = false;
	if (JobState_End == m_nThreadJobState)
	{
		bFinishWork = true;
	}
	return bFinishWork;

}

void CServerWorkerForTest::_ThreadJob()
{
	BOOST_LOG_FUNCTION();

	switch (m_nThreadJobState)
	{
	case JobState_Begin:
		m_nThreadJobState = JobState_InitServer;
		break;
	case JobState_InitServer:
		_Process_InitServer();
		m_nThreadJobState = JobState_WaitMessage;
		break;
	case JobState_WaitMessage:
		_Process_WaitMessage();
		m_nThreadJobState = JobState_PorcessMessage;
		break;
	case JobState_PorcessMessage:
		_Process_PorcessMessage();
		m_nThreadJobState = JobState_WaitMessage;
		break;
	case JobState_UnInitServer:
		_Process_UnInitServer();
		TA_Base_Test::CBoostThread::sleep(DEF_INT_LongMonitorThreadSleep);
		break;
	case JobState_End:
		TA_Base_Test::CBoostThread::sleep(DEF_INT_MonitorThreadSleep);
		break;
	default:
		TA_Base_Test::CBoostThread::sleep(DEF_INT_MonitorThreadSleep);
		break;

	}//switch
}

void CServerWorkerForTest::handleDisconnected(const SessionInfo &stSessionInfo)
{
	BOOST_LOG_FUNCTION();

	_RemoveClient(stSessionInfo);
}


void CServerWorkerForTest::handleDeliverFailure(Message::Ptr pGetMessage)
{
	BOOST_LOG_FUNCTION();
	std::string strLogInfo;
	strLogInfo = "CServerManager::handleDeliverFailure:";
	LOG_DEBUG<<strLogInfo;

}

void CServerWorkerForTest::handleConnected(const SessionInfo &stSessionInfo)
{
	BOOST_LOG_FUNCTION();

	//add to list
	_AddClient(stSessionInfo);

}

void CServerWorkerForTest::handleReceivedMessage(Message::Ptr pGetMessage)
{
	BOOST_LOG_FUNCTION();

	_ReceiveMessage(pGetMessage);
	
}

int CServerWorkerForTest::_Process_InitServer()
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;

	if (g_string_strServerAddress.empty())
	{
		LOG_ERROR<<"error! g_string_strServerAddress="<<g_string_strServerAddress;
		nFunRes = -1;
		return nFunRes;
	}

	_LogServerInfo();

	return nFunRes;
}
int CServerWorkerForTest::_Process_WaitMessage()
{
	int nFunRes = 0;
	bool  bWaiting = true;

	{
		boost::mutex::scoped_lock lock(m_mutexLstWaitProcessMessages);
		if (false == m_lstWaitProcessMessages.empty())
		{
			bWaiting = false;
		}		
	}

	if (bWaiting)
	{
		boost::mutex::scoped_lock lock(m_mutexWait);

		boost::xtime timeTmp;
		boost::xtime_get(&timeTmp, boost::TIME_UTC_);
		timeTmp.sec += 2;

		m_conditionWait.timed_wait(lock, timeTmp);
	}


	return nFunRes;
}


int CServerWorkerForTest::_Process_PorcessMessage()
{
	int nFunRes = 0;
	_GetMessageForPorcessing();
	_PorcessingMessage();
	return nFunRes;
}


int CServerWorkerForTest::_Process_UnInitServer()
{
	int nFunRes = 0;

	return nFunRes;
}


void CServerWorkerForTest::_LogServerInfo()
{
	LOG_DEBUG<<"ServerAddress="<<g_string_strServerAddress
		<<" "<<"m_nClientsCount="<<m_nClientsCount
		<<" "<<"m_nTotalRecvMsgCount="<<m_nTotalRecvMsgCount
		<<" "<<"m_nWaitProcessMsgCount="<<m_nWaitProcessMsgCount
		<<" "<<"m_nPorcessingMsgCount="<<m_nPorcessingMsgCount;
		
}



int CServerWorkerForTest::_GetMessageForPorcessing()
{
	int nFunRes = 0;

	LstMessageIterT iterList;

	{
		boost::mutex::scoped_lock lockRecv(m_mutexLstWaitProcessMessages);

		boost::mutex::scoped_lock lockPorcess(m_mutexLstPorcessingMessages);

		iterList = m_lstWaitProcessMessages.begin();
		while (iterList != m_lstWaitProcessMessages.end())
		{
			cms::Message::Ptr pMessage = (*iterList);
			m_lstPorcessingMessages.push_back(pMessage);
			m_nWaitProcessMsgCount--;
			m_nPorcessingMsgCount++;
			_LogServerInfo();

			iterList++;
		}
		m_lstWaitProcessMessages.clear();

	}

	return nFunRes;
}



int CServerWorkerForTest::_PorcessingMessage()
{
	int nFunRes = 0;

	LstMessageIterT iterList;

	{
		boost::mutex::scoped_lock lockPorcess(m_mutexLstPorcessingMessages);

		iterList = m_lstPorcessingMessages.begin();
		while (iterList != m_lstPorcessingMessages.end())
		{
			cms::Message::Ptr pMessage = (*iterList);

			//TODO ....
			m_pProcessReqHelper->processMessage(pMessage);

			m_nPorcessingMsgCount--;
			_LogServerInfo();

			iterList++;
		}
		m_lstPorcessingMessages.clear();

	}

	return nFunRes;
}


void CServerWorkerForTest::_AddClient(const SessionInfo &stSessionInfo)
{
	BOOST_LOG_FUNCTION();

	cms::SessionID ClientSessionID = 0;
	CClientSessionActor* pClientSessionActor = NULL;

	//add to list
	{
		boost::mutex::scoped_lock lock(m_mutexMapClientsSessionIDActor);

		pClientSessionActor = new CClientSessionActor();
		pClientSessionActor->setSessionInfo(stSessionInfo);
		ClientSessionID = pClientSessionActor->getSessionID();

		m_mapClientsSessionIDActor.insert(MapSessinIDActorValueT(ClientSessionID, pClientSessionActor));
		m_nClientsCount++;

		pClientSessionActor = NULL;
	}


	//log
	std::string strLogInfo;
	strLogInfo = g_string_strServerAddress + " accept new client: ";
	m_pHelpClass->log_SeeionINfo(stSessionInfo, strLogInfo);
	LOG_DEBUG<<"m_nClientsCount="<<m_nClientsCount;
}


void CServerWorkerForTest::_RemoveClient(const SessionInfo &stSessionInfo)
{
	BOOST_LOG_FUNCTION();

	MapSessinIDActorIterT iterMap;
	cms::SessionID ClientSessionID = 0;
	CClientSessionActor* pClientSessionActor = NULL;

	//add to list
	{
		boost::mutex::scoped_lock lock(m_mutexMapClientsSessionIDActor);
		ClientSessionID = stSessionInfo.sessionID;
		iterMap = m_mapClientsSessionIDActor.find(ClientSessionID);
		if (iterMap != m_mapClientsSessionIDActor.end())
		{
			pClientSessionActor = (iterMap->second);
			m_mapClientsSessionIDActor.erase(iterMap);
			m_nClientsCount--;
		}
	}

	if (NULL != pClientSessionActor)
	{
		delete pClientSessionActor;
		pClientSessionActor = NULL;
	}

	//log
	std::string strLogInfo;
	strLogInfo = " disconnected one client ";
	m_pHelpClass->log_SeeionINfo(stSessionInfo, strLogInfo);
}



void CServerWorkerForTest::_ReceiveMessage(Message::Ptr pGetMessage)
{
	BOOST_LOG_FUNCTION();

	{
		boost::mutex::scoped_lock lock(m_mutexLstWaitProcessMessages);
		m_lstWaitProcessMessages.push_back(pGetMessage);
		m_nWaitProcessMsgCount++;	
		m_nTotalRecvMsgCount++;
		_LogServerInfo();
	}

	m_conditionWait.notify_one();

}

NS_END(TA_Base_Test) 



