#ifndef REENTRANTTHREADLOCKABLE_H_INCLUDED
#define REENTRANTTHREADLOCKABLE_H_INCLUDED

/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/core/synchronisation/src/ReEntrantThreadLockable.h $
* @author:  Robert Young
* @version: $Revision: #2 $
*
* Last modification: $DateTime: 2012/02/06 16:15:14 $
* Last modified by:  $Author: haijun.li $
*
* Implementation of the IThreadLockable interface that allows the same thread to acquire the
* lock multiple times (i.e. consecutively). A count of consecutive acquisitions by the same
* thread is maintained, with the lock only being released when the equivalent number of calls
* to release() are made.
*
* This lock should not be acquired directly, but managed using a temporary object (i.e. 
* ThreadGuard) which ensures the lock is acquired during construction, and subsequently 
* released during destruction.
*
* Note, this class wrappers the omni_mutex class, which in turn wrappers the OS-specific 
* threading API. 
*
* NOTE: Condition is made a friend class so it can access backupReEntrantAcquisitionState() and
* restoreReEntrantAcquisitionState(). This is necessary to allow Condition to backup and
* restore the state before and after calling wait() or timedWait() on it's omni_condition.
* Such backup and restore is needed as the linked omni_mutex (wrappered by this class) is intrinisically
* released and reacquired at these times by the omni_condition. By allowing Condition to wrapper
* the omni_condition's linked omni_mutex with this class, it can offer re-entrant access of the
* mutex to it's clients.
*/

#include "IThreadLockable.h"
#include "ThreadLockableHolderVerifier.h"
//
// Forward declarations
//



namespace TA_Base_Core
{




class ReEntrantThreadLockable : public IThreadLockable
{
public:	

	/** 
	* Default Constructor
	*/
	ReEntrantThreadLockable();


	/** 
	* Destructor
	*/
	virtual ~ReEntrantThreadLockable();


	/** 
	* acquire
	*
	* Acquire the lock. Consecutive acquisitions from the same thread is
	* allowed, but the lock will not be subsequently released until an
	* equivalent number of calls to release are made.
	*
	*/
	virtual void acquire();


	/** 
	* release
	*
	* Release the lock. If a thread has made multiple acquisitions, the lock
	* will not be released until an equivalent number of calls to this method
	* are made.
	*
	* PreCondition: The lock is currently acquired.  
	*/
	virtual void release();

private:

	//
	// Disable the default copy and assignment constructors as shouldn't used.
	//
	ReEntrantThreadLockable( const ReEntrantThreadLockable& );
	ReEntrantThreadLockable& operator=( const ReEntrantThreadLockable& );


	// Indicates when some thread has currently aquired this lock and the number
	// of times it has re-entered.
	volatile unsigned int m_numTimesThreadAcquiredLock;

	// The underlying omni_mutex this class is wrapping.
	pthread_mutex_t  m_lock;	                /* Log Lock */


	// Struct used to store the identifier of the thread that has currently aquired
	// the lock and inform client code whether its thread is a match.
	ThreadIdWrapper* m_threadLockableHolderVerifier;


	////////////////////////////////////////////////////////////////////////////////
	//// Private state and methods to allow friend class Condition to offer     ////
	//// its linked mutex to client code wrappered in re-entrant access. These  ////
	//// state and methods allow Condition to backup and restore the re-entrant ////
	//// lock acquisition state. Condition needs to backup just before calling  ////
	//// wait or timedWait on its wrappered omni_condition, and restore just    ////
	//// after the wait completes. This is because the associated mutex is      ////
	//// intrinisically released just before entering wait and intrinisically   ////
	//// re-acquired directly afterwards. The period inbetween allows           ////
	//// competing thread to acquire and release the lock and hence modify the  ////
	//// re-entrant acquisition state.                                          ////
	////////////////////////////////////////////////////////////////////////////////


	/** 
	* backupReEntrantAcquisitionState
	*
	* Private method so as to allow friend class Condition to backup 
	* re-entrant acquisition state before intrinsically releasing its
	* linked mutex prior to entering wait.
	*/
	void backupReEntrantAcquisitionState();


	/** 
	* restoreReEntrantAcquisitionState
	*
	* Private method so as to allow friend class Condition to restore the previous
	* re-entrant acquisition state after intrinsically re-acquiring its
	* linked mutex after completing wait.
	*/
	void restoreReEntrantAcquisitionState();



	// Member used to backup the number of times the current lockholding thread
	// has acquired the lock.
	volatile unsigned int m_previousNumTimesThreadAcquiredLock;

	// Member used to backup the identifier of the thread that has currently aquired
	// the lock.
	ThreadIdWrapper* m_previousThreadLockableHolderVerifier;

};
}//namespace TA_Base_Core

#endif // REENTRANTTHREADLOCKABLE_H_INCLUDED

