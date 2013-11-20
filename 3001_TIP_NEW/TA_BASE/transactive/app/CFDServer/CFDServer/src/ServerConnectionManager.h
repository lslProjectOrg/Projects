#ifndef __CLASS_SERVER_CONNECTION_MANAGER__HH__
#define __CLASS_SERVER_CONNECTION_MANAGER__HH__



#include "CommonData.h"
#include "core/threads/src/Thread.h"
#include "core/synchronisation/src/NonReEntrantThreadLockable.h"

NS_BEGIN(TA_Base_App)


class CServerConnectionManager : public TA_Base_Core::Thread
{

private:
	enum EThreadJobState
	{
		Job_State_Begin,

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

		
private:
	bool	m_toTerminate;
	EThreadJobState m_nThreadJobState;	
private:
	int    _ThreadJob();
	
//////////////////////////////////////////////////////////////////////////
public:
	bool  isFinishWork();

private:
	int	  _ProcessUserTerminate();  
};


NS_END(TA_Base_App)

#endif  // __CLASS_SERVER_CONNECTION_MANAGER__HH__
