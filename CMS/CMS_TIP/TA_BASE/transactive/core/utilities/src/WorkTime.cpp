#include "WorkTime.h"



NS_BEGIN(TA_Base_Core)



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

CAWorkTime::CAWorkTime()
{

}
CAWorkTime::~CAWorkTime()
{

}


std::string CAWorkTime::getTimeString(struct timeb* pfbtime)
{
	std::string	strTimeString;
	char* pszCurTime = NULL;
	int nBufferSize = 256;
	struct tm* pTM = NULL;

	if (NULL == pfbtime)
	{
		return strTimeString;
	}

	pszCurTime =new char[nBufferSize];
	memset(pszCurTime, 0, nBufferSize);

	pTM = localtime(&(pfbtime->time));

	//format to string
	if ( NULL != pTM )
	{
		sprintf(pszCurTime, "[%04d-%02d-%02d %02d:%02d:%02d.%03d]",
			pTM->tm_year + 1900, pTM->tm_mon + 1, pTM->tm_mday,
			pTM->tm_hour, pTM->tm_min, pTM->tm_sec, pfbtime->millitm); 

		strTimeString = pszCurTime;     
	}

	delete pszCurTime;
	pszCurTime = NULL;
	pTM = NULL;

	return strTimeString;
}


std::string CAWorkTime::getTimeNow(void)
{
	std::string	strCurrentTime;
	struct timeb fTimeNow;
	getCurrentTime(&fTimeNow);

	strCurrentTime = getTimeString(&fTimeNow);
	return strCurrentTime;
}
std::string CAWorkTime::getBeginTime(void)
{
	return getTimeString(&m_fTimeWorkBegin);
}
std::string CAWorkTime::getEndTime(void)
{
	return getTimeString(&m_fTimeWorkEnd);
}

std::string CAWorkTime::getCurrentTime( struct timeb* pfbtime )
{
	std::string			strCurrentTime;

	if (NULL != pfbtime)
	{
		ftime(pfbtime);
	}
	strCurrentTime = getTimeString(pfbtime);
	return strCurrentTime;
}

BigInt64 CAWorkTime::getDiffTime( struct timeb* pfbtimeBegin, struct timeb* pfbtimeEnd )
{
	BigInt64             nDiffTimeRes = 0;
	time_t				 diffSeconds;

	if (NULL != pfbtimeBegin && NULL != pfbtimeEnd)
	{
		diffSeconds = pfbtimeEnd->time - pfbtimeBegin->time;
		nDiffTimeRes = (BigInt64)(diffSeconds*1000.0 + pfbtimeEnd->millitm - pfbtimeBegin->millitm);//double
	}

	return nDiffTimeRes;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

CWorkTimeLock::CWorkTimeLock(int nWorkTimeCode )
{
	m_nWorkTimeCode = nWorkTimeCode;	
	getCurrentTime(&m_fTimeWorkBegin);
	getCurrentTime(&m_fTimeWorkEnd);
	m_nWorkTime = 0;	
	m_bSetWorkBegin = false;
	m_bSetWorkEnd = false;
	m_nNotWorkTime = 0;

}
CWorkTimeLock::~CWorkTimeLock(void)
{
}

void CWorkTimeLock::workBegin()
{
	int nFunRes = 0;
	boost::mutex::scoped_lock lock(m_lockWorkTime);
	getCurrentTime(&m_fTimeWorkBegin);
	getCurrentTime(&m_fTimeWorkEnd);
	m_bSetWorkBegin = true;
	m_bSetWorkEnd = false;

	return;
}

BigInt64 CWorkTimeLock::workEnd()
{
	int nFunRes = 0;
	boost::mutex::scoped_lock lock(m_lockWorkTime);

	if (m_bSetWorkBegin)
	{ 	
		getCurrentTime(&m_fTimeWorkEnd);

		m_nWorkTime = getDiffTime(&m_fTimeWorkBegin, &m_fTimeWorkEnd);

		m_bSetWorkEnd = true;
		m_bSetWorkBegin = false;
	} 
	else
	{
		m_bSetWorkEnd = false;		
		m_nWorkTime = 0;
	}

	return m_nWorkTime;
}

BigInt64 CWorkTimeLock::getNotworkTime()
{
	int nFunRes = 0;
	struct timeb m_fTimeNow;

	boost::mutex::scoped_lock lock(m_lockWorkTime);

	if (true == m_bSetWorkBegin && false == m_bSetWorkEnd)
	{
		getCurrentTime(&m_fTimeNow);
		m_nNotWorkTime = getDiffTime(&m_fTimeWorkBegin, &m_fTimeNow);
	}
	else
	{
		m_nNotWorkTime = 0;
		return m_nNotWorkTime;
	}



	return m_nNotWorkTime;
}


BigInt64 CWorkTimeLock::getWorkTime()
{
	int nFunRes = 0;
	boost::mutex::scoped_lock lock(m_lockWorkTime);
	return m_nWorkTime;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

CWorkTimeNoLock::CWorkTimeNoLock(int nWorkTimeCode )
{
	m_nWorkTimeCode = nWorkTimeCode;	
	getCurrentTime(&m_fTimeWorkBegin);
	getCurrentTime(&m_fTimeWorkEnd);
	m_nWorkTime = 0;	
	m_bSetWorkBegin = false;
	m_bSetWorkEnd = false;
	m_nNotWorkTime = 0;

}
CWorkTimeNoLock::~CWorkTimeNoLock(void)
{
}

void CWorkTimeNoLock::workBegin()
{
	int nFunRes = 0;
	getCurrentTime(&m_fTimeWorkBegin);
	getCurrentTime(&m_fTimeWorkEnd);
	m_bSetWorkBegin = true;
	m_bSetWorkEnd = false;

	return;
}

BigInt64 CWorkTimeNoLock::workEnd()
{
	int nFunRes = 0;

	if (m_bSetWorkBegin)
	{ 	
		getCurrentTime(&m_fTimeWorkEnd);
		m_nWorkTime = getDiffTime(&m_fTimeWorkBegin, &m_fTimeWorkEnd);

		m_bSetWorkEnd = true;
		m_bSetWorkBegin = false;
	} 
	else
	{
		m_bSetWorkEnd = false;		
		m_nWorkTime = 0;
	}

	return m_nWorkTime;
}

BigInt64 CWorkTimeNoLock::getNotworkTime()
{
	int nFunRes = 0;
	struct timeb m_fTimeNow;


	if (true == m_bSetWorkBegin && false == m_bSetWorkEnd)
	{
		getCurrentTime(&m_fTimeNow);
		m_nNotWorkTime = getDiffTime(&m_fTimeWorkBegin, &m_fTimeNow);
	}
	else
	{
		m_nNotWorkTime = 0;
		return m_nNotWorkTime;
	}



	return m_nNotWorkTime;
}


BigInt64 CWorkTimeNoLock::getWorkTime()
{
	int nFunRes = 0;
	return m_nWorkTime;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

NS_END(TA_Base_Core)












