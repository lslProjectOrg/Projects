#include "ClientWorkerForTest.h"
#include "HelpClass.h"
#include "ClientManager.h"

#include "ProcessServerAckHelperForTest.h"

#include "CTOSReqUserLoginFrame.h"
#include "STOCAckUserLoginFrame.h"
#include "CTOSReqBeginTestFrame.h"
#include "STOCAckBeginTestFrame.h"
#include "CTOSReqDoTestFrame.h"
#include "STOCAckDoTestFrame.h"
#include "CTOSReqEndTestFrame.h"
#include "STOCAckEndTestFrame.h"
#include "CTOSReqUserLogOutFrame.h"
#include "STOCAckUserLogOutFrame.h"


#include "BoostLogger.h"
USING_BOOST_LOG;

//extern boost::condition_variable g_conditionMainRun;

using namespace TA_Base_Test;

NS_BEGIN(TA_Base_Test) 
//
extern std::string g_string_strServerAddress;
extern int g_n_EachClientSendFrameNUM;
extern int g_n_ClientNUM;
extern int g_n_TotalClientSendFrameNUM;
//


CClientWorkerForTest::CClientWorkerForTest(int nClientIndex)
{
	BOOST_LOG_FUNCTION();


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

	m_nClientIndex = nClientIndex;
	m_pDoTestWorkTime = new CWorkTimeLock();
	m_pDoTestWorkTime->workBegin();

	m_nSendMsgCount = 0;
	m_bClientConnected = false;
	m_toTerminate = false;
	m_pHelpClass = new CHelpClass();
	m_pProcessHelper = new CProcessServerAckHekperForTest();
	_LogClientInfo();

}

CClientWorkerForTest::~CClientWorkerForTest(void)
{
	BOOST_LOG_FUNCTION();

	DEF_DELETE(m_pDoTestWorkTime);
	DEF_DELETE(m_pHelpClass);

	DEF_DELETE(m_pProcessHelper);
	_LogClientInfo();

}


void CClientWorkerForTest::run()
{
	BOOST_LOG_FUNCTION();

	m_nThreadJobState = JobState_Begin;


	while(false == m_toTerminate)
	{		
		_ThreadJob();
		TA_Base_Test::CBoostThread::sleep(DEF_INT_MonitorThreadSleep);
		
	}

	_ProcessUserTerminate();

}	

void CClientWorkerForTest::_ThreadJob()
{
	BOOST_LOG_FUNCTION();

	switch (m_nThreadJobState)
	{
	case JobState_Begin:
		m_nThreadJobState = JobState_InitClient;
		break;
	case JobState_InitClient:
		_Process_InitClient();
		m_nThreadJobState = JobState_SendConnectReq;
		break;

	case JobState_SendConnectReq:
		_Process_SendConnectReq();
		m_nThreadJobState = JobState_WaitConnectAck;
		break;
	case JobState_WaitConnectAck:
		_Process_WaitConnectAck();
		break;

	case JobState_SendLoginReq:
		_Process_SendLoginReq();
		m_nThreadJobState = JobState_WaitLoginAck;
		break;
	case JobState_WaitLoginAck:
		_Process_WaitLoginAck();
		break;

	case JobState_SendBeginTestReq:
		_Process_SendBeginTestReq();
		m_nThreadJobState = JobState_WaitBeginTestAck;
		break;
	case JobState_WaitBeginTestAck:
		_Process_WaitBeginTestAck();
		break;


	case JobState_SendDoTestReq:
		if (m_pDoTestWorkTime->getNotworkTime() > TIME_MIN_FIVE)//millsecs
		{
			m_pDoTestWorkTime->workBegin();
			_Process_SendDoTestReq();			
			m_nThreadJobState = JobState_WaitDoTestAck;
		}
		break;
	case JobState_WaitDoTestAck:
		_Process_WaitDoTestAck();
		break;

	case JobState_SendLogOutReq:
		_Process_SendLogOutReq();
		m_nThreadJobState = JobState_WaitLogOutAck;
		break;
	case JobState_WaitLogOutAck:
		_Process_WaitLogOutAck();
		break;

	case JobState_FinishClientWork:
		m_nThreadJobState = JobState_UnInitClient;
		break;
	case JobState_UnInitClient:
		_LogClientInfo();
		m_pClientManager->disConnectToServer(m_clientSessionInfo.sessionID);
		m_nThreadJobState = JobState_End;
			break;
	case JobState_End:
		break;
	default:
		break;

	}//switch


}

