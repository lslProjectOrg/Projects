#include "ClientsManagerWorker.h"
#include "ClientManager.h"
#include "HelpClass.h"

#include "BoostLogger.h"
USING_BOOST_LOG;


//extern boost::condition_variable g_conditionMainRun;

using namespace TA_Base_Test;

NS_BEGIN(TA_Base_Test) 

extern int g_n_ClientNUM;




//////////////////////////////////////////////////////////////////////////
CClientsManagerWorker::CClientsManagerWorker(void)
{	
	BOOST_LOG_FUNCTION();

	{
		boost::mutex::scoped_lock lock(m_mutexMapClients);
		m_mapClients.clear();
		m_nClientCount = 0;
		m_nCurrentClientIndex = 0;
	}

	m_nThreadJobState = JobState_Begin;
	m_toTerminate = false;
	
}

CClientsManagerWorker::~CClientsManagerWorker(void)
{	
	BOOST_LOG_FUNCTION();
	
}


void CClientsManagerWorker::_CreateALLClient()
{
	BOOST_LOG_FUNCTION();

	boost::mutex::scoped_lock lock(m_mutexMapClients);

	CClientManager* pClient = NULL;	
	for (int nIndex = 0; nIndex < g_n_ClientNUM; nIndex++)
	{

		pClient = new CClientManager(m_nCurrentClientIndex);
		m_mapClients.insert(std::map<int, CClientManager*>::value_type(m_nCurrentClientIndex, pClient));
		pClient = NULL;
		m_nCurrentClientIndex++;
		m_nClientCount++;

		LOG_DEBUG<<"CClientsManagerWorker new CClientManager"
			<<" "<<"m_nCurrentClientIndex="<<m_nCurrentClientIndex
			<<" "<<"m_nClientCount=%d"<<m_nClientCount;
	}//for

}


void CClientsManagerWorker::_MonitorALLClient()
{
	BOOST_LOG_FUNCTION();
	
	boost::mutex::scoped_lock lock(m_mutexMapClients);

	CClientManager* pClient = NULL;
	std::map<int, CClientManager*>::iterator iterMap;
	int nClientIndex = 0;

	//stop
	iterMap = m_mapClients.begin();
	while (iterMap != m_mapClients.end())
	{
		nClientIndex = (iterMap->first);
		pClient = (iterMap->second);

		if (pClient->isFinishWork())
		{
			delete pClient;
			pClient = NULL;

			m_nClientCount--;

			LOG_DEBUG<<"delete one client"
				<<" "<<"nClientIndex="<<nClientIndex
				<<" "<<"still connected m_nClientCount="<<m_nClientCount;

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


void CClientsManagerWorker::_DestoryAllClient()
{
	BOOST_LOG_FUNCTION();

	boost::mutex::scoped_lock lock(m_mutexMapClients);

	CClientManager* pClient = NULL;
	std::map<int, CClientManager*>::iterator iterMap;
	int nClientIndex = 0;

	//stop
	iterMap = m_mapClients.begin();
	while (iterMap != m_mapClients.end())
	{
		nClientIndex = (iterMap->first);
		pClient = (iterMap->second);

		delete pClient;
		pClient = NULL;

		m_nClientCount--;

		LOG_DEBUG<<"delete one client"
			<<" "<<"nClientIndex="<<nClientIndex
			<<" "<<"still connected m_nClientCount="<<m_nClientCount;

		iterMap++;
	}//while
	m_mapClients.clear();

}

void CClientsManagerWorker::run()
{
	BOOST_LOG_FUNCTION();

	m_nThreadJobState = JobState_Begin;


	while (false == m_toTerminate)
	{
		_ThreadJob();
		TA_Base_Test::CBoostThread::sleep(DEF_INT_MonitorThreadSleep);

	}

	_ProcessUserTerminate();
}

void CClientsManagerWorker::terminate()
{
	BOOST_LOG_FUNCTION();

	m_toTerminate = true;
}


int CClientsManagerWorker::_ProcessUserTerminate()
{
	BOOST_LOG_FUNCTION();

	int nFunRes = 0;
	_DestoryAllClient();
	m_nThreadJobState = JobState_End;
	return nFunRes;
}

bool CClientsManagerWorker::isFinishWork()
{
	BOOST_LOG_FUNCTION();

	bool bFinishWork = false;
	if (JobState_End == m_nThreadJobState)
	{
		bFinishWork = true;
	}
	return bFinishWork;

}

void CClientsManagerWorker::_ThreadJob()
{
	BOOST_LOG_FUNCTION();


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



