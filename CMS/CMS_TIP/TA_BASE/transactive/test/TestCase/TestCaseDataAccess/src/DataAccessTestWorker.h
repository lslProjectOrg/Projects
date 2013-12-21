#ifndef __CLASS_DATA_ACCESS_TEST_WORKDER__HH__
#define __CLASS_DATA_ACCESS_TEST_WORKDER__HH__

#include "core/utilities/src/UtilitiesCommonData.h"
#include "core/threads/src/BoostThread.h"
#include <boost/chrono.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>


NS_BEGIN(TA_Base_Test) 



class CDataAccesssTestWorker : public TA_Base_Core::CBoostThread
{
private:
	enum EThreadJobState
	{
		JobState_Begin,
		JobState_DO_LogTest,
		JobState_DO_ThreadTest,
		JobState_DO_SQliteTest,
		JobState_DO_DataAccessTest,
		JobState_End,
	};

public:
	CDataAccesssTestWorker(void);
	~CDataAccesssTestWorker(void);

public:
	virtual void run();	
	virtual void terminate();
	bool  isFinishWork();
private:
	void _ThreadJob();
	int	 _ProcessUserTerminate(); 

private:
	void _Process_JobState_DO_LogTest();
	void _Process_JobState_DO_ThreadTest();
	void _Process_JobState_DO_SQliteTest();
	void _Process_JobState_DO_DataAccessTest();
private:
	bool	m_toTerminate;
	EThreadJobState  m_nThreadJobState;
};

NS_END(TA_Base_Test) 


#endif //__CLASS_SERVER_WORKER_FOR_TEST_HH__


