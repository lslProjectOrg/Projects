#include "CFDServerUtilityFun.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <list> 
#include <time.h>


#include "core/utilities/src/BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_App)


CCFDServerUtilityFun::CCFDServerUtilityFun( void )
{
	BOOST_LOG_FUNCTION();

}

CCFDServerUtilityFun::~CCFDServerUtilityFun( void )
{
	BOOST_LOG_FUNCTION();

}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//"%d-%d-%d %d:%d:%d"
time_t CCFDServerUtilityFun::strToDateTime(const std::string& strTimeValue)
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


//"%04d-%02d-%02d %02d:%02d:%02d"
std::string CCFDServerUtilityFun::dataTimeToStr(time_t nTimeValue)
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

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


void CCFDServerUtilityFun::logBarInfo(const std::string& strInfo, int interval, Bar* pBarInfo)
{
	BOOST_LOG_FUNCTION();
	std::string strBarTime;

	strBarTime = dataTimeToStr(pBarInfo->Time);
	LOG_DEBUG<<strInfo<<"interval="<<interval<<" "<<"pBarInfo->Time="<<pBarInfo->Time;
	LOG_DEBUG<<strInfo<<"interval="<<interval<<" "<<"strBarTime="<<strBarTime;
	LOG_DEBUG<<strInfo<<"interval="<<interval<<" "<<"pBarInfo->Open="<<pBarInfo->Open;
	LOG_DEBUG<<strInfo<<"interval="<<interval<<" "<<"pBarInfo->Close="<<pBarInfo->Close;
	LOG_DEBUG<<strInfo<<"interval="<<interval<<" "<<"pBarInfo->High="<<pBarInfo->High;
	LOG_DEBUG<<strInfo<<"interval="<<interval<<" "<<"pBarInfo->Low="<<pBarInfo->Low;
	LOG_DEBUG<<strInfo<<"interval="<<interval<<" "<<"pBarInfo->Volume="<<pBarInfo->Volume;
}


void CCFDServerUtilityFun::logMarketDataInfo(const std::string& strInfo, const MarketData &marketData)
{
	BOOST_LOG_FUNCTION();
	//temp value
	MarketData::PriceType nPriceTypeValue = MarketData::LAST_TRADED_PRICE;
	MarketData::VolumeType nVolumeTypeValue = MarketData::LAST_TRADED_VOLUME;
	unsigned int nSecurityIDValue = 0;
	MarketData::MarketStatus nMarkerStatusValue = (MarketData::MarketStatus)MarketData::PRE_OPEN;
	unsigned int nTimeValue = 0;
	std::string  strTimeValue = "";
	int nVolumeValue = 0;
	int nBidVolValue = 0;
	int nAskVolValue = 0;
	float fPriceValue = 0;
	float fBidPxValue = 0;
	float fAskPxValue = 0;
	unsigned int nDataBitsValue = 0;
	unsigned int nChangeBitsValue = 0;
	//


	//
	nSecurityIDValue = marketData.getSecurityID();
	nMarkerStatusValue = (MarketData::MarketStatus)(marketData.getMarketStatus());
	nTimeValue = marketData.getTime();
	strTimeValue = dataTimeToStr(nTimeValue);
	nVolumeValue = marketData.getVolume(nVolumeTypeValue);
	nBidVolValue = marketData.getBidVol(0);
	nAskVolValue = marketData.getAskVol(0);
	fPriceValue = marketData.getPrice(nPriceTypeValue);
	fBidPxValue = marketData.getBidPx(0);
	fAskPxValue = marketData.getAskPx(0);
	nDataBitsValue = marketData.getDataBits();
	nChangeBitsValue = marketData.getChangeBits();
	//
	LOG_DEBUG<<"nSecurityIDValue="<<nSecurityIDValue;
	LOG_DEBUG<<"nMarkerStatusValue="<<nMarkerStatusValue;
	LOG_DEBUG<<"nTimeValue="<<nTimeValue;
	LOG_DEBUG<<"strTimeValue="<<strTimeValue;
	LOG_DEBUG<<"nVolumeValue="<<nVolumeValue;
	LOG_DEBUG<<"fPriceValue="<<fPriceValue;



}



NS_END(TA_Base_App)
















