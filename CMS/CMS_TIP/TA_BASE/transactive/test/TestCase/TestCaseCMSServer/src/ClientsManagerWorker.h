#ifndef __CLASS_CLIENTS_MAMAGER_WORKER__HH__
#define __CLASS_CLIENTS_MAMAGER_WORKER__HH__

#include "CommonData.h"
#include <boost/chrono.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include "MessageBroker.h"
#include "UtilityFun.h"
#include "BoostThread.h"
using namespace cms;


NS_BEGIN(TA_Base_Test) 

class CClientManager;

class CClientsManagerWorker : public TA_Base_Test::CBoostThread
{
private:
	enum EThreadJobState
	{
		JobState_Begin,
		JobState_CreateALLClient,
		JobState_MonitorAllClient,
		JobState_End,
	};
public:
	CClientsManagerWorker(void);
	~CClientsManagerWorker(void);
public:
	virtual void run();	
	virtual void terminate();
	bool  isFinishWork();
private:
	void _ThreadJob();
	int	 _ProcessUserTerminate();  

private:
	void _CreateALLClient();
	void _MonitorALLClient();
	void _DestoryAllClient();
private:
	bool	m_toTerminate;
	EThreadJobState  m_nThreadJobState;
private:
	boost::mutex m_mutexBrokerClient;
	MessageBroker m_BrokerClient;


private:
	int    m_nClientCount;
	int    m_nCurrentClientIndex;
	std::map<int, CClientManager*> m_mapClients;
	boost::mutex m_mutexMapClients;
};

NS_END(TA_Base_Test) 


#endif //__CLASS_CLIENTS_MAMAGER_WORKER__HH__


