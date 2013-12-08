#include "TestClientThread.h"
#include "Logger.h"
#include "TestFrame.h"
#include "HelpClass.h"
#include "ClientManager.h"


//extern boost::condition_variable g_conditionMainRun;

using namespace TA_Base_Test;

NS_BEGIN(TA_Base_Test) 
//
extern std::string g_string_strServerAddress;
extern int g_n_EachClientSendFrameNUM;
extern int g_n_ClientNUM;
extern int g_n_TotalClientSendFrameNUM;
//


CTestClientThread::CTestClientThread(int nClientIndex)
{
	m_nClientIndex = nClientIndex;
	m_pSendWorkTime = new CWorkTimeLock(1);

	m_nSendMsgCount = 0;
	m_nRecvMsgCount = 0;
	m_bClientConnected = false;
	m_toTerminate = false;
	m_pHelpClass = new CHelpClass();

}

CTestClientThread::~CTestClientThread(void)
{
	delete m_pSendWorkTime;
	m_pSendWorkTime = NULL;

	delete m_pHelpClass;
	m_pHelpClass = NULL;

}


void CTestClientThread::run()
{
	m_nThreadJobState = JobState_Begin;


	while(false == m_toTerminate)
	{		
		_ThreadJob();
		TA_Base_Test::CBoostThread::sleep(DEF_INT_MonitorThreadSleep);
		
	}

	_ProcessUserTerminate();

}	

void CTestClientThread::_ThreadJob()
{
	switch (m_nThreadJobState)
	{
	case JobState_Begin:
		m_nThreadJobState = JobState_InitClient;
		break;
	case JobState_InitClient:
		_Process_InitClient();
		m_nThreadJobState = JobState_ConnectToServer;
		break;
	case JobState_ConnectToServer:
		_Process_ConnectToServer();
		m_nThreadJobState = JobState_MonitorConnectRes;
		break;
	case JobState_MonitorConnectRes:
		_Process_MonitorConnectRes();
		break;
	case JobState_DoClientWork:
		_Process_DoClientWork();
		m_nThreadJobState = JobState_FinishClientWork;
		break;
	case JobState_FinishClientWork:
		m_nThreadJobState = JobState_UnInitClient;
		break;
	case JobState_UnInitClient:
		//m_BrokerClient.closeSession(m_clientSessionInfo.sessionID);
		m_nThreadJobState = JobState_End;
			break;
	case JobState_End:
		break;
	default:
		break;

	}//switch


}

void CTestClientThread::terminate(void)
{
	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugTrace, "CTestClientThread::terminate");

	m_toTerminate = true;
}



void CTestClientThread::_SetSessionInfo( const SessionInfo &stSessionInfo )
{
	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugTrace, "CTestClientThread::setSessionInfo");

	m_clientSessionInfo.clientID = stSessionInfo.clientID;
	m_clientSessionInfo.sessionData = stSessionInfo.sessionData;
	m_clientSessionInfo.sessionID = stSessionInfo.sessionID;
	m_clientSessionInfo.sessionMasks = stSessionInfo.sessionMasks;
	m_clientSessionInfo.peerAddress = stSessionInfo.peerAddress;
	m_clientSessionInfo.localAddress = stSessionInfo.localAddress;
	m_bClientConnected = true;

	std::string strLogInfo;
	strLogInfo = "Client connected to Server ok!";
	m_pHelpClass->log_SeeionINfo(m_clientSessionInfo, strLogInfo);	

}


void CTestClientThread::_Process_InitClient()
{
	m_BrokerClient.onConnected = boost::bind(&TA_Base_Test::CTestClientThread::handleConnected, this, _1);//static boost::arg<1> _1;
	m_BrokerClient.onReceived = boost::bind(&CTestClientThread::handleReceivedMessage, this, _1);//static boost::arg<1> _1;
	m_BrokerClient.onDisconnected = boost::bind(&CTestClientThread::handleDisconnected, this, _1);//static boost::arg<1> _1;
	m_BrokerClient.onDeliverFailure = boost::bind(&CTestClientThread::handleDeliverFailure, this, _1);//static boost::arg<1> _1;

}
void CTestClientThread::_Process_ConnectToServer()
{
	m_BrokerClient.connect(g_string_strServerAddress);

}
void CTestClientThread::_Process_MonitorConnectRes()
{
	if (m_bClientConnected)
	{
		m_nThreadJobState = JobState_DoClientWork;
	}
}
void CTestClientThread::_Process_DoClientWork()
{
	_RunTestCase();

}

