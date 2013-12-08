#include "ServerManage.h"
#include "Logger.h"
#include "HelpClass.h"
#include "WorkTime.h"



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
	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugTrace, "CServerManager::CServerManager");
	
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
	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugTrace, "CServerManager::~CClientManager");
	delete m_pRecvWorkTime;
	m_pRecvWorkTime = NULL;

	delete m_pHelpClass;
	m_pHelpClass = NULL;
}


void CServerManager::handleDisconnected(const SessionInfo &stSessionInfo)
{
	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugTrace, "CServerManager::handleDisconnected");
	std::string strLogInfo;
	strLogInfo = "handleDisconnected:";
	m_pHelpClass->log_SeeionINfo(stSessionInfo, strLogInfo);

}


void CServerManager::handleDeliverFailure(Message::Ptr pGetMessage)
{
	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugTrace, "CServerManager::handleDeliverFailure");

}

void CServerManager::handleConnected(const SessionInfo &stSessionInfo)
{
	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugTrace, "CServerManager::handleConnected");

	std::string strLogInfo;

	m_nHandleConnections++;
	strLogInfo = g_string_strServerAddress + "handle handleConnected:";
	m_pHelpClass->log_SeeionINfo(stSessionInfo, strLogInfo);
	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugTrace, "CServerManager m_nHandleConnections =%d", m_nHandleConnections);


}

void CServerManager::handleReceivedMessage(Message::Ptr pGetMessage)
{
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
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugError, 
			"m_nHandleConnections=%d, m_nRecvMsgCount=%d, nFrameLength=%d, checkFrameData Error!!", 
			m_nHandleConnections, m_nRecvMsgCount, pGetMessage->length());
	}
	//log
	if (m_nRecvMsgCount % 50000 == 0)
	{
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugInfo, "m_nHandleConnections=%d, m_nRecvMsgCount=%d,", 
			m_nHandleConnections, m_nRecvMsgCount);
	}
	//
	if (g_n_TotalClientSendFrameNUM == m_nRecvMsgCount)
	{	
		BigInt64 nTotal = 0;//m_pRecvWorkTime->getWorkTime();
		nTotal = m_pRecvWorkTime->workEnd();

		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugInfo, 
			"m_nRecvMsgCount=%d, messages recv and verified successfully, m_pRecvWorkTime=%I64d  ms", 
			m_nRecvMsgCount, nTotal);
	}
	
}

void CServerManager::_InitServer()
{
	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugTrace, "CServerManager::initServer");

	m_brokerServer.onConnected = boost::bind(&TA_Base_Test::CServerManager::handleConnected, this, _1);//static boost::arg<1> _1;
	m_brokerServer.onDisconnected = boost::bind(&TA_Base_Test::CServerManager::handleDisconnected, this, _1);//static boost::arg<1> _1;
	m_brokerServer.onReceived = boost::bind(&TA_Base_Test::CServerManager::handleReceivedMessage, this, _1);//static boost::arg<1> _1;
	m_brokerServer.onDeliverFailure = boost::bind(&TA_Base_Test::CServerManager::handleDeliverFailure, this, _1);//static boost::arg<1> _1;

	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugTrace, "begin listen m_strServerAddress : %s", g_string_strServerAddress.c_str());
	m_brokerServer.listen(g_string_strServerAddress);
	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugTrace, "end listen m_strServerAddress : %s", g_string_strServerAddress.c_str());

}


void CServerManager::runTestCase()
{
	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugTrace, "CServerManager::runTestCase");
	_InitServer();
}

NS_END(TA_Base_Test) 



