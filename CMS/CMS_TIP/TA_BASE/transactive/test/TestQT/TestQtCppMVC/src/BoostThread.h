#ifndef __CLASS_BOOST_THREAD__HH__
#define __CLASS_BOOST_THREAD__HH__

#include "ProjectCommonData.h"

#include <boost/chrono.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>



NS_BEGIN(TA_Base_App) 


class CBoostThread
{
public:
	typedef enum enThreadStates
	{
		THREAD_STATE_NEW,

		THREAD_STATE_RUNNING,
		THREAD_STATE_TERMINATED,

		THREAD_STATE_FINISH,
		THREAD_STATE_UNKNOWN                                     
	}EThreadStates;
public:
	CBoostThread(); 
	virtual ~CBoostThread();
	virtual void run() = 0;	
	virtual void terminate() = 0;

public:
	void start();
	void terminateAndWait();

public:	
	unsigned int getId() const;
	static void sleep(unsigned int milliseconds);
	static unsigned int getCurrentThreadId();
public:	
	EThreadStates getCurrentState() const;

public:
	void  runThread(void * args);   

private: 
	boost::thread*  m_thread; 

public:
	EThreadStates    m_ThreadState;
	int             m_nThreadID;
};


NS_END(TA_Base_App) 

#endif  // __CLASS_BOOST_THREAD__HH__



