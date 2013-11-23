#ifndef THREADLOCKABLEHOLDERVERIFIER_H_INCLUDED
#define THREADLOCKABLEHOLDERVERIFIER_H_INCLUDED

/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/core/synchronisation/src/ThreadLockableHolderVerifier.h $
* @author:  Robert Young
* @version: $Revision: #2 $
*
* Last modification: $DateTime: 2012/02/06 16:15:14 $
* Last modified by:  $Author: haijun.li $
*
* Platform-independant helper used to maintain the identity of a thread, and to 
* determine whether the currently executing thread has the same id.
*
*/

// #if defined ( SOLARIS ) || defined ( LINUX )
// //#include <pthread.h>
// #else
// //#include <windows.h>
// #endif
// 
// //use
// #include "pthread.h"
#include "core/threads/src/Thread.h"  

namespace TA_Base_Core
{

class ThreadIdWrapper
{
public: 
	ThreadIdWrapper() 
	  {
		  //m_threadId = pthread_self();//use
		  m_threadId = TA_Base_Core::Thread::getCurrentThreadId();
	  }

	  /** 
	  * setCurrentThreadId
	  *
	  * Set the id to the current thread id.
	  *
	  */
	  void setCurrentThreadId()
	  {
	  
		  m_threadId = TA_Base_Core::Thread::getCurrentThreadId();

		  return;
	  }


	  /** 
	  * isCurrentThreadId
	  *
	  * returns true if the id is the same as the current thread's id.
	  *
	  *
	  * @return bool
	  */
	  bool isCurrentThreadId()
	  {
// #if defined ( SOLARIS ) || defined ( LINUX )
// //		  return ( 0 != pthread_equal( pthread_self(), m_threadId ) ); 
// #else
// //		  return ( GetCurrentThreadId() == m_threadId );
// #endif  
// 
// 		  return ( 0 != pthread_equal( pthread_self(), m_threadId ) );//use

		  unsigned int nCurrentThreadID = 0;

		  nCurrentThreadID = TA_Base_Core::Thread::getCurrentThreadId();

		  if (m_threadId == nCurrentThreadID)
		  {
			  return true;
		  }
		  return false;
	  }


	  unsigned int getCurrentThreadId()
	  {
		  return TA_Base_Core::Thread::getCurrentThreadId();
	  }

private:
#if defined ( SOLARIS ) || defined ( LINUX )
//	pthread_t m_threadId;
#else
//	DWORD m_threadId; 
#endif  


private:
	unsigned int m_threadId;  //use


};
}//namespace TA_Base_Core

#endif // THREADLOCKABLEHOLDERVERIFIER_H_INCLUDED