void CClientWorkerForTest::terminate(void)
{
	BOOST_LOG_FUNCTION();

	m_toTerminate = true;
}



void CClientWorkerForTest::_SetSessionInfo( const SessionInfo &stSessionInfo )
{
	BOOST_LOG_FUNCTION();

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

//////////////////////////////////////////////////////////////////////////
void CClientWorkerForTest::_Process_InitClient()
{
	m_pClientManager->initClient();
	_LogClientInfo();

}
void CClientWorkerForTest::_Process_SendConnectReq()
{
	m_bClientConnected = false;
	m_pClientManager->connectToServer();
	_LogClientInfo();

}
void CClientWorkerForTest::_Process_WaitConnectAck()
{
	if (m_bClientConnected)
	{
		_LogClientInfo();
		m_nThreadJobState = JobState_SendLoginReq;
	}
}



int CClientWorkerForTest::_Process_SendLoginReq()
{
	int nFunRes = 0;
	CTOSReqUserLoginFrame  CTOSReqUserLoginFrameTmp;
	CTOSReqUserLoginFrameTmp.setSampleValue();
	cms::Message::Ptr pMessage = CTOSReqUserLoginFrameTmp.getMessage();
	m_pClientManager->sendData(m_clientSessionInfo.sessionID, pMessage);

	return nFunRes;
}

int CClientWorkerForTest::_Process_WaitLoginAck()
{
	int nFunRes = 0;
	ProcessedFrameRes nProcessedFrameRes = FrameRes_NoData;

	nProcessedFrameRes = _PorcessingMessage();

	if (FrameRes_NoData == nProcessedFrameRes)
	{
		return nFunRes;//continue wait
	}
	else if (FrameRes_LoginAck_CheckOK == nProcessedFrameRes)
	{
		m_nThreadJobState = JobState_SendBeginTestReq;
		return nFunRes;//stop wait
	}
	else if (FrameRes_LoginAck_CheckError == nProcessedFrameRes)
	{
		m_nThreadJobState = JobState_UnInitClient;
		return nFunRes;//continue wait
	}
	else
	{
		LOG_ERROR<<"_Process_WaitLoginAck  not expected Res";
	}


	return nFunRes;
}

int CClientWorkerForTest::_Process_SendBeginTestReq()
{
	int nFunRes = 0;
	CTOSReqBeginTestFrame  CTOSReqBeginTestFrameTmp;
	CTOSReqBeginTestFrameTmp.setSampleValue();
	cms::Message::Ptr pMessage = CTOSReqBeginTestFrameTmp.getMessage();
	m_pClientManager->sendData(m_clientSessionInfo.sessionID, pMessage);

	return nFunRes;
}

int CClientWorkerForTest::_Process_WaitBeginTestAck()
{
	int nFunRes = 0;
	ProcessedFrameRes nProcessedFrameRes = FrameRes_NoData;

	nProcessedFrameRes = _PorcessingMessage();

	if (FrameRes_NoData == nProcessedFrameRes)
	{
		return nFunRes;//continue wait
	}
	else if (FrameRes_BeginTestAck_CheckOK == nProcessedFrameRes)
	{
		m_nThreadJobState = JobState_SendDoTestReq;
		return nFunRes;//stop wait
	}
	else if (FrameRes_BeginTestAck_CheckError == nProcessedFrameRes)
	{
		m_nThreadJobState = JobState_UnInitClient;
		return nFunRes;//continue wait
	}
	else
	{
		LOG_ERROR<<"_Process_WaitBeginTestAck  not expected Res";
	}


	return nFunRes;
}

int CClientWorkerForTest::_Process_SendDoTestReq()
{
	int nFunRes = 0;
	
	CTOSReqDoTestFrame  CTOSReqDoTestFrameTmp;
	CTOSReqDoTestFrameTmp.setSampleValue();
	cms::Message::Ptr pMessage = CTOSReqDoTestFrameTmp.getMessage();
	m_pClientManager->sendData(m_clientSessionInfo.sessionID, pMessage);
	_LogClientInfo();

	return nFunRes;
}

int CClientWorkerForTest::_Process_WaitDoTestAck()
{
	int nFunRes = 0;
	ProcessedFrameRes nProcessedFrameRes = FrameRes_NoData;

	nProcessedFrameRes = _PorcessingMessage();

	if (FrameRes_NoData == nProcessedFrameRes)
	{
		return nFunRes;//continue wait
	}
	else if (FrameRes_DoTestAck_CheckOK == nProcessedFrameRes)
	{
		_LogClientInfo();

		m_nThreadJobState = JobState_SendDoTestReq;
		return nFunRes;//stop wait
	}
	else if (FrameRes_DoTestAck_CheckError == nProcessedFrameRes)
	{
		m_nThreadJobState = JobState_UnInitClient;
		return nFunRes;//continue wait
	}
	else
	{
		LOG_ERROR<<"_Process_WaitDoTestAck  not expected Res";
	}


	return nFunRes;
}

int CClientWorkerForTest::_Process_SendEndTestReq()
{
	int nFunRes = 0;
	CTOSReqEndTestFrame  CTOSReqEndTestFrameTmp;
	CTOSReqEndTestFrameTmp.setSampleValue();
	cms::Message::Ptr pMessage = CTOSReqEndTestFrameTmp.getMessage();
	m_pClientManager->sendData(m_clientSessionInfo.sessionID, pMessage);

	return nFunRes;
}

int CClientWorkerForTest::_Process_WaitEndTestAck()
{
	int nFunRes = 0;
	ProcessedFrameRes nProcessedFrameRes = FrameRes_NoData;

	nProcessedFrameRes = _PorcessingMessage();

	if (FrameRes_NoData == nProcessedFrameRes)
	{
		return nFunRes;//continue wait
	}
	else if (FrameRes_EndTestAck_CheckOK == nProcessedFrameRes)
	{
		m_nThreadJobState = JobState_SendLogOutReq;
		return nFunRes;//stop wait
	}
	else if (FrameRes_EndTestAck_CheckError == nProcessedFrameRes)
	{
		m_nThreadJobState = JobState_UnInitClient;
		return nFunRes;//continue wait
	}
	else
	{
		LOG_ERROR<<"_Process_WaitEndTestAck  not expected Res";
	}


	return nFunRes;
}


int CClientWorkerForTest::_Process_SendLogOutReq()
{
	int nFunRes = 0;
	CTOSReqUserLogOutFrame  CTOSReqUserLogOutFrameTmp;
	CTOSReqUserLogOutFrameTmp.setSampleValue();
	cms::Message::Ptr pMessage = CTOSReqUserLogOutFrameTmp.getMessage();
	m_pClientManager->sendData(m_clientSessionInfo.sessionID, pMessage);

	return nFunRes;
}

int CClientWorkerForTest::_Process_WaitLogOutAck()
{
	int nFunRes = 0;
	ProcessedFrameRes nProcessedFrameRes = FrameRes_NoData;

	nProcessedFrameRes = _PorcessingMessage();

	if (FrameRes_NoData == nProcessedFrameRes)
	{
		return nFunRes;//continue wait
	}
	else if (FrameRes_LogOutAck_CheckOK == nProcessedFrameRes)
	{
		m_nThreadJobState = JobState_UnInitClient;
		return nFunRes;//stop wait
	}
	else if (FrameRes_LogOutAck_CheckError == nProcessedFrameRes)
	{
		m_nThreadJobState = JobState_UnInitClient;
		return nFunRes;//continue wait
	}
	else
	{
		LOG_ERROR<<"_Process_WaitLogOutAck  not expected Res";
	}


	return nFunRes;
}

/////////////////////////////////////////////////////////////////////////
void CClientWorkerForTest::handleDisconnected(const SessionInfo &stSessionInfo)
{
	BOOST_LOG_FUNCTION();
	m_nThreadJobState = JobState_UnInitClient;

}


void CClientWorkerForTest::handleDeliverFailure(Message::Ptr pGetMessage)
{
	BOOST_LOG_FUNCTION();

}

void CClientWorkerForTest::handleConnected(const SessionInfo &stSessionInfo)
{
	BOOST_LOG_FUNCTION();
	_SetSessionInfo(stSessionInfo);
}

void CClientWorkerForTest::handleReceivedMessage(Message::Ptr pGetMessage)
{
	BOOST_LOG_FUNCTION();

	{
		boost::mutex::scoped_lock lock(m_mutexLstWaitProcessMessages);
		m_lstWaitProcessMessages.push_back(pGetMessage);
		m_nWaitProcessMsgCount++;
		m_nTotalRecvMsgCount++;
		_LogClientInfo();
	}
}


int CClientWorkerForTest::_ProcessUserTerminate()
{
	BOOST_LOG_FUNCTION();

	int nFunRes = 0;
	m_nThreadJobState = JobState_End;
	return nFunRes;
}

bool CClientWorkerForTest::isFinishWork()
{
	BOOST_LOG_FUNCTION();

	bool bFinishWork = false;
	if (JobState_End == m_nThreadJobState)
	{
		bFinishWork = true;
	}
	return bFinishWork;

}

void CClientWorkerForTest::setClientManagerHandle( CClientManager* pClientManager )
{
	m_pClientManager = pClientManager;
}



int CClientWorkerForTest::_GetMessageForPorcessing()
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
			_LogClientInfo();

			iterList++;
		}
		m_lstWaitProcessMessages.clear();

	}

	return nFunRes;
}

