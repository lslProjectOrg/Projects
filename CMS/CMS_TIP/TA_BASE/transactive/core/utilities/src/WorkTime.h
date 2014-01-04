#ifndef __CLASS_WORK_TIME__HH__
#define __CLASS_WORK_TIME__HH__

#include <iostream>
#include <fstream>
#include <list> 
 
#include <sys/timeb.h>
#include <stdlib.h>

#include <boost/chrono.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>  
#include <boost/thread.hpp>  
#define BOOST_DATE_TIME_SOURCE

#include "core/utilities/src/UtilitiesCommonData.h"

using namespace TA_Base_Core;

NS_BEGIN(TA_Base_Core)



class CAWorkTime
{
public:
	CAWorkTime();
	virtual ~CAWorkTime(void); 
public:
	//set begin work 
	virtual void workBegin()= 0; 
	//set end work
	//return WorkTime : end work time - begin work time  (millsecs)
	virtual BigInt64 workEnd()= 0; 

	//WorkTime valuse is end work time - begin work time
	//for log time sprintf("%llu or %lld")
	virtual BigInt64 getWorkTime()= 0;

	//NotWorkTime valuse is now time - begin work time
	virtual BigInt64 getNotworkTime()= 0;//for monitor 


public:
	std::string getTimeString(boost::posix_time::ptime* pfbtime);
	virtual std::string getTimeNow(void);
	virtual std::string getBeginTime(void);
	virtual std::string getEndTime(void);
public:
	virtual std::string getCurrentTime(boost::posix_time::ptime* pfbtime);
	virtual boost::posix_time::millisec_posix_time_system_config::time_duration_type getDiffTime(
		boost::posix_time::ptime* pfbtimeBegin, 
		boost::posix_time::ptime* pfbtimeEnd);
	virtual BigInt64 getTimeSeconds(boost::posix_time::millisec_posix_time_system_config::time_duration_type* pfbtime);
	virtual BigInt64 getTimeMilliseconds(boost::posix_time::millisec_posix_time_system_config::time_duration_type* pfbtime);
protected:
	int  m_nWorkTimeCode;
	//begin work time
	boost::posix_time::ptime m_fTimeWorkBegin;	
	//end work time
	boost::posix_time::ptime m_fTimeWorkEnd;		
	//WorkTime valuse is end work time - begin work time
	boost::posix_time::millisec_posix_time_system_config::time_duration_type	m_nWorkTime;//millsecs	 
	//NotWorkTime valuse is now time - begin work time
	boost::posix_time::millisec_posix_time_system_config::time_duration_type    m_nNotWorkTime;
	//set begin work 
	bool         m_bSetWorkBegin;
	//set end work 
	bool         m_bSetWorkEnd;
};

class CWorkTimeLock	: public  CAWorkTime
{
public:
	CWorkTimeLock(int nWorkTimeCode=1);
	~CWorkTimeLock(void); 
public:	
	virtual void workBegin(); 
	virtual BigInt64 workEnd();
	virtual BigInt64 getWorkTime();   //for log time 
	virtual BigInt64 getNotworkTime(); //for monitor	
private:
	boost::mutex	m_lockWorkTime; 

};

class CWorkTimeNoLock	: public  CAWorkTime
{
public:
	CWorkTimeNoLock(int nWorkTimeCode=1);
	~CWorkTimeNoLock(void); 
public:	
	virtual void workBegin(); 
	virtual BigInt64 workEnd();
	virtual BigInt64 getWorkTime();   //for log time 
	virtual BigInt64 getNotworkTime(); //for monitor	
};

NS_END(TA_Base_Core)




#endif//__CLASS_WORK_TIME__HH__



