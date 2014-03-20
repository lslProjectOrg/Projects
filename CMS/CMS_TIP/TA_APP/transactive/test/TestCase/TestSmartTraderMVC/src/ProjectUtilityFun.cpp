#include "ProjectUtilityFun.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <list> 
#include <time.h>

#include "BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_App_App)


CProjectUtilityFun::CProjectUtilityFun( void )
{
	BOOST_LOG_FUNCTION();
}

CProjectUtilityFun::~CProjectUtilityFun( void )
{
	BOOST_LOG_FUNCTION();

}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//"%d-%d-%d %d:%d:%d"
//YYYY-MM-DD HH:MM:SS
time_t CProjectUtilityFun::strToDateTime(const std::string& strTimeValue)
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
std::string CProjectUtilityFun::dataTimeToStr(time_t nTimeValue)
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

void CProjectUtilityFun::log_Instrument_info(const std::string& strLogInfo, const Instrument &instrument )
{
	unsigned int nGetInstrumentID = 0;
	std::string  strGetInstrumentCode;
	std::string  strGetUnderlyingCode;
	std::string  strGetExchangeName;
	char chGetDecimalPlace = 0;
	float fGetTickSize = 0;
	float fGetTickValue = 0;
	int nGetBidVolume = 0;
	int nGetAskVolume = 0;
	int nGetLastVolume = 0;
	int nGetTotalVolume = 0;
	float fGetBidPrice = 0;
	float fGetAskPrice = 0;
	float fGetLowPrice = 0;
	float fGetHighPrice = 0;
	float fGetLastPrice = 0;
	float fGetOpeningPrice = 0;
	float fGetSettlementPrice = 0;
	unsigned int timeGetUpdateTime = 0;//time
	std::string strGetupdateTime;
	double fGetValuePerPoint = 0;
	int nGetLegSize = 0;
	unsigned int nGetLegSecurityID = 0;
	int nGetMarketStatus = 0;
	int nGetSecurityType = 0;
	unsigned int nGetOrderTypes = 0;

	nGetInstrumentID = instrument.getInstrumentID();
	strGetInstrumentCode = instrument.getInstrumentCode();
	strGetUnderlyingCode = instrument.getUnderlyingCode();
	strGetExchangeName = instrument.getExchangeName();
	chGetDecimalPlace = instrument.getDecimalPlace();
	fGetTickSize = instrument.getTickSize();
	fGetTickValue = instrument.getTickValue();
	nGetBidVolume = instrument.getBidVolume();
	nGetAskVolume = instrument.getAskVolume();
	nGetLastVolume = instrument.getLastVolume();
	nGetTotalVolume = instrument.getTotalVolume();
	fGetBidPrice = instrument.getBidPrice();
	fGetAskPrice = instrument.getAskPrice();
	fGetLowPrice = instrument.getLowPrice();
	fGetHighPrice = instrument.getHighPrice();
	fGetLastPrice = instrument.getLastPrice();
	fGetOpeningPrice = instrument.getOpeningPrice();
	fGetSettlementPrice = instrument.getSettlementPrice();
	timeGetUpdateTime = instrument.getUpdateTime();
	strGetupdateTime = dataTimeToStr(timeGetUpdateTime);
	fGetValuePerPoint = instrument.getValuePerPoint();
	nGetLegSize = instrument.getLegSize();
	//nGetLegSecurityID = instrument.getLegSecurityID();
	nGetMarketStatus = instrument.getMarketStatus();
	nGetSecurityType = instrument.getSecurityType();
	nGetOrderTypes = instrument.getOrderTypes();


	LOG_DEBUG<<strLogInfo
		<<" "<<"nGetInstrumentID="<<nGetInstrumentID
		<<" "<<"strGetInstrumentCode="<<strGetInstrumentCode
		<<" "<<"strGetUnderlyingCode="<<strGetUnderlyingCode
		<<" "<<"strGetExchangeName="<<strGetExchangeName
		<<" "<<"chGetDecimalPlace="<<chGetDecimalPlace
		<<" "<<"fGetTickSize="<<fGetTickSize
		<<" "<<"fGetTickValue="<<fGetTickValue
		<<" "<<"nGetBidVolume="<<nGetBidVolume
		<<" "<<"nGetAskVolume="<<nGetAskVolume
		<<" "<<"nGetLastVolume="<<nGetLastVolume
		<<" "<<"nGetTotalVolume="<<nGetTotalVolume
		<<" "<<"fGetBidPrice="<<fGetBidPrice
		<<" "<<"fGetAskPrice="<<fGetAskPrice
		<<" "<<"fGetLowPrice="<<fGetLowPrice
		<<" "<<"fGetHighPrice="<<fGetHighPrice
		<<" "<<"fGetLastPrice="<<fGetLastPrice
		<<" "<<"fGetOpeningPrice="<<fGetOpeningPrice
		<<" "<<"fGetSettlementPrice="<<fGetSettlementPrice
		<<" "<<"fGetBidPrice="<<fGetBidPrice
		<<" "<<"timeGetUpdateTime="<<timeGetUpdateTime
		<<" "<<"strGetupdateTime="<<strGetupdateTime		
		<<" "<<"fGetValuePerPoint="<<fGetValuePerPoint
		<<" "<<"nGetLegSize="<<nGetLegSize
		//<<" "<<"nGetLegSecurityID="<<nGetLegSecurityID
		<<" "<<"nGetMarketStatus="<<nGetMarketStatus
		<<" "<<"nGetSecurityType="<<nGetSecurityType
		<<" "<<"nGetOrderTypes="<<nGetOrderTypes;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////



NS_END(TA_App_App)
















