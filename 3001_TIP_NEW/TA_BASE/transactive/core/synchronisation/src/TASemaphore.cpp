/**
* The source code in this file is the property of 
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/core/synchronisation/src/Semaphore.cpp $
* @author:  B Fountain
* @version: $Revision: #2 $
*
* Last modification: $DateTime: 2012/02/06 16:15:14 $
* Last modified by:  $Author: haijun.li $
* 
* Implementation of the ThreadGuard class
*
*/

#include "core/synchronisation/src/TASemaphore.h"

#include "pthread.h"

namespace TA_Base_Core
{


	CTASemaphore::CTASemaphore( unsigned int initial )
	{
		if (sem_init(&m_sem, 0, initial) == -1)
		{
			//printf("\n sem_init error! \n ");
		}
		
	}


	CTASemaphore::~CTASemaphore()
	{
		sem_destroy(&m_sem);
	}


	void CTASemaphore::wait(void)
	{
		sem_wait(&m_sem);
	}


	bool CTASemaphore::tryWait(void)
	{
		if ( 0 == sem_trywait(&m_sem) )
		{
			return false;
		}

		return true;
	}


	void CTASemaphore::post(void)
	{
		sem_post(&m_sem);
	}


}//namespace TA_Base_Core



