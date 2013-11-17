#ifndef __TEST_CLASS_ThreadPoolExecuteItem_IMP__HH__
#define __TEST_CLASS_ThreadPoolExecuteItem_IMP__HH__



#include "core/threads/src/IThreadPoolBase.h"
#include "core/synchronisation/src/ReEntrantThreadLockable.h"



class ClassThreadPoolExecuteItemImp : public TA_Base_Core::IThreadPoolExecuteItem
{
public:
	typedef enum  emum_JOB_STATE
	{
		JOB_STATE_NEW,
		JOB_STATE_PROCEEING,
		JOB_STATE_DONE,
	}JobState;

public:
	ClassThreadPoolExecuteItemImp(void);
	~ClassThreadPoolExecuteItemImp(void);

	virtual void executeWorkItem() ;


public:
	void ClassThreadPoolExecuteItemImp::newJob();
	ClassThreadPoolExecuteItemImp::JobState ClassThreadPoolExecuteItemImp::getJobState();

private:
	TA_Base_Core::ReEntrantThreadLockable m_LockJobState;
	JobState m_nJobState;


};





#endif  //__TEST_CLASS_ThreadPoolExecuteItem_IMP__HH__




