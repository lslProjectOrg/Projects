#ifndef __CLASS_SERVER_MAMAGER__HH__
#define __CLASS_SERVER_MAMAGER__HH__

#include "app/dataserver/cfd_server/src/CFDServerCommonData.h"

#include <boost/chrono.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include "MessageBroker.h"

using namespace cms;


NS_BEGIN(TA_Base_App) 

class CServerWorkerForTest;

class CServerManager 
{
public:
	static CServerManager& getInstance();
	static void removeInstance();

public:
	int sendData(const cms::SessionID& destSessionID, cms::Message::Ptr pMessage);


private:
	static CServerManager*                      m_pInstance;
	static boost::mutex							m_instanceLock;
private:
	CServerManager(void);
	~CServerManager(void);
	CServerManager(const CServerManager& ServerManager); 
	CServerManager& operator=(const CServerManager& ServerManager);



private:
	int _InitServer();

private:
	void handleConnected(const SessionInfo &stSessionInfo);
	void handleDisconnected(const SessionInfo &stSessionInfo);
	void handleReceivedMessage(Message::Ptr pGetMessage);
	void handleDeliverFailure(Message::Ptr pGetMessage);

	
private:
	boost::mutex m_mutexBrokerServer;
	MessageBroker m_brokerServer;

private:
	CServerWorkerForTest* m_pServerWorkerForTest;
};

NS_END(TA_Base_App) 


#endif //__CLASS_SERVER_MAMAGER__HH__


