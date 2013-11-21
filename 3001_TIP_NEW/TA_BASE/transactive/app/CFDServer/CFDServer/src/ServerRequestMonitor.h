#ifndef __CLASS_SERVER_REQUEST_MONITOR__HH__
#define __CLASS_SERVER_REQUEST_MONITOR__HH__



#include "CommonData.h"
#include "core/threads/src/Thread.h"

#include "core/sockets/src/TcpSocket.h"
#include "core/sockets/src/TcpServerSocket.h"
#include "core/sockets/src/SocketSet.h"

#include "core/synchronisation/src/NonReEntrantThreadLockable.h"
#include "core/synchronisation/src/TASemaphore.h"
NS_BEGIN(TA_Base_App)

class CConnectionActor;

class CServerRequestMonitor : public TA_Base_Core::Thread
{

private:
	enum EThreadJobState
	{
		Job_State_Begin,

		Job_State_waitConActor,
		Job_State_MonitorData,
		Job_State_MonitorRequest,

		Job_State_End,
	};
private:
	typedef std::map<TA_Base_Core::TcpSocket*, CConnectionActor*>               MapSocketConActorT;
	typedef std::map<TA_Base_Core::TcpSocket*, CConnectionActor*>::iterator     MapSocketConActorIterT;
	typedef std::map<TA_Base_Core::TcpSocket*, CConnectionActor*>::value_type   MapSocketConActorValueTypeT;

public:
	static CServerRequestMonitor& getInstance();
	static void removeInstance();

private:
	static CServerRequestMonitor*                      m_pInstance;
	static TA_Base_Core::NonReEntrantThreadLockable	     m_instanceLock;
private:
	CServerRequestMonitor(void);	
	~CServerRequestMonitor(void);

	DEF_DISABLE_COPY_AND_ASSIGNMENT(CServerRequestMonitor);



public:
	virtual void run() ;	
	virtual void terminate() ;
	bool  isFinishWork();

public:
	void  addActor(CConnectionActor* pActor);
	

private:
	int _ThreadJob();
	int _ProcessUserTerminate();  

	void _UnInitMap();
	void _IniMap();
	int _Process_MonitorData();
	int _Process_MonitorRequest();

private:
	bool	m_toTerminate;
	EThreadJobState m_nThreadJobState;	
	TA_Base_Core::CTASemaphore m_semaphore;

private:
	TA_Base_Core::NonReEntrantThreadLockable	     m_LockMapSocketConActor;
	MapSocketConActorT* m_pMapSocketConActor;
	MapSocketConActorT* m_pMapSocketConActorTmp;

	int m_nListConActorSize;

private:	
	TA_Base_Core::SocketSet< TA_Base_Core::TcpSocket >	m_pSetSocket;//server Moniter socket Data
	bool 	      m_bTCPRead;
	bool		  m_bTCPWrite; 
	int			  m_nTCPSelectRes; 
	unsigned int  m_timeTCPWait;//MilliSeconds
};


NS_END(TA_Base_App)

#endif  // __CLASS_SERVER_REQUEST_MONITOR__HH__