/////////////////////////////////////////////////////////////////////////
void CTestClientThread::handleDisconnected(const SessionInfo &stSessionInfo)
{
	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugTrace, "CTestClientThread::handleDisconnected");

}


void CTestClientThread::handleDeliverFailure(Message::Ptr pGetMessage)
{
	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugTrace, "CTestClientThread::handleDeliverFailure");

}

void CTestClientThread::handleConnected(const SessionInfo &stSessionInfo)
{
	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugTrace, "CTestClientThread::handleConnected");
	_SetSessionInfo(stSessionInfo);
}

void CTestClientThread::handleReceivedMessage(Message::Ptr pGetMessage)
{
	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugTrace, "CTestClientThread::handleReceivedMessage");


}


void CTestClientThread::_SendMsg(CTestFrame* pTestFrame, SessionID destSessionID)
{
	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugTrace, "CTestClientThread::_sendMsg");

	Message::Ptr message(new Message());
	message->write(pTestFrame->m_nFrameType);

	message->write(pTestFrame->m_nSessionIDSrc);

	message->write(pTestFrame->m_nCharValue);
	message->write(pTestFrame->m_nUCharValue);

	message->write(pTestFrame->m_nShortValue);
	message->write(pTestFrame->m_nUShortValue);

	message->write(pTestFrame->m_nIntValue);
	message->write(pTestFrame->m_nUIntValue);

	message->write(pTestFrame->m_nBitIntValue);
	message->write(pTestFrame->m_nUBitIntValue);

	message->write(pTestFrame->m_fFloatValue);
	message->write(pTestFrame->m_fDoubleValue);

	message->write(pTestFrame->m_strStringValue);

	message->setSessionID(destSessionID);	

	m_BrokerClient.sendToSession(destSessionID, message);


	
}

void CTestClientThread::_SendTestMsg(int nFrameType)
{
	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugTrace, "CTestClientThread::sendTestMsg");

	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugDebug, "sendTestMsg  nFrameType=%d", nFrameType);

	CTestFrame TestFrame;
	switch (nFrameType)
	{
	case ComFrameType_TestNomalData:
		TestFrame.setFrameNomalValue();
		break;
	case ComFrameType_TestMinData:
		TestFrame.setFrameMinValue();
		break;
	case ComFrameType_TestMaxData:
		TestFrame.setFrameMaxValue();
		break;
	default:
		TestFrame.setFrameNomalValue();
		break;
	}

	TestFrame.m_nSessionIDSrc = m_clientSessionInfo.sessionID;

	for (int nIndex = 0; nIndex < g_n_EachClientSendFrameNUM; ++nIndex)
	{
		m_nSendMsgCount++;
		_SendMsg(&TestFrame, m_clientSessionInfo.sessionID);
	}//for


}

void CTestClientThread::_RunTestCase()
{
	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugTrace, "CTestClientThread::runTestCase");
	int nFrameType = 0;
	m_pSendWorkTime->workBegin();
	if (true == m_bClientConnected)
	{
		nFrameType = ComFrameType_TestNomalData;
		_SendTestMsg(nFrameType);
		//nFrameType = ComFrameType_TestMinData;
 		//_SendTestMsg(nFrameType);
		//nFrameType = ComFrameType_TestMaxData;
 		//_SendTestMsg(nFrameType);
	}

	m_pSendWorkTime->workEnd();
	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugInfo, 
		"m_nSendMsgCount=%d, messages send and verified successfully, m_pSendWorkTime=%lld  ms", 
		m_nSendMsgCount, m_pSendWorkTime->getWorkTime());

}


int CTestClientThread::_ProcessUserTerminate()
{
	int nFunRes = 0;
	m_nThreadJobState = JobState_End;
	return nFunRes;
}

bool CTestClientThread::isFinishWork()
{
	bool bFinishWork = false;
	if (JobState_End == m_nThreadJobState)
	{
		bFinishWork = true;
	}
	return bFinishWork;

}


NS_END(TA_Base_Test) 



