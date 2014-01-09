#include "CFDInstrumentBarInfo.h"

#include "core/utilities/src/BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_App)


CCFDInstrumentBarInfo::CCFDInstrumentBarInfo(const CCFDRequest& cfdRequest)
{
	BOOST_LOG_FUNCTION();
	m_CFDRequest = cfdRequest;
}

CCFDInstrumentBarInfo::~CCFDInstrumentBarInfo(void)
{
	BOOST_LOG_FUNCTION();
}


void CCFDInstrumentBarInfo::setBarInfoFirst( const MarketData& nBarInfoFirst )
{
	BOOST_LOG_FUNCTION();
	m_nBarInfoFirst = nBarInfoFirst;

}

void CCFDInstrumentBarInfo::setBarInfoSecond( const MarketData& nBarInfoSecond )
{
	BOOST_LOG_FUNCTION();
	m_nBarInfoSecond = nBarInfoSecond;

}


void CCFDInstrumentBarInfo::setCFDRequest(const CCFDRequest& cfdRequest)
{
	BOOST_LOG_FUNCTION();

	m_CFDRequest = cfdRequest;
}


int CCFDInstrumentBarInfo::buidCFDBarInfo()
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;

	unsigned int nFirstInstrumentTime = 0;
	unsigned int nSecondInstrumentTime = 0;
	int x = 1;
	int y = 1;
	float ABid = 0;
	float AAsk = 0;
	float BBid = 0;
	float BAsk = 0;

	float xABid = 0;
	float xAAsk = 0;
	float yBBid = 0;
	float yBAsk = 0;

	float bidPx = 0;
	float askPx = 0;
	float price = 0;

	x = m_CFDRequest.m_nXValue; 
	y = m_CFDRequest.m_nYValue;

	nFirstInstrumentTime = m_nBarInfoFirst.getTime();
	nSecondInstrumentTime = m_nBarInfoSecond.getTime();

	ABid = m_nBarInfoFirst.getBidPx(MarketData::REFERENCE_PRICE);
	AAsk = m_nBarInfoFirst.getAskPx(MarketData::REFERENCE_PRICE);
	BBid = m_nBarInfoSecond.getAskPx(MarketData::REFERENCE_PRICE);
	BAsk = m_nBarInfoSecond.getBidPx(MarketData::REFERENCE_PRICE);

	xABid = (x * ABid);
	xAAsk = (x * AAsk);

	yBAsk = (x * BAsk);
	yBBid = (x * BBid);

	bidPx = xABid - yBAsk;
	askPx = xAAsk - yBBid;
	price = ((xABid - yBAsk + xAAsk - yBBid) / 2);


	//TODO. use min time
	if (nFirstInstrumentTime <= nSecondInstrumentTime)
	{
		m_CFDBarInfo.Time = nFirstInstrumentTime;//TODO,  
	}
	else
	{
		m_CFDBarInfo.Time = nSecondInstrumentTime;//TODO, 
	}

	m_CFDBarInfo.Open = 0;
	m_CFDBarInfo.Close = 0;
	m_CFDBarInfo.High = price;
	m_CFDBarInfo.Low = 0;
	m_CFDBarInfo.Volume = 0;



	//TODO.
	//bid  buy  
	//ask  sell
	/*
	Jake: A B
	xA - yB
	ABid
	AAsk
	BAsk
	BBid
	xABid
	xAAsk
	yBAsk
	yBBid

	bidPx = xABid - yBAsk
	askPx = xAAsk - yBBid
	price = (bidPx + askPx) / 2
	price = (xAska + xAskb - yAska - yBidb) / 2
	*/
	return nFunRes;
}



void CCFDInstrumentBarInfo::logInfo()
{
	BOOST_LOG_FUNCTION();
	std::string strInfo;
	std::string strBarTime;
	Bar* pBarInfo = NULL;

	pBarInfo = &m_CFDBarInfo;
	strInfo = "CCFDInstrumentBarInfo";

	//strBarTime = dataTimeToStr(pBarInfo->Time);
	LOG_DEBUG<<strInfo<<"interval="<<m_CFDRequest.m_nCFDInterval<<" "<<"pBarInfo->Time="<<pBarInfo->Time;
	//LOG_DEBUG<<strInfo<<"interval="<<interval<<" "<<"strBarTime="<<strBarTime;
	LOG_DEBUG<<strInfo<<"interval="<<m_CFDRequest.m_nCFDInterval<<" "<<"pBarInfo->Open="<<pBarInfo->Open;
	LOG_DEBUG<<strInfo<<"interval="<<m_CFDRequest.m_nCFDInterval<<" "<<"pBarInfo->Close="<<pBarInfo->Close;
	LOG_DEBUG<<strInfo<<"interval="<<m_CFDRequest.m_nCFDInterval<<" "<<"pBarInfo->High="<<pBarInfo->High;
	LOG_DEBUG<<strInfo<<"interval="<<m_CFDRequest.m_nCFDInterval<<" "<<"pBarInfo->Low="<<pBarInfo->Low;
	LOG_DEBUG<<strInfo<<"interval="<<m_CFDRequest.m_nCFDInterval<<" "<<"pBarInfo->Volume="<<pBarInfo->Volume;

	return;
}

NS_END(TA_Base_App)
















