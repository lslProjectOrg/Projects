#ifndef __CLASS_START_CLIENT_THREAD_HH__
#define __CLASS_START_CLIENT_THREAD_HH__

#include "CommonData.h"
#include "BoostThread.h"

NS_BEGIN(TA_Base_Test) 


class CStartClientThread : public TA_Base_Test::CBoostThread
{
private:
	enum EThreadJobState
	{
		JobState_Begin,
		JobState_InitClient,
		JobState_JustWait,
		JobState_End,
	};
public:
	CStartClientThread(int nClientIndex);
	~CStartClientThread(void);
public:
	virtual void run();	
	virtual void terminate();

public:
	bool  isFinishWork();
private:
	void _ThreadJob();
	int	  _ProcessUserTerminate();  

private:
	void _Process_InitClient();

private:
	int     m_nClientIndex;
	bool	m_toTerminate;
	EThreadJobState  m_nThreadJobState;

};

NS_END(TA_Base_Test) 


#endif //__CLASS_START_CLIENT_THREAD_HH__


