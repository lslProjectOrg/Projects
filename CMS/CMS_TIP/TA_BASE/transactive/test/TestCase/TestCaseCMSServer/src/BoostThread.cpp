#include "BoostThread.h"
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include "Logger.h"
#include "BoostLogger.h"

NS_BEGIN(TA_Base_Test) 


CBoostThread::CBoostThread()
{
	BOOST_LOG_FUNCTION();

	m_ThreadState = THREAD_STATE_UNKNOWN;
	m_thread = NULL;	
	m_nThreadID = 0;
}

CBoostThread::~CBoostThread()
{
	BOOST_LOG_FUNCTION();


}


void CBoostThread::runThread(void* ptr)
{
	BOOST_LOG_FUNCTION();


	CBoostThread* myThread = static_cast<CBoostThread *>(ptr);
	myThread->m_ThreadState = THREAD_STATE_RUNNING;

	// printf("&i = %p\n", &i)	// printf Thread address
	myThread->m_nThreadID = TA_Base_Test::CBoostThread::getCurrentThreadId();

	LOG_DEBUG<<"runThread(): [Thrd: "<<myThread->m_nThreadID<<"%d] begin  call run()",  

	try
	{
		myThread->run();
	}
	catch (std::exception& e)
	{
		std::string msg( "runThread(): " );
		msg.append( e.what() );
		LOG_DEBUG<<"runThread(): [Thrd: "<<myThread->m_nThreadID<<"%d] begin  call run()",  

		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugError, "runThread(): std::exception  msg=%s", msg.c_str());
		return;
	}
	catch (...)
	{
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugError, "runThread(): Unknown exception");
		return;
	}

	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugDebug, "runThread(): [Thrd: %d] end  call run()", myThread->m_nThreadID );

	myThread->m_ThreadState = THREAD_STATE_FINISH; 
	return;
}



void CBoostThread::start()
{
	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugTrace, "start");

	int status = 0;		 
	if (THREAD_STATE_RUNNING ==	m_ThreadState
		|| THREAD_STATE_FINISH == m_ThreadState)
	{
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugWarn, "start(): [Thrd: %d] is already running", this->m_nThreadID );
		return;
	}
	//CTestClientThread* pClient = NULL;	
	//boost::thread* pThread = NULL;
	//pThread = new boost::thread(boost::bind(&CTestClientThread::loop, pClient, 100));


	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugDebug, "start(): [Thrd: %d] begin start", this->m_nThreadID );
	//m_thread = new boost::thread(runThread, this);
	m_thread = new boost::thread(boost::bind(&CBoostThread::runThread, this, this));
	m_ThreadState = THREAD_STATE_NEW;
	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugDebug, "start(): [Thrd: %d] end start", this->m_nThreadID );

}


void CBoostThread::terminateAndWait()
{
	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugTrace, "terminateAndWait");

	int status = 0; 

	if (NULL != m_thread)
	{
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugDebug, "terminateAndWait(): [Thrd: %d] begin call terminate()", this->m_nThreadID );

		terminate();
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugDebug, "terminateAndWait(): [Thrd: %d] end call terminate()", this->m_nThreadID );

		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugDebug, "terminateAndWait(): [Thrd: %d] begin begin wait end", this->m_nThreadID );

        m_thread->join();

		m_ThreadState = THREAD_STATE_TERMINATED;
		m_ThreadState = THREAD_STATE_FINISH;
		delete m_thread;
		m_thread = NULL;
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugDebug, "terminateAndWait(): [Thrd: %d] begin end wait end", this->m_nThreadID );

	}
}


void CBoostThread::sleep(unsigned int milliSeconds)
{  
	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugTrace, "sleep");

	boost::xtime timeTmp;
	boost::xtime_get(&timeTmp, boost::TIME_UTC_);
	timeTmp.nsec += milliSeconds%1000*1000*1000;
	timeTmp.sec += milliSeconds/1000;
	boost::thread::sleep(timeTmp);
	//boost::this_thread::sleep(boost::posix_time::milliseconds(500));
}


unsigned int CBoostThread::getId() const
{
	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugTrace, "getId");

	return (m_thread != NULL) ? m_nThreadID : static_cast< unsigned int >( -1 );
}

unsigned int CBoostThread::getCurrentThreadId()
{
	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugTrace, "getCurrentThreadId");

	std::string threadId = boost::lexical_cast<std::string>(boost::this_thread::get_id());
	unsigned int threadNumber = 0;
	sscanf(threadId.c_str(), "%lx", &threadNumber);
	return threadNumber;
}
EThreadState CBoostThread::getCurrentState() const
{
	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugTrace, "getCurrentState");

	return m_ThreadState;
}
NS_END(TA_Base_Test) 




