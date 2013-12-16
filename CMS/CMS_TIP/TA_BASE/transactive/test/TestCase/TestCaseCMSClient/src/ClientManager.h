#ifndef __CLASS_CLIENT_MAMAGER__HH__
#define __CLASS_CLIENT_MAMAGER__HH__

#include "CommonData.h"
#include <boost/chrono.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include "MessageBroker.h"
#include "UtilityFun.h"
#include "BoostThread.h"
using namespace cms;


NS_BEGIN(TA_Base_Test) 

class CClientWorkerForTest;

class CClientManager 
{
public:
	CClientManager(int nClientIndex);
	~CClientManager(void);

private:
	void handleConnected(const SessionInfo &stSessionInfo);
	void handleDisconnected(const SessionInfo &stSessionInfo);
	void handleReceivedMessage(Message::Ptr pGetMessage);
	void handleDeliverFailure(Message::Ptr pGetMessage);

public:
	int initClient();
	int connectToServer();
	int sendData(const cms::SessionID& destSessionID, cms::Message::Ptr pMessage);
	int disConnectToServer(const cms::SessionID& destSessionID);
	bool isFinishWork();
private:
	boost::mutex m_mutexBrokerClient;
	MessageBroker m_BrokerClient;
	CClientWorkerForTest* m_pClientWorkerForTest;
	int m_nClientIndex;
};

NS_END(TA_Base_Test) 


#endif //__CLASS_CLIENT_MAMAGER__HH__


