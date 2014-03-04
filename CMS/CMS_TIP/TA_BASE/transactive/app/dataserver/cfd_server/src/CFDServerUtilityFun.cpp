#include "CFDServerUtilityFun.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <list> 
#include <time.h>
#include "WorkTime.h"

#include "BarCalculator.h"
#include <boost/date_time/posix_time/posix_time.hpp>  

#include "BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_App)

//static const int DEF_VALUE_INT_MAXBUFFERSIZE = 1024;



CCFDServerUtilityFun::CCFDServerUtilityFun( void )
{
	BOOST_LOG_FUNCTION();
	m_pWorkTime = NULL;
	m_pWorkTime = new CWorkTimeNoLock();

}

CCFDServerUtilityFun::~CCFDServerUtilityFun( void )
{
	BOOST_LOG_FUNCTION();
	if (NULL != m_pWorkTime)
	{
		delete m_pWorkTime;
		m_pWorkTime = NULL;
	}

}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//"%d-%d-%d %d:%d:%d"
//YYYY-MM-DD HH:MM:SS
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

#if 0

//"%04d-%02d-%02d %02d:%02d:%02d"
std::string CCFDServerUtilityFun::dataTimeToStr(time_t nTimeValue)
{
	BOOST_LOG_FUNCTION();
	std::string	strTimeString;
	char* pszCurTime = NULL;
	struct tm* pTM = NULL;
	time_t time_Value = (time_t)nTimeValue;

	pszCurTime =new char[DEF_VALUE_INT_MAXBUFFERSIZE];
	memset(pszCurTime, 0, DEF_VALUE_INT_MAXBUFFERSIZE);

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

	if (NULL != pszCurTime)
	{
		delete[] pszCurTime;
		pszCurTime = NULL;
	}

	pTM = NULL;

	return strTimeString;
}
#endif



//"%04d-%02d-%02d %02d:%02d:%02d"
std::string CCFDServerUtilityFun::dataTimeToStr(time_t nTimeValue)
{
	BOOST_LOG_FUNCTION();
	std::string	strTimeString;
	boost::posix_time::ptime boostTimeValue;
	boostTimeValue = boost::posix_time::from_time_t(nTimeValue);
	strTimeString = m_pWorkTime->getTimeString(&boostTimeValue);
	//"%04d-%02d-%02d %02d:%02d:%02d"
	return strTimeString;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


void CCFDServerUtilityFun::logBarInfo(const std::string& strInfo, int interval, Bar* pBarInfo)
{
	BOOST_LOG_FUNCTION();
	std::string strBarTime;

	strBarTime = dataTimeToStr(pBarInfo->Time);
	LOG_TRACE<<strInfo<<"interval="<<interval<<" "<<"pBarInfo->Time="<<pBarInfo->Time;
	LOG_TRACE<<strInfo<<"interval="<<interval<<" "<<"strBarTime="<<strBarTime;
	LOG_TRACE<<strInfo<<"interval="<<interval<<" "<<"pBarInfo->Open="<<pBarInfo->Open;
	LOG_TRACE<<strInfo<<"interval="<<interval<<" "<<"pBarInfo->Close="<<pBarInfo->Close;
	LOG_TRACE<<strInfo<<"interval="<<interval<<" "<<"pBarInfo->High="<<pBarInfo->High;
	LOG_TRACE<<strInfo<<"interval="<<interval<<" "<<"pBarInfo->Low="<<pBarInfo->Low;
	LOG_TRACE<<strInfo<<"interval="<<interval<<" "<<"pBarInfo->Volume="<<pBarInfo->Volume;

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

	nSecurityIDValue = marketData.getSecurityID();
	nMarkerStatusValue = (MarketData::MarketStatus)(marketData.getMarketStatus());

// 	Tick  newTick;
// 	newTick.Time = marketData.getTime();//nTimeValue
// 	newTick.BidPx = marketData.getBidPx(1);//fBidPxValue
// 	newTick.AskPx = marketData.getAskPx(1);//fAskPxValue
// 	newTick.LastPx = marketData.getPrice(MarketData::LAST_TRADED_PRICE);//fPriceValue
// 	newTick.BidVol = marketData.getBidVol(1);//nBidVolValue
// 	newTick.AskVol = marketData.getAskVol(1);//nAskVolValue
// 	newTick.LastVol = marketData.getVolume(MarketData::LAST_TRADED_VOLUME);//nVolumeValue

	//
	nTimeValue = marketData.getTime();
	strTimeValue = dataTimeToStr(nTimeValue);
	fBidPxValue = marketData.getBidPx(1);
	fAskPxValue = marketData.getAskPx(1);
	fPriceValue = marketData.getPrice(nPriceTypeValue);

	nBidVolValue = marketData.getBidVol(1);
	nAskVolValue = marketData.getAskVol(1);
	nVolumeValue = marketData.getVolume(nVolumeTypeValue);

	nDataBitsValue = marketData.getDataBits();
	nChangeBitsValue = marketData.getChangeBits();
	//
	LOG_TRACE<<"MarketData"<<"->"<<"nSecurityIDValue="<<nSecurityIDValue;
	LOG_TRACE<<"MarketData"<<"->"<<"nMarkerStatusValue="<<nMarkerStatusValue;
	LOG_TRACE<<"MarketData"<<"->"<<"Tick"<<"->"<<"nTimeValue="<<nTimeValue;
	LOG_TRACE<<"MarketData"<<"->"<<"Tick"<<"->"<<"strTimeValue="<<strTimeValue;
	LOG_TRACE<<"MarketData"<<"->"<<"Tick"<<"->"<<"BidPx="<<fBidPxValue;
	LOG_TRACE<<"MarketData"<<"->"<<"Tick"<<"->"<<"AskPx="<<fAskPxValue;
	LOG_TRACE<<"MarketData"<<"->"<<"Tick"<<"->"<<"LastPx="<<fPriceValue;
	LOG_TRACE<<"MarketData"<<"->"<<"Tick"<<"->"<<"BidVol="<<nBidVolValue;
	LOG_TRACE<<"MarketData"<<"->"<<"Tick"<<"->"<<"AskVol="<<nAskVolValue;
	LOG_TRACE<<"MarketData"<<"->"<<"Tick"<<"->"<<"LastVol="<<nVolumeValue;

}



NS_END(TA_Base_App)
















