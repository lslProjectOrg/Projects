#ifndef __CLASS_SERVER_WORKER_FOR_TEST_HH__
#define __CLASS_SERVER_WORKER_FOR_TEST_HH__

#include "CommonData.h"
#include <boost/chrono.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include "BoostThread.h"
#include "MessageBroker.h"
#include "UtilityFun.h"
#include "HelpClass.h"
using namespace cms;


NS_BEGIN(TA_Base_Test) 

class CWorkTimeLock;
class CClientSessionActor;
class CProcessClientReqHelperForTest;


class CServerWorkerForTest : public TA_Base_Test::CBoostThread
{
public:
	typedef   std::map<cms::SessionID, CClientSessionActor*>					MapSessinIDActorT;
	typedef   std::map<cms::SessionID, CClientSessionActor*>::iterator			MapSessinIDActorIterT;
	typedef   std::map<cms::SessionID, CClientSessionActor*>::value_type		MapSessinIDActorValueT;

	typedef   std::list<cms::Message::Ptr>               LstMessageT;
	typedef   std::list<cms::Message::Ptr>::iterator     LstMessageIterT;

private:
	enum EThreadJobState
	{
		JobState_Begin,
		JobState_InitServer,
		JobState_WaitMessage,
		JobState_PorcessMessage,
		JobState_UnInitServer,
		JobState_End,
	};

public:
	CServerWorkerForTest(void);
	~CServerWorkerForTest(void);

public:
	virtual void run();	
	virtual void terminate();
	bool  isFinishWork();
private:
	void _ThreadJob();
	int	 _ProcessUserTerminate();  


public:
	void handleConnected(const SessionInfo &stSessionInfo);
	void handleDisconnected(const SessionInfo &stSessionInfo);
	void handleReceivedMessage(Message::Ptr pGetMessage);
	void handleDeliverFailure(Message::Ptr pGetMessage);
private:
	int _Process_InitServer();
	int _Process_WaitMessage();
	int _Process_PorcessMessage();
	int _Process_UnInitServer();
private:
	int _GetMessageForPorcessing();
	int _PorcessingMessage();
	void _RemoveClient(const SessionInfo &stSessionInfo);
	void _AddClient(const SessionInfo &stSessionInfo);
	void _LogServerInfo();
	void _ReceiveMessage(Message::Ptr pGetMessage);
private:
	bool	m_toTerminate;
	EThreadJobState  m_nThreadJobState;


private:
	BigInt64			m_nTotalRecvMsgCount;


private:
	CWorkTimeLock* m_pRecvWorkTime;
	CHelpClass* m_pHelpClass;


private:
	boost::mutex		m_mutexMapClientsSessionIDActor;
	MapSessinIDActorT   m_mapClientsSessionIDActor;
	int					m_nClientsCount;
private:
	boost::mutex		m_mutexLstWaitProcessMessages;
	LstMessageT         m_lstWaitProcessMessages; 
	int					m_nWaitProcessMsgCount;


private:
	boost::mutex		m_mutexLstPorcessingMessages;
	LstMessageT         m_lstPorcessingMessages; 
	int					m_nPorcessingMsgCount;

private:
	boost::mutex		m_mutexWait;
	boost::condition_variable m_conditionWait;

private:
	CProcessClientReqHelperForTest* m_pProcessReqHelper;

};

NS_END(TA_Base_Test) 


#endif //__CLASS_SERVER_WORKER_FOR_TEST_HH__


