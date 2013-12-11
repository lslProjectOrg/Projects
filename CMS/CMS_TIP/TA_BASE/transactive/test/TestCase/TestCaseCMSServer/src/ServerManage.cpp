#include "ServerManage.h"
#include "HelpClass.h"
#include "WorkTime.h"

#include "BoostLogger.h"
USING_BOOST_LOG;

//extern boost::condition_variable g_conditionMainRun;

using namespace TA_Base_Test;

NS_BEGIN(TA_Base_Test) 

extern std::string g_string_strServerAddress;
extern int g_n_EachClientSendFrameNUM;
extern int g_n_ClientNUM;
extern int g_n_TotalClientSendFrameNUM;





//////////////////////////////////////////////////////////////////////////
CServerManager::CServerManager(void)
{	
	BOOST_LOG_FUNCTION();	
	m_nHandleConnections = 0;
	m_nRecvMsgCount =0;
	m_nSendMsgCount = 0;
	m_pThisServerManager = this;

	//
	m_pRecvWorkTime = new CWorkTimeLock(2);
	m_pHelpClass = new CHelpClass();
}

CServerManager::~CServerManager(void)
{
	BOOST_LOG_FUNCTION();

	delete m_pRecvWorkTime;
	m_pRecvWorkTime = NULL;

	delete m_pHelpClass;
	m_pHelpClass = NULL;
}


void CServerManager::handleDisconnected(const SessionInfo &stSessionInfo)
{
	BOOST_LOG_FUNCTION();
	std::string strLogInfo;
	strLogInfo = "CServerManager::handleDisconnected:";
	m_pHelpClass->log_SeeionINfo(stSessionInfo, strLogInfo);

}


void CServerManager::handleDeliverFailure(Message::Ptr pGetMessage)
{
	BOOST_LOG_FUNCTION();
	std::string strLogInfo;
	strLogInfo = "CServerManager::handleDeliverFailure:";
	LOG_DEBUG<<strLogInfo;

}

void CServerManager::handleConnected(const SessionInfo &stSessionInfo)
{
	BOOST_LOG_FUNCTION();

	std::string strLogInfo;

	m_nHandleConnections++;
	strLogInfo = g_string_strServerAddress + " handleConnected: ";
	m_pHelpClass->log_SeeionINfo(stSessionInfo, strLogInfo);

	LOG_DEBUG<<"CServerManager m_nHandleConnections="<<m_nHandleConnections;
}

void CServerManager::handleReceivedMessage(Message::Ptr pGetMessage)
{
	BOOST_LOG_FUNCTION();

	int nFunRes = 0;

	if (0 == m_nRecvMsgCount)
	{
		m_pRecvWorkTime->workBegin();
	}

	m_nRecvMsgCount++;	


	CTestFrame TestFrame;
	TestFrame.setDataWithMessage(pGetMessage);
	//check
	nFunRes = TestFrame.checkFrameData();
	if (0 != nFunRes)
	{
		LOG_ERROR<<"m_nHandleConnections="<<m_nHandleConnections
			<<" "<<"m_nRecvMsgCount="<<m_nRecvMsgCount
			<<" "<<"nFrameLength="<<pGetMessage->length()
			<<" "<<"checkFrameData Error!!";
	}
	//log
	if (m_nRecvMsgCount % 10000 == 0)
	{
		LOG_INFO<<"m_nHandleConnections="<<m_nHandleConnections
			<<" "<<"m_nRecvMsgCount="<<m_nRecvMsgCount;
	}
	//
	if (g_n_TotalClientSendFrameNUM == m_nRecvMsgCount)
	{	
		BigInt64 nTotal = 0;//m_pRecvWorkTime->getWorkTime();
		nTotal = m_pRecvWorkTime->workEnd();

		LOG_INFO<<"m_nHandleConnections="<<m_nHandleConnections
			<<" "<<"m_nRecvMsgCount="<<m_nRecvMsgCount
			<<" "<<"messages recv and verified successfully,"
			<<" "<<"m_pRecvWorkTime="<<nTotal;
	}
	
}

void CServerManager::_InitServer()
{
	BOOST_LOG_FUNCTION();

	m_brokerServer.onConnected = boost::bind(&TA_Base_Test::CServerManager::handleConnected, this, _1);//static boost::arg<1> _1;
	m_brokerServer.onDisconnected = boost::bind(&TA_Base_Test::CServerManager::handleDisconnected, this, _1);//static boost::arg<1> _1;
	m_brokerServer.onReceived = boost::bind(&TA_Base_Test::CServerManager::handleReceivedMessage, this, _1);//static boost::arg<1> _1;
	m_brokerServer.onDeliverFailure = boost::bind(&TA_Base_Test::CServerManager::handleDeliverFailure, this, _1);//static boost::arg<1> _1;

	LOG_DEBUG<< "begin listen m_strServerAddress="<<g_string_strServerAddress;
	m_brokerServer.listen(g_string_strServerAddress);

}


void CServerManager::runTestCase()
{
	BOOST_LOG_FUNCTION();
	_InitServer();
}

NS_END(TA_Base_Test) 



