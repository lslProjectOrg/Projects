#ifndef THREAD_H_INCLUDED
#define THREAD_H_INCLUDED


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


	class Thread
	{
	public:
		Thread(); 
		virtual ~Thread();
		virtual void run() = 0;	
		virtual void terminate() = 0;

	public:
		void start();
		void terminateAndWait();

	public:	
		void setPriority(int newPriority);
		unsigned int getId() const;
		static void sleep(unsigned int milliseconds);
		static unsigned int getCurrentThreadId();

	public:	
		EThreadState getCurrentState() const;

	public:
		friend void * runThread(void * args);   

	private: 
		pthread_t* m_thread; 

	public:
		unsigned int    m_priority;
		EThreadState    m_ThreadState;
	};
	void* runThread(void * args);


}//namespace TA_Base_Core

#endif  // THREAD_H_INCLUDED
