#include "DataAccessTestWorker.h"
#include "core/utilities/src/BoostLogger.h"
USING_BOOST_LOG;



NS_BEGIN(TA_Base_Test) 




//////////////////////////////////////////////////////////////////////////
CDataAccesssTestWorker::CDataAccesssTestWorker(void)
{	
	BOOST_LOG_FUNCTION();	


}

CDataAccesssTestWorker::~CDataAccesssTestWorker(void)
{
	BOOST_LOG_FUNCTION();

}


void CDataAccesssTestWorker::run()
{
	BOOST_LOG_FUNCTION();

	m_nThreadJobState = JobState_Begin;


	while (false == m_toTerminate)
	{
		_ThreadJob();
		//TA_Base_Test::CBoostThread::sleep(DEF_INT_MonitorThreadSleep);

	}

	_ProcessUserTerminate();
}

void CDataAccesssTestWorker::terminate()
{
	BOOST_LOG_FUNCTION();

	m_toTerminate = true;
}


int CDataAccesssTestWorker::_ProcessUserTerminate()
{
	BOOST_LOG_FUNCTION();

	int nFunRes = 0;
	m_nThreadJobState = JobState_End;
	return nFunRes;
}

bool CDataAccesssTestWorker::isFinishWork()
{
	BOOST_LOG_FUNCTION();

	bool bFinishWork = false;
	if (JobState_End == m_nThreadJobState)
	{
		bFinishWork = true;
	}
	return bFinishWork;

}

void CDataAccesssTestWorker::_ThreadJob()
{
	BOOST_LOG_FUNCTION();

	switch (m_nThreadJobState)
	{
	case JobState_Begin:
		break;

	case JobState_End:
		break;
	default:
		TA_Base_Test::CBoostThread::sleep(1000);
		break;

	}//switch
}

NS_END(TA_Base_Test) 



