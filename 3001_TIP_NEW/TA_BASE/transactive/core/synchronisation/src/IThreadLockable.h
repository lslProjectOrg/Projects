#if !defined(ITHREADLOCKABLE_H__INCLUDED_)
#define ITHREADLOCKABLE_H__INCLUDED_

/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/core/synchronisation/src/IThreadLockable.h $
  * @author:  Robert Young
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  * Interface definition for an object that allows atomic acquisition of resources
  * in a thread-safe manner. Allows for varying ThreadLockable implementations to 
  * be interchanged without modification to client code.
  *
  */

namespace TA_Base_Core
{
    class IThreadLockable
    {

    public:

        /** 
          * acquire
          *
          * Acquire the lock.
          *
          */
	    virtual void acquire() = 0;


        /** 
          * acquireRead
          *
          * Acquire the read lock.
          *
          * This function will be overridden by those 
          * drived classes that support read locks
          */
        virtual void acquireRead();


        /** 
          * release
          *
          * Release the lock.
          *
          * PreCondition: This IThreadLockable is currently acquired. 
          */
        virtual void release() = 0;


        /** 
          * release
          *
          * Release the lock.
          *
          * PreCondition: This IThreadLockable is currently acquired. 
          */
        virtual void releaseRead();


        /** 
          * Destructor
          */
        virtual ~IThreadLockable(){ };
    };
} // namespace TA_Base_App

#endif // !defined(ITHREADLOCKABLE_H__INCLUDED_)
