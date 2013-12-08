#include "StartClientThread.h"
#include "Logger.h"



//extern boost::condition_variable g_conditionMainRun;

using namespace TA_Base_Test;

NS_BEGIN(TA_Base_Test) 
//
//


CStartClientThread::CStartClientThread(int nClientIndex)
{
	m_nClientIndex = nClientIndex;
	m_toTerminate = false;
}

CStartClientThread::~CStartClientThread(void)
{

}


void CStartClientThread::run()
{
	m_nThreadJobState = JobState_Begin;

	while(false == m_toTerminate)
	{	
		_ThreadJob();
		TA_Base_Test::CBoostThread::sleep(DEF_INT_MonitorThreadSleep);
		
	}

	_ProcessUserTerminate();

}	

void CStartClientThread::_ThreadJob()
{
	switch (m_nThreadJobState)
	{
	case JobState_Begin:
		m_nThreadJobState = JobState_InitClient;
		break;
	case JobState_InitClient:
		_Process_InitClient();
		m_nThreadJobState = JobState_JustWait;
		break;
	case JobState_JustWait:
		break;
	case JobState_End:
		break;
	default:
		break;

	}//switch


}

void CStartClientThread::terminate(void)
{
	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugTrace, "CStartClientThread::terminate");

	m_toTerminate = true;
}




void CStartClientThread::_Process_InitClient()
{
	char szTemp[256];

	memset(szTemp, 0, 256);
	sprintf(szTemp, "TestCaseCMSClient.exe %d", m_nClientIndex);

	printf("begin system szTemp = %s \n", szTemp);

	system(szTemp);
}


int CStartClientThread::_ProcessUserTerminate()
{
	int nFunRes = 0;
	m_nThreadJobState = JobState_End;
	return nFunRes;
}

bool CStartClientThread::isFinishWork()
{
	bool bFinishWork = false;
	if (JobState_End == m_nThreadJobState)
	{
		bFinishWork = true;
	}
	return bFinishWork;

}


NS_END(TA_Base_Test) 



