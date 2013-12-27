#include "ServerManage.h"

#include "core/utilities/src/BoostLogger.h"
USING_BOOST_LOG;

//extern boost::condition_variable g_conditionMainRun;


NS_BEGIN(TA_Base_App) 

extern std::string g_string_strServerAddress;


CServerManager* CServerManager::m_pInstance = 0;
boost::mutex CServerManager::m_instanceLock;


CServerManager& CServerManager::getInstance()
{
	BOOST_LOG_FUNCTION();	
	boost::mutex::scoped_lock lock(m_instanceLock);

	if (NULL == m_pInstance)
	{
		m_pInstance = new CServerManager();
	}

	return (*m_pInstance);
}

void CServerManager::removeInstance()
{
	BOOST_LOG_FUNCTION();	

	boost::mutex::scoped_lock lock(m_instanceLock);

	if (NULL != m_pInstance)
	{
		DEF_DELETE(m_pInstance);
	}

}


//////////////////////////////////////////////////////////////////////////
CServerManager::CServerManager(void)
{	
	BOOST_LOG_FUNCTION();	

	_InitServer();
	
// 	m_pServerWorkerForTest = NULL;
// 	
// 	m_pServerWorkerForTest = new CServerWorkerForTest();
// 	m_pServerWorkerForTest->start();
	
}

CServerManager::~CServerManager(void)
{
	BOOST_LOG_FUNCTION();

// 	if (NULL != m_pServerWorkerForTest)
// 	{
// 		m_pServerWorkerForTest->terminateAndWait();
// 		delete m_pServerWorkerForTest;
// 		m_pServerWorkerForTest = NULL;
// 	}


}


void CServerManager::handleDisconnected(const SessionInfo &stSessionInfo)
{
	BOOST_LOG_FUNCTION();

	//m_pServerWorkerForTest->handleDisconnected(stSessionInfo);
}


void CServerManager::handleDeliverFailure(Message::Ptr pGetMessage)
{
	BOOST_LOG_FUNCTION();
	//m_pServerWorkerForTest->handleReceivedMessage(pGetMessage);
	

}

void CServerManager::handleConnected(const SessionInfo &stSessionInfo)
{
	BOOST_LOG_FUNCTION();

	//m_pServerWorkerForTest->handleDisconnected(stSessionInfo);
	
}

void CServerManager::handleReceivedMessage(Message::Ptr pGetMessage)
{
	BOOST_LOG_FUNCTION();
	//m_pServerWorkerForTest->handleReceivedMessage(pGetMessage);
	
}

int CServerManager::_InitServer()
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;

	if (g_string_strServerAddress.empty())
	{
		LOG_ERROR<<"error! g_string_strServerAddress="<<g_string_strServerAddress;
		nFunRes = -1;
		return nFunRes;
	}

	{
		boost::mutex::scoped_lock lock(m_mutexBrokerServer);
		m_brokerServer.onConnected = boost::bind(&TA_Base_App::CServerManager::handleConnected, this, _1);//static boost::arg<1> _1;
		m_brokerServer.onDisconnected = boost::bind(&TA_Base_App::CServerManager::handleDisconnected, this, _1);//static boost::arg<1> _1;
		m_brokerServer.onReceived = boost::bind(&TA_Base_App::CServerManager::handleReceivedMessage, this, _1);//static boost::arg<1> _1;
		m_brokerServer.onDeliverFailure = boost::bind(&TA_Base_App::CServerManager::handleDeliverFailure, this, _1);//static boost::arg<1> _1;

		LOG_DEBUG<< "begin listen g_string_strServerAddress="<<g_string_strServerAddress;
		m_brokerServer.listen(g_string_strServerAddress);
	}



	return nFunRes;
}

int CServerManager::sendData( const cms::SessionID& destSessionID, cms::Message::Ptr pMessage )
{
	boost::mutex::scoped_lock lock(m_mutexBrokerServer);
	int nFunRes = 0;
	m_brokerServer.sendToSession(destSessionID, pMessage);
	return nFunRes;
}



NS_END(TA_Base_App) 



