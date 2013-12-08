#ifndef __CLASS_SERVER_MAMAGER__HH__
#define __CLASS_SERVER_MAMAGER__HH__

#include "CommonData.h"
#include <boost/chrono.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include "MessageBroker.h"
#include "UtilityFun.h"
#include "HelpClass.h"
using namespace cms;


NS_BEGIN(TA_Base_Test) 
class CWorkTimeLock;

class CServerManager
{
public:
	CServerManager(void);
	~CServerManager(void);

public:
	void runTestCase();

private:
	void handleConnected(const SessionInfo &stSessionInfo);
	void handleDisconnected(const SessionInfo &stSessionInfo);
	void handleReceivedMessage(Message::Ptr pGetMessage);
	void handleDeliverFailure(Message::Ptr pGetMessage);
private:
	void _InitServer();
private:
	CServerManager*   m_pThisServerManager;

private:
	boost::mutex m_mutexBrokerServer;
	MessageBroker m_brokerServer;

private:
	int    m_nHandleConnections;
	int    m_nRecvMsgCount;
	int    m_nSendMsgCount;

	CWorkTimeLock* m_pRecvWorkTime;
	CHelpClass* m_pHelpClass;


};

NS_END(TA_Base_Test) 


#endif //__CLASS_SERVER_MAMAGER__HH__


