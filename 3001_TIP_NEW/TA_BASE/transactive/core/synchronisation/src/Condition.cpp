/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/core/synchronisation/src/Condition.cpp $
  * @author:  Rob Young
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  * 
  * This class provides a wrappered thread condition implementation. This can be used to pause threads
  * until signalled (or optionally timed out waiting). The underlying omni_condition is automatically 
  * linked to an underlying omni_mutex. The linked mutex is intrinsically released before condition enters wait() 
  * or timedWait(), and intrinsically re-acquired upon completion of the wait(). This intrinsic behaviour allows 
  * client code to mutually exclude the setting of state related to the condition, with the checking of that state
  * before entering into a wait() or timedWait(). The linked mutex is wrappered here so as to be ReEntrant (can be 
  * acquired multiple times consecutively by the same Thread) and is accessible from this class through 
  * getLinkedThreadLockable().
  */

#include "core/synchronisation/src/Condition.h"
#include "core/synchronisation/src/ReEntrantThreadLockable.h"
#include "core/synchronisation/src/ThreadGuard.h"


namespace TA_Base_Core
{

    //
    // Default Constructor
    //
    Condition::Condition()
    {
		int nFunRes = 0;
		nFunRes = pthread_mutex_init(&m_lock, NULL);
		nFunRes = pthread_cond_init(&m_condition, NULL);
    }


    //
    // Destructor
    //
    Condition::~Condition()
    {
		int nFunRes = 0;
		nFunRes = pthread_cond_destroy(&m_condition);
		nFunRes = pthread_mutex_destroy(&m_lock);
    }


    //
    // wait
    //
    void Condition::wait()
    {
		int nFunRes = 0;
		nFunRes = pthread_mutex_lock(&m_lock);

        // Backup the re-entrant acquisition state so we can
        // restore on other side of wait.
        //m_linkedThreadLockable->backupReEntrantAcquisitionState();
        
        // Note: Mutex intrinsically released.
        //m_condition->wait();
		nFunRes = pthread_cond_wait(&m_condition, &m_lock);

        // Note: Mutex intrinisically acquired.

        // Now that mutex has been reacquired, restore the re-entrant
        // acquisition state.
       // m_linkedThreadLockable->restoreReEntrantAcquisitionState();
    }


    //
    // timedWait
    //
    int Condition::timedWait(unsigned long milliSeconds)
    {
		int nFunRes = 0;
		nFunRes = pthread_mutex_lock(&m_lock);

        // Backup the re-entrant acquisition state so we can
        // restore on other side of wait.
        //m_linkedThreadLockable->backupReEntrantAcquisitionState();

        // convert from millisecs to secs, nanosecs AND
        // convert to absolute time from relative time from now.
        unsigned long absoluteSecs = 0;
        unsigned long absoluteNanoSecs = 0;

		struct timespec nABSTime; 

		long  nSleepMilliseconds = 0; 
		long  nSleepMicroseconds = 0;
		long  nSleepNanoseconds = 0; 
		long  nSleepSeconds = 0;

		nSleepSeconds =  milliSeconds / 1000;

		nSleepMilliseconds = milliSeconds % 1000;
		nSleepMicroseconds = nSleepMilliseconds * 1000;
		nSleepNanoseconds = nSleepMicroseconds * 1000;

		nABSTime.tv_sec = nSleepSeconds;
		nABSTime.tv_nsec = nSleepNanoseconds;	

		pthread_delay_np (&nABSTime);

        // now call omni implementation
        
        // Note: Mutex intrinsically released.
        //int result = m_condition->timedwait(absoluteSecs, absoluteNanoSecs);
		nFunRes = pthread_cond_timedwait(&m_condition, &m_lock, &nABSTime);

        // Note: Mutex intrinisically acquired.

        // Now that mutex has been reacquired, restore the re-entrant
        // acquisition state.
        //m_linkedThreadLockable->restoreReEntrantAcquisitionState();

        return nFunRes;
    }


    //
    // signal
    //
    void Condition::signal()
    {
		int nFunRes = 0;
		nFunRes = pthread_mutex_lock(&m_lock);
		nFunRes = pthread_cond_signal(&m_condition);

        //ThreadGuard guard( *m_linkedThreadLockable );
        //m_condition->signal();
    }


    //
    // broadcast
    //
    void Condition::broadcast()
    {
		int nFunRes = 0;
		nFunRes = pthread_mutex_lock(&m_lock);
		nFunRes = pthread_cond_broadcast(&m_condition);

        //ThreadGuard guard( *m_linkedThreadLockable );
        //m_condition->broadcast();
    }


  
}
