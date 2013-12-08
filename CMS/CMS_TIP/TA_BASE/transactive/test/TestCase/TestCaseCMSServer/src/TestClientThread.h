#ifndef __CLASS_TEST_CLIENT_THREAD__HH__
#define __CLASS_TEST_CLIENT_THREAD__HH__

#include "CommonData.h"
#include "BoostThread.h"
#include "MessageBroker.h"
#include "WorkTime.h"
#include "HelpClass.h"
#include "UtilityFun.h"
#include "TestFrame.h"
using namespace cms;


NS_BEGIN(TA_Base_Test) 

class CClientManager;

class CTestClientThread : public TA_Base_Test::CBoostThread
{
private:
	enum EThreadJobState
	{
		JobState_Begin,
		JobState_InitClient,
		JobState_ConnectToServer,
		JobState_MonitorConnectRes,
		JobState_DoClientWork,
		JobState_FinishClientWork,
		JobState_UnInitClient,
		JobState_End,
	};
public:
	CTestClientThread(int nClientIndex);
	~CTestClientThread(void);
public:
	virtual void run();	
	virtual void terminate();



public:
	bool  isFinishWork();
private:
	void _ThreadJob();
	int	  _ProcessUserTerminate();  


private:
	void handleConnected(const SessionInfo &stSessionInfo);
	void handleDisconnected(const SessionInfo &stSessionInfo);
	void handleReceivedMessage(Message::Ptr pGetMessage);
	void handleDeliverFailure(Message::Ptr pGetMessage);

private:
	void _Process_InitClient();
	void _Process_ConnectToServer();
	void _Process_MonitorConnectRes();
	void _Process_DoClientWork();

private:
	void _SendTestMsg(int nFrameType);
	void _SendMsg(CTestFrame* pTestFrame, SessionID destSessionID);
	void _RunTestCase();
	void _SetSessionInfo(const SessionInfo &stSessionInfo);

private:
	SessionInfo m_clientSessionInfo;
	MessageBroker m_BrokerClient;
	int    m_nSendMsgCount;
	int    m_nRecvMsgCount;
	bool   m_bClientConnected;
	int    m_nClientIndex;

private:
	bool	m_toTerminate;
	EThreadJobState  m_nThreadJobState;
	CWorkTimeLock* m_pSendWorkTime;
	CHelpClass* m_pHelpClass;

};

NS_END(TA_Base_Test) 


#endif //__CLASS_TEST_CLIENT_THREAD__HH__


