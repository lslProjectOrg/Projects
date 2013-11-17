#ifndef THREAD_H_INCLUDED
#define THREAD_H_INCLUDED

/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/core/threads/src/Thread.h $
  * @author:  B. Fountain
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  * 
  * Platform independent wrapper which allows us to manage a single thread. This
  * is an abstract class - extend it to specify run() and terminate() methods
  */
  
  

#include "pthread.h"

namespace TA_Base_Core
{


	enum EThreadState
	{
		THREAD_STATE_NEW,

		THREAD_STATE_RUNNING,
		THREAD_STATE_TERMINATED,

		THREAD_STATE_RUNEND,
		THREAD_STATE_UNKNOWN                                     
	};

	// Using the Thread class:
    // Thread is used when the client needs to know when the thread has finished
    // executing, as distinct from a worker thread that deletes itself when it has
    // finished - pthreads would refer to these as undetached and detached threads,
    // respectively.  
    
    // A client starts Thread by calling start().  The worker thread will terminate 
    // when Thread::run() exits, but resources are not actually reclaimed until 
    // terminateAndWait() returns.  A client can wait (in an idle state) until the 
    // worker thread terminates by calling terminateAndWait().  start() can be called 
    // multiple times, but each call must be followed by a call to terminateAndWait().  


    // The Thread class is NOT actually threadsafe.  setPriority(), getId(), etc, should NOT
    // be called from a different thread to that calling terminateAndWait().  It is actually
    // impossible to make it completely threadsafe - see the comment in terminateAndWait().
	
	
	

	class Thread
	{
	public:
		Thread(); 
        // terminateAndWait() MUST have been called, and the thread stopped, before 
        // the destructor is called.
		
		virtual ~Thread();
		
		
		// run() is called by the new thread that is spawned when start() is called.  
        // NOTE: dont call run() directly, because it would then be called by the
        // calling thread rather than a newly-spawned thread.
		
		virtual void run() = 0;	
		
		
		// terminate() is called by terminateAndWait(), but it can also be called
        // directly (be sure the thread isn't running before starting it again though).  
		
		
		virtual void terminate() = 0;

	public:
     	// start() spawns a new thread which then executes run(), which is implemented
        // by a derived class.  The thread will finish when run() exits, but start() 
        // must not be called again until terminateAndWait() has been called.
		
		void start();
		// Calls terminate(), which is implemented by a derived class, then blocks until run()
        // has exited.  Can be called even if the thread isn't already running.
		
		void terminateAndWait();

	public:	

        // Specify the priority of this Thread object, one of
		// 0=LOW_PRIORITY, 1=NORMAL (default), 2=HIGH

		void setPriority(int newPriority);

        // Returns the thread id of this Thread object. NOTE that this id can NOT be
        // compared to the id returned by getCurrentThreadId().  nothrow.

		unsigned int getId() const;
        // STATIC METHODS WHICH OPERATE ON THE CALLING THREAD

		// Use this method to sleep, rather than the platform-specific method
        // Sleep for *at least* the given number of milliseconds (depending on load
        // it could actually be longer of course)


		static void sleep(unsigned int milliseconds);
		static unsigned int getCurrentThreadId();

	public:	
	
	     // Returns the thread id of the calling thread.  nothrow.
	
		EThreadState getCurrentState() const;

	public:
		friend void * runThread(void * args);   

	private: 
		pthread_t* m_thread; 

	public:
		unsigned int    m_priority;
		EThreadState    m_ThreadState;
		int             m_nThreadID;
	};
	void* runThread(void * args);


}//namespace TA_Base_Core

#endif  // THREAD_H_INCLUDED
