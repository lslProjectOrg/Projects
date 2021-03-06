#include "ClientManager.h"
#include "Logger.h"
#include "StartClientThread.h"



//extern boost::condition_variable g_conditionMainRun;

using namespace TA_Base_Test;

NS_BEGIN(TA_Base_Test) 


int g_n_ClientNUM = 1;


//////////////////////////////////////////////////////////////////////////
CClientManager::CClientManager(void)
{	
	boost::mutex::scoped_lock lock(m_mutexMapClients);

	m_mapClients.clear();
	m_nClientCount = 0;
	m_nCurrentClientIndex = 0;
	m_nThreadJobState = JobState_Begin;
	m_toTerminate = false;
	
}

CClientManager::~CClientManager(void)
{
	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugTrace, "CClientManager::~CClientManager");

	
}


void CClientManager::_CreateALLClient()
{
	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugTrace, "CClientManager::_CreateALLClient");

	boost::mutex::scoped_lock lock(m_mutexMapClients);

	CStartClientThread* pClient = NULL;	
	for (int nIndex = 0; nIndex < g_n_ClientNUM; nIndex++)
	{

		pClient = new CStartClientThread(m_nCurrentClientIndex);
		m_mapClients.insert(std::map<int, CStartClientThread*>::value_type(m_nCurrentClientIndex, pClient));
		pClient->start();
		pClient = NULL;
		m_nCurrentClientIndex++;
		m_nClientCount++;

		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugDebug, 
			"CClientManager new CTestClientThread m_nCurrentClientIndex=%d, m_nClientCount=%d", 
			m_nClientCount);
	}//for

}


void CClientManager::_MonitorALLClient()
{
	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugTrace, "CClientManager::_MonitorALLClient");
	
	boost::mutex::scoped_lock lock(m_mutexMapClients);

	CStartClientThread* pClient = NULL;
	std::map<int, CStartClientThread*>::iterator iterMap;
	int nClientIndex = 0;

	//stop
	iterMap = m_mapClients.begin();
	while (iterMap != m_mapClients.end())
	{
		nClientIndex = (iterMap->first);
		pClient = (iterMap->second);

		if (pClient->isFinishWork())
		{
			pClient->terminateAndWait();
			delete pClient;
			pClient = NULL;

			m_nClientCount--;
			_SysLog(LogSourceFLInfo, TA_Base_Test::DebugDebug, 
				"delete one client nClientIndex=%d, connected m_nClientCount=%d",
				nClientIndex, m_nClientCount);

			m_mapClients.erase(iterMap);
			iterMap = m_mapClients.begin();
		}
		else
		{
			iterMap++;
		}
	
	}//while

	if (m_mapClients.empty())
	{
		m_nThreadJobState = JobState_End;
	}
	
}


void CClientManager::_DestoryAllClient()
{
	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugTrace, "CClientManager::_DestoryAllClient");

	boost::mutex::scoped_lock lock(m_mutexMapClients);

	CStartClientThread* pClient = NULL;
	std::map<int, CStartClientThread*>::iterator iterMap;
	int nClientIndex = 0;

	//stop
	iterMap = m_mapClients.begin();
	while (iterMap != m_mapClients.end())
	{
		nClientIndex = (iterMap->first);
		pClient = (iterMap->second);
		pClient->terminateAndWait();

		delete pClient;
		pClient = NULL;

		m_nClientCount--;
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugDebug, 
			"delete one client nClientIndex=%d, connected m_nClientCount=%d",
			nClientIndex, m_nClientCount);

		iterMap++;
	}//while
	m_mapClients.clear();

}

void CClientManager::run()
{
	m_nThreadJobState = JobState_Begin;


	while (false == m_toTerminate)
	{
		_ThreadJob();
		TA_Base_Test::CBoostThread::sleep(DEF_INT_MonitorThreadSleep);

	}

	_ProcessUserTerminate();
}

void CClientManager::terminate()
{
	m_toTerminate = true;
}


int CClientManager::_ProcessUserTerminate()
{
	int nFunRes = 0;
	_DestoryAllClient();
	m_nThreadJobState = JobState_End;
	return nFunRes;
}

bool CClientManager::isFinishWork()
{
	bool bFinishWork = false;
	if (JobState_End == m_nThreadJobState)
	{
		bFinishWork = true;
	}
	return bFinishWork;

}

void CClientManager::_ThreadJob()
{

	switch (m_nThreadJobState)
	{
	case JobState_Begin:
		m_nThreadJobState = JobState_CreateALLClient;
		break;
	case JobState_CreateALLClient:
		_CreateALLClient();
		m_nThreadJobState = JobState_MonitorAllClient;
		break;
	case JobState_MonitorAllClient:
		_MonitorALLClient();
		TA_Base_Test::CBoostThread::sleep(DEF_INT_LongMonitorThreadSleep);
		break;
	case JobState_End:
		TA_Base_Test::CBoostThread::sleep(DEF_INT_MonitorThreadSleep);
		break;
	default:
		break;

	}//switch
}


NS_END(TA_Base_Test) 



