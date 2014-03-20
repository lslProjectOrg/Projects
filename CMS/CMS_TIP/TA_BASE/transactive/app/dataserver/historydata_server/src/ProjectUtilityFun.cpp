#include "ProjectUtilityFun.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <list> 
#include <time.h>

#include "BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_App)


CHistoryDataServerUtilityFun::CHistoryDataServerUtilityFun( void )
{
	BOOST_LOG_FUNCTION();
}

CHistoryDataServerUtilityFun::~CHistoryDataServerUtilityFun( void )
{
	BOOST_LOG_FUNCTION();

}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//"%d-%d-%d %d:%d:%d"
//YYYY-MM-DD HH:MM:SS
time_t CHistoryDataServerUtilityFun::strToDateTime(const std::string& strTimeValue)
{
	tm tmTime;
	int nYearValue = 0;
	int nMonthValue = 0;
	int nDayValue = 0;
	int nHourValue = 0;
	int nMinuteValue = 0;
	int nSecondValue = 0;
	time_t timeGetTimeValue = time(NULL);
	
	if (strTimeValue.empty())
	{
		return timeGetTimeValue;
	}

	sscanf(strTimeValue.c_str(),
		"%d-%d-%d %d:%d:%d", 
		&nYearValue, &nMonthValue, &nDayValue, 
		&nHourValue, &nMinuteValue, &nSecondValue);

	tmTime.tm_year  = nYearValue-1900;
	tmTime.tm_mon   = nMonthValue-1;
	tmTime.tm_mday  = nDayValue;
	tmTime.tm_hour  = nHourValue;
	tmTime.tm_min   = nMinuteValue;
	tmTime.tm_sec   = nSecondValue;
	tmTime.tm_isdst = 0;

	timeGetTimeValue = mktime(&tmTime); //dec 8 TC
	return timeGetTimeValue; //seconds
}

#if 0

//"%04d-%02d-%02d %02d:%02d:%02d"
std::string CHistoryDataServerUtilityFun::dataTimeToStr(time_t nTimeValue)
{
	BOOST_LOG_FUNCTION();
	std::string	strTimeString;
	char* pszCurTime = NULL;
	int nBufferSize = 256;
	struct tm* pTM = NULL;
	time_t time_Value = (time_t)nTimeValue;

	pszCurTime =new char[nBufferSize];
	memset(pszCurTime, 0, nBufferSize);

	pTM = localtime(&(time_Value));

	//TIMESTR--%Y%m%d%H%M%S
	//format to string
	if ( NULL != pTM )
	{
		sprintf(pszCurTime, "%04d-%02d-%02d %02d:%02d:%02d",
			pTM->tm_year + 1900, pTM->tm_mon + 1, pTM->tm_mday,
			pTM->tm_hour, pTM->tm_min, pTM->tm_sec); 
		strTimeString = pszCurTime;     
	}

	delete pszCurTime;
	pszCurTime = NULL;
	pTM = NULL;

	return strTimeString;
}

#endif



//"%04d-%02d-%02d %02d:%02d:%02d"
std::string CHistoryDataServerUtilityFun::dataTimeToStr(time_t nTimeValue)
{
	BOOST_LOG_FUNCTION();

	std::string strSubTmp;
	std::string strYear;
	std::string strMonth;
	std::string strDay;
	std::string strHour;
	std::string strMin;
	std::string strSecond;
	std::string strMicSecond;
	int pos = 0;
	boost::posix_time::ptime nPosixTimeValue;
	std::string strTimeStringTmp;
	std::string	strTimeStringGet;


	nPosixTimeValue = boost::posix_time::from_time_t(nTimeValue);

	//linshenglong take care   20140218T150717.514548 or 20140218T150717
	//YYYYMMDDTHHMMSS.SSSSSS or YYYYMMDDTHHMMSS
	//std::string strTime = boost::posix_time::to_iso_string(boost::posix_time::second_clock::local_time());
	strTimeStringTmp = boost::posix_time::to_iso_string(nPosixTimeValue);

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
		strTimeStringGet.clear();
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
		strTimeStringGet.clear();
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
		strTimeStringGet.clear();
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
		strTimeStringGet.clear();
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
		strTimeStringGet.clear();
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
		strTimeStringGet.clear();
		return strTimeStringGet;
	}

	//"%04d-%02d-%02d %02d:%02d:%02d"
	strTimeStringGet = strYear + "-" + strMonth + "-" + strDay + " " + strHour + ":" + strMin + ":" + strSecond;

	return strTimeStringGet;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////



NS_END(TA_Base_App)
















