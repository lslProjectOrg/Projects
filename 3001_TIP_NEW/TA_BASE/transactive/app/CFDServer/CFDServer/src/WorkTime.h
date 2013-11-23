/**
*
* Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/dbsynch/SynchEngine/src/WorkTime.h $
* @author:  Shenglong.lin
* @version: $Revision: #1 $
*
* Last modification: $DateTime: 2013/02/19 09:15:14 $
* Last modified by:  $Author: Shenglong.lin $
* 
* This class is used to get Thread Work time 
*/


#ifndef __CLASS_WORK_TIME__HH__
#define __CLASS_WORK_TIME__HH__

#include <iostream>
#include <fstream>
#include <list> 
#include "core/threads/src/Thread.h"
#include "core/synchronisation/src/ReEntrantThreadLockable.h"
#include "CommonData.h"
 
#include <stdio.h>
#include <sys/timeb.h>
#include <stdlib.h>

NS_BEGIN(TA_Base_App)

using namespace TA_Base_Core;


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
	virtual systype_ulong64 workEnd()= 0; 

	//WorkTime valuse is end work time - begin work time
	//for log time sprintf("%llu or %lld")
	virtual systype_ulong64 getWorkTime()= 0;

	//NotWorkTime valuse is now time - begin work time
	virtual systype_ulong64 getNotworkTime()= 0;//for monitor 


public:
	std::string getTimeString(struct timeb* pfbtime);
	virtual std::string getTimeNow(void);
	virtual std::string getBeginTime(void);
	virtual std::string getEndTime(void);
public:
	virtual std::string getCurrentTime(struct timeb* pfbtime);
	virtual systype_ulong64 getDiffTime(struct timeb* pfbtimeBegin, struct timeb* pfbtimeEnd);

protected:
	int  m_nWorkTimeCode;
	//begin work time
	struct timeb m_fTimeWorkBegin;	
	//end work time
	struct timeb m_fTimeWorkEnd;		
	//WorkTime valuse is end work time - begin work time
	systype_ulong64		 m_nWorkTime;//millsecs	 
	//NotWorkTime valuse is now time - begin work time
	systype_ulong64         m_nNotWorkTime;
	//set begin work 
	bool         m_bSetWorkBegin;
	//set end work 
	bool         m_bSetWorkEnd;


};

class CWorkTimeLock	: public  CAWorkTime
{
public:
	CWorkTimeLock(int nWorkTimeCode);
	~CWorkTimeLock(void); 
public:	
	virtual void workBegin(); 
	virtual systype_ulong64 workEnd();
	virtual systype_ulong64 getWorkTime();   //for log time 
	virtual systype_ulong64 getNotworkTime(); //for monitor	
private:
	ReEntrantThreadLockable	m_lockWorkTime; 
};

class CWorkTimeNoLock	: public  CAWorkTime
{
public:
	CWorkTimeNoLock(int nWorkTimeCode);
	~CWorkTimeNoLock(void); 
public:	
	virtual void workBegin(); 
	virtual systype_ulong64 workEnd();
	virtual systype_ulong64 getWorkTime();   //for log time 
	virtual systype_ulong64 getNotworkTime(); //for monitor	
};

NS_END(TA_Base_App)




#endif//__CLASS_WORK_TIME__HH__



