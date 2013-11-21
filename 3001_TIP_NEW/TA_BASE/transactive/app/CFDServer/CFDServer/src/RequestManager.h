#ifndef __CLASS_REQUEST_MANAGER__HH__
#define __CLASS_REQUEST_MANAGER__HH__



#include "CommonData.h"
#include "core/threads/src/Thread.h"
#include "core/synchronisation/src/NonReEntrantThreadLockable.h"
#include "core/synchronisation/src/TASemaphore.h"

NS_BEGIN(TA_Base_Core)
class LFThreadPoolManager;
NS_END(TA_Base_Core)

NS_BEGIN(TA_Base_App)

class CConnectionActor;

class CRequestManager : public TA_Base_Core::Thread
{

private:
	enum EThreadJobState
	{
		Job_State_Begin,

		Job_State_WaitRequest,
		Job_State_ProcessRequest,
		Job_State_MonitorRequest,

		Job_State_End,
	};

public:
	static CRequestManager& getInstance();
	static void removeInstance();

private:
	static CRequestManager*                      m_pInstance;
	static TA_Base_Core::NonReEntrantThreadLockable	     m_instanceLock;
private:
	CRequestManager(void);	
	~CRequestManager(void);

	DEF_DISABLE_COPY_AND_ASSIGNMENT(CRequestManager);



public:
	virtual void run() ;	
	virtual void terminate() ;

	bool  isFinishWork();
	
public:
	void addActor( CConnectionActor* pActor );
	void removeActor( CConnectionActor* pActor );


private:
	int	_ThreadJob();
	int	_ProcessUserTerminate();  
	void _UnInitListActor();
	void _InitListActor();

private:
	int	_Process_WaitRequest();
	int	_Process_ProcessRequest();
	int	_Process_MonitorRequest();
private:
	bool	m_toTerminate;
	EThreadJobState m_nThreadJobState;	
	TA_Base_Core::CTASemaphore m_semaphore;

private:
	TA_Base_Core::NonReEntrantThreadLockable	     m_LockListConActor;
	std::list<CConnectionActor*>* m_pListProcessConActor;
	std::list<CConnectionActor*>* m_pListMonitorConActor;
	int m_nListConActorSize;

private:
	TA_Base_Core::LFThreadPoolManager* m_pThreadPoolManager;
	int m_nNumberOfThreads;
	bool m_bIsAutoRun;
};


NS_END(TA_Base_App)

#endif  // __CLASS_REQUEST_MANAGER__HH__
