#include "WorkTime.h"

#include "BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_App)



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

CAWorkTime::CAWorkTime()
{

}
CAWorkTime::~CAWorkTime()
{

}

BigInt64 CAWorkTime::getTimeSeconds(boost::posix_time::millisec_posix_time_system_config::time_duration_type* pfbtime)
{
	BigInt64 nFunRes = 0;

	if (NULL != pfbtime)
	{
		nFunRes = pfbtime->total_seconds();
	}

	return nFunRes;
}

BigInt64 CAWorkTime::getTimeMilliseconds(boost::posix_time::millisec_posix_time_system_config::time_duration_type* pfbtime)
{
	BigInt64 nFunRes = 0;

	if (NULL != pfbtime)
	{
		nFunRes = pfbtime->total_milliseconds();
	}

	return nFunRes;
}


std::string CAWorkTime::getTimeString(boost::posix_time::ptime* pfbtime)
{
	std::string	strTimeStringTmp;
	std::string	strTimeStringGet;
	int pos = 0;
	std::string strSubTmp;
	std::string strYear;
	std::string strMonth;
	std::string strDay;
	std::string strHour;
	std::string strMin;
	std::string strSecond;

	if (NULL == pfbtime)
	{
		return strTimeStringGet;
	}

	//linshenglong take care   20140218T150717.514548 20140218T150717


	//20140218T150717.514548 20140218T150717
	//std::string strTime = boost::posix_time::to_iso_string(boost::posix_time::second_clock::local_time());

	//20140218T150717.514548
	//YYYYMMDDTHHMMSS.SSSSSS
	//std::string strTime = boost::posix_time::to_iso_string(boost::posix_time::second_clock::local_time());
	strTimeStringTmp = boost::posix_time::to_iso_string(*pfbtime);

	if (strTimeStringTmp.size() > 4)
	{
		//YYYY
		strYear = strTimeStringTmp.substr(0, 4);
		//MMDDTHHMMSS.SSSSSS
		strSubTmp = strTimeStringTmp.substr(4);
	}
	else
	{
		LOG_ERROR<<"error! get strYear from strTimeString="<<strTimeStringTmp;
		return strTimeStringGet;
	}

	if (strSubTmp.size() > 2)
	{
		//MM
		strMonth = strSubTmp.substr(0, 2);
		//DDTHHMMSS.SSSSSS
		strSubTmp = strSubTmp.substr(2);
	}
	else
	{
		LOG_ERROR<<"error! get strMonth from strTimeString="<<strTimeStringTmp<<" "<<"strSubTmp="<<strSubTmp;
		return strTimeStringGet;
	}
	
	if (strSubTmp.size() > 3)
	{
		//DD
		strDay = strSubTmp.substr(0, 2);
		//HHMMSS.SSSSSS
		strSubTmp = strSubTmp.substr(3);
	}
	else
	{
		LOG_ERROR<<"error! get strDay from strTimeString="<<strTimeStringTmp<<" "<<"strSubTmp="<<strSubTmp;
		return strTimeStringGet;
	}

	if (strSubTmp.size() > 2)
	{
		//HH
		strHour = strSubTmp.substr(0, 2);
		//MMSS.SSSSSS
		strSubTmp = strSubTmp.substr(2);
	}
	else
	{
		LOG_ERROR<<"error! get strHour from strTimeString="<<strTimeStringTmp<<" "<<"strSubTmp="<<strSubTmp;
		return strTimeStringGet;
	}

	if (strSubTmp.size() > 2)
	{
		//MM
		strMin = strSubTmp.substr(0, 2);
		//SS.SSSSSS
		strSubTmp = strSubTmp.substr(2);
	}
	else
	{
		LOG_ERROR<<"error! get strMin from strTimeString="<<strTimeStringTmp<<" "<<"strSubTmp="<<strSubTmp;
		return strTimeStringGet;
	}

	if (strSubTmp.size() >= 2)
	{
		//SS
		strSecond = strSubTmp.substr(0, 2);
	}
	else
	{
		LOG_ERROR<<"error! get strSecond from strTimeString="<<strTimeStringTmp<<" "<<"strSubTmp="<<strSubTmp;
		return strTimeStringGet;
	}

	//"%04d-%02d-%02d %02d:%02d:%02d"
	strTimeStringGet = strYear + "-" + strMonth + "-" + strDay + " " 
		+ strHour + ":" + strMin + ":" + strSecond;

	return strTimeStringGet;
}


