
/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/core/threads/src/Thread.cpp $
  * @author:  B Fountain
  * @version: $Revision: #3 $
  *
  * Last modification: $DateTime: 2012/03/22 16:16:32 $
  * Last modified by:  $Author: limin.zhu $
  * 
  * Platform independent wrapper which allows us to manage a single thread. This
  * is an abstract class - extend it to specify run() and terminate() methods
  *
  */
#include "Thread.h"

// #ifdef WIN32
// #include <windows.h>
// #elif defined ( SOLARIS ) || defined ( LINUX )
// #include "pthread.h"
// #else
// #error Unsupported platform!
// #endif // WIN32


#include "core/utilities/src/TA_String.h"
#include "core/utilities/src/DebugUtil.h"

namespace TA_Base_Core
{



	// runThread()
	//
	// This is a C-style function which is called from the thread object. It
	// expects its parameter to be a pointer to a Thread object, which it
	// will run
	//
	//static void* runThread(void* ptr)
	void* runThread(void* ptr)
	{
		//TA_DEBUG_ASSERT(ptr != NULL, "Null pointer passed to runThread()");

		//Thread* myThread = (Thread*) ptr;
		Thread* myThread = static_cast<Thread *>(ptr);
		myThread->m_ThreadState = THREAD_STATE_RUNNING;

		// terminateAndWait() can't set m_thread back to NULL until after this method finishes,
		// so m_thread MUST be valid here.
		//TA_Assert(myThread->m_thread != NULL);

		// printf("&i = %p\n", &i)	// printf Thread address
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, 
			"runThread(): myThread=%s about to call run()", 
			gPtrToStr(myThread).c_str() );

		try
		{
			myThread->run();
		}
		catch (std::exception& e)
		{
			std::string msg( "runThread(): " );
			msg.append( e.what() );
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "std::exception  name=%s  msg=%s", 
				typeid(e).name(), msg.c_str() );
		}
		catch (...)
		{
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "runThread(): Unknown exception" );
		}

		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, 
			"runThread(): myThread=%s finished calling run()",  
			gPtrToStr(myThread).c_str());

		myThread->m_ThreadState = THREAD_STATE_RUNEND; 
		return NULL;
	}


	Thread::Thread()
	{
		//m_priority(omni_thread::PRIORITY_NORMAL)

		m_ThreadState = THREAD_STATE_UNKNOWN;

		m_thread = (pthread_t *)malloc(sizeof(pthread_t));
		if(NULL == m_thread)
		{
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "malloc error...");
		} 
		else
		{
			m_ThreadState = THREAD_STATE_NEW;
		}

	}

	Thread::~Thread()
	{
		if(NULL != m_thread) 
		{
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug,
				"Thread::~Thread(): this=%s destructor called", 
				gPtrToStr(this).c_str());

			free(m_thread);
			m_thread = NULL; 
		}

	}


	void Thread::start()
	{
		int status = 0;		 

		// start() must not be called if the thread is already running.
		// terminateAndWait() must be called in between successive calls to start().
		if (THREAD_STATE_RUNNING ==	m_ThreadState
			|| THREAD_STATE_RUNEND == m_ThreadState)
		{
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError,
				"Thread::start(): this=%s is already running.", gPtrToStr(this).c_str() );
			return;
		}

		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, 
			"Thread::start(): this=%s about to start.", gPtrToStr(this).c_str() );

		// THREAD_STATE_NEW only occurs during the following two calls
		status = pthread_create(m_thread, NULL, runThread, this); 

		if(0 != status)
		{
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "pthread_create [status=%d][m_thread=%s]", 
				status, gPtrToStr(m_thread).c_str());
		}


		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, 
			"Thread::start(): this=%s has been started successfully. m_thread=%s", 
			gPtrToStr(this).c_str(), gPtrToStr(m_thread).c_str() );
	}


	void Thread::terminateAndWait()
	{
		int status = 0; 

		if (NULL != m_thread)
		{
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, 
				"Thread::terminateAndWait(): this=%s about to call terminate()", 
				gPtrToStr(this).c_str() );

			terminate();

			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, 
				"Thread::terminateAndWait(): this=%s about to wait.",
				gPtrToStr(this).c_str() );

			status = pthread_join(*m_thread, NULL);
			if(status != 0)
			{
				LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "pthread_join [status=%d][m_thread=%s]", 
					status, gPtrToStr(m_thread).c_str());
			}
			m_ThreadState = THREAD_STATE_TERMINATED;
			m_ThreadState = THREAD_STATE_RUNEND;


			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, 
				"Thread::terminateAndWait(): this=%s has stopped.", 
				gPtrToStr(this).c_str() );
		}
	}

	/*
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 500;
	select(0,NULL, NULL, NULL, &tv); 
	*/
	void Thread::sleep(unsigned int milliSeconds)
	{  
		//0.001 s = 1 Millisecond = 1000 Microseconds = 1000 000	Nanoseconds

		struct timespec delayVal; 

		long  nSleepMilliseconds = 0; 
		long  nSleepMicroseconds = 0;
		long  nSleepNanoseconds = 0; 
		long  nSleepSeconds = 0;

		nSleepSeconds =  milliSeconds / 1000;

		nSleepMilliseconds = milliSeconds % 1000;
		nSleepMicroseconds = nSleepMilliseconds * 1000;
		nSleepNanoseconds = nSleepMicroseconds * 1000;

		delayVal.tv_sec = nSleepSeconds;
		delayVal.tv_nsec = nSleepNanoseconds;	

		pthread_delay_np (&delayVal);
	}




	void Thread::setPriority(int newPriority)
	{
		//TA_DEBUG_ASSERT((newPriority >= 0)&&(newPriority < 3), "Invalid priority level");      

		m_priority = newPriority;
	}


	unsigned int Thread::getId() const
	{
		return (m_thread != NULL) ? m_thread->x : static_cast< unsigned int >( -1 );
	}


	unsigned int Thread::getCurrentThreadId()
	{
#ifdef WIN32
		//	return ::GetCurrentThreadId();
#elif defined ( SOLARIS ) || defined ( LINUX )
		// 	return pthread_self();
#else
#error Unsupported Platform!
#endif // WIN32
		unsigned int threadWin32Tmp = 0;
		pthread_t threadLinuxTmp;

		//threadWin32Tmp = ::GetCurrentThreadId();

		threadLinuxTmp = pthread_self();

		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, 
			"fun getCurrentThreadId() [GetCurrentThreadId=%d][threadLinuxTmp.x=%d]", 
			threadWin32Tmp, threadLinuxTmp.x);

		return threadLinuxTmp.x;

	}


	EThreadState Thread::getCurrentState() const
	{
		return m_ThreadState;
	}
}//namespace TA_Base_Core