void CClientWorkerForTest::_LogClientInfo()
{
	LOG_DEBUG<<"ClientInfo:"
		<<" "<<"m_nClientIndex="<<m_nClientIndex
		<<" "<<"m_bClientConnected="<<m_bClientConnected		
		<<" "<<"m_nTotalRecvMsgCount="<<m_nTotalRecvMsgCount
		<<" "<<"m_nWaitProcessMsgCount="<<m_nWaitProcessMsgCount
		<<" "<<"m_nPorcessingMsgCount="<<m_nPorcessingMsgCount;
}

CClientWorkerForTest::ProcessedFrameRes CClientWorkerForTest::_PorcessingMessage()
{
	ProcessedFrameRes nFunRes = ProcessedFrameRes_Begin;

	LstMessageIterT iterList;

	_GetMessageForPorcessing();


	{
		boost::mutex::scoped_lock lockPorcess(m_mutexLstPorcessingMessages);
		if (m_lstPorcessingMessages.empty())
		{
			nFunRes = FrameRes_NoData;
			return nFunRes;//continue wait
		}
	}



	{
		boost::mutex::scoped_lock lockPorcess(m_mutexLstPorcessingMessages);

		iterList = m_lstPorcessingMessages.begin();
		if (iterList != m_lstPorcessingMessages.end())
		{
			cms::Message::Ptr pMessage = (*iterList);

			//TODO ....
			nFunRes = m_pProcessHelper->processMessage(pMessage);

			m_nPorcessingMsgCount--;
			_LogClientInfo();

			m_lstPorcessingMessages.erase(iterList);
		}



	}

	return nFunRes;
}


NS_END(TA_Base_Test) 