std::string CAWorkTime::getTimeNow(void)
{
	std::string	strCurrentTime;
	boost::posix_time::ptime timeNow;
	strCurrentTime = getCurrentTime(&timeNow);
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

std::string CAWorkTime::getCurrentTime(boost::posix_time::ptime* pfbtime)
{
	std::string			strCurrentTime;
	boost::posix_time::ptime  timeNow;
	if (NULL != pfbtime)
	{
		timeNow = boost::posix_time::microsec_clock::universal_time() + boost::posix_time::hours(8); 
		(*pfbtime) = timeNow;
	}
	strCurrentTime = getTimeString(pfbtime);
	return strCurrentTime;
}

boost::posix_time::millisec_posix_time_system_config::time_duration_type CAWorkTime::getDiffTime(
	boost::posix_time::ptime* pfbtimeBegin, 
	boost::posix_time::ptime* pfbtimeEnd )
{
	boost::posix_time::millisec_posix_time_system_config::time_duration_type timeElapse;

	if (NULL != pfbtimeBegin && NULL != pfbtimeEnd)
	{
		timeElapse = (*pfbtimeEnd) - (*pfbtimeBegin);  
	}
	//%10.3f ms

	return timeElapse;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

CWorkTimeLock::CWorkTimeLock(int nWorkTimeCode )
{
	m_nWorkTimeCode = nWorkTimeCode;	
	getCurrentTime(&m_fTimeWorkBegin);
	getCurrentTime(&m_fTimeWorkEnd);
	m_nWorkTime = getDiffTime(&m_fTimeWorkEnd, &m_fTimeWorkEnd);//0
	m_nNotWorkTime = getDiffTime(&m_fTimeWorkEnd, &m_fTimeWorkEnd);//0
	m_bSetWorkBegin = false;
	m_bSetWorkEnd = false;


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
	BigInt64 nFunRes = 0;
	boost::posix_time::ptime fTimeNow;
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
		getCurrentTime(&fTimeNow);
		m_nWorkTime = getDiffTime(&fTimeNow, &fTimeNow);//0
	}

	nFunRes = getTimeMilliseconds(&m_nWorkTime);
	return nFunRes;
}

BigInt64 CWorkTimeLock::getNotworkTime()
{
	BigInt64 nFunRes = 0;
	boost::posix_time::ptime fTimeNow;

	boost::mutex::scoped_lock lock(m_lockWorkTime);

	if (true == m_bSetWorkBegin && false == m_bSetWorkEnd)
	{
		getCurrentTime(&fTimeNow);
		m_nNotWorkTime = getDiffTime(&m_fTimeWorkBegin, &fTimeNow);
	}
	else
	{
		m_nNotWorkTime = getDiffTime(&fTimeNow, &fTimeNow);//0
	}

	nFunRes = getTimeMilliseconds(&m_nNotWorkTime);

	return nFunRes;
}


BigInt64 CWorkTimeLock::getWorkTime()
{
	BigInt64 nFunRes = 0;
	boost::mutex::scoped_lock lock(m_lockWorkTime);
	nFunRes = getTimeMilliseconds(&m_nWorkTime);

	return nFunRes;
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
	m_nWorkTime = getDiffTime(&m_fTimeWorkEnd, &m_fTimeWorkEnd);//0
	m_bSetWorkBegin = false;
	m_bSetWorkEnd = false;
	m_nNotWorkTime = getDiffTime(&m_fTimeWorkEnd, &m_fTimeWorkEnd);//0
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
	BigInt64 nFunRes = 0;
	boost::posix_time::ptime fTimeNow;

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
		getCurrentTime(&fTimeNow);
		m_nWorkTime = getDiffTime(&fTimeNow, &fTimeNow);//0
	}

	nFunRes = getTimeMilliseconds(&m_nWorkTime);
	return nFunRes;
}

BigInt64 CWorkTimeNoLock::getNotworkTime()
{
	BigInt64 nFunRes = 0;
	boost::posix_time::ptime fTimeNow;

	if (true == m_bSetWorkBegin && false == m_bSetWorkEnd)
	{
		getCurrentTime(&fTimeNow);
		m_nNotWorkTime = getDiffTime(&m_fTimeWorkBegin, &fTimeNow);
	}
	else
	{
		m_nNotWorkTime = getDiffTime(&fTimeNow, &fTimeNow);//0
	}

	nFunRes = getTimeMilliseconds(&m_nNotWorkTime);

	return nFunRes;
}


BigInt64 CWorkTimeNoLock::getWorkTime()
{
	BigInt64 nFunRes = 0;
	nFunRes = getTimeMilliseconds(&m_nWorkTime);

	return nFunRes;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

NS_END(TA_Base_App)












