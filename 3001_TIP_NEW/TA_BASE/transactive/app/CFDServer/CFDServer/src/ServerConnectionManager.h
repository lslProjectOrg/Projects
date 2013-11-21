#ifndef __CLASS_SERVER_CONNECTION_MANAGER__HH__
#define __CLASS_SERVER_CONNECTION_MANAGER__HH__



#include "CommonData.h"
#include "core/threads/src/Thread.h"
#include "core/synchronisation/src/NonReEntrantThreadLockable.h"
#include "core/synchronisation/src/TASemaphore.h"



NS_BEGIN(TA_Base_App)

class CConnectionActor;

class CServerConnectionManager : public TA_Base_Core::Thread
{

private:
	enum EThreadJobState
	{
		Job_State_Begin,

		Job_State_WaitConActor,
		Job_State_ProcessConActor,

		Job_State_End,
	};

public:
	static CServerConnectionManager& getInstance();
	static void removeInstance();

private:
	static CServerConnectionManager*                      m_pInstance;
	static TA_Base_Core::NonReEntrantThreadLockable	     m_instanceLock;
private:
	CServerConnectionManager(void);	
	~CServerConnectionManager(void);

	DEF_DISABLE_COPY_AND_ASSIGNMENT(CServerConnectionManager);



public:
	virtual void run() ;	
	virtual void terminate() ;

	bool  isFinishWork();
public:
	void  addActor(CConnectionActor* pActor);

private:
	int   _ThreadJob();
	int	  _ProcessUserTerminate();

	void _InitListActor();
	void _UnInitListActor();

	int _Process_ProcessConActor();
	int _Process_WaitConActor();
private:
	bool	m_toTerminate;
	EThreadJobState m_nThreadJobState;	
	TA_Base_Core::CTASemaphore m_semaphore;

	TA_Base_Core::NonReEntrantThreadLockable	     m_LockListConActor;
	std::list<CConnectionActor*>* m_pListConActor;
	int                           m_nListConActorSize;


};


NS_END(TA_Base_App)

#endif  // __CLASS_SERVER_CONNECTION_MANAGER__HH__
