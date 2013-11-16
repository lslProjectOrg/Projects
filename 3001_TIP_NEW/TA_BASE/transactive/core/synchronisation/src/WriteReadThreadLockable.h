#ifndef WRITEREADTHREADLOCKABLE_H_INCLUDED
#define WRITEREADTHREADLOCKABLE_H_INCLUDED

/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/core/synchronisation/src/WriteReadThreadLockable.h $
  * @author:  Andrew Del Carlo
  * @version: $Revision: #3 $
  *
  * Last modification: $DateTime: 2012/09/13 16:29:10 $
  * Last modified by:  $Author: limin.zhu $
  *
  * Implementation of the IThreadLockable interface that allows multipule threads to acquire a
  * read lock. A count of read locks held is maintained, with the lock only being released 
  * when the equivalent number of calls to release() are made.
  *
  * It also has a non-rentrant write lock. 
  *
  * Write lock requests are given priority over read lock requests. 
  * ie: a write lock request will block further read lock request until the write lock
  *     has been aquired and released.
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

#include "ReadWriteThreadLockable.h"

//
// Forward declarations
//

class omni_mutex;

namespace TA_Base_Core
{
    // Forward declared struct so as not to bring in any dependencies here that are
    // needed to identify the current thread on various platforms.
	class ThreadIdWrapper;
}

namespace TA_Base_Core
{
    typedef ReadWriteThreadLockable WriteReadThreadLockable;



}

#endif // WRITEREADTHREADLOCKABLE_H_INCLUDED

