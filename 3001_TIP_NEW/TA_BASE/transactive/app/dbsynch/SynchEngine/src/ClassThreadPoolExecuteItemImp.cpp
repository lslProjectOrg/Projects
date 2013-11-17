#include "ClassThreadPoolExecuteItemImp.h"

#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/DebugUtil.h"


#include "core/threads/src/Thread.h"
#include "core/synchronisation/src/ThreadGuard.h"


ClassThreadPoolExecuteItemImp::ClassThreadPoolExecuteItemImp(void)
{
	m_nJobState = JOB_STATE_NEW;
}


ClassThreadPoolExecuteItemImp::~ClassThreadPoolExecuteItemImp(void)
{
}

void ClassThreadPoolExecuteItemImp::newJob()
{
	{
		TA_THREADGUARD(m_LockJobState);
		m_nJobState = JOB_STATE_NEW;
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "newJob  m_isJobDone=%d", m_nJobState);
	}
}

ClassThreadPoolExecuteItemImp::JobState ClassThreadPoolExecuteItemImp::getJobState()
{
	{
		TA_THREADGUARD(m_LockJobState);
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugTrace, "getJobState  m_isJobDone=%d", m_nJobState);
		return m_nJobState;
	}
}

void ClassThreadPoolExecuteItemImp::executeWorkItem()
{
	{
		TA_THREADGUARD(m_LockJobState);
		m_nJobState = JOB_STATE_PROCEEING;
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "executeWorkItem  m_isJobDone=%d", m_nJobState);
	}

	LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "begin sleep 1000*30");

	TA_Base_Core::Thread::sleep(1000*30);
	LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "end sleep 1000*30");

	{
		TA_THREADGUARD(m_LockJobState);
		m_nJobState = JOB_STATE_DONE;
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "executeWorkItem  m_isJobDone=%d", m_nJobState);
	}

}
