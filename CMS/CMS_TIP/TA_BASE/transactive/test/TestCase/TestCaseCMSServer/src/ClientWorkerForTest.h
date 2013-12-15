#ifndef __CLASS_TEST_CLIENT_THREAD__HH__
#define __CLASS_TEST_CLIENT_THREAD__HH__

#include "CommonData.h"
#include "BoostThread.h"
#include "MessageBroker.h"
#include "WorkTime.h"
#include "HelpClass.h"
#include "UtilityFun.h"
using namespace cms;


NS_BEGIN(TA_Base_Test) 

class CClientManager;
class CProcessServerAckHekperForTest;

class CClientWorkerForTest : public TA_Base_Test::CBoostThread
{
public:
	typedef   std::list<cms::Message::Ptr>               LstMessageT;
	typedef   std::list<cms::Message::Ptr>::iterator     LstMessageIterT;



public:
	typedef enum EProcessedFrameRes
	{
		ProcessedFrameRes_Begin = 0,
		FrameRes_NoData,


		FrameRes_LoginAck_CheckOK,
		FrameRes_LoginAck_CheckError,


		FrameRes_BeginTestAck_CheckOK,
		FrameRes_BeginTestAck_CheckError,


		FrameRes_DoTestAck_CheckOK,
		FrameRes_DoTestAck_CheckError,


		FrameRes_EndTestAck_CheckOK,
		FrameRes_EndTestAck_CheckError,


		FrameRes_LogOutAck_CheckOK,
		FrameRes_LogOutAck_CheckError,

		ProcessedFrameRes_End,
	}ProcessedFrameRes;

private:
	enum EThreadJobState
	{
		JobState_Begin,
		JobState_InitClient,
		JobState_SendConnectReq,
		JobState_WaitConnectAck,

		JobState_SendLoginReq,
		JobState_WaitLoginAck,


		JobState_SendBeginTestReq,
		JobState_WaitBeginTestAck,

		JobState_SendDoTestReq,
		JobState_WaitDoTestAck,


		JobState_SendEndTestReq,
		JobState_WaitEndTestAck,


		JobState_SendLogOutReq,
		JobState_WaitLogOutAck,


		JobState_FinishClientWork,
		JobState_UnInitClient,
		JobState_End,
	};



public:
	CClientWorkerForTest(int nClientIndex);
	~CClientWorkerForTest(void);
public:
	virtual void run();	
	virtual void terminate();

public:
	void handleConnected(const SessionInfo &stSessionInfo);
	void handleDisconnected(const SessionInfo &stSessionInfo);
	void handleReceivedMessage(Message::Ptr pGetMessage);
	void handleDeliverFailure(Message::Ptr pGetMessage);
public:
	void setClientManagerHandle(CClientManager* pClientManager);

public:
	bool  isFinishWork();
private:
	void _ThreadJob();
	int	 _ProcessUserTerminate();  



private:
	void _Process_InitClient();
	void _Process_SendConnectReq();
	void _Process_WaitConnectAck();

	int _Process_SendLoginReq();
	int _Process_WaitLoginAck();

	int _Process_SendBeginTestReq();
	int _Process_WaitBeginTestAck();

	int _Process_SendDoTestReq();
	int _Process_WaitDoTestAck();

	int _Process_SendEndTestReq();
	int _Process_WaitEndTestAck();

	int _Process_SendLogOutReq();
	int _Process_WaitLogOutAck();

private:
	void _SetSessionInfo(const SessionInfo &stSessionInfo);
	void _LogClientInfo();
	int _GetMessageForPorcessing();
	CClientWorkerForTest::ProcessedFrameRes _PorcessingMessage();
private:
	boost::mutex		m_mutexLstWaitProcessMessages;
	LstMessageT         m_lstWaitProcessMessages; 
	int					m_nWaitProcessMsgCount;

private:
	boost::mutex		m_mutexLstPorcessingMessages;
	LstMessageT         m_lstPorcessingMessages; 
	int					m_nPorcessingMsgCount;
	BigInt64                 m_nTotalRecvMsgCount;
private:
	SessionInfo m_clientSessionInfo;
	int    m_nSendMsgCount;
	int    m_nClientIndex;
	bool   m_bClientConnected;

private:
	bool	m_toTerminate;
	EThreadJobState  m_nThreadJobState;
	CWorkTimeLock* m_pDoTestWorkTime;
	CHelpClass* m_pHelpClass;
	CClientManager* m_pClientManager;
	CProcessServerAckHekperForTest* m_pProcessHelper;


	

};

NS_END(TA_Base_Test) 


#endif //__CLASS_TEST_CLIENT_THREAD__HH__


