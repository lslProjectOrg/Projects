#include "BoostThread.h"
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>


#include "core/utilities/src/BoostLogger.h"


USING_BOOST_LOG;

NS_BEGIN(TA_Base_Core) 


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
	myThread->m_nThreadID = TA_Base_Core::CBoostThread::getCurrentThreadId();

	LOG_DEBUG<<"runThread(): [Thrd: "<<myThread->m_nThreadID<<"] begin  call run()";

	try
	{
		myThread->run();
	}
	catch (std::exception& e)
	{
		std::string msg( "runThread(): " );
		msg.append( e.what() );
		LOG_ERROR<<"runThread(): std::exception  msg="<<msg;
		return;
	}
	catch (...)
	{
		LOG_ERROR<<"runThread(): Unknown exception";		
		return;
	}

	LOG_DEBUG<<"runThread(): [Thrd: "<<myThread->m_nThreadID<<"] end  call run()";

	myThread->m_ThreadState = THREAD_STATE_FINISH; 
	return;
}



void CBoostThread::start()
{
	BOOST_LOG_FUNCTION();
	
	int status = 0;		 
	if (THREAD_STATE_RUNNING ==	m_ThreadState
		|| THREAD_STATE_FINISH == m_ThreadState)
	{
		LOG_WARNING<<"start(): [Thrd: "<<this->m_nThreadID<<"] is already running";
		return;
	}
	//CTestClientThread* pClient = NULL;	
	//boost::thread* pThread = NULL;
	//pThread = new boost::thread(boost::bind(&CTestClientThread::loop, pClient, 100));


	LOG_DEBUG<<"start(): [Thrd: "<<this->m_nThreadID<<"] begin start";

	//m_thread = new boost::thread(runThread, this);
	m_thread = new boost::thread(boost::bind(&CBoostThread::runThread, this, this));
	m_ThreadState = THREAD_STATE_NEW;
	LOG_DEBUG<<"start(): [Thrd: "<<this->m_nThreadID<<"] end start";

}


void CBoostThread::terminateAndWait()
{
	BOOST_LOG_FUNCTION();

	int status = 0; 

	if (NULL != m_thread)
	{
		LOG_DEBUG<<"terminateAndWait(): [Thrd: "<<this->m_nThreadID<<"] begin call terminate()";

		terminate();

		LOG_DEBUG<<"terminateAndWait(): [Thrd: "<<this->m_nThreadID<<"] end call terminate()";


		LOG_DEBUG<<"terminateAndWait(): [Thrd: "<<this->m_nThreadID<<"] begin  join";

        m_thread->join();

		m_ThreadState = THREAD_STATE_TERMINATED;
		m_ThreadState = THREAD_STATE_FINISH;
		delete m_thread;
		m_thread = NULL;

		LOG_DEBUG<<"terminateAndWait(): [Thrd: "<<this->m_nThreadID<<"] end  join";

	}
}


void CBoostThread::sleep(unsigned int milliSeconds)
{  
	BOOST_LOG_FUNCTION();

	boost::xtime timeTmp;
	boost::xtime_get(&timeTmp, boost::TIME_UTC_);
	timeTmp.nsec += milliSeconds%1000*1000*1000;
	timeTmp.sec += milliSeconds/1000;
	boost::thread::sleep(timeTmp);
	//boost::this_thread::sleep(boost::posix_time::milliseconds(500));
}


unsigned int CBoostThread::getId() const
{
	BOOST_LOG_FUNCTION();

	return (m_thread != NULL) ? m_nThreadID : static_cast< unsigned int >( -1 );
}

unsigned int CBoostThread::getCurrentThreadId()
{
	BOOST_LOG_FUNCTION();

	std::string threadId = boost::lexical_cast<std::string>(boost::this_thread::get_id());
	unsigned int threadNumber = 0;
	sscanf(threadId.c_str(), "%lx", &threadNumber);
	return threadNumber;
}
EThreadState CBoostThread::getCurrentState() const
{
	BOOST_LOG_FUNCTION();

	return m_ThreadState;
}
NS_END(TA_Base_Core) 




